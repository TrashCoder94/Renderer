#!/bin/sh

set -e

cd Renderer/ThirdParty/GLFW
sudo apt-get build-dep glfw
cd ../../../

echo "Generating Project with Premake!"
chmod +x ThirdParty/premake/Binaries/premake5
ThirdParty/premake/Binaries/premake5 --file=premake5.lua gmake2

make config=debug
make config=release
make config=distribution