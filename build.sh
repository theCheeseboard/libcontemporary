#!/bin/bash

set -e

export PATH=$(pwd)/node_modules/.bin:$PATH
OUTPUT=$(pwd)/build-output

mkdir -p $OUTPUT

for dir in *; do
    if [ -f $dir/action.yml ]; then
        cp -r $dir $OUTPUT/
        if [ -f $dir/index.js ]; then
            pushd $OUTPUT/$dir
            ncc build index.js -o dist -m --source-map -C
            popd
        fi
    fi
done
