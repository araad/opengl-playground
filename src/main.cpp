#include "display.h"
#include "camera.h"

#include "spline.h"
#include "cube.h"
#include "platform.h"
#include "light.h"
#include "terrain.h"
#include "skybox.h"

#include <glm.hpp>
#include "shader.hpp"
#include <iostream>

MyDisplay* display;
Camera* camera;

Spline* spline;
Cube* cube;
Platform* platform;
Light* whiteLight;
Terrain* terrain;
Skybox* skybox;

glm::vec3 initialCameraPos = glm::vec3(0.0f, 800.0f, 0.0f);

bool actionCam = false;
bool generated = false;
float speed = 1.0f;

struct State
{
	float x;
	float y;
	float z;
};

State current;

void keyPressed(GLFWwindow *_window, int key, int scancode, int action, int mods) {
	static double lastTime = 0.0f;
	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	if (deltaTime > 0.2f)
	{
		printf("press %d\n", key);

		switch (key)
		{
		// Toggle Draw Mode
		case GLFW_KEY_P:
			spline->SetDrawMode(GL_POINTS);
			break;
		case GLFW_KEY_L:
			spline->SetDrawMode(GL_LINE_STRIP);
			break;
		case GLFW_KEY_S:
			spline->SetDrawMode(GL_LINES);
			break;
		case GLFW_KEY_T:
			spline->SetDrawMode(GL_TRIANGLE_STRIP);
			break;
		case GLFW_KEY_Y:
			spline->SetDrawMode(GL_TRIANGLES);
			break;

		// Control camera
		case GLFW_KEY_KP_MULTIPLY:
			camera->SetProjectionMode(Camera::ORTHOGRAPHIC);
			break;
		case GLFW_KEY_KP_DIVIDE:
			camera->SetProjectionMode(Camera::PERSPECTIVE);
			break;
		case GLFW_KEY_LEFT:
			camera->MoveLeft();
			break;
		case GLFW_KEY_RIGHT:
			camera->MoveRight();
			break;
		case GLFW_KEY_UP:
			camera->MoveForward();
			break;
		case GLFW_KEY_DOWN:
			camera->MoveBackward();
			break;
		case GLFW_KEY_PAGE_UP:
			camera->MoveUp();
			break;
		case GLFW_KEY_PAGE_DOWN:
			camera->MoveDown();
			break;
		case GLFW_KEY_KP_ADD:
			camera->ZoomIn();
			break;
		case GLFW_KEY_KP_SUBTRACT:
			camera->ZoomOut();
			break;
		case GLFW_KEY_A:
			actionCam = !actionCam;
			break;

		// Control Light
		case GLFW_KEY_KP_4:
			whiteLight->MoveLeft();
			break;
		case GLFW_KEY_KP_6:
			whiteLight->MoveRight();
			break;
		case GLFW_KEY_KP_8:
			whiteLight->MoveForward();
			break;
		case GLFW_KEY_KP_2:
			whiteLight->MoveBackward();
			break;
		case GLFW_KEY_KP_0:
			whiteLight->MoveUp();
			break;
		case GLFW_KEY_KP_DECIMAL:
			whiteLight->MoveDown();
			break;
		case GLFW_KEY_KP_7:
			whiteLight->DecreasePower();
			break;
		case GLFW_KEY_KP_9:
			whiteLight->IncreasePower();
			break;
		case GLFW_KEY_KP_5:
			whiteLight->ToggleState();
			break;

		// Actions
		case GLFW_KEY_ENTER:
			if (!generated)
			{
				whiteLight->SetLightState(1);
				spline->GenerateSpline(terrain);
				cube->GenerateMesh();
				cube->SetPathVertices(spline->GetPathVertices());
				cube->SetPathNormals(spline->GetPathNormals());
				cube->SetPathTangents(spline->GetPathTangents());
				//camera->SetPosition(glm::vec3(display->GetWidth() / 2.0f, 300, display->GetHeight() / 4.0f));
				camera->SetPosition(glm::vec3(0, 400, 300));
				camera->LookToGround();
				camera->SetProjectionMode(Camera::PERSPECTIVE);
				current.x = cube->GetPosition().x;
				current.y = cube->GetPosition().y;
				current.z = cube->GetPosition().z;
				//current.position = cube->GetNextPoint();
				//current.t = 0;
				//actionCam = true;
				generated = true;
			}
			break;
		case GLFW_KEY_BACKSPACE:
			spline->Clear();
			cube->Clear();
			camera->GetPosition().x = 0;
			camera->GetPosition().z = 0;
			camera->GetPosition().y = 800;
			camera->SetPosition(initialCameraPos);
			camera->LookToGround();
			camera->SetProjectionMode(Camera::ORTHOGRAPHIC);
			actionCam = false;
			generated = false;
			break;
		case GLFW_KEY_COMMA:
			if (speed >= 20) speed -= 10;
			if (speed < 10) speed--;
			if (speed < 1) speed = 1;
			break;
		case GLFW_KEY_PERIOD:
			if (speed >= 10) speed += 10;
			if (speed < 10) speed++;
			if (speed > 4000) speed = 4000;
			break;

		}

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;
	}

	return;
}

void mouseButtonPressed(GLFWwindow * _window, int button, int action, int mods)
{
	double xpos = -1;
	double ypos = -1;;

	
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		if (action == GLFW_PRESS) {
			glfwGetCursorPos(_window, &xpos, &ypos);
			int win_w = -1, win_h = -1;
			glfwGetWindowSize(_window, &win_w, &win_h);
			float terr_x = (xpos - win_w/2);
			float terr_z = -(win_h - ypos - win_h/2);
			float elev = terrain->GetTerrainPoint(terr_x, terr_z).elevation+1;
			//printf("X: %2f, Y: %2f, Z: %2f\n", terr_x, terr_z, elev);
			spline->AddPoint(terr_x, elev, terr_z);
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_3)
	{
		if (action == GLFW_PRESS) {
			// Reset mouse position for next frame
			display->SetCursorPos(display->GetWidth() / 2, display->GetHeight() / 2);
			camera->IsRotating = true;
		}
		else
		{
			camera->IsRotating = false;
		}
	}
}

void windowResized(GLFWwindow * _window, int width, int height)
{
	display->SetViewportSize(width, height);
	camera->UpdateProjection((float)width, (float)height);
}

void initialize() {
	// Instantiate the display object which will initialize opengl
	display = new MyDisplay(800, 800, "OpenGL Playground");

	//Register the keyboard callback function: keyPressed(...)
	display->ListenKeyEvents(keyPressed);
	display->ListenMouseButtonEvents(mouseButtonPressed);
	display->ListenWindowSizeEvents(windowResized);

	GLuint splineShaderId = LoadShaders("shaders/splineShader.vs", "shaders/splineShader.fss");
	GLuint cubeShaderId = LoadShaders("shaders/cubeShader.vs", "shaders/cubeShader.fss");
	GLuint skyboxShaderId = LoadShaders("shaders/skyboxShader.vs", "shaders/skyboxShader.fss");

	spline = new Spline(splineShaderId, "MVP");
	//cube = new Cube(cubeShaderId, "res/uvmap.DDS", "MVP", "TextureSampler");
	cube = new Cube(cubeShaderId, "res/sphere_tex.bmp", "MVP", "TextureSampler");
	whiteLight = new Light(cubeShaderId);
	terrain = new Terrain(cubeShaderId);
	skybox = new Skybox(skyboxShaderId);

	float width = display->GetWidth();
	float height = display->GetHeight();

	// Create a new camera object which will handle the view and projection matrices
	camera = new Camera(initialCameraPos, 70.f * DEG_TO_RAD, width, height, 0.01f, 5000.0f);

	current.x = 0;
	current.y = 0;
	current.z = 0;
}

void cleanUp() {
	delete spline;
	delete cube;
	delete platform;
	delete terrain;

	delete display;
	delete camera;
}

State interpolate(const State &previous, const State &current, float alpha)
{
	State state;
	state.x = current.x*alpha + previous.x*(1 - alpha);
	state.y = current.y*alpha + previous.y*(1 - alpha);
	state.z = current.z*alpha + previous.z*(1 - alpha);
	return state;
}

int pointCount = 0;
float captureTime = 0;
float nextTime = 0;
float halfTime = 0;
float distance = 0;
float increments = -1;
glm::vec3 nextPt;

void integrate(State &state, double t, double dt)
{	
	glm::vec3 oldPt(state.x, state.y, state.z);
	// Check if nextTime passed and skip to next point
	if (captureTime + nextTime <= t)
	{
		if (increments == 0 || increments == 1)
		{
			// Check current distance and
			// Get incremental points if necessary
			state.x = nextPt.x;
			state.y = nextPt.y;
			state.z = nextPt.z;
		}

		pointCount++;
		nextPt = cube->GetPointAt(pointCount);
		captureTime = t;
		distance = glm::length(nextPt - oldPt);
		nextTime = distance / speed;
		increments = nextTime / dt;
		halfTime = captureTime + nextTime / 2.0f;

		if (increments < 1)
		{
			pointCount += round(1 / increments);
			nextPt = cube->GetPointAt(pointCount);
			state.x = nextPt.x;
			state.y = nextPt.y;
			state.z = nextPt.z;
		}
	}

	if (increments > 1 && halfTime <= t)
	{
		glm::vec3 midPt = nextPt - oldPt;
		increments = 0;
		state.x += midPt.x/2.0f;
		state.y += midPt.y/2.0f;
		state.z += midPt.z/2.0f;
	}
}

void render(State &state)
{
	if (camera->IsRotating)
	{
		double xpos = 0, ypos = 0;
		display->GetCursorPos(&xpos, &ypos);
		display->SetCursorPos(display->GetWidth() / 2, display->GetHeight() / 2);

		camera->UpdateDirection(xpos, ypos);
	}

	skybox->Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	terrain->Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	whiteLight->Apply();

	spline->Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	cube->SetViewMatrix(camera->GetViewMatrix());
	cube->SetProjectionMatrix(camera->GetProjectionMatrix());
	cube->SetPosition(glm::vec3(state.x, state.y, state.z));

	if (actionCam)
	{
		camera->SetPosition(cube->GetPosition());
		camera->GetPosition().y += 5;
		camera->SetLookDirection(cube->GetTangentAt(pointCount), cube->GetNormalAt(pointCount));
		//camera->LookToGround();
	}
	else
	{
		cube->Draw(camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}
}

int main() {
	///Make sure the size of vec3 is the same as 3 floats
	assert(sizeof(glm::vec3) == sizeof(float) * 3);
	assert(sizeof(glm::uvec3) == sizeof(unsigned int) * 3);

	initialize();

	State previous = current;

	float t = 0.0f;
	float dt = 0.1f;

	float currentTime = 0.0f;
	float accumulator = 0.0f;

	while (!display->Closed())
	{
			// Clear the display
			display->Clear(0.0f, 0.0f, 0.0f, 1.0f);

			if (generated)
			{
				const float newTime = glfwGetTime();
				float deltaTime = newTime - currentTime;
				currentTime = newTime;

				if (deltaTime > 0.25f)
					deltaTime = 0.25f;

				accumulator += deltaTime;

				while (accumulator >= dt)
				{
					accumulator -= dt;
					previous = current;
					integrate(current, t, dt);
					t += dt;
				}
			}


			State state = interpolate(previous, current, accumulator / dt);
			printf("Speed: %.2f\n", speed);
			//printf("Velocity:(x:%.2f,y:%.2f,z:%.2f)\tPosition:(x:%.2f,y:%.2f,z:%.2f)\n", state.vx, state.vy, state.vz, state.x, state.y, state.z);

			render(state);

			display->Update();
    }

	cleanUp();
	return 0;
}
