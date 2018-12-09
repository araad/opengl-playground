#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <gtx/transform.hpp>

#define DEG_TO_RAD	M_PI/180.0f

class Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float width, float height, float near, float far)
	{
		mFov = fov;
		mWidth = width;
		mHeight = height;
		mNear = near;
		mFar = far;
		mAspect = mWidth / mHeight;

		// Start with ortho projection
		SetProjectionMode(ORTHOGRAPHIC);

		mPosition = pos;
		mForward = glm::vec3(0.0f, -1.0f, 0.0f);
		mRight = glm::vec3(1.0f, 0.0f, 0.0f);
		mUp = glm::cross(mRight, mForward);

		mHorizontalAngle = M_PI;
		mVerticalAngle = -M_PI / 2.0f;
		moveUnits = 10.0f;
		rotateUnits = 0.5f;

		IsRotating = false;
	}

	enum ProjectionMode
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	inline void SetProjectionMode(ProjectionMode mode)
	{
		if (mode == ORTHOGRAPHIC)
		{
			mProjectionMode = ORTHOGRAPHIC;

			//mProjection = glm::ortho(0.0f, mWidth, 0.0f, mHeight, mNear, mFar);
			mProjection = glm::ortho(-mWidth/2.0f, mWidth/2.0f, -mHeight/2.0f, mHeight/2.0f, mNear, mFar);
			//mPosition.x = 0.0f;
			//mPosition.z = 0.0f;
		}
		else
		{
			mProjectionMode = PERSPECTIVE;

			mProjection = glm::perspective(mFov, mAspect, mNear, mFar);
			//mPosition.x = mWidth / 2.0f;
			//mPosition.z = -mHeight / 2.0f;
		}
	}

	inline glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(mPosition, mPosition + mForward, mUp);
	}

	inline glm::mat4 GetProjectionMatrix()
	{
		return mProjection;
	}

	inline glm::mat4 GetViewProjectionMatrix()
	{
		if (mProjectionMode == ORTHOGRAPHIC)
		{
			//mProjection = glm::ortho(0.0f, mWidth, 0.0f, mHeight, mNear, mFar);
			mProjection = glm::ortho(-mWidth / 2.0f, mWidth / 2.0f, -mHeight / 2.0f, mHeight / 2.0f, mNear, mFar);
		}
		else
		{
			mProjection = glm::perspective(mFov, mAspect, mNear, mFar);
		}
		return mProjection * glm::lookAt(mPosition, mPosition + mForward, mUp);
	}

	inline glm::vec3& GetPosition()
	{
		return mPosition;
	}

	inline void SetOrthoProjection(float width, float height)
	{
		mWidth = width;
		mHeight = height;
		SetProjectionMode(ORTHOGRAPHIC);
	}

	inline void UpdateProjection(float width, float height)
	{
		mWidth = width;
		mHeight = height;
		mAspect = mWidth/mHeight;

		if (mProjectionMode == ORTHOGRAPHIC)
		{
			SetProjectionMode(ORTHOGRAPHIC);
		}
		else
		{
			SetProjectionMode(PERSPECTIVE);
		}
	}

	inline void MoveLeft()
	{
		mPosition -= mRight * moveUnits;
	}
	inline void MoveRight()
	{
		mPosition += mRight * moveUnits;
	}
	inline void MoveUp()
	{
		mPosition += mUp * moveUnits;
	}
	inline void MoveDown()
	{
		mPosition -= mUp * moveUnits;
	}
	inline void MoveForward()
	{
		mPosition += mForward * moveUnits;
	}
	inline void MoveBackward()
	{
		mPosition -= mForward * moveUnits;
	}
	inline void UpdateDirection(float xpos, float ypos)
	{
		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		float speed = 3.0f; // 3 units / second
		float mouseSpeed = 0.005f;

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		// Compute new orientation
		mHorizontalAngle += mouseSpeed * float(mWidth / 2 - xpos);
		mVerticalAngle += mouseSpeed * float(mHeight / 2 - ypos);

		updateAngles();

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;
	}
	inline void ZoomIn()
	{
		if (mProjectionMode == PERSPECTIVE)
		{
			mFov += 0.01f;
			if (mFov >= M_PI) {
				mFov = 0.0f;
			}
		}
	}
	inline void ZoomOut()
	{
		if (mProjectionMode == PERSPECTIVE)
		{
			mFov -= 0.01f;
			if (mFov < 0) {
				mFov = M_PI;
			}
		}
	}

	inline void LookToHorizon()
	{
		mHorizontalAngle = M_PI ;
		mVerticalAngle = -M_PI / 5.0f;

		updateAngles();
	}

	inline void LookToGround()
	{
		mHorizontalAngle = M_PI;
		mVerticalAngle = -M_PI / 2.0f;

		updateAngles();
	}

	inline void SetPosition(glm::vec3 position)
	{
		mPosition = position;
	}

	inline void SetUpDirection(glm::vec3 up)
	{
		mUp = up;

		mRight = glm::cross(mForward, mUp);
	}


	inline void SetLookDirection(glm::vec3 forward, glm::vec3 normal)
	{
		//mVerticalAngle = asin(forward.y);
		//mHorizontalAngle = asin(forward.x / cos(mVerticalAngle));
		////mHorizontalAngle = acos(forward.z / cos(mVerticalAngle));

		//updateAngles();

		mForward = forward;
		mUp = normal;

		mRight = glm::cross(mForward, mUp);
	}
	bool IsRotating;
private:
	glm::mat4 mProjection;
	glm::vec3 mPosition;
	glm::vec3 mForward;
	glm::vec3 mRight;
	glm::vec3 mUp;
	float mWidth;
	float mHeight;
	float mFov;
	float mAspect;
	float mNear;
	float mFar;

	ProjectionMode mProjectionMode;

	float mHorizontalAngle;
	float mVerticalAngle;
	float moveUnits;
	float rotateUnits;

	inline void updateAngles()
	{
		// Direction : Spherical coordinates to Cartesian coordinates conversion
		mForward = glm::vec3(
			cos(mVerticalAngle) * sin(mHorizontalAngle),
			sin(mVerticalAngle),
			cos(mVerticalAngle) * cos(mHorizontalAngle)
			);

		// Right vector
		mRight = glm::vec3(
			sin(mHorizontalAngle - M_PI / 2.0f),
			0,
			cos(mHorizontalAngle - M_PI / 2.0f)
			);

		// Up vector
		mUp = glm::cross(mRight, mForward);
	}
};

#endif