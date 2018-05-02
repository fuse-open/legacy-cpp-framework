#!/bin/bash
set -e
cd "`dirname "$0"`/.."

# TeamCity sets $BUILD_NUMBER, so use that if set
if [ -n "$BUILD_NUMBER" ]; then
    VERSION="$BUILD_NUMBER"
else
    SHA=`git rev-parse HEAD`
    DATE=`date +"%y%m%d"`
    VERSION="$DATE-${SHA:0:6}"
fi

BASE="uno-base-$VERSION"
rm -rf lib

if [ -n "$1" ]; then
    case $1 in
    android)
        make android
        if [ "$OSTYPE" = msys ]; then
            "C:\Program Files\7-Zip\7z.exe" a -y "$BASE-android.zip" include lib
        else
            zip -r "$BASE-android.zip" include lib
        fi
        ;;
    iOS|ios)
        make ios
        zip -r "$BASE-iOS.zip" include lib bin
        ;;
    *)
        echo "Unknown target: $1" >&2
        exit 1
        ;;
    esac
else
    case $OSTYPE in
    darwin*)
        make
        chmod -R +w include lib
        zip -r "$BASE-OSX.zip" include lib
        ;;
    linux*)
        make
        zip -r "$BASE-linux-`uname -m`.zip" include lib
        ;;
    msys*)
        cmd //c build.bat
        "C:\Program Files\7-Zip\7z.exe" a -y "$BASE-win32.zip" include lib
        ;;
    esac
fi
