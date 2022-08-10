@echo off
pushd %~dp0\..\..\

@set MESA_GL_VERSION_OVERRIDE=4.5
@set MESA_GLSL_VERSION_OVERRIDE=410

set BinariesDirectory=Binaries\Debug-windows-x86_64\BuildMachineTest\

cd %BinariesDirectory%
echo "Running Debug Renderer BuildMachineTest executable!"
call BuildMachineTest.exe
if %errorlevel% neq 0 exit /b %errorlevel%

popd