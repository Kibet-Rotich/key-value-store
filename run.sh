#!/usr/bin/env bash

set -euo pipefail

cmake -S . -B build
cmake --build build --target kv_server
exec ./build/kv_server