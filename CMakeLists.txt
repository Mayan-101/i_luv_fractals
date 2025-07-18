# Add this line BEFORE project() in CMakeLists.txt
set(CMAKE_TOOLCHAIN_FILE "C:/Users/mayan/i_luv_fractals/vcpkg/scripts/buildsystems/vcpkg.cmake")

cmake_minimum_required(VERSION 3.15)
project(i_luv_fractals)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(main ${SOURCES})
target_link_libraries(main PRIVATE glfw glad::glad glm::glm)