call ..\ENV.bat

set TARGET=tests

del .hbmk\win\msvc\*.obj
copy %STATIC_LIB% .
copy %DYNAMIC_LIB% .
%HB_ROOT%\bin\win\msvc\hbmk2 %TARGET%.hbp
rem move %TARGET%.exe %DIST%
rem if exist %TARGET%.pdb move %TARGET%.pdb %DIST%
if %errorlevel% neq 0 goto error

csc /out:%TARGET%.dll /target:library %TARGET%.cs
if %errorlevel% neq 0 goto error
rem move %TARGET%.dll %DIST%

call testall.bat

echo SUCCESSULLY FINISHED

:error
if %errorlevel% neq 0 (
	color %ERR_COLOR%
	echo.
	echo.
	echo Build Failed. Error Occured. 
	echo Press any key to exit
	exit /b %errorlevel%
)