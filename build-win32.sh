#!/usr/bin/env bash
set -Eeuo pipefail

mkdir -p binaries/{client,game,r1q2ded,ref_gl}/.depends
make -C binaries win32 -j

