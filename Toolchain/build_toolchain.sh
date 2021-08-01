#!/bin/zsh

DIR="$(pwd)"
BIN_DIR="$DIR/bin"
PATCHES_DIR="$DIR/patches"
SOURCES_DIR="$DIR/sources"

ARCH="x86_64"
TARGET="$ARCH-elf"
PREFIX="$DIR/$ARCH/bin"

GCC_VERSION="11.1.0"
GCC_MD5SUM="333068a65c119e74c9d7bfcc75a8eeba"
GCC_PKG_NAME="gcc-$GCC_VERSION"
GCC_FILENAME="$GCC_PKG_NAME.tar.gz"
GCC_PATCHFILE="$PATCHES_DIR/gcc.diff"
GCC_BASE_URL="https://ftp.gnu.org/gnu/gcc/"

BINUTILS_VERSION="2.36.1"
BINUTILS_MD5SUM="3df9c3bbd944f9b57c1496f06741197b"
BINUTILS_PKG_NAME="binutils-$BINUTILS_VERSION"
BINUTILS_FILENAME="$BINUTILS_PKG_NAME.tar.gz"
BINUTILS_PATCHFILE="$PATCHES_DIR/binutils.diff"
BINUTILS_BASE_URL="https://ftp.gnu.org/gnu/binutils/"

exit_build() {
	echo "$1"
	echo "EXITING..."
	exit 1
}

mkdir -p "$DIR/tarballs"

pushd "$DIR/tarballs"
#==========================================================
# Getting the tarballs from the gnu ftp servers
#==========================================================
md5=""

echo "FETCHING GCC TARBALL..."
if [[ -e $GCC_FILENAME ]]; then
	echo "GCC TARBALL ALREADY EXISTS. SKIPPING THIS STEP..."
else
	curl -sLO "$GCC_BASE_URL/$GCC_PKG_NAME/$GCC_FILENAME"
	echo "DONE."
fi

echo "VERIFYING GCC INTEGRITY..."
md5="$(md5sum $GCC_FILENAME | cut -f1 -d' ')"
if [[ md5 == $GCC_MD5SUM ]]; then
	echo "EXPECTED: $md5"
	echo "GOT: $GCC_MD5SUM"
	exit_build "FILE INTEGRITY OF GCC IS COMPRIMIZED!"
fi
echo "DONE."

echo "FETCHING BINUTILS TARBALL..."
if [[ -e $BINUTILS_FILENAME ]]; then
	echo "BINUTILS TARBALL ALREADY EXISTS. SKIPPING THIS STEP..."
else
	curl -sLO "$BINUTILS_BASE_URL/$BINUTILS_FILENAME"
	echo "DONE."
fi

echo "VERIFYING BINUTILS INTEGRITY..."
md5="$(md5sum $BINUTILS_FILENAME | cut -f1 -d' ')"
if [[ md5 == $BINUTILS_MD5SUM ]]; then
	echo "EXPECTED: $md5"
	echo "GOT: $GCC_MD5SUM"
	exit_build "FILE INTEGRITY OF BINUTILS IS COMPRIMIZED!"
fi
echo "DONE."

#==========================================================
# Extract tarballs
#==========================================================
mkdir -p "$SOURCES_DIR"

echo "EXTRACTING GCC..."
if [[ -e $SOURCES_DIR/$GCC_PKG_NAME ]]; then
	echo "GCC IS ALREADY EXTRACTED. SKIPPING THIS STEP..."
else
	tar xf "$GCC_FILENAME" --cd "$SOURCES_DIR"
	echo "DONE."
fi

echo "EXTRACTING BINUTILS..."
if [[ -e $SOURCES_DIR/$BINUTILS_PKG_NAME ]]; then
	echo "BINUTILS IS ALREADY EXTRACTED. SKIPPING THIS STEP..."
else
	tar xf "$BINUTILS_FILENAME" --cd "$SOURCES_DIR"
	echo "DONE."
fi

popd

#==========================================================
# Patch gcc and binutills
#==========================================================
echo "PATCHING GCC..."
#pushd $SOURCES_DIR/gcc-$GCC_VERSION
#git init . > /dev/null
#git commit -am "init" > /dev/null
#git apply $GCC_PATCHFILE > /dev/null
#popd
echo "DONE."

echo "PATCHING BINUTILS..."
#pushd $SOURCES_DIR/binutils-$BINUTILS_VERSION
#git init . > /dev/null
#git commit -am "init" > /dev/null
#git apply $BINUTILS_PATCHFILE > /dev/null
#popd
echo "DONE."
popd

#==========================================================
# Build gcc and binutils
#==========================================================
echo "BUILDING GCC..."
echo "DONE."

echo "BUILDING BINUTILS..."
echo "DONE."
