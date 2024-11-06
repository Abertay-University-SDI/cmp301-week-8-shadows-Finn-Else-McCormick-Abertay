@echo off
cd /d "%~dp0"
echo Downloading libraries...
powershell -Command "$ProgressPreference = 'SilentlyContinue'; Invoke-WebRequest https://github.com/Abertay-University-SDI/CMP301_Libraries/archive/refs/heads/main.zip -OutFile GenLibs.zip"
echo Download complete.
echo Unzipping...
powershell -Command "$ProgressPreference = 'SilentlyContinue'; Expand-Archive GenLibs.zip"
move /Y GenLibs\CMP301_Libraries-main\lib "%~dp0"
move /Y GenLibs\CMP301_Libraries-main\x64 "%~dp0"
del GenLibs.zip
rmdir GenLibs /S /Q