#ifndef MESH_H
#define MESH_H

#include <glm.hpp>
#include <glew.h>
#include <vector>
#include "transform.h"

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	virtual void Draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) = 0;
	void SetDrawMode(GLenum mode);
	void Clear();
protected:
	GLuint mShaderProgram;
	GLuint mMvpMatrixId;

	GLuint mVertexArrayId;
	unsigned int mDrawCount;
	GLenum mDrawMode;

	std::vector<glm::vec3> mVertices;
	Transform* mTransform;
};

#endif