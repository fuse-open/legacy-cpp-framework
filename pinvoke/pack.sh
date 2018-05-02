#!/bin/bash
set -e
cd "`dirname "$0"`"

# TeamCity sets $BUILD_NUMBER, so use that if set
if [ -n "$BUILD_NUMBER" ]; then
    VERSION="$BUILD_NUMBER"
else
    SHA=`git rev-parse HEAD`
    DATE=`date +"%y%m%d"`
    VERSION="$DATE-${SHA:0:6}"
fi

rm -rf lib release upload
mkdir release

./build.sh
cp Uno.Native/bin/Release/* release

case $OSTYPE in
darwin*)
    cp lib/*.dylib release
    mono ./stuff.exe pack release \
        --name=Uno.Native \
        --suffix=-$VERSION-OSX \
        --output-dir=upload
    ;;
linux*)
    cp lib/*.so release
    mono ./stuff.exe pack release \
        --name=Uno.Native \
        --suffix=-$VERSION-linux-`uname -m` \
        --output-dir=upload
    ;;
msys*)
    mkdir release/x86 release/x64
    cp lib/x86/Release/*.dll release/x86
    cp lib/x64/Release/*.dll release/x64
    ./stuff.exe pack release \
        --name=Uno.Native \
        --suffix=-$VERSION-win32 \
        --output-dir=upload
    ;;
esac
