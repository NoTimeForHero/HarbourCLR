call ..\ENV.bat

set INCLUDE=%INCLUDE%;%HB_ROOT%\include
set LIB=%LIB%;%HB_ROOT%\lib\win\msvc
rem cl.exe /Zi /EHs /MTd demo_lib.cpp runtime.cpp dllmain.cpp /link /DLL /OUT:demo_lib.dll /DEBUG:FULL
cl.exe /EHs /MT demo_lib.cpp runtime.cpp dllmain.cpp /link /DLL /OUT:demo_lib.dll
del *.ilk
del *.obj
del *.exp
move *.dll %DIST%
move *.pdb %DIST%