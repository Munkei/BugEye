#!/bin/sh

set -eux

root_dir="$(cd "$(dirname "$0")" && pwd)"
build_dir="build"

rm -frv "${build_dir}"
mkdir -pv "${build_dir}"
cd "${build_dir}"
cmake "${root_dir}"
cmake --build .
ctest --extra-verbose
