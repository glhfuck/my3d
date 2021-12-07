project(My3dLib)
file(GLOB SRC_FILES "*.cpp")
#message(STATUS ${SRC_FILES})
add_library(${PROJECT_NAME} ${SRC_FILES})