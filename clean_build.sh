#!/bin/bash
# Bash Script to clean the ./build folder of ErodeTransport* and *.obj files

rm -r ./build/ErosionTransport* || true
rm ./build/*.obj || true
