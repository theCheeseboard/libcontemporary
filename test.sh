#!/bin/bash

set -e

export PATH=$(pwd)/node_modules/.bin:$PATH
jest --runInBand