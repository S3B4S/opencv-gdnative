message("LETS GO!")

cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# set the project name
project(TouchThaBomb  VERSION 0.1
                      DESCRIPTION "Defuse bomb game with hand tracking"
                      LANGUAGES CXX)


# https://cmake.org/cmake/help/latest/command/find_package.html#search-procedure 
find_package(OpenCV 4.3 EXACT REQUIRED PATHS /OpenCV/opencv-4.3.0-vc16/)
include_directories(AFTER "${OpenCV_INCLUDE_DIRS}" "include")
link_libraries(${OpenCV_LIBS})

# add the executable
#file(GLOB source_list "src/*.h" "src/*.cpp")
add_library(bomb SHARED "src/gdexample.cpp" "src/gdlibrary.cpp")