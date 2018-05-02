#!/bin/bash
SELF=`echo $0 | sed 's/\\\\/\\//g'`
cd "`dirname "$SELF"`" || exit 1

for f in *; do
    if [ -f $f/.git ]; then
        pushd $f > /dev/null
        echo "`pwd`"
        git checkout master
        git pull --rebase
        popd > /dev/null
    fi
done
