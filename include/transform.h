#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm.hpp>
#include <gtx/transform.hpp>

class Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
		: mPosition(pos), mRotation(rot), mScale(scale) {}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 positionMat = glm::translate(mPosition);

		glm::mat4 rotationXMat = glm::rotate(mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotationYMat = glm::rotate(mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotationZMat = glm::rotate(mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 rotationMat = rotationZMat * rotationYMat * rotationXMat;

		glm::mat4 scaleMat = glm::scale(mScale);

		return positionMat * rotationMat * scaleMat;
	}

	inline glm::vec3& GetPosition() { return mPosition; }
	inline glm::vec3& GetRotation() { return mRotation; }
	inline glm::vec3& GetScale() { return mScale; }

	inline void SetPosition(const glm::vec3& pos) { mPosition = pos; }
	inline void SetRotation(const glm::vec3& rot) { mRotation = rot; }
	inline void SetScale(const glm::vec3& scale) { mScale = scale; }
private:
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;
};

#endif