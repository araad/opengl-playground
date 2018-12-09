#include "display.h"
#include <iostream>

MyDisplay::MyDisplay(int width, int height, const std::string& title)
{
	/// Initialize GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		std::cerr << "ERROR: could not start GLFW3\n" << std::endl;
	}

	mWidth = width;
	mHeight = height;

	/// Create a window of size 640x480 and with title "Lecture 2: First Triangle"
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	window = glfwCreateWindow(mWidth, mHeight, title.c_str(), NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	/// Initialize GLEW extension handler
	glewExperimental = GL_TRUE;	///Needed to get the latest version of OpenGL
	glewInit();

	/// Get the current OpenGL version 
	const GLubyte* renderer = glGetString(GL_RENDERER); /// Get renderer string
	const GLubyte* version = glGetString(GL_VERSION); /// Version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	/// Enable the depth test i.e. draw a pixel if it's closer to the viewer
	glEnable(GL_DEPTH_TEST); /// Enable depth-testing
	glDepthFunc(GL_LESS);	/// The type of testing i.e. a smaller value as "closer"

	glEnable(GL_CULL_FACE);
}

MyDisplay::~MyDisplay()
{
	// Close GL context and any other GLFW resources
	glfwTerminate();
}

void MyDisplay::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MyDisplay::Update()
{
	// put the stuff we've been drawing onto the display
	glfwSwapBuffers(window);

	// update other events like input handling 
	glfwPollEvents();
}

bool MyDisplay::Closed()
{
	return glfwWindowShouldClose(window);
}

int MyDisplay::GetWidth()
{
	return mWidth;
}

int MyDisplay::GetHeight()
{
	return mHeight;
}

void MyDisplay::SetViewportSize(int width, int height)
{
	mWidth = width;
	mHeight = height;
	glViewport(0, 0, width, height);
}

void MyDisplay::ListenKeyEvents(GLFWkeyfun handler)
{
	glfwSetKeyCallback(window, handler);
}

void MyDisplay::ListenMouseButtonEvents(GLFWmousebuttonfun handler)
{
	glfwSetMouseButtonCallback(window, handler);
}

void MyDisplay::ListenCursorPosEvents(GLFWcursorposfun handler)
{
	glfwSetCursorPosCallback(window, handler);
}

void MyDisplay::ListenWindowSizeEvents(GLFWwindowsizefun handler)
{
	glfwSetWindowSizeCallback(window, handler);
}

void MyDisplay::SetCursorPos(float xpos, float ypos)
{
	glfwSetCursorPos(window, xpos, ypos);
}

void MyDisplay::GetCursorPos(double* xpos, double* ypos)
{
	glfwGetCursorPos(window, xpos, ypos);
}