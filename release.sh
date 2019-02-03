#!/usr/bin/env bash

if [[ "$CIRRUS_RELEASE" == "" ]]; then
  exit 0
fi

if [[ "$BINTRAY_TOKEN" == "" ]]; then
  exit 1
fi

files_to_upload=(
  build/translate
  build_mingw/translate.exe
)

for fpath in ${files_to_upload[@]}
do
  echo "Uploading $fpath..."
  name=$(basename "$fpath")
  url_to_upload="https://api.bintray.com/content/neverwin/test/translate/$CIRRUS_RELEASE/$name?publish=1?override=1"
  curl -T $fpath -uneverwin:$BINTRAY_TOKEN $url_to_upload
done
