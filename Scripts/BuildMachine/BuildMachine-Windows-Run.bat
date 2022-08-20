@echo off
pushd %~dp0\..\..\

set BinariesDirectory=Binaries\Debug-windows-x86_64\BuildMachineTest\

cd %BinariesDirectory%
echo "Running Debug Renderer BuildMachineTest executable!"
call BuildMachineTest.exe
if %errorlevel% neq 0 exit /b %errorlevel%

popd