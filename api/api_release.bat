SET RELEASE_DIR=D:\Homepage\betterwaysystems.github.io\packer\api\

:: ----------------------------------------------------------------
::    CLEAR ORDINARY API DOCUMENTS
:: ----------------------------------------------------------------
IF EXIST "%RELEASE_DIR%" rd "%RELEASE_DIR%" /S /Q
mkdir "%RELEASE_DIR%"

:: ----------------------------------------------------------------
::    C++ API DOCUMENTS
:: ----------------------------------------------------------------
::DOCUMENTATE
::doxygen cpp.doxygen

::rename "%RELEASE_DIR%html" cpp

:: ----------------------------------------------------------------
::    TYPE_SCRIPT API DOCUMENT
:: ----------------------------------------------------------------
SET TS_SRC_DIR=../ts/src/
SET TS_DEFINE_DIR=../ts/packages/

::DOCUMENTATE
call typedoc --target ES5 --out "%RELEASE_DIR%ts" "%TS_SRC_DIR%" "%TS_DEFINE_DIR%" --mode file --includeDeclarations

pause