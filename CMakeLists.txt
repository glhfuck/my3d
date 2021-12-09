cmake_minimum_required(VERSION 3.20)
project(my3d)

set(CMAKE_CXX_STANDARD 23)

#------------------BOOST------------------
set(BOOST_ROOT "/usr/local/boost_1_77_0")
find_package(Boost)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif ()
#-----------------------------------------

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories(
        ${Boost_INCLUDE_DIR}
        ${SFML_INCLUDE_DIR}
        include
)

add_subdirectory(libs/My3dLib)
#add_subdirectory(libs/MyRaster)
add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        My3dLib
        sfml-graphics
        sfml-audio
        )



