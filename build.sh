#!/bin/bash

set -e

export PATH=$(pwd)/node_modules/.bin:$PATH
OUTPUT=$(pwd)/build-output

mkdir -p $OUTPUT

for dir in *; do
    if [ -f $dir/action.yml ]; then
        if [ -f $dir/index.js ]; then
            pushd $dir
            ncc build index.js -o $OUTPUT/$dir/dist -m --source-map -C
            cp action.yml $OUTPUT/$dir

            if [ -f ".resources" ]; then
                while read resource; do
                    cp $resource $OUTPUT/$dir/dist
                done <.resources
            fi

            popd
        else
          cp -r $dir $OUTPUT/
        fi
    fi
done
