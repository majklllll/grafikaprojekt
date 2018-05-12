# Add test executable
add_executable(seminar04 main.cpp Application.cpp Application.hpp)

add_dependencies(seminar04 glfw framework)

target_link_libraries(seminar04 ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} framework)
target_include_directories(seminar04
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.frag" COPYONLY)

# Copy images to compile directory
configure_file("${IMAGE_DIR}/Lenna.png" "${CMAKE_CURRENT_BINARY_DIR}/images/Lenna.png" COPYONLY)
configure_file("${IMAGE_DIR}/rocks.jpg" "${CMAKE_CURRENT_BINARY_DIR}/images/rocks.jpg" COPYONLY)
configure_file("${IMAGE_DIR}/wood.jpg" "${CMAKE_CURRENT_BINARY_DIR}/images/wood.jpg" COPYONLY)
configure_file("${IMAGE_DIR}/dice1.png" "${CMAKE_CURRENT_BINARY_DIR}/images/dice1.png" COPYONLY)
configure_file("${IMAGE_DIR}/dice2.png" "${CMAKE_CURRENT_BINARY_DIR}/images/dice2.png" COPYONLY)
configure_file("${IMAGE_DIR}/dice3.png" "${CMAKE_CURRENT_BINARY_DIR}/images/dice3.png" COPYONLY)
configure_file("${IMAGE_DIR}/dice4.png" "${CMAKE_CURRENT_BINARY_DIR}/images/dice4.png" COPYONLY)
configure_file("${IMAGE_DIR}/dice5.png" "${CMAKE_CURRENT_BINARY_DIR}/images/dice5.png" COPYONLY)
configure_file("${IMAGE_DIR}/dice6.png" "${CMAKE_CURRENT_BINARY_DIR}/images/dice6.png" COPYONLY)
configure_file("${IMAGE_DIR}/alpha_circle.png" "${CMAKE_CURRENT_BINARY_DIR}/images/alpha_circle.png" COPYONLY)
