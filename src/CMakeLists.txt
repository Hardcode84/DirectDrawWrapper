project(DirectDrawWrapper)
cmake_minimum_required(VERSION 2.8.6)

set(SOURCE
    libmain.cpp
    MyDirectDrawProxy.cpp
    MyDirectDrawSurfaceProxy.cpp
    MyDirectDrawPaletteProxy.cpp
    MyDirectDrawClipperProxy.cpp
    MyDirectDrawSw.cpp
    MyDirectDrawSw1.cpp
    MyDirectDrawSw2.cpp
    MyDirectDrawSw4.cpp
    MyDirectDrawSw7.cpp
    MyDirectDrawPaletteSw.cpp
    MyDirectDrawSurfaceSw.cpp
    MyDirectDrawSurfaceSw1.cpp
    MyDirectDrawSurfaceSw2.cpp
    MyDirectDrawSurfaceSw3.cpp
    MyDirectDrawSurfaceSw4.cpp
    MyDirectDrawSurfaceSw7.cpp
    MyDirectDrawClipperSw.cpp
    MyWinapiFuncs.cpp
    common.cpp
    common_structs.cpp
    serialize_helpers.cpp
    IniFile.cpp
    Settings.cpp
    )

set(HEADERS
    libmain.hpp
    MyDirectDrawProxy.hpp
    MyDirectDrawSurfaceProxy.hpp
    MyDirectDrawPaletteProxy.hpp
    MyDirectDrawClipperProxy.hpp
    MyDirectDrawSw.hpp
    MyDirectDrawPaletteSw.hpp
    MyDirectDrawSurfaceSw.hpp
    MyDirectDrawClipperSw.hpp
    MyWinapiFuncs.hpp
    common.hpp
    common_structs.hpp
    serialize_helpers.hpp
    IniFile.hpp
    Settings.hpp
    )

set(CMAKE_SHARED_LIBRARY_PREFIX "")

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fno-rtti -pedantic -fno-exceptions")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++ -DINITGUID")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Ofast -flto -ffast-math -funroll-loops -mtune=native")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--kill-at")
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/simple_logger")
add_subdirectory("${PROJECT_SOURCE_DIR}/tools")

#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})
include(GenerateExportHeader)
add_compiler_export_flags()
add_library(ddraw SHARED ${SOURCE} ${HEADERS})
generate_export_header(ddraw
        EXPORT_FILE_NAME "${PROJECT_SOURCE_DIR}/ddraw_export.h")

