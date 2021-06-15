cmake_minimum_required( VERSION 3.13 )
project( axiom CXX )

set( CMAKE_CXX_STANDARD 20 )

# TODO: Make a toolchain!
set( CMAKE_CXX_COMPILER gcc )

include_directories( . )

add_subdirectory( Kernel )
