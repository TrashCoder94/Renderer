@echo off
pushd %~dp0\..\..\

echo "Generating Projects with Premake"
call ThirdParty\premake\Binaries\premake5.exe vs2019
devenv Foundation.sln /rebuild Debug /project Premake
if %errorlevel% neq 0 exit /b %errorlevel%

popd