@echo off
pushd %~dp0\..\..\

echo "Generating Projects with Premake"
call ThirdParty\premake\Binaries\premake5.exe vs2022
devenv Renderer.sln /rebuild Debug /project Premake
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building Debug Configuration"
devenv Renderer.sln /rebuild Debug
if %errorlevel% neq 0 exit /b %errorlevel%

popd