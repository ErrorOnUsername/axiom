#!/bin/zsh

DIR="$(pwd)"
SOURCES_DIR="$DIR/sources"

ARCH="x86_64"
TARGET="$ARCH-pc-elf"
PREFIX="$DIR/local"
BUILD_DIR="$DIR/../build"
SYS_ROOT="$BUILD_DIR/root"

GCC_VERSION="11.2.0"
GCC_MD5SUM="dc6886bd44bb49e2d3d662aed9729278"
GCC_PKG_NAME="gcc-$GCC_VERSION"
GCC_FILENAME="$GCC_PKG_NAME.tar.gz"
GCC_BASE_URL="https://ftp.gnu.org/gnu/gcc/"

BINUTILS_VERSION="2.37"
BINUTILS_MD5SUM="1e55743d73c100b7a0d67ffb32398cdb"
BINUTILS_PKG_NAME="binutils-$BINUTILS_VERSION"
BINUTILS_FILENAME="$BINUTILS_PKG_NAME.tar.gz"
BINUTILS_BASE_URL="https://ftp.gnu.org/gnu/binutils/"

exit_build() {
	echo "$1"
	echo "EXITING..."
	exit 1
}

step_begin() {
	echo "\x1b[1;33m$1\x1b[0m"
}

step_end() {
	echo "\x1b[1;32m$1\x1b[0m"
}

step_err() {
	echo "\x1b[1;31m$1\x1b[0m"
}

mkdir -p "$DIR/tarballs"

pushd "$DIR/tarballs"
#==========================================================
# Getting the tarballs from the gnu ftp servers
#==========================================================
md5=""

#==== GCC ====
step_begin "FETCHING GCC TARBALL..."
if [[ -e $GCC_FILENAME ]]; then
	step_end "GCC TARBALL ALREADY EXISTS. SKIPPING THIS STEP..."
else
	curl -sLO "$GCC_BASE_URL/$GCC_PKG_NAME/$GCC_FILENAME"
	step_end "DONE."
fi

step_begin "VERIFYING GCC INTEGRITY..."
md5="$(md5sum $GCC_FILENAME | cut -f1 -d' ')"
if [[ "$md5" != "$GCC_MD5SUM" ]]; then
	step_err "EXPECTED: $GCC_MD5SUM"
	step_err "GOT:      $md5"
	exit_build "FILE INTEGRITY OF GCC IS COMPRIMIZED!"
fi
step_end "DONE."


#==== BINUTILS ====
step_begin "FETCHING BINUTILS TARBALL..."
if [[ -e $BINUTILS_FILENAME ]]; then
	step_end "BINUTILS TARBALL ALREADY EXISTS. SKIPPING THIS STEP..."
else
	curl -sLO "$BINUTILS_BASE_URL/$BINUTILS_FILENAME"
	step_end "DONE."
fi

step_begin "VERIFYING BINUTILS INTEGRITY..."
md5="$(md5sum $BINUTILS_FILENAME | cut -f1 -d' ')"
if [[ "$md5" != "$BINUTILS_MD5SUM" ]]; then
	step_err "EXPECTED: $BINUTILS_MD5SUM"
	step_err "GOT:      $md5"
	exit_build "FILE INTEGRITY OF BINUTILS IS COMPRIMIZED!"
fi
step_end "DONE."




#==========================================================
# Extract tarballs
#==========================================================
mkdir -p "$SOURCES_DIR"

#==== GCC ====
step_begin "EXTRACTING GCC..."
if [[ -e $SOURCES_DIR/$GCC_PKG_NAME ]]; then
	step_end "GCC IS ALREADY EXTRACTED. SKIPPING THIS STEP..."
else
	tar xf "$GCC_FILENAME" --cd "$SOURCES_DIR"
	step_end "DONE."
fi


#==== BINUTILS ====
step_begin "EXTRACTING BINUTILS..."
if [[ -e $SOURCES_DIR/$BINUTILS_PKG_NAME ]]; then
	step_end "BINUTILS IS ALREADY EXTRACTED. SKIPPING THIS STEP..."
else
	tar xf "$BINUTILS_FILENAME" --cd "$SOURCES_DIR"
	step_end "DONE."
fi

popd




#==========================================================
# Build gcc and binutils
#==========================================================

# Who in their right mind would build this with debug symbols??
export CFLAGS="-g0 -O2 -mtune=native"
export CXXFLAGS="-g0 -O2 -mtune=native"

rm -rf $PREFIX
mkdir -p $PREFIX

PARALLEL_JOBS=$(nproc)

mkdir -p "$DIR/build"
pushd "$DIR/build"
	step_begin "BUILDING BINUTILS..."
	rm -rf binutils
	mkdir binutils
	pushd binutils
		$SOURCES_DIR/$BINUTILS_PKG_NAME/configure \
			--prefix="$PREFIX"                    \
			--target="$TARGET"                    \
			--with-sysroot                        \
			--disable-nls                         \
			--disable-werror || exit 1
		make -j $PARALLEL_JOBS || exit 1
		make install
	popd
	step_end "DONE."

	step_begin "BUILDING GCC..."
	rm -rf gcc
	mkdir gcc
	pushd gcc
		$SOURCES_DIR/$GCC_PKG_NAME/configure \
			--prefix="$PREFIX"               \
			--target="$TARGET"               \
			--enable-languages=c,c++         \
			--disable-nls                    \
			--without-headers || exit 1

		make all-gcc -j $PARALLEL_JOBS || exit 1
		make all-target-libgcc -j $PARALLEL_JOBS || exit 1
		make install-gcc -j $PARALLEL_JOBS || exit 1
		make install-target-libgcc -j $PARALLEL_JOBS || exit 1
	popd
	step_end "DONE."
popd
