# Add test executable
add_executable(projekt main.cpp Application.cpp Application.hpp)

add_dependencies(projekt glfw glad framework)

target_link_libraries(projekt ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} framework)
target_include_directories(projekt
        PRIVATE ${GLFW_INCLUDE_DIR}
        PRIVATE ${GLAD_INCLUDE_DIR}
        PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
        PRIVATE ${FRAMEWORK_INCLUDE_DIR}
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.frag" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/objects/projekt.obj" "${CMAKE_CURRENT_BINARY_DIR}/objects/projekt.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/objects/projekt.mtl" "${CMAKE_CURRENT_BINARY_DIR}/projekt.mtl" COPYONLY)
