#include "skybox.h"
//#include "texture.hpp"
#include <SOIL.h>
#include <gtc/type_ptr.hpp>

GLfloat skyboxVertices[] = {
	// Positions          
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

GLuint loadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

Skybox::Skybox(GLuint shaderProgram)
{
	mShaderProgram = shaderProgram;	

	mVertices = skyboxVertices;

	glGenBuffers(NUM_BUFFERS, mVertexArrayBuffer);

	glBindVertexArray(mVertexArrayId);

	glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	mModelMatrixID = glGetUniformLocation(mShaderProgram, "M");
	mViewMatrixID = glGetUniformLocation(mShaderProgram, "V");
	mProjectionMatrixID = glGetUniformLocation(mShaderProgram, "P");

	mFaces.push_back("res/skybox/amh_rt.jpg");
	mFaces.push_back("res/skybox/amh_lf.jpg");
	mFaces.push_back("res/skybox/amh_up.jpg");
	mFaces.push_back("res/skybox/amh_down.jpg");
	mFaces.push_back("res/skybox/amh_bk.jpg");
	mFaces.push_back("res/skybox/amh_ft.jpg");
	//mFaces.push_back("res/skybox/right.jpg");
	//mFaces.push_back("res/skybox/left.jpg");
	//mFaces.push_back("res/skybox/top.jpg");
	//mFaces.push_back("res/skybox/bottom.jpg");
	//mFaces.push_back("res/skybox/back.jpg");
	//mFaces.push_back("res/skybox/front.jpg");
	mTexture = loadCubemap(mFaces);

	mTransform->SetPosition(glm::vec3(0, 0, 0));
	mTransform->SetScale(glm::vec3(2000, 2000, 2000));
}

Skybox::~Skybox()
{

}

void Skybox::Draw(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
	glDepthFunc(GL_LEQUAL);
	glUseProgram(mShaderProgram);

	glUniformMatrix4fv(mModelMatrixID, 1, GL_FALSE, glm::value_ptr(mTransform->GetModel()));
	glUniformMatrix4fv(mViewMatrixID, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(mProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(mVertexArrayId);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer[POSITION_VB]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(mShaderProgram, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}