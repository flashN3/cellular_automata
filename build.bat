@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
taskkill /f /im cellular_automata.exe 2>nul
if not exist build mkdir build
pushd build
cl /nologo /Zi /FC /W4 ^
   /I"../src" ^
   /I"C:/vcpkg/installed/x64-windows/include" ^
   ..\src\main.cpp ^
   ..\src\grid.cpp ^
   /link SDL3.lib ^
   /LIBPATH:"C:/vcpkg/installed/x64-windows/lib" ^
   /SUBSYSTEM:CONSOLE ^
   /out:cellular_automata.exe

if %errorlevel% == 0 (
    echo Build successful

    echo copying SDL3.dll...
    copy "C:\vcpkg\installed\x64-windows\bin\SDL3.dll" . >nul 2>&1

) else (
    echo Build failed with error %errorlevel%
)

popd