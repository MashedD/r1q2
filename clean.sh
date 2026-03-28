#!/usr/bin/env bash
set -Eeuo pipefail

cd binaries
make distclean
rm -rf client/*.o client/.depends/* \
       game/*.o game/.depends/* \
       r1q2ded/*.o r1q2ded/.depends/* \
       ref_gl/*.o ref_gl/.depends/* \
       client/r1q2 \
       client/r1q2.exe \
       client/r1q232.exe \
       game/gamex86.dll \
       game/gamex86_64.dll \
       game/gamex86_64.so \
       r1q2ded/r1q2ded \
       r1q2ded/r1q2ded.exe \
       r1q2ded/r1q2ded32.exe \
       ref_gl/ref_gl.dll \
       ref_gl/ref_gl.so

