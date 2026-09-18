# Invoking the CMake build from the command line is a two step process, first, *generate* a build by running the following:
#
#   cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=Release
# 
# Where `Release` can be replaced with `Debug`, which contains gdb and address sanitiser definitions.
# Once a build is created, proceed with *compilation*:
# 
#   cmake --build build --target RenderProject --config Release
#
# This creates the executable in the build directory. You only need to *regenerate* a build if you modify this CMakeList.txt file.
# For any other changes to the source code, simply recompile.


cmake_minimum_required(VERSION 3.12)
project(RenderProject)

set(CMAKE_CXX_STANDARD 11)

# Note, we do this for glm because it's a header-only library and because we shipped it with the project
# normally you would use find_package(<package_name>) for libraries with actual objects
set(GLM_INCLUDE_DIRS libs/glm-0.9.7.2)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
include_directories(libs/sdw)

file(GLOB sdwFiles libs/sdw/*.cpp)
file(GLOB studentFiles src/*.cpp)
add_executable(RenderProject
        ${sdwFiles}
        ${studentFiles})

if (MSVC)
    target_compile_options(RenderProject
            PUBLIC
            /W3
            /Zc:wchar_t
            )
    set(DEBUG_OPTIONS /MTd)
    set(RELEASE_OPTIONS /MT /GF /Gy /O2 /fp:fast)
    if (NOT DEFINED SDL2_LIBRARIES)
        set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)
    endif()
else ()
    target_compile_options(RenderProject
        PUBLIC
        -Wall
        -Wextra
        -Wcast-align
        -Wfatal-errors
        -Werror=return-type
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-ignored-attributes)

    set(DEBUG_OPTIONS -O2 -fno-omit-frame-pointer -g)
    set(RELEASE_OPTIONS -O3 -march=native -mtune=native)
    # If the Address Sanitizer library is not available on your platform, you may need to remove ",-lasan" from the below 
    target_link_libraries(RenderProject PUBLIC $<$<CONFIG:Debug>:-Wl,-lasan>)

endif()


target_compile_options(RenderProject PUBLIC "$<$<CONFIG:RelWithDebInfo>:${RELEASE_OPTIONS}>")
target_compile_options(RenderProject PUBLIC "$<$<CONFIG:Release>:${RELEASE_OPTIONS}>")
target_compile_options(RenderProject PUBLIC "$<$<CONFIG:Debug>:${DEBUG_OPTIONS}>")
 
target_link_libraries(RenderProject PRIVATE ${SDL2_LIBRARIES})
