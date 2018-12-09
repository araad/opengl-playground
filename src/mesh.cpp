#include "mesh.h"
#include <iostream>
#include <vector>

Mesh::Mesh()
{
	// Generate and bind to a vertex array object
	glGenVertexArrays(1, &mVertexArrayId);
	glBindVertexArray(mVertexArrayId);

	// Set the drawmode to triangles by default
	mDrawMode = GL_TRIANGLES;

	mTransform = new Transform();
}

Mesh::~Mesh()
{
	// Clean up
	glDeleteProgram(mShaderProgram);
	glDeleteVertexArrays(1, &mVertexArrayId);

	delete mTransform;
}

void Mesh::SetDrawMode(GLenum mode)
{
	mDrawMode = mode;
}

void Mesh::Clear()
{
	mVertices.clear();
}