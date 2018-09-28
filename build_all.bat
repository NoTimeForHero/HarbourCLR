call ENV.bat

del %DIST%\*.exe
del %DIST%\*.dll
del %DIST%\*.lib

cd %ROOT%\dynamic_lib
call build.bat
if %errorlevel% neq 0 (
	pause
	exit /b %errorlevel%
)

cd %ROOT%\static_lib
call build.bat
if %errorlevel% neq 0 (
	pause
	exit /b %errorlevel%
)

cd %ROOT%\tests
call build.bat
if %errorlevel% neq 0 (
	pause
	exit /b %errorlevel%
)

cd %ROOT%\EXAMPLE
call build.bat