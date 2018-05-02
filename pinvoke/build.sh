#!/bin/bash
set -e
cd "`dirname "$0"`"

if [ "$OSTYPE" = "msys" ]; then
    cmd //c build.bat
    exit 0
fi

../cmake/cmakemake "$@"
xbuild /property:Configuration=Release Uno.Native.sln
