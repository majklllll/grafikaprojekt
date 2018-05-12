# Add test executable
add_executable(seminar02 main.cpp Application.cpp Application.hpp)

add_dependencies(seminar02 glfw glad framework)

target_link_libraries(seminar02 ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} framework)
target_include_directories(seminar02
        PRIVATE ${GLFW_INCLUDE_DIR}
        PRIVATE ${GLAD_INCLUDE_DIR}
        PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
        PRIVATE ${FRAMEWORK_INCLUDE_DIR}
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.frag" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/objects/more.obj" "${CMAKE_CURRENT_BINARY_DIR}/objects/more.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/objects/twosq.obj" "${CMAKE_CURRENT_BINARY_DIR}/objects/twosq.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/objects/twosq.mtl" "${CMAKE_CURRENT_BINARY_DIR}/twosq.mtl" COPYONLY)
