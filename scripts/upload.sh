#!/bin/sh
set -e

if [ $# != 2 ]; then
    echo "USAGE $0 <token> <upload url>"
    exit 1
fi

TOKEN=$1
UPLOAD_URL=$2

WIN32=uno-base-*-win32.zip
OSX=uno-base-*-OSX.zip
ANDROID=uno-base-*-android.zip
IOS=uno-base-*-iOS.zip

STUFF_FILE=uno-base.stuff
LOG_FILE=upload-stuff.log

TEMPLATE="if ANDROID {
    \"Android\": \"ANDROID_URL\",
}
if IOS {
    \"iOS\": \"IOS_URL\"
}
if OSX {
    \"OSX\": \"OSX_URL\"
}
if WIN32 {
    \"Win32\": \"WIN32_URL\"
}"

rm -f $STUFF_FILE $LOG_FILE

upload() {
    set -e
    echo "Uploading $1" >> $LOG_FILE
    UPLOAD_CMD="curl -H 'X-API-Token: $TOKEN' -F filedata=@$1 $UPLOAD_URL"
    echo $UPLOAD_CMD >> $LOG_FILE
    FILE_URLS=`eval $UPLOAD_CMD`
    echo $FILE_URLS >> $LOG_FILE
    echo $FILE_URLS | grep -q '"url"'
    echo $FILE_URLS| awk -F\" '{print $14}'
}

WIN32_URL=`upload $WIN32`
OSX_URL=`upload $OSX`
ANDROID_URL=`upload $ANDROID`
IOS_URL=`upload $IOS`

echo "WIN32_URL:"$WIN32_URL
echo "OSX_URL:"$OSX_URL
echo "ANDROID_URL:"$ANDROID_URL
echo "IOS_URL:"$IOS_URL

echo "$TEMPLATE" | sed s#WIN32_URL#"$WIN32_URL"# | sed s#OSX_URL#"$OSX_URL"# | sed s#ANDROID_URL#"$ANDROID_URL"# | sed s#IOS_URL#"$IOS_URL"# > $STUFF_FILE
