call ..\ENV.bat

SetLocal EnableExtensions EnableDelayedExpansion
IF ERRORLEVEL 1 echo [ERROR]: Can't enable EnableExtensions/EnableDelayedExpansion

set INCLUDE=%INCLUDE%;%HB_ROOT%\include
set LIB=%LIB%;%HB_ROOT%\lib\win\msvc
set TARGET=clrdyn

for %%i in (*.cpp) do set CPP_FILES=%%i !CPP_FILES!

rem cl.exe /Zi /EHs /MTd demo_lib.cpp runtime.cpp dllmain.cpp /link /DLL /OUT:demo_lib.dll /DEBUG:FULL
cl.exe /EHs /MT %CPP_FILES% /link /DLL /OUT:%TARGET%.dll
del *.ilk
del *.obj
del *.exp

move %TARGET%.dll %DIST%
if exist %TARGET%.pdb move %TARGET%.pdb %DIST%
del *.lib

if %errorlevel% neq 0 (
	color %ERR_COLOR%
	echo.
	echo.
	echo Build Failed. Error Occured. 
	echo Press any key to exit
	pause > nul
	exit /b %errorlevel%
)