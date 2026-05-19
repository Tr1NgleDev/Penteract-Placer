cmake_minimum_required(VERSION 3.21)
project(pp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FILES 
	"main.cpp"
	"audio.cpp"
	"Chunk.cpp"
	"Directory.cpp"
	"File.cpp"
	"Framebuffer.cpp"
	"GPUBuffer.cpp"
	"Math5D.cpp"
	"QuadRenderer.cpp"
	"QuadRendererBasic.cpp"
	"Shader.cpp"
	"StateManager.cpp"
	"StateControls.cpp"
	"StateCreateWorld.cpp"
	"StateGame.cpp"
	"StateGeneratingWorld.cpp"
	"StateLoadWorld.cpp"
	"StateTitleScreen.cpp"
	"TexRenderer.cpp"
	"TextRenderer.cpp"
	"Texture.cpp"
	"TextureBuffer.cpp"
	"World.cpp"
	"ui.cpp"
)
set(HEADER_FILES 
	"audio.h"
	"Block.h"
	"Chunk.h"
	"Directory.h"
	"File.h"
	"Framebuffer.h"
	"GPUBuffer.h"
	"MashPool.h"
	"Math5D.h"
	"OpenSimplexNoise.hpp"
	"QuadRenderer.h"
	"QuadRendererBasic.h"
	"Shader.h"
	"State.h"
	"StateManager.h"
	"StateControls.h"
	"StateCreateWorld.h"
	"StateGame.h"
	"StateGeneratingWorld.h"
	"StateLoadWorld.h"
	"StateTitleScreen.h"
	"stb_image.h"
	"TexRenderer.h"
	"TextRenderer.h"
	"Texture.h"
	"TextureBuffer.h"
	"World.h"
	"tuple_hash.h"
	"ui.h"
	"utils.h"
	"vec5.h"
	"vec6.h"
)

add_executable(pp ${SRC_FILES} ${HEADER_FILES} "icon.rc")

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
	install(IMPORTED_RUNTIME_ARTIFACTS glfw DESTINATION .)

	install(TARGETS pp RUNTIME DESTINATION .)

	install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)

elseif(LINUX)
	# idk man
endif()
