# AI-Assisted: Claude Opus 5.5: boilerplate, entire file

param(
    [Parameter(Mandatory = $true)][ValidateSet('install', 'uninstall')][string]$Action,
    [string]$LayerDir = ''
)

$ErrorActionPreference = 'Stop'
# Windows PowerShell 5.1 adds a Count property to arrays, ConvertTo-Json would then write arrays wrongly
Remove-TypeData -TypeName System.Array -ErrorAction SilentlyContinue

$LayerKey     = 'VK_LAYER_VUT_DebuggerLayer'
$ManifestName = 'vkDebugger_windows.json'
$ConfigName   = 'VkDebugger'
$LunarG       = Join-Path $env:LOCALAPPDATA 'LunarG'
$VkconfigDir  = Join-Path $LunarG 'vkconfig'
$VkconfigJson = Join-Path $VkconfigDir 'vkconfig.json'
$SettingsTxt  = Join-Path $VkconfigDir 'override\vk_layer_settings.txt'
$OverrideJson = Join-Path $VkconfigDir 'override\VkLayer_override.json'
$LoaderJson   = Join-Path $LunarG 'vulkan\vk_loader_settings.json'
$Utf8NoBom    = New-Object System.Text.UTF8Encoding $false
$QSettingsKey = if ($env:VKDBG_QSETTINGS_KEY) { $env:VKDBG_QSETTINGS_KEY } else { 'Software\LunarG\vkconfig' }

function Fail([string]$message) {
    Write-Host "ERROR: $message" -ForegroundColor Red
    exit 1
}

function Read-Json([string]$path) {
    return [System.IO.File]::ReadAllText($path) | ConvertFrom-Json
}

function Write-Json([string]$path, $object) {
    [System.IO.File]::WriteAllText($path, ($object | ConvertTo-Json -Depth 100), $Utf8NoBom)
}

function Same-Path([string]$a, [string]$b) {
    return $a.TrimEnd('\', '/').Replace('/', '\') -ieq $b.TrimEnd('\', '/').Replace('/', '\')
}

function Has-Property($object, [string]$name) {
    return $null -ne $object -and $object.PSObject.Properties.Name -contains $name
}

function Set-Property($object, [string]$name, $value) {
    if (Has-Property $object $name) {
        $object.$name = $value
    } else {
        $object | Add-Member -NotePropertyName $name -NotePropertyValue $value
    }
}

# vkconfig 3.x keeps the layer paths in vkconfig.json under interface.LAYERS
function Get-LayersSection($vkconfig) {
    if ((Has-Property $vkconfig 'interface') -and (Has-Property $vkconfig.interface 'LAYERS')) {
        return $vkconfig.interface.LAYERS
    }
    return $null
}

# vkconfig 2.x keeps configurations in folders configurations / configurations_X_Y_Z
function Get-ConfigDirs {
    if (-not (Test-Path -LiteralPath $VkconfigDir)) {
        return @()
    }
    return @(Get-ChildItem -LiteralPath $VkconfigDir -Directory |
             Where-Object { $_.Name -eq 'configurations' -or $_.Name -match '^configurations_\d+_\d+_\d+$' })
}

# reads all configuration files of one folder, skips unreadable ones
function Get-Configurations([string]$dir) {
    $result = @()
    foreach ($file in @(Get-ChildItem -LiteralPath $dir -Filter '*.json' -File)) {
        try { $json = Read-Json $file.FullName } catch { Write-Host "Skipped (cannot read): $($file.FullName)"; continue }
        if (Has-Property $json 'configuration') {
            $result += @{ File = $file; Json = $json }
        }
    }
    return $result
}

# removes our layer from a configuration, returns the manifests it was taken from
function Remove-LayerDescriptors($layers) {
    $manifests = @()
    if (-not (Has-Property $layers 'layers')) {
        return $manifests
    }
    foreach ($property in @($layers.layers.PSObject.Properties)) {
        $descriptors = @($property.Value)
        $kept = @($descriptors | Where-Object { $_.key -ne $LayerKey })
        if ($kept.Count -eq $descriptors.Count) {
            continue
        }
        $manifests += $property.Name
        if ($kept.Count -eq 0) {
            $layers.layers.PSObject.Properties.Remove($property.Name)
        } else {
            $layers.layers.($property.Name) = $kept
        }
    }
    return $manifests
}

function Assert-VkconfigClosed {
    $running = Get-Process -Name 'vkconfig-gui', 'vkconfig' -ErrorAction SilentlyContinue
    if ($running) {
        Fail 'Vulkan Configurator is running. Close it first (it rewrites its files when it exits).'
    }
}

# before a file is changed, a copy goes to %TEMP%\vkdbg-uninstall-..., index.txt says where each came from
function Backup([string]$path) {
    if (-not $script:BackupDir) {
        $script:BackupDir = Join-Path $env:TEMP ('vkdbg-uninstall-' + (Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
        New-Item -ItemType Directory -Path $script:BackupDir -Force | Out-Null
        $script:BackupCount = 0
    }
    $script:BackupCount++
    $name = '{0:D2}_{1}' -f $script:BackupCount, (Split-Path $path -Leaf)
    Copy-Item -LiteralPath $path -Destination (Join-Path $script:BackupDir $name)
    Add-Content -LiteralPath (Join-Path $script:BackupDir 'index.txt') -Value "$name <- $path"
}


# finds the installed vkconfig versions to install into (3.x and/or 2.x folders)
function Find-Targets {
    $targets = @()
    if (Test-Path -LiteralPath $VkconfigJson) {
        try { $vkconfig = Read-Json $VkconfigJson } catch { $vkconfig = $null; Write-Host "Skipped (cannot read): $VkconfigJson" }
        if ($null -ne $vkconfig -and "$($vkconfig.file_format_version)" -like '3.*') {
            $ready = $null -ne (Get-LayersSection $vkconfig)
            $targets += @{ Kind = '3'; Ready = $ready
                           Label = "vkconfig 3.x (vkconfig.json, format $($vkconfig.file_format_version)) - one layer path list"
                           Note = 'vkconfig.json has no layer section yet: start this vkconfig once, close it, run install again' }
        }
    }
    foreach ($dir in @(Get-ConfigDirs | Where-Object { $_.Name -ne 'configurations' })) {
        $configs = @(Get-Configurations $dir.FullName)
        $format = ($dir.Name -replace '^configurations_', '') -replace '_', '.'
        $targets += @{ Kind = '2'; Dir = $dir.FullName; Configs = $configs; Ready = $configs.Count -gt 0
                       Label = "vkconfig 2.x (configurations format $format) - path added to each of its $($configs.Count) configurations"
                       Note = 'no configuration saved yet: start this vkconfig once, close it, run install again' }
    }
    return $targets
}

# app_path setting pointing to VkDebugger.exe in the install folder
function New-AppPathSetting([string]$folder) {
    return [pscustomobject][ordered]@{ key = 'app_path'; type = 'LOAD_FILE'; value = (Join-Path $folder 'VkDebugger.exe') }
}

# creates the VkDebugger configuration in vkconfig; an existing one is only updated, not overwritten
function New-VkDebuggerConfiguration([string]$dir, $json, [string]$label, [string]$folder) {
    $path = Join-Path $dir "$ConfigName.json"
    if (-not (Test-Path -LiteralPath $path)) {
        Write-Json $path $json
        Write-Host "  ${label}: configuration '$ConfigName' created (not selected - choose it in vkconfig)"
        return
    }
    $existing = Read-Json $path
    $layer = @($existing.configuration.layers) | Where-Object { $_.name -eq $LayerKey } | Select-Object -First 1
    if ($null -eq $layer) {
        Write-Host "  ${label}: configuration '$ConfigName' exists without the VkDebugger layer, left as it is"
        return
    }
    $settings = @()
    if (Has-Property $layer 'settings') {
        $settings = @($layer.settings | Where-Object { $_.key -ne 'app_path' })
    }
    Set-Property $layer 'settings' (@(New-AppPathSetting $folder) + $settings)
    Write-Json $path $existing
    Write-Host "  ${label}: configuration '$ConfigName' already exists, App Path set to $(Join-Path $folder 'VkDebugger.exe')"
}

function Install-Into3([string]$folder) {
    $vkconfig = Read-Json $VkconfigJson
    $format = "$($vkconfig.file_format_version)"
    $platforms = @('WINDOWS_X86', 'WINDOWS_ARM')
    $configuration = [ordered]@{
        file_format_version = $format
        configuration = [ordered]@{
            name = $ConfigName; version = 1; platforms = $platforms
            default_layer_control = 'auto'; override_settings = $false; override_settings_path = ''
            selected_layer_name = $LayerKey
            layers = @([ordered]@{
                name = $LayerKey; type = 'Explicit'; control = 'on'; rank = 0; was_explicitly_rank = $false
                version = 'latest'; manifest = (Join-Path $folder $ManifestName); platforms = $platforms
                override_settings = $true; settings = @(New-AppPathSetting $folder)
            })
        }
    }
    $configsDir = Join-Path $VkconfigDir 'configurations'
    New-Item -ItemType Directory -Path $configsDir -Force | Out-Null
    New-VkDebuggerConfiguration $configsDir $configuration 'vkconfig 3.x' $folder

    $layers = Get-LayersSection $vkconfig
    Remove-LayerDescriptors $layers | Out-Null
    if (-not (Has-Property $layers 'layers')) {
        $layers | Add-Member -NotePropertyName 'layers' -NotePropertyValue ([pscustomobject]@{})
    }
    $descriptor = [pscustomobject][ordered]@{
        enabled = $true; key = $LayerKey; last_modified = ''; removed = $false; validated = 'NONE'
    }
    $layers.layers | Add-Member -NotePropertyName (Join-Path $folder $ManifestName) -NotePropertyValue @($descriptor) -Force

    $paths = @()
    if (Has-Property $layers 'paths') {
        $paths = @($layers.paths)
    }
    if (@($paths | Where-Object { Same-Path $_ $folder }).Count -eq 0) {
        $paths += $folder
    }
    Set-Property $layers 'paths' $paths
    Write-Json $VkconfigJson $vkconfig
    Write-Host "  vkconfig 3.x: layer registered"
}

function Install-Into2($target, [string]$folder) {
    $added = 0
    foreach ($config in $target.Configs) {
        $body = $config.Json.configuration
        $paths = @()
        if (Has-Property $body 'layers_paths') {
            $paths = @($body.layers_paths)
        }
        if (@($paths | Where-Object { Same-Path $_ $folder }).Count -gt 0) {
            continue
        }
        Set-Property $body 'layers_paths' ($paths + $folder)
        Write-Json $config.File.FullName $config.Json
        $added++
    }
    Write-Host "  vkconfig 2.x: added to $added of $($target.Configs.Count) configurations (the rest had it already)"
    Write-Host '  Note: configurations created later in vkconfig 2.x will not have it - run install again then.'

    $format = ((Split-Path $target.Dir -Leaf) -replace '^configurations_', '') -replace '_', '.'
    $configuration = [ordered]@{
        file_format_version = $format
        configuration = [ordered]@{
            name = $ConfigName; description = 'Only the VkDebugger layer (created by VkDebugger install.bat)'
            platforms = @('WINDOWS'); view_advanced_settings = $false
            layers = @([ordered]@{ name = $LayerKey; platforms = @('WINDOWS'); rank = 0; settings = @(New-AppPathSetting $folder); state = 'OVERRIDDEN' })
            layers_paths = @($folder)
        }
    }
    New-VkDebuggerConfiguration $target.Dir $configuration 'vkconfig 2.x' $folder
}

function Install {
    if ($LayerDir -eq '') {
        $LayerDir = Join-Path $PSScriptRoot '..\out\build\x64-debug'
    }
    if (-not (Test-Path -LiteralPath $LayerDir -PathType Container)) {
        Fail "Folder not found: $LayerDir (build VkDebugger first, or pass the folder as an argument)"
    }
    $LayerDir = (Resolve-Path -LiteralPath $LayerDir).ProviderPath.TrimEnd('\')
    foreach ($file in @($ManifestName, 'VkDebuggerLayer.dll', 'VkDebugger.exe', 'VkDebuggerAgent.exe')) {
        if (-not (Test-Path -LiteralPath (Join-Path $LayerDir $file))) {
            Fail "$file is missing in $LayerDir"
        }
    }
    Assert-VkconfigClosed

    Write-Host "VkDebugger folder: $LayerDir"
    $sdks = @(Get-ChildItem 'C:\VulkanSDK\*\Bin\vkconfig*.exe' -ErrorAction SilentlyContinue |
              ForEach-Object { $_.Directory.Parent.Name } | Sort-Object -Unique)
    if ($sdks.Count -gt 0) {
        Write-Host "Vulkan SDKs with vkconfig in C:\VulkanSDK: $($sdks -join ', ')"
    }

    $targets = @(Find-Targets)
    if ($targets.Count -eq 0) {
        Fail 'No vkconfig data found. Start Vulkan Configurator once, close it, then run this again.'
    }
    Write-Host ''
    Write-Host 'vkconfig versions found:'
    for ($i = 0; $i -lt $targets.Count; $i++) {
        if ($targets[$i].Ready) {
            Write-Host ("  {0}) {1}" -f ($i + 1), $targets[$i].Label)
        } else {
            Write-Host ("  -) {0}" -f $targets[$i].Label)
            Write-Host ("     cannot install: {0}" -f $targets[$i].Note) -ForegroundColor Yellow
        }
    }
    $ready = @(0..($targets.Count - 1) | Where-Object { $targets[$_].Ready })
    if ($ready.Count -eq 0) {
        Fail 'No vkconfig version can take the install yet (see above).'
    }

    $answer = Read-Host 'Install into which? Numbers separated by commas, A = all, Enter = cancel'
    $answer = $answer.Trim()
    if ($answer -eq '') {
        Write-Host 'Cancelled, nothing written.'
        return
    }
    if ($answer -ieq 'A') {
        $chosen = $ready
    } else {
        $chosen = @()
        foreach ($part in $answer.Split(',')) {
            $number = 0
            if (-not [int]::TryParse($part.Trim(), [ref]$number) -or $ready -notcontains ($number - 1)) {
                Fail "'$($part.Trim())' is not one of the numbers above. Nothing written."
            }
            $chosen += $number - 1
        }
    }

    foreach ($index in ($chosen | Sort-Object -Unique)) {
        $target = $targets[$index]
        if ($target.Kind -eq '3') { Install-Into3 $LayerDir } else { Install-Into2 $target $LayerDir }
    }
    Write-Host "Done. Open Vulkan Configurator - '$LayerKey' is in the layer list."
}


# removes our layer's section from vk_layer_settings.txt
function Clean-SettingsTxt([string]$path) {
    if (-not (Test-Path -LiteralPath $path)) {
        return
    }
    $lines = [System.IO.File]::ReadAllLines($path)
    $separator = '# =========================================='
    $result = New-Object System.Collections.Generic.List[string]
    $skipping = $false
    for ($i = 0; $i -lt $lines.Count; $i++) {
        $isHeader = ($i + 2 -lt $lines.Count) -and $lines[$i].StartsWith('#') -and
                    ($lines[$i + 1] -eq $separator) -and ($lines[$i + 2] -match '^# (\S+) - ')
        if ($isHeader) {
            $skipping = ($Matches[1] -eq $LayerKey)
        }
        if ($skipping -or $lines[$i].StartsWith("$LayerKey.")) {
            continue
        }
        $result.Add($lines[$i])
    }
    if ($result.Count -eq $lines.Count) {
        return
    }
    Backup $path
    $settingsLeft = @($result | Where-Object { $_.Trim() -ne '' -and -not $_.StartsWith('#') }).Count
    if ($settingsLeft -eq 0) {
        Remove-Item -LiteralPath $path
        Write-Host "Deleted (only VkDebugger settings were in it): $path"
    } else {
        [System.IO.File]::WriteAllLines($path, $result, $Utf8NoBom)
        Write-Host "Removed VkDebugger settings from: $path"
    }
}

# is this folder ours (contains our manifest or is the layer folder)?
function Is-OurFolder([string]$folder, $manifestDirs) {
    if ($folder -eq '') { return $false }
    if (Test-Path -LiteralPath (Join-Path $folder $ManifestName)) { return $true }
    if (@($manifestDirs | Where-Object { Same-Path $_ $folder }).Count -gt 0) { return $true }
    return ($LayerDir -ne '' -and (Same-Path $folder $LayerDir))
}

# removes our manifest from the vkconfig cache in the registry (HKCU), deletes keys left empty
function Clean-QSettingsCache([Microsoft.Win32.RegistryKey]$key, [string]$path) {
    $removed = $false
    foreach ($name in @($key.GetSubKeyNames())) {
        $sub = $key.OpenSubKey($name, $true)
        $subRemovedAndEmpty = Clean-QSettingsCache $sub "$path\$name"
        $sub.Close()
        if ($subRemovedAndEmpty) {
            $key.DeleteSubKey($name)
            $removed = $true
        }
    }
    if ($path -ne $QSettingsKey -and @($key.GetValueNames()) -contains $ManifestName) {
        $key.DeleteValue($ManifestName)
        Write-Host "Removed vkconfig 2.x manifest cache: HKCU\$path\$ManifestName"
        $removed = $true
    }
    return ($removed -and $path -ne $QSettingsKey -and $key.SubKeyCount -eq 0 -and $key.ValueCount -eq 0)
}

function Uninstall {
    Assert-VkconfigClosed
    $manifestDirs = @()

    if (Test-Path -LiteralPath $VkconfigJson) {
        try { $vkconfig = Read-Json $VkconfigJson } catch { Fail "Cannot read $VkconfigJson : $($_.Exception.Message)" }
        $layers = Get-LayersSection $vkconfig
        if ($null -ne $layers) {
            $manifests = @(Remove-LayerDescriptors $layers)
            $manifestDirs = @($manifests | ForEach-Object { Split-Path $_ -Parent })
            $removedPaths = @()
            if (Has-Property $layers 'paths') {
                $kept = @()
                foreach ($folder in @($layers.paths)) {
                    if (Is-OurFolder $folder $manifestDirs) { $removedPaths += $folder } else { $kept += $folder }
                }
                $layers.paths = $kept
            }
            $selectionCleared = @()
            if ((Has-Property $vkconfig.interface 'CONFIGURATIONS') -and
                "$($vkconfig.interface.CONFIGURATIONS.selected_global_configuration)" -eq $ConfigName) {
                $vkconfig.interface.CONFIGURATIONS.selected_global_configuration = ''
                $selectionCleared += 'global configuration'
            }
            if ((Has-Property $vkconfig 'executables') -and (Has-Property $vkconfig.executables 'list')) {
                foreach ($executable in @($vkconfig.executables.list.PSObject.Properties)) {
                    if ("$($executable.Value.configuration)" -eq $ConfigName) {
                        $executable.Value.configuration = ''
                        $selectionCleared += $executable.Name
                    }
                }
            }
            if ($manifests.Count -gt 0 -or $removedPaths.Count -gt 0 -or $selectionCleared.Count -gt 0) {
                foreach ($what in $selectionCleared) { Write-Host "Cleared selection of '$ConfigName' in vkconfig 3.x: $what" }
                Backup $VkconfigJson
                Write-Json $VkconfigJson $vkconfig
                foreach ($folder in $removedPaths) { Write-Host "Removed layer folder from vkconfig 3.x: $folder" }
                foreach ($manifest in $manifests) { Write-Host "Removed layer entry from vkconfig 3.x: $manifest" }
            }

            if ((Has-Property $vkconfig 'executables') -and (Has-Property $vkconfig.executables 'list')) {
                foreach ($executable in @($vkconfig.executables.list.PSObject.Properties)) {
                    foreach ($option in @($executable.Value.options)) {
                        $folder = "$($option.working_folder)"
                        if ($folder -ne '' -and -not $folder.Contains('${')) {
                            Clean-SettingsTxt (Join-Path $folder 'vk_layer_settings.txt')
                        }
                    }
                }
            }
        }
    }

    foreach ($dir in @(Get-ConfigDirs)) {
        foreach ($config in @(Get-Configurations $dir.FullName)) {
            $body = $config.Json.configuration
            if ("$($body.name)" -eq $ConfigName) {
                Backup $config.File.FullName
                Remove-Item -LiteralPath $config.File.FullName
                Write-Host "Deleted configuration '$ConfigName': $($dir.Name)\$($config.File.Name)"
                continue
            }
            $changed = $false
            if (Has-Property $body 'layers') {
                $all = @($body.layers)
                $kept = @($all | Where-Object { $_.name -ne $LayerKey })
                if ($kept.Count -ne $all.Count) {
                    $body.layers = $kept
                    $changed = $true
                }
            }
            if ((Has-Property $body 'selected_layer_name') -and $body.selected_layer_name -eq $LayerKey) {
                $body.selected_layer_name = ''
                $changed = $true
            }
            if (Has-Property $body 'layers_paths') {
                $all = @($body.layers_paths)
                $kept = @($all | Where-Object { -not (Is-OurFolder $_ $manifestDirs) })
                if ($kept.Count -ne $all.Count) {
                    $body.layers_paths = $kept
                    $changed = $true
                }
            }
            if ($changed) {
                Backup $config.File.FullName
                Write-Json $config.File.FullName $config.Json
                Write-Host "Removed VkDebugger from configuration: $($dir.Name)\$($config.File.Name)"
            }
        }
    }

    Clean-SettingsTxt $SettingsTxt

    if (Test-Path -LiteralPath $OverrideJson) {
        try { $override = Read-Json $OverrideJson } catch { $override = $null; Write-Host "Skipped (cannot read): $OverrideJson" }
        if (Has-Property $override 'layer') {
            $layer = $override.layer
            $changed = $false
            foreach ($name in @('component_layers', 'blacklisted_layers')) {
                if (Has-Property $layer $name) {
                    $all = @($layer.$name)
                    $kept = @($all | Where-Object { $_ -ne $LayerKey })
                    if ($kept.Count -ne $all.Count) { $layer.$name = $kept; $changed = $true }
                }
            }
            if (Has-Property $layer 'override_paths') {
                $all = @($layer.override_paths)
                $kept = @($all | Where-Object { -not (Is-OurFolder $_ $manifestDirs) })
                if ($kept.Count -ne $all.Count) { $layer.override_paths = $kept; $changed = $true }
            }
            if ($changed) {
                Backup $OverrideJson
                Write-Json $OverrideJson $override
                Write-Host "Removed VkDebugger from the vkconfig 2.x override layer: $OverrideJson"
            }
        }
    }

    if (Test-Path -LiteralPath $LoaderJson) {
        try { $loader = Read-Json $LoaderJson } catch { $loader = $null; Write-Host "Skipped (cannot read): $LoaderJson" }
        if (Has-Property $loader 'settings_array') {
            $changed = $false
            foreach ($settings in @($loader.settings_array)) {
                if (-not (Has-Property $settings 'layers')) { continue }
                $all = @($settings.layers)
                $kept = @($all | Where-Object { $_.name -ne $LayerKey })
                if ($kept.Count -ne $all.Count) {
                    $settings.layers = $kept
                    $changed = $true
                }
            }
            if ($changed) {
                Backup $LoaderJson
                Write-Json $LoaderJson $loader
                Write-Host "Removed VkDebugger from loader settings: $LoaderJson"
            }
        }
    }

    $root = [Microsoft.Win32.Registry]::CurrentUser.OpenSubKey($QSettingsKey, $true)
    if ($null -ne $root) {
        Clean-QSettingsCache $root $QSettingsKey | Out-Null
        if ("$($root.GetValue('ActiveConfiguration'))" -eq $ConfigName) {
            $root.SetValue('ActiveConfiguration', '')
            Write-Host "Cleared active configuration '$ConfigName' of vkconfig 2.x (HKCU\$QSettingsKey\ActiveConfiguration)"
        }
        $root.Close()
    }

    foreach ($kind in @('ExplicitLayers', 'ImplicitLayers')) {
        $key = "HKCU:\SOFTWARE\Khronos\Vulkan\$kind"
        if (Test-Path $key) {
            foreach ($name in (Get-Item $key).GetValueNames()) {
                if ($name -like "*\$ManifestName") {
                    Remove-ItemProperty -Path $key -Name $name
                    Write-Host "Removed registry value: $key\$name"
                }
            }
        }
        $machineKey = "HKLM:\SOFTWARE\Khronos\Vulkan\$kind"
        if (Test-Path $machineKey) {
            foreach ($name in (Get-Item $machineKey).GetValueNames()) {
                if ($name -like "*\$ManifestName") {
                    Write-Host "NOTE: $machineKey has '$name' - remove it as administrator (not done here)." -ForegroundColor Yellow
                }
            }
        }
    }

    if ($script:BackupDir) {
        Write-Host "Copies of the changed files: $script:BackupDir"
    } else {
        Write-Host 'Nothing of VkDebugger was found in vkconfig.'
    }
    Write-Host 'VkDebugger removed from vkconfig (all versions). The VkDebugger folder itself is not deleted.'
}

if ($Action -eq 'install') { Install } else { Uninstall }
