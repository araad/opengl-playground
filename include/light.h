#ifndef LIGHT_H
#define LIGHT_H

#include <glm.hpp>
#include <glew.h>

class Light
{
public:
	Light(GLuint shaderProgram);
	virtual ~Light();

	void Apply();

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void MoveForward();
	void MoveBackward();

	void IncreasePower();
	void DecreasePower();

	void SetLightState(int state);
	void ToggleState();
private:
	GLuint mShaderProgram;

	glm::vec3 mLightColor;
	GLuint mLightColorID;

	GLfloat mLightPower;
	GLuint mLightPowerID;

	glm::vec3 mAmbientLight;
	GLuint mAmbientLightID;

	glm::vec3 mLightPosition;
	GLuint mLightPositionID;

	int mLightState;
	GLuint mLightStateID;

	int moveUnits;
};

#endif