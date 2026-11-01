@echo off
:: AI-Assisted: Claude Opus 5.5: boilerplate
:: find Visual Studio (or Build Tools) with the C++ tools and load its x64 build environment
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo vswhere.exe not found, install Visual Studio 2022 or Build Tools with C++ tools
    exit /b 1
)
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSDIR=%%i"
if not defined VSDIR (
    echo no Visual Studio with C++ tools found
    exit /b 1
)
set "PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%"
call "%VSDIR%\VC\Auxiliary\Build\vcvars64.bat"
:: AI-Assisted: end
where cl.exe
where cmake.exe
where ninja.exe
:: AI-Assisted: Claude Opus 5.5: boilerplate
cd /d "%~dp0"
:: AI-Assisted: end
cmake --preset x64-debug
cmake --build out/build/x64-debug
