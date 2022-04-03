#!/bin/bash

for dir in *; do
    if [ -f $dir/package.json ]; then
        pushd $dir
        npm run test
        popd
    fi
done
