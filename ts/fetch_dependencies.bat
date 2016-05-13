:: ---------------------------------------------------------
::	FETCH DEPENDENCIES
:: ---------------------------------------------------------
:: DIRECTORIES TO FETCH
SET STD_DIR=D:\OneDrive\Project\Samchon\stl\lib
SET FRAMEWORK_DIR=D:\OneDrive\Project\Samchon\framework\release

:: DEFINITIONS (TYPESCRIPT HEADER FILE)
xcopy %STD_DIR%\*.d.ts src\std\ /Y
xcopy %FRAMEWORK_DIR%\*.ts src\samchon\ /Y

:: INCLUDES (JAVASCRIPT FILE)
xcopy %STD_DIR%\*.js include\ /Y
xcopy %FRAMEWORK_DIR%\*.js include\ /Y