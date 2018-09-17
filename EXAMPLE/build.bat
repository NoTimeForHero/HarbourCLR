call ..\ENV.bat

del .hbmk\win\msvc\*.obj
%HB_ROOT%\bin\win\msvc\hbmk2 demo.hbp
move *.exe %DIST%
move *.pdb %DIST%

csc /out:MyAssembly.dll /target:library my_assembly.cs
move MyAssembly.dll %DIST%