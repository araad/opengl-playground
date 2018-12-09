#ifndef VERTEX_H
#define VERTEX_H

#include <glm.hpp>

class Vertex
{
public:
	Vertex() {}

	Vertex(const glm::vec3& pos)
	{
		this->pos = pos;
	}

	float GetX() { return pos.x; }
	float GetY() { return pos.y; }
	float GetZ() { return pos.z; }
private:
	glm::vec3 pos;
};

#endif