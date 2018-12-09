#include "light.h"
#include <gtc/type_ptr.hpp>
#include <iostream>

Light::Light(GLuint shaderProgram)
{
	mShaderProgram = shaderProgram;

	mLightColor = glm::vec3(1, 1, 1);
	mLightColorID = glGetUniformLocation(mShaderProgram, "LightColor");

	mLightPower = 50000.0f;
	mLightPowerID = glGetUniformLocation(mShaderProgram, "LightPower");

	mLightPosition = glm::vec3(0, 400, 0);
	mLightPositionID = glGetUniformLocation(mShaderProgram, "LightPosition_worldspace");

	mAmbientLight = glm::vec3(0.5f, 0.5f, 0.5f);
	mAmbientLightID = glGetUniformLocation(mShaderProgram, "AmbientLight");

	mLightState = false;
	mLightStateID = glGetUniformLocation(mShaderProgram, "LightState");

	moveUnits = 10;
}

Light::~Light()
{

}

void Light::Apply()
{
	glUniform1i(mLightStateID, mLightState);
	glUniform3fv(mLightColorID, 1, glm::value_ptr(mLightColor));
	glUniform1f(mLightPowerID, mLightPower);
	glUniform3fv(mLightPositionID, 1, glm::value_ptr(mLightPosition));
	glUniform3fv(mAmbientLightID, 1, glm::value_ptr(mAmbientLight));
}

void Light::SetLightState(int state)
{
	mLightState = state;
}

void Light::ToggleState()
{
	mLightState = !mLightState;
}

void Light::MoveLeft()
{
	mLightPosition.x -= moveUnits;
}
void Light::MoveRight()
{
	mLightPosition.x += moveUnits;
}
void Light::MoveUp()
{
	mLightPosition.y -= moveUnits;
	printf("Light: %.2f", mLightPosition.y);
}
void Light::MoveDown()
{
	mLightPosition.y += moveUnits;
	printf("Light: %.2f", mLightPosition.y);
}
void Light::MoveForward()
{
	mLightPosition.z -= moveUnits;
}
void Light::MoveBackward()
{
	mLightPosition.z += moveUnits;
}

void Light::IncreasePower()
{
	mLightPower *= moveUnits;
}
void Light::DecreasePower()
{
	mLightPower /= moveUnits;
}