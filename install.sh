#!/bin/sh

# FlyingTokyo19

# Clone Cinder
git clone --recursive https://github.com/simongeilfus/Cinder.git cinder
cd cinder
git checkout libcinder.dylib

# Build Cinder
cd xcode
CURRENT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
CINDER_XCODEPROJ="${CURRENT_DIR}/cinder.xcodeproj"
xcrun xcodebuild -project ${CINDER_XCODEPROJ} -target cinder_dynamic -configuration Release $@
xcrun xcodebuild -project ${CINDER_XCODEPROJ} -target cinder_dynamic -configuration Debug $@
xcrun xcodebuild -project ${CINDER_XCODEPROJ} -target cinder -configuration Release $@
xcrun xcodebuild -project ${CINDER_XCODEPROJ} -target cinder -configuration Debug $@
cd ..

# Clone Blocks
cd blocks
git clone https://github.com/simongeilfus/Cinder-Runtime.git
git clone https://github.com/simongeilfus/Watchdog.git
git clone --recursive https://github.com/simongeilfus/Cinder-ImGui.git
git clone https://github.com/simongeilfus/SimplexNoise.git
git clone --recursive https://github.com/simongeilfus/Cinder-Cereal.git

# Install Cinder-Runtime
cd Cinder-Runtime
sh install.sh