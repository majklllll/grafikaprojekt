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

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/skybox.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/skybox.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/skybox.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/skybox.frag" COPYONLY)


configure_file("${CMAKE_CURRENT_SOURCE_DIR}/objects/house.obj" "${CMAKE_CURRENT_BINARY_DIR}/objects/house.obj" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/objects/house.mtl" "${CMAKE_CURRENT_BINARY_DIR}/house.mtl" COPYONLY)


configure_file("${CMAKE_CURRENT_SOURCE_DIR}/images/up.png" "${CMAKE_CURRENT_BINARY_DIR}/up.png" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/images/down.png" "${CMAKE_CURRENT_BINARY_DIR}/down.png" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/images/front.png" "${CMAKE_CURRENT_BINARY_DIR}/front.png" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/images/back.png" "${CMAKE_CURRENT_BINARY_DIR}/back.png" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/images/left.png" "${CMAKE_CURRENT_BINARY_DIR}/left.png" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/images/right.png" "${CMAKE_CURRENT_BINARY_DIR}/right.png" COPYONLY)


