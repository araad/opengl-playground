#ifndef MY_DISPLAY_H
#define MY_DISPLAY_H

#include <string>
#include <glew.h>
#include "glfw3.h"  // include GLFW helper library

class MyDisplay
{
public:
	MyDisplay(int width, int height, const std::string& title);

	void Clear(float r, float g, float b, float a);
	void Update();
	bool Closed();

	int GetWidth();
	int GetHeight();

	void SetViewportSize(int width, int height);

	void ListenKeyEvents(GLFWkeyfun handler);
	void ListenMouseButtonEvents(GLFWmousebuttonfun handler);
	void ListenCursorPosEvents(GLFWcursorposfun handler);
	void ListenWindowSizeEvents(GLFWwindowsizefun handler);

	void SetCursorPos(float xpos, float ypos);
	void GetCursorPos(double* xpos, double* ypos);

	virtual ~MyDisplay();
private:
	GLFWwindow* window;

	int mWidth;
	int mHeight;

	void *keyboardEventsHandler;
	void *mouseEventsHandler;
};

#endif