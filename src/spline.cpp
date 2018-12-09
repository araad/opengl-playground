#include "spline.h"
#include <gtc/type_ptr.hpp>

Spline::Spline(GLuint shaderProgram, const char * mvp_uniform_name)
{
	mShaderProgram = shaderProgram;
	mMvpMatrixId = glGetUniformLocation(mShaderProgram, mvp_uniform_name);

	// Generate vertex buffer objects for position, color and vertex indices
	glGenBuffers(NUM_BUFFERS, mVertexArrayBuffer);

	mDrawMode = GL_POINTS;
}

Spline::~Spline()
{
	glDeleteBuffers(1, &mVertexArrayBuffer[POSITION_VB]);
}

void Spline::Draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	if (!mVertices.empty()) {
		glUseProgram(mShaderProgram);
		glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * mTransform->GetModel();
		glUniformMatrix4fv(mMvpMatrixId, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

		// Enable and set the position attribute buffer to be passed to the shader
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[POSITION_VB]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(mDrawMode, 0, mDrawCount);

		glDisableVertexAttribArray(0);
	}
}

void Spline::AddPoint(float x, float y, float z)
{
	mVertices.push_back(glm::vec3(x, 200, z));

	mDrawCount = mVertices.size();
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, mDrawCount * sizeof(mVertices[0]), &mVertices[0], GL_STATIC_DRAW);
}

void Spline::GenerateSpline(Terrain* terrain)
{
	if (mVertices.size() < 2) return;

	for (int i = 0; i < mVertices.size(); i++)
	{
		mVertices[i].y = 0;
	}

	mVertices.front().y = 5;
	mVertices.insert(mVertices.begin(), mVertices.front());
	mVertices.push_back(mVertices.front());
	mVertices.push_back(mVertices.front());

	mVertices = getInterpolatedPoints(mVertices, 0.5f, 32.0f, terrain, true);
	mVertices.insert(mVertices.begin(), mVertices.back());
	mVertices.push_back(mVertices[0]);
	mVertices.push_back(mVertices[1]);
	mVertices = getInterpolatedPoints(mVertices, 0.5f, 1.0f, terrain, false);

	mDrawCount = mVertices.size();

	glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, mDrawCount * sizeof(mVertices[0]), &mVertices[0], GL_STATIC_DRAW);
}

std::vector<glm::vec3> Spline::getInterpolatedPoints(std::vector<glm::vec3> points, float tension, float minDistance, Terrain* terrain, bool addElevationPoints)
{
	glm::mat4 basisMat = glm::transpose(glm::mat4(-tension, 2 - tension, tension - 2, tension,
		2 * tension, tension - 3, 3 - (2 * tension), -tension,
		-tension, 0, tension, 0,
		0, 1, 0, 0));

	std::vector<glm::vec3> intPts;
	intPts.push_back(points.front());
	mSurfaceNormals.clear();
	mHorizontalTangents.clear();

	for (int i = 1; i < points.size() - 2; i++)
	{
		glm::vec3 p0 = points[i - 1], p1 = points[i], p2 = points[i + 1], p3 = points[i + 2];
		glm::mat3x4 ctrlMat = glm::transpose(glm::mat4x3(p0, p1, p2, p3));
		float distance = sqrtf(powf((p2.x - p1.x), 2.0f) + powf((p2.z - p1.z), 2.0f));

		float inc = 1.0f / (distance / minDistance);

		for (float u = inc; u < 1.0f; u += inc)
		{
			glm::vec4 paramVec = glm::vec4(powf(u, 3), powf(u, 2), u, 1);
			glm::vec3 newVertex = paramVec * basisMat * ctrlMat;

			Terrain::TerrainPoint terrPt;
			//terrPt = terrain->GetTerrainPoint(newVertex.x - 400.0f, newVertex.z + 400.0f);
			terrPt = terrain->GetTerrainPoint(newVertex.x, newVertex.z);
			mSurfaceNormals.push_back(terrPt.normal);

			if (addElevationPoints)
			{
				newVertex.y = terrPt.elevation + 5;
			}

			intPts.push_back(newVertex);

			int size = intPts.size();
			glm::vec3 tangent = intPts[size - 1] - intPts[size - 2];
			mHorizontalTangents.push_back(glm::normalize(tangent));
		}
	}

	return intPts;
}

std::vector<glm::vec3> Spline::GetPathVertices()
{
	return mVertices;
}

std::vector<glm::vec3> Spline::GetPathNormals()
{
	return mSurfaceNormals;
}

std::vector<glm::vec3> Spline::GetPathTangents()
{
	return mHorizontalTangents;
}