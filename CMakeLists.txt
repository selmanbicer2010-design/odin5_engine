cmake_minimum_required(VERSION 3.15)
project(odin5
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(odin
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/graphics/vulkangraphicsapi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/window/glfwwindowapi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/file.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/math/spatial.cpp
)

target_include_directories(odin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(odin PRIVATE
    glm::glm
    glfw
    Vulkan::Vulkan
)
