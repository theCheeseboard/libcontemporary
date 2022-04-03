#!/bin/bash

for dir in *; do
    if [ -f $dir/package.json ]; then
        pushd $dir
        npm ci
        npm run test
        popd
    fi
done
