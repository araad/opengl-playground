#ifndef PLATFORM_H
#define PLATFORM_H

#include "cube.h"

class Platform : public Cube
{
public:
	Platform(GLuint shaderProgram, const char * texture_file_path, const char * mvp_uniform_name, const char * texture_uniform_name)
		: Cube(shaderProgram, texture_file_path, mvp_uniform_name, texture_uniform_name)
	{
		GenerateMesh();

		mTransform->SetPosition(glm::vec3(400, 400, -110));
		mTransform->SetScale(glm::vec3(400, 400, 50));
	}
};

#endif