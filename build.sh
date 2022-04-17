#!/bin/bash

set -e

export PATH=$(pwd)/node_modules/.bin:$PATH
for dir in *; do
    if [ -f $dir/action.yml ]; then
        if [ -f $dir/index.js ]; then
            pushd $dir
            ncc build index.js -o dist -m --source-map -C
            popd
        fi
    fi
done
