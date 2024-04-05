@echo off
setlocal

echo starting kcounter installation!
:: Change this to the path where your executable is
set EXECUTABLE_PATH=%~dp0kcounter.exe

:: Copy the executable to the /bin directory
copy %EXECUTABLE_PATH% %USERPROFILE%\bin\kcounter.exe

:: Add the /bin directory to the PATH
setx PATH "%PATH%;%USERPROFILE%\bin"

echo Your program has been installed successfully!