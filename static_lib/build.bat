call ..\ENV.bat

rem del .hbmk\win\msvc\*.obj
del clr.lib
%HB_ROOT%\bin\win\msvc\hbmk2 clr.hbp
copy clr.lib %DIST%\

if %errorlevel% neq 0 (
	color %ERR_COLOR%
	echo.
	echo.
	echo Build Failed. Error Occured. 
	echo Press any key to exit
	exit /b %errorlevel%
)