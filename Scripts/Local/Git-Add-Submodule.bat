@echo off
pushd %~dp0\..\..\

set /P repo=What git repo do you want to add to Renderer/ThirdParty (please reply with .git link not just the normal url)?
set /P folderName=What do you want to call the parent folder?

echo Adding %repo% to Renderer/ThirdParty/%folderName%/
git submodule add %repo% Renderer/ThirdParty/%folderName%/

popd
PAUSE