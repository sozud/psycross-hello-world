cmake_minimum_required(VERSION 3.10)

project(cube)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if (WIN32)
  set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
#endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -Wno-narrowing -fpermissive")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -Wno-narrowing -fpermissive")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-return-type")
endif()

find_package(SDL2 REQUIRED)

set(SOURCE_FILES
    cube.c
)

set(SOURCE_FILES_PSYCROSS
    ./PsyCross/src/PsyX_main.cpp
    ./PsyCross/src/util/crash_handler.cpp
    ./PsyCross/src/util/timer.c

    ./PsyCross/src/gpu/PsyX_GPU.cpp
    ./PsyCross/src/gte/half_float.cpp
    ./PsyCross/src/gte/PsyX_GTE.cpp
    ./PsyCross/src/pad/PsyX_pad.cpp
    ./PsyCross/src/render/glad.c
    ./PsyCross/src/render/PsyX_render.cpp

    ./PsyCross/src/psx/ABS.C
    ./PsyCross/src/psx/INLINE_C.C
    ./PsyCross/src/psx/LIBAPI.C
    ./PsyCross/src/psx/LIBCD.C
    ./PsyCross/src/psx/LIBETC.C
    ./PsyCross/src/psx/LIBGPU.C
    ./PsyCross/src/psx/LIBGTE.C
    ./PsyCross/src/psx/LIBMCRD.C
    ./PsyCross/src/psx/LIBPAD.C
    ./PsyCross/src/psx/LIBSN.C
    #./PsyCross/src/psx/LIBSPU.C
)

add_executable(${PROJECT_NAME} 
    ${SOURCE_FILES}
    ${SOURCE_FILES_PSYCROSS}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    #./include
    ./PsyCross/include/psx
    ./PsyCross/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${SDL2_LIBRARIES}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    #__psyz
)
