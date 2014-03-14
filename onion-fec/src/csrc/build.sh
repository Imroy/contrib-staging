#!/bin/sh
JAVA_HOME="$(dirname "$(dirname "$(readlink -f "$(which javac)")")")"
DIST_DIR=../../bin/lib/"$(uname -s | tr A-Z a-z)"-"$(uname -m | sed -e 's|armv[0-9].*|arm|')"

set -x

JAVA_HOME="$JAVA_HOME" make
mkdir -p "$DIST_DIR"
cp libfec*.so "$DIST_DIR"
