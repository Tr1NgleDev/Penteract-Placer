#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ndtf/ndtf.h>
#include "Math5D.h"

#include <string>

#include "StateManager.h"

#include "StateGame.h"

#include "QuadRenderer.h"
#include "Texture.h"
#include "Shader.h"

#ifdef _WIN32
extern "C"
{
	_declspec(dllexport) int NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

StateManager stateManager{};

void handleRawMouseInput(GLFWwindow* window, double xpos, double ypos)
{
	stateManager.mouseInput(xpos, ypos);
}
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	stateManager.scrollInput(xoffset, yoffset);
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	stateManager.mouseButtonInput(button, action, mods);
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	stateManager.keyInput(key, scancode, action, mods);
}
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	stateManager.windowResize(width, height);
}
void charCallback(GLFWwindow* window, unsigned int codepoint)
{
	stateManager.charInput(codepoint);
}
void fileDropCallback(GLFWwindow* window, int path_count, const char* paths[])
{
	stateManager.fileDrop(path_count, paths);
}

#ifndef NDEBUG
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	printf("[GL Debug]: ");

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:				printf("[Error]: "); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	printf("[Deprecated Behaviour]: "); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	printf("[Undefined Behaviour]: "); break;
	case GL_DEBUG_TYPE_PORTABILITY:			printf("[Portability]: "); break;
	case GL_DEBUG_TYPE_PERFORMANCE:			printf("[Performance]: "); break;
	case GL_DEBUG_TYPE_MARKER:				printf("[Marker]: "); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:			printf("[Push Group]: "); break;
	case GL_DEBUG_TYPE_POP_GROUP:			printf("[Pop Group]: "); break;
	case GL_DEBUG_TYPE_OTHER:				printf("[Other]: "); break;
	}

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             printf("[API]: "); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("[Window System]: "); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("[Shader Compiler]: "); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("[Third Party]: "); break;
	case GL_DEBUG_SOURCE_APPLICATION:     printf("[Application]: "); break;
	case GL_DEBUG_SOURCE_OTHER:           printf("[Other]: "); break;
	}
	printf("\n(%i): %s\n", id, message);
}
#endif

int main()
{
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW!\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	GLFWwindow* window = glfwCreateWindow(1280, 720, "5D Miner by Mashpoe and Tr1Ngle", nullptr, nullptr);
	if (!window)
	{
		printf("Failed to initialize the window!\n");
		return 2;
	}

	stateManager.window = window;
	glfwSetWindowUserPointer(window, &stateManager);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, true);

	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, true);
	}
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, true);

	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, handleRawMouseInput);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetDropCallback(window, fileDropCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCharCallback(window, charCallback);

	glfwSwapInterval(1);

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit())
	{
		printf("Failed to initialize GLEW!\n");
		glfwSetWindowShouldClose(window, true);
		return 3;
	}

#ifndef NDEBUG
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QuadRenderer::init();

	stateManager.changeState(&StateGame::instanceObj);

	int wW, wH;
	stateManager.getSize(&wW, &wH);
	stateManager.windowResize(wW, wH);

	double frameTime = 0.0;
	double lastTime = glfwGetTime();
	while (stateManager.isRunning())
	{
		double curTime = glfwGetTime();
		double dt = curTime - lastTime;
		lastTime = curTime;

		frameTime += dt;

		if (frameTime > 0.1)
		{
			frameTime = 0.01;
		}

		while (frameTime >= 0.01)
		{
			glfwPollEvents();
			stateManager.update(0.01);
			frameTime -= 0.01;
		}

		stateManager.render();
	}

	Texture::destroy();
	Shader::destroy();
	QuadRenderer::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
