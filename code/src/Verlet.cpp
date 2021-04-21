#include "Verlet.h"

Verlet::Verlet() {}
Verlet::Verlet(glm::vec3 sphereC, float r, float rebound, float friction, bool useSphereCollision) : Solver(sphereC, r, rebound, friction, useSphereCollision) {}

void Verlet::Update(Mesh& mesh, float dt)
{
	for (int i = 0; i < mesh.maxParticles; i++)
	{
		iPos = mesh.positions[i];
		mesh.positions[i] = mesh.positions[i] + (mesh.positions[i] - mesh.positionsBefore[i]) + mesh.forces[i] * glm::pow(dt, 2.f);

		if (glm::distance(iPos, mesh.positions[i]) > (mesh.LStretch * 0.1))
			mesh.positions[i] = iPos + glm::normalize(mesh.positions[i] - iPos) * (mesh.LStretch * 0.1f);

		mesh.celerities[i] = (mesh.positions[i] - iPos) / dt;

		if (mesh.useCollision)
		{
			//Collision Sphere
			if (CheckCollisionSphere(mesh.positions[i], sphere.c, sphere.r) && useSphereCollision)
			{
				glm::vec3 colPos = GetCollisionPoint(iPos, mesh.positions[i], sphere.c, sphere.r);
				glm::vec3 norm = GetCollisionNorm(colPos, sphere.c);
				float d = GetDFromPlane(colPos, norm);
				iPos = mirror_point(norm.x, norm.y, norm.z, d, iPos.x, iPos.y, iPos.z);
				ReboundPlane(mesh.positions[i], mesh.celerities[i], norm, d);
			}

			//Collision Walls
			for (int p = 0; p < 6; p++)
			{
				if ((glm::dot(box.norms[p], iPos) + box.d[p]) * (glm::dot(box.norms[p], mesh.positions[i]) + box.d[p]) <= 0)
				{
					iPos = mirror_point(box.norms[p].x, box.norms[p].y, box.norms[p].z, box.d[p], iPos.x, iPos.y, iPos.z);
					ReboundPlane(mesh.positions[i], mesh.celerities[i], box.norms[p], box.d[p]);
				}
			}
		}

		mesh.positionsBefore[i] = iPos;
	}
}