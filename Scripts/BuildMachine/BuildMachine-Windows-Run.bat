@echo off
pushd %~dp0\..\..\

set BinariesDirectory=Binaries\Debug-windows-x86_64\BuildMachineTest\

cd %BinariesDirectory%
echo "DirectX12"
echo "========="
echo "Running Debug Renderer BuildMachineTest executable!"
call BuildMachineTest.exe -API DirectX12
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Vulkan"
echo "======"
echo "Running Debug Renderer BuildMachineTest executable!"
call BuildMachineTest.exe -API Vulkan
if %errorlevel% neq 0 exit /b %errorlevel%

popd