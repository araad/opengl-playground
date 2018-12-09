#ifndef SKYBOX_H
#define SKYBOX_H

#include "mesh.h"

class Skybox : public Mesh
{
public:
	Skybox(GLuint shaderProgram);
	~Skybox();

	void Draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) override;
private:
	enum
	{
		POSITION_VB,

		NUM_BUFFERS
	};

	GLuint mVertexArrayBuffer[NUM_BUFFERS];

	GLuint mModelMatrixID;

	GLuint mViewMatrixID;
	GLuint mProjectionMatrixID;

	GLuint mTexture;
	GLuint mTextureID;

	GLfloat* mVertices;
	std::vector<const GLchar*> mFaces;
};

#endif