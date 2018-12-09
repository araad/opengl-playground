#include "cube.h"
#include "objloader.hpp"
#include "vboindexer.hpp"
#include "texture.hpp"
#include <gtc/type_ptr.hpp>

Cube::Cube(GLuint shaderProgram, const char * texture_file_path, const char * mvp_uniform_name, const char * texture_uniform_name)
{
	mShaderProgram = shaderProgram;
	//mMvpMatrixId = glGetUniformLocation(mShaderProgram, mvp_uniform_name);

	mTexture = loadBMP_custom(texture_file_path);
	mTextureID = glGetUniformLocation(mShaderProgram, texture_uniform_name);

	mScale = glm::vec3(10, 10, 10);
	mTransform->SetScale(mScale);
	mModelMatrixID = glGetUniformLocation(mShaderProgram, "M");

	mViewMatrixID = glGetUniformLocation(mShaderProgram, "V");

	mProjectionMatrixID = glGetUniformLocation(mShaderProgram, "P");

	// Generate vertex buffer objects for position, color and vertex indices
	glGenBuffers(NUM_BUFFERS, mVertexArrayBuffer);

	mAngle = 0.0f;
	mCount = 0;
}

Cube::~Cube()
{
	glDeleteBuffers(1, &mVertexArrayBuffer[POSITION_VB]);
	glDeleteBuffers(1, &mVertexArrayBuffer[UV_VB]);
	glDeleteBuffers(1, &mVertexArrayBuffer[NORMAL_VB]);
	glDeleteBuffers(1, &mVertexArrayBuffer[ELEMENT_VB]);

	glDeleteTextures(1, &mTexture);
}

void Cube::Clear()
{
	Mesh::Clear();
	mCount = 0;
}

void Cube::SetViewMatrix(const glm::mat4 &viewMatrix)
{
	mViewMatrix = viewMatrix;
}

void Cube::SetProjectionMatrix(const glm::mat4 &projectionMatrix)
{
	mProjectionMatrix = projectionMatrix;
}

void Cube::Draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	if (!mVertices.empty()) {
		glUseProgram(mShaderProgram);

		glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mTransform->GetModel()));

		glUniformMatrix4fv(mViewMatrixID, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		//glm::mat4 mvpMatrix = viewProjectionMatrix * mTransform->GetModel();
		//glUniformMatrix4fv(mMvpMatrixId, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(mTextureID, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[POSITION_VB]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[UV_VB]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[NORMAL_VB]);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexArrayBuffer[ELEMENT_VB]);

		glDrawElements(mDrawMode, mIndices.size(), GL_UNSIGNED_SHORT, (void*)0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}

void Cube::GenerateMesh()
{
	bool res = loadOBJ("res/sphere.obj", mVertices, mUvs, mNormals);

	if (res)
	{
		indexVBO(mVertices, mUvs, mNormals, mIndices, mIndexed_vertices, mIndexed_uvs, mIndexed_normals);

		mDrawCount = mVertices.size();

		glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, mIndexed_vertices.size() * sizeof(glm::vec3), &mIndexed_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[UV_VB]);
		glBufferData(GL_ARRAY_BUFFER, mIndexed_uvs.size() * sizeof(glm::vec2), &mIndexed_uvs[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, mIndexed_normals.size() * sizeof(glm::vec3), &mIndexed_normals[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexArrayBuffer[ELEMENT_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned short), &mIndices[0], GL_STATIC_DRAW);
	}
}

void Cube::SetPathVertices(std::vector<glm::vec3> vertices)
{
	if (vertices.size() > 0)
	{
		mPathVertices = vertices;
		mPathVertices.pop_back();
		mTransform->SetPosition(mPathVertices[0]);
	}
}

void Cube::SetPathNormals(std::vector<glm::vec3> normals)
{
	if (normals.size() > 0)
	{
		mPathNormals = normals;
		//mTransform->SetPosition(mPathNormals[0]);
	}
}

void Cube::SetPathTangents(std::vector<glm::vec3> tangents)
{
	if (tangents.size() > 0)
	{
		mPathTangents = tangents;
		//mTransform->GetRotation().y = asin(mPathTangents[0].y);
	}
}

void Cube::MoveAlongPath(float speed)
{
	if (mVertices.size() > 0 && mPathVertices.size() > 0)
	{
		if (mCount >= mPathVertices.size()) mCount = 0;
		//glm::vec3 tangent = mPathTangents[mCount];
		//float horizontalAngle = 0.0f;
		//

		////horizontalAngle = tan(mPathTangents[mCount].x / mPathTangents[mCount].z);
		////horizontalAngle = sin(mPathTangents[mCount].x);
		//float verticalAngle = asin(tangent.y);
		//horizontalAngle = asin(tangent.x / cos(verticalAngle));
		//if (tangent.x < 0)
		//{
		//	//horizontalAngle -= glm::radians(90.0f);
		//}
		//printf("Rotate Y: %.5f\t", horizontalAngle*(180.0f / 3.141592f));
		//printf("Rotate X: %.5f\n", verticalAngle*(180.0f / 3.141592f));
		//mTransform->GetRotation().y = horizontalAngle;
		////mTransform->GetRotation().x = verticalAngle;
		mTransform->SetPosition(mPathVertices[mCount++]);
		mTransform->GetPosition().y += mTransform->GetScale().y;
	}
}

void Cube::Rotate()
{
	mAngle += 5.0f;
	if (mAngle >= 360.0f)
	{
		mAngle = 0.0f;
	}

	mTransform->GetRotation().z = glm::radians(mAngle);
}

void Cube::SetPosition(glm::vec3 pos)
{
	mTransform->SetPosition(pos);
}

glm::vec3 Cube::GetPosition()
{
	return mTransform->GetPosition();
}

glm::vec3 Cube::GetHorizontalTangent()
{
	//if (mPathVertices.size() == 0) return glm::vec3(0, 1, 0);

	//if (mCount >= mPathVertices.size()) mCount = 0;
	//return mPathTangents[mCount];

	if (mCount >= mPathTangents.size() - 1) return mPathTangents[0];
	return mPathTangents[mCount + 1];
}

glm::vec3 Cube::GetTangentAt(int index)
{
	return mPathTangents[index];
}

glm::vec3 Cube::GetNormalAt(int index)
{
	return mPathNormals[index];
}

glm::vec3 Cube::GetNormal()
{
	if (mPathVertices.size() == 0) return glm::vec3(0, 1, 0);

	if (mCount >= mPathVertices.size()) mCount = 0;
	return mPathNormals[mCount];
}

glm::vec3 Cube::GetNextPoint()
{
	if (mCount >= mPathVertices.size() - 1)
	{
		mCount = 0;
	}
	return mPathVertices[mCount++];
}

glm::vec3 Cube::GetPointAt(int &index)
{
	if (mPathVertices.size() <= index)
	{
		index = 1;
	}
	return mPathVertices[index];
}