#ifndef TERRAIN_H
#define TERRAIN_H

#include "mesh.h"
#include "CImg.h"

using namespace cimg_library;

class Terrain : public Mesh
{
public:
	struct TerrainPoint
	{
		float elevation;
		glm::vec3 normal;
	};

	Terrain(GLuint shaderProgram);
	~Terrain();
	TerrainPoint GetTerrainPoint(float x, float z);
	std::vector<glm::vec3> GetVertices();

	void Draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) override;
private:
	enum
	{
		POSITION_VB,
		UV_VB,
		NORMAL_VB,
		ELEMENT_VB,

		NUM_BUFFERS
	};

	GLuint mVertexArrayBuffer[NUM_BUFFERS];

	std::vector<glm::vec2> mUvs;
	std::vector<glm::vec3> mNormals;

	GLuint mModelMatrixID;

	glm::mat4 mViewMatrix;
	GLuint mViewMatrixID;

	glm::mat4 mProjectionMatrix;
	GLuint mProjectionMatrixID;

	GLuint mTexture;
	GLuint mTextureID;

	std::vector<unsigned short> mIndices;
	std::vector<glm::vec3> mIndexed_vertices;
	std::vector<glm::vec2> mIndexed_uvs;
	std::vector<glm::vec3> mIndexed_normals;

	std::vector<unsigned short> mElevatedVertexIndices;
	std::vector<glm::vec3> mElevatedVertices;

	glm::vec3 mScale;
};

#endif