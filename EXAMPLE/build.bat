call ..\ENV.bat

set TARGET=demo

del .hbmk\win\msvc\*.obj
copy %STATIC_LIB% .
%HB_ROOT%\bin\win\msvc\hbmk2 demo.hbp
move %TARGET%.exe %DIST%
if exist %TARGET%.pdb move %TARGET%.pdb %DIST%
if %errorlevel% neq 0 goto error

csc /out:MyAssembly.dll /target:library my_assembly.cs
move MyAssembly.dll %DIST%

:error
if %errorlevel% neq 0 (
	color %ERR_COLOR%
	echo.
	echo.
	echo Build Failed. Error Occured. 
	echo Press any key to exit
	exit /b %errorlevel%
)