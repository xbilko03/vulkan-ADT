# Installing VkDebugger into Vulkan Configurator (vkconfig)

<!-- AI-Assisted: Claude Opus 5.5: boilerplate, entire file -->

## Files

| File | What it does |
|---|---|
| `install.bat` | Registers VkDebugger in vkconfig. Finds every vkconfig version with data on this PC (2.x, 3.x), asks which ones to install into, and creates a configuration named **VkDebugger** (only the VkDebugger layer, switched on, not selected) whose **App Path** is the exact path of `VkDebugger.exe` in the installed folder, so the layer can start the UI. Running install again sets App Path again. |
| `uninstall.bat` | Removes everything VkDebugger left in vkconfig, for all versions: layer folder, layer entries, the VkDebugger configuration, the layer in other configurations, layer/loader/override settings, the vkconfig 2.x manifest cache and HKCU registry values. Other layers are not touched. Copies of every changed file go to `%TEMP%\vkdbg-uninstall-<time>`. |
| `vkdbg_vkconfig.ps1` | The logic behind both `.bat` files (PowerShell 5.1+). |

## Usage

1. **Close Vulkan Configurator first.** It rewrites its files when it exits; both scripts refuse to run while it is open.
2. Run `install.bat`. With no argument it installs the folder `..\out\build\x64-debug` (next to this folder). Another folder can be given as an argument:
   ```bat
   install.bat "C:\path\to\folder\with\VkDebuggerLayer.dll"
   ```
   The folder must contain `vkDebugger_windows.json`, `VkDebuggerLayer.dll`, `VkDebugger.exe` and `VkDebuggerAgent.exe`.
3. Choose the vkconfig versions (numbers separated by commas, `A` = all, Enter = cancel).
4. Open vkconfig and select the **VkDebugger** configuration.

To remove: close vkconfig, run `uninstall.bat`.

## Moving the VkDebugger folder

vkconfig stores the **absolute path** of the VkDebugger folder. After the folder is moved or renamed, the stored path no longer works.

**Before moving: run `uninstall.bat`. After moving: run `install.bat` from the new location.**

If the folder was already moved, pass the **old** folder to uninstall so it can recognise the old entries:
```bat
uninstall.bat "C:\old\path\x64-debug"
```

## Notes

- **vkconfig 2.x** keeps layer folders separately in every configuration. Configurations created after the install do not get the VkDebugger folder; run `install.bat` again.
- **Use a vkconfig at least as new as the Vulkan SDK.** An older vkconfig (e.g. 2.6.1 from SDK 1.3.290) checks the layers of a newer SDK against its old schema and shows "... is not a valid layer manifest" for them. That message is about the SDK layers, not VkDebugger.
