#include "terrain.h"
#include "objloader.hpp"
#include "vboindexer.hpp"
#include "texture.hpp"
#include <gtc/type_ptr.hpp>
#include <algorithm>


bool sortByXPos(glm::vec3 &lhs, glm::vec3 &rhs)
{
	return lhs.x < rhs.x;
}

bool sortByZPos(glm::vec3 &lhs, glm::vec3 &rhs)
{
	return lhs.z < rhs.z;
}

bool sortByYPos(glm::vec3 &lhs, glm::vec3 &rhs)
{
	return lhs.y < rhs.y;
}

Terrain::Terrain(GLuint shaderProgram)
{
	mShaderProgram = shaderProgram;

	mTexture = loadBMP_custom("res/terrain.bmp");
	mTextureID = glGetUniformLocation(mShaderProgram, "TextureSampler");

	mModelMatrixID = glGetUniformLocation(mShaderProgram, "M");

	mViewMatrixID = glGetUniformLocation(mShaderProgram, "V");

	mProjectionMatrixID = glGetUniformLocation(mShaderProgram, "P");

	mScale = glm::vec3(15, 10, 15);

	mTransform->SetPosition(glm::vec3(0, 0, 0));
	mTransform->SetScale(mScale);

	// Generate vertex buffer objects for position, color and vertex indices
	glGenBuffers(NUM_BUFFERS, mVertexArrayBuffer);

	bool res = loadOBJ("res/terrain.obj", mVertices, mUvs, mNormals);

	if (res)
	{
		indexVBO(mVertices, mUvs, mNormals, mIndices, mIndexed_vertices, mIndexed_uvs, mIndexed_normals);

		//for (int i = 0; i < mIndexed_vertices.size(); i++)
		//{
		//	if (mIndexed_vertices[i].y > 0)
		//	{
		//		mElevatedVertices.push_back(mIndexed_vertices[i]);
		//	}
		//}
		//std::sort(mElevatedVertices.begin(), mElevatedVertices.end(), sortByXPos);

		//for (int i = 0; i < mElevatedVertices.size(); i++)
		//{
		//	glm::vec3 vert = mElevatedVertices[i];
		//	float y = GetTerrainPoint(vert.x*10.0f, vert.z*10.0f).elevation/10.0f;

		//	if (abs(vert.y-y) > 0.000001f)
		//	{
		//		int x = 0;
		//	}
		//}

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

Terrain::~Terrain()
{
	glDeleteBuffers(1, &mVertexArrayBuffer[POSITION_VB]);
	glDeleteBuffers(1, &mVertexArrayBuffer[UV_VB]);
	glDeleteBuffers(1, &mVertexArrayBuffer[NORMAL_VB]);
	glDeleteBuffers(1, &mVertexArrayBuffer[ELEMENT_VB]);

	glDeleteTextures(1, &mTexture);
}

void Terrain::Draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
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

Terrain::TerrainPoint Terrain::GetTerrainPoint(float terr_x, float terr_z)
{
	Terrain::TerrainPoint terrPt;
	terrPt.elevation = 0;
	terrPt.normal = glm::vec3(0, 1, 0);// Points upward by default

	terr_x /= mScale.x;
	terr_z /= mScale.z;

	int vertIncr = 6250;
	float adjFactor = 10000.0f;

	int adjDown_x = (int)(terr_x * adjFactor) % vertIncr;
	int adjDown_z = (int)(terr_z * adjFactor) % vertIncr;

	int adjDiff_x = vertIncr - abs(adjDown_x);
	int adjDiff_z = vertIncr - abs(adjDown_z);

	int adjUp_x = adjDown_x > 0 ? adjDiff_x : -adjDiff_x;
	int adjUp_z = adjDown_z > 0 ? adjDiff_z : -adjDiff_z;

	float adjTerr_x = terr_x - (float)(abs(adjDown_x) > vertIncr / 2 ? -adjUp_x : adjDown_x) / adjFactor;
	float adjTerr_z = terr_z - (float)(abs(adjDown_z) > vertIncr / 2 ? -adjUp_z : adjDown_z) / adjFactor;

	for (int i = 0; i < mIndexed_vertices.size(); i++)
	{
		glm::vec3 vert = mIndexed_vertices[i];

			if (abs(vert.x - adjTerr_x) < 0.1f && abs(vert.z - adjTerr_z) < 0.1f)
			{
				terrPt.elevation = vert.y*mScale.y;
				terrPt.normal = mIndexed_normals[i];
				break;
			}
	}

	return terrPt;
}

std::vector<glm::vec3> Terrain::GetVertices()
{
	return mIndexed_vertices;
}