project(My3dLib)

file(GLOB SRC_FILES "src/*.cpp")

include_directories(include)
add_library(${PROJECT_NAME} ${SRC_FILES})