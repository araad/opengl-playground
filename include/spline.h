#ifndef SPLINE_H
#define SPLINE_H

#include "mesh.h"
#include "terrain.h"

class Spline : public Mesh
{
public:
	Spline(GLuint shaderProgram, const char * mvp_uniform_name);
	~Spline();

	void Draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) override;
	void AddPoint(float x, float y, float z);
	void GenerateSpline(Terrain* terrain);
	std::vector<glm::vec3> GetPathVertices();
	std::vector<glm::vec3> GetPathNormals();
	std::vector<glm::vec3> GetPathTangents();
private:
	enum
	{
		POSITION_VB,

		NUM_BUFFERS
	};

	GLuint mVertexArrayBuffer[NUM_BUFFERS];

	std::vector<glm::vec3> mSurfaceNormals;
	std::vector<glm::vec3> mHorizontalTangents;
	std::vector<glm::vec3> mVerticalTangents;

	std::vector<glm::vec3> getInterpolatedPoints(std::vector<glm::vec3> points, float tension, float minDistance, Terrain* terrain, bool addElevationPoints);
};

#endif