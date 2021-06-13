cmake_minimum_required( VERSION 3.10 )
project( aeternum C CXX ASM )

set( CMAKE_BUILD_TYPE Debug )

set( CMAKE_CXX_STANDARD 20 )

set( CMAKE_CXX_COMPILER gcc )
set( CMAKE_ASM_COMPILER gcc )

include_directories( . )

add_subdirectory( Kernel )
