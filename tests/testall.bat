cls

%TARGET%.exe

if %errorlevel% neq 0 (
	color %WARN_COLOR%
	echo.
	echo.
	echo Testing Failed.
	echo Press any key to continue
	pause
	exit /b 0
)