cmake_minimum_required(VERSION 3.21)
project(pp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SRC_FILES src/*.c src/*.cpp)

add_executable(pp ${SRC_FILES} "icon.rc")

set_target_properties(pp PROPERTIES OUTPUT_NAME "Penteract Placer")

find_package(OpenGL REQUIRED)
target_link_libraries(pp PRIVATE ${OPENGL_LIBRARIES})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(pp PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(pp PRIVATE glm::glm-header-only)
target_compile_definitions(pp PRIVATE GLM_EXPERIMENTAL)

find_package(Boost REQUIRED)
target_link_libraries(pp PRIVATE Boost::headers)

add_subdirectory("libs/ndtf")
target_link_libraries(pp PRIVATE ndtf)

add_subdirectory("libs/soloud/contrib")
target_link_libraries(pp PRIVATE SoLoud)

add_subdirectory("libs/glad")
target_link_libraries(pp PRIVATE glad)

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX)
endif()

if(WIN32)
	set_target_properties(pp PROPERTIES WIN32_EXECUTABLE "$<NOT:$<CONFIG:Debug>>")

	install(IMPORTED_RUNTIME_ARTIFACTS glfw DESTINATION .)

	install(TARGETS pp RUNTIME DESTINATION .)

	install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)

elseif(LINUX)
	# idk man
endif()
