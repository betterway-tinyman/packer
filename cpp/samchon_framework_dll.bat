SET SAMCHON_DIR=D:\OneDrive\Project\Samchon\framework\cpp\

xcopy %SAMCHON_DIR%*.hpp /s /Y
xcopy %SAMCHON_DIR%*.lib /s /Y
xcopy %SAMCHON_DIR%*.dll /s /Y

del samchon\documentation\mainpage.hpp