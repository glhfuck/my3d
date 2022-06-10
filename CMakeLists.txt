cmake_minimum_required(VERSION 3.20)
project(my3d)

set(CMAKE_CXX_STANDARD 23)

#------------------BOOST------------------
find_package(Boost)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost!")
endif ()
#-----------------------------------------

#------------------SDL2-------------------
find_package(SDL2 REQUIRED)
if (NOT SDL2_FOUND)
    message(FATAL_ERROR "Could not find SDL2!")
endif ()
#-----------------------------------------

include_directories(
        include
        ${Boost_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR}
)

add_subdirectory(libs/My3dLib)
add_executable(${PROJECT_NAME} src/test.cpp)

target_link_libraries(${PROJECT_NAME}
        My3dLib
        ${SDL2_LIBRARIES}
        )



