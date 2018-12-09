#ifndef CUBE_H
#define CUBE_H

#include "mesh.h"

class Cube : public Mesh
{
public:
	Cube(GLuint shaderProgram, const char * texture_file_path, const char * mvp_uniform_name, const char * texture_uniform_name);
	~Cube();

	void SetViewMatrix(const glm::mat4 &viewMatrix);
	void SetProjectionMatrix(const glm::mat4 &projectionMatrix);
	void Draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) override;
	void SetPathVertices(std::vector<glm::vec3> vertices);
	void SetPathNormals(std::vector<glm::vec3> normals);
	void SetPathTangents(std::vector<glm::vec3> tangents);
	void GenerateMesh();
	void MoveAlongPath(float speed);
	void Rotate();
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition();
	glm::vec3 GetHorizontalTangent();
	glm::vec3 GetTangentAt(int index);
	glm::vec3 GetNormal();
	glm::vec3 GetNextPoint();
	glm::vec3 GetPointAt(int &index);
	glm::vec3 GetNormalAt(int index);
	void Clear();
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

	std::vector<glm::vec3> mPathVertices;
	std::vector<glm::vec3> mPathNormals;
	std::vector<glm::vec3> mPathTangents;

	float mAngle;
	int mCount;

	glm::vec3 mScale;
};

#endif