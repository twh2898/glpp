#!/bin/bash

# Exit if any command fails
set -e

git submodule update --init --recursive

make config
