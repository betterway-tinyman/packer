:: ---------------------------------------------------------------------------------
::	Copy to Release
:: ---------------------------------------------------------------------------------
IF EXIST release\cpp\ rd release\cpp\ /S /Q
IF EXIST release\ts\ rd release\ts\ /S /Q
mkdir release\cpp\

xcopy cpp\x64\Release\Packer.exe release\cpp\ /Y
xcopy cpp\x64\Release\SamchonFramework.dll release\cpp\ /Y
xcopy flex\bin-release\* release\ts\ /s /Y

tsc -p ts\tsconfig.json

:: ---------------------------------------------------------------------------------
::	Copy to github.io
:: ---------------------------------------------------------------------------------
SET HOME_DIR=D:\Homepage\betterwaysystems.github.io\packer

IF EXIST %HOME_DIR%\demo\ rd %HOME_DIR%\demo\ /S /Q
xcopy release\demo\* %HOME_DIR%\demo\ /s /Y
xcopy release\ts\* %HOME_DIR%\demo\ /s /Y

:: ---------------------------------------------------------------------------------
::	Git Commit
:: ---------------------------------------------------------------------------------
git add .
git commit -m "%date% %time%"
git push origin master