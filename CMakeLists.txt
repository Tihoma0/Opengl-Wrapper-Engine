cmake_minimum_required(VERSION 3.31)
project(GLWrapper_II)

set(CMAKE_CXX_STANDARD 20)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_SHARED_LIBS OFF)

# SHUT THE FUCKING CLANG!
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
            -Wno-gnu-anonymous-struct
            -Wno-nested-anon-types
    )
endif()

include(FetchContent)

FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG VER-2-13-3
)

FetchContent_MakeAvailable(freetype)

# -------------------------------
# GLFW
# -------------------------------
add_subdirectory(external/glfw)

# -------------------------------
# GLAD
# -------------------------------
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
        src/rendercore/*.cpp
)

add_library(engine ${ENGINE_SRC}
        "src/ util/Path.cpp"
        "src/ util/Path.h"
)

add_executable(GLWrapper_II main.cpp)


target_include_directories(GLWrapper_II PUBLIC
        src
        external/glfw/include
        external/glad/include
        external/stb_image
        external/stb-master
        external/glm
)

target_link_libraries(engine PRIVATE freetype)

target_link_libraries(engine PUBLIC glad)

target_include_directories(engine PUBLIC
        src
        external/glfw/include
        external/glad/include
        external/stb_image
        external/stb-master
        external/glm
)


target_link_libraries(GLWrapper_II PRIVATE freetype)

target_link_libraries(GLWrapper_II
        PRIVATE
        engine
        glfw
        glad
        opengl32
        ${CMAKE_DL_LIBS}
)

if(MSVC)
    target_compile_options(engine PRIVATE /W4)
else()
    target_compile_options(engine PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
    )
endif()

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

    target_link_libraries(GLWrapper_II PRIVATE mingwex ucrt winpthread)
endif()
