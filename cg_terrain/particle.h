#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Particle{
	glm::vec3 velocity;
	glm::vec3 angularVelocity;
	glm::vec4 color;
	GLfloat life;
	Mesh* mesh;

	Particle(): velocity(0.f), angularVelocity(0.f), color(1.f), life(0.f), mesh(new Mesh(new Quad(color))){}
};