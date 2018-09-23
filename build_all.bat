call ENV.bat

cd %ROOT%\CLR_LIB
call build.bat
if %errorlevel% neq 0 exit /b %errorlevel%

cd %ROOT%\EXAMPLE
call build.bat