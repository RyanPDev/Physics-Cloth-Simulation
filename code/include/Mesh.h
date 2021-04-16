#pragma once
#include "ParticleSystem.h"

class Mesh : public ParticleSystem
{
private:

	int GetIndex(int, int);
	bool isInMesh(int, int);
	glm::vec3 CalculateForce(int, int, int, int, float, float, float);

public:
	Mesh();
	Mesh(int, int, glm::vec3, float);

	int width, height;
	float LStretch, LShear, LBending;
	glm::vec3 initPos;

	float kEStretch, kEShear, kEBend, stretchDamping, shearDamping, bendDamping;

	void GetSpringForces(glm::vec3);
};