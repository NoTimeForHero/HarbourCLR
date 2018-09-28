cls
color 0F
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
set ERR_COLOR=0C
set WARN_COLOR=0E

set HB_ROOT=C:\MiniGUI\HarbourSRC

set ROOT=%~dp0%~1
set DIST=%ROOT%\DIST

set STATIC_LIB=%DIST%\clr.lib
set DYNAMIC_LIB=%DIST%\clrdyn.dll