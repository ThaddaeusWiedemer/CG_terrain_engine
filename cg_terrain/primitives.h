#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <GLFW/glfw3.h>
#include "vertex.h"

class Primitive{
private:
	

public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Primitive() {}
	virtual ~Primitive() {}

	//Functions
	void set(const Vertex* vertices, const unsigned nOfVertices, const GLuint* indices, const unsigned nOfIndices){
		for(size_t i = 0; i < nOfVertices; i++){
			this->vertices.push_back(vertices[i]);
		}

		for(size_t i = 0; i < nOfIndices; i++){
			this->indices.push_back(indices[i]);
		}
	}

	void set(const std::vector<Vertex> vertices, const unsigned nOfVertices, const std::vector<GLuint> indices, const unsigned nOfIndices){
		for(size_t i = 0; i < nOfVertices; i++){
			this->vertices.push_back(vertices[i]);
		}

		for(size_t i = 0; i < nOfIndices; i++){
			this->indices.push_back(indices[i]);
		}
	}

	inline Vertex* getVertices(){return this->vertices.data();}
	inline GLuint* getIndices(){return this->indices.data();}
	inline const unsigned getNrOfVertices(){return this->vertices.size();}
	inline const unsigned getNrOfIndices(){return this->indices.size();}
};

class Triangle: public Primitive{
public:
	Triangle() : Primitive(){
		Vertex vertices[] = {
			//Position								//Color								//Texcoords					//Normals
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec4(1.f, 0.f, 0.f, 1.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec4(0.f, 1.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.f),			glm::vec4(0.f, 0.f, 1.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
		};
		unsigned nOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] = {
			0, 1, 2	//Triangle 1
		};
		unsigned nOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nOfVertices, indices, nOfIndices);
	}
};

class Quad: public Primitive{
public:
	Quad() : Primitive(){
		Vertex vertices[] =	{
			//Position								//Color								//Texcoords					//Normals
			glm::vec3(-0.5f, 0.5f, 0.f),			glm::vec4(1.f, 0.f, 0.f, 1.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec4(0.f, 1.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.f),			glm::vec4(0.f, 0.f, 1.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.f),				glm::vec4(1.f, 1.f, 0.f, 1.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f)
		};
		unsigned nOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] = {
			0, 1, 2,	//Triangle 1
			0, 2, 3		//Triangle 2
		};
		unsigned nOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nOfVertices, indices, nOfIndices);
	}

	Quad(glm::vec4 color): Primitive(){
		Vertex vertices[] = {
			//Position								//Color		//Texcoords					//Normals
			glm::vec3(-0.5f, 0.5f, 0.f),			color,		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.f),			color,		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.f),			color,		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.f),				color,		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f)
		};
		unsigned nOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] = {
			0, 1, 2,	//Triangle 1
			0, 2, 3		//Triangle 2
		};
		unsigned nOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nOfVertices, indices, nOfIndices);
	}
};

class Pyramid: public Primitive{
public:
	Pyramid() : Primitive(){
		Vertex vertices[] =	{
			//Position								//Color								//Texcoords					//Normals
			//Triangle front
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec4(1.f, 0.f, 0.f, 1.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec4(0.f, 1.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec4(0.f, 0.f, 1.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),

			//Triangle left
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec4(1.f, 1.f, 0.f, 1.f),		glm::vec2(0.5f, 1.f),		glm::vec3(-1.f, 0.f, 0.f),
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec4(0.f, 0.f, 1.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(-1.f, 0.f, 0.f),
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec4(0.f, 0.f, 1.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(-1.f, 0.f, 0.f),

			//Triangle back
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec4(1.f, 1.f, 0.f, 1.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec4(0.f, 0.f, 1.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec4(0.f, 0.f, 1.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),

			//Triangles right
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec4(1.f, 1.f, 0.f, 1.f),		glm::vec2(0.5f, 1.f),		glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec4(0.f, 0.f, 1.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec4(0.f, 0.f, 1.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(1.f, 0.f, 0.f),
		};
		unsigned nOfVertices = sizeof(vertices) / sizeof(Vertex);

		this->set(vertices, nOfVertices, nullptr, 0);
	}
};

class Cube: public Primitive{
public:
	Cube(): Primitive(){
		Vertex vertices[] =	{
			//Position								//Color								//Texcoords					//Normals
			glm::vec3(-0.5f, 0.5f, 0.5f),			glm::vec4(1.f, 1.f, 1.f, 1.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec4(1.f, 1.f, 1.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec4(1.f, 1.f, 1.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.5f, 0.5f, 0.5f),			glm::vec4(1.f, 1.f, 1.f, 1.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, 1.f),

			glm::vec3(0.5f, 0.5f, -0.5f),			glm::vec4(1.f, 1.f, 1.f, 1.f),		glm::vec2(0.f, 1.f),		glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec4(1.f, 1.f, 1.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec4(1.f, 1.f, 1.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(-0.5f, 0.5f, -0.5f),			glm::vec4(1.f, 1.f, 1.f, 1.f),		glm::vec2(1.f, 1.f),		glm::vec3(0.f, 0.f, -1.f)
		};
		unsigned nOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,

			7, 6, 1,
			7, 1, 0,

			4, 5, 6,
			4, 6, 7,

			3, 2, 5,
			3, 5, 4
		};
		unsigned nOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nOfVertices, indices, nOfIndices);
	}
};

class Sphere: public Primitive{
public:
	Sphere(float radius, unsigned int rings, unsigned int stacks): Primitive(){
		float const ringStep = 2 * M_PI / rings;
		float const stackStep = M_PI / stacks;
		int s, t;
		float stackAngle, ringAngle, x, y, z, xz;

		std::vector<Vertex> vertices;
		for(int i = 0; i <= stacks; ++i){
			stackAngle = M_PI / 2 - i * stackStep;
			xz = radius * cosf(stackAngle);
			y = radius * sinf(stackAngle);

			for(int j = 0; j <= rings; ++j){
				ringAngle = j * ringStep;
				x = xz * cosf(ringAngle);
				z = xz * sinf(ringAngle);

				Vertex vertex;
				vertex.position = glm::vec3(x, y, z);
				vertex.color = glm::vec4(1.f);
				vertex.texcoord = glm::vec2((float)j / rings, (float)i / stacks);
				vertex.normal = glm::vec3(x / radius, y / radius, z / radius);
				vertices.push_back(vertex);
			}
		}

		std::vector<GLuint> indices;
		int k1, k2;
		for(int i = 0; i < stacks; ++i){
			k1 = i * (rings + 1);
			k2 = k1 + rings + 1;
			for(int j = 0; j < rings; ++j, ++k1, ++k2){
				if(i != 0){
					indices.push_back(k1);
					indices.push_back(k1 + 1);
					indices.push_back(k2);
				}
				if(i != stacks - 1){
					indices.push_back(k1 + 1);
					indices.push_back(k2 + 1);
					indices.push_back(k2);
				}
			}
		}

		this->set(vertices, vertices.size(), indices, indices.size());
	}
};

class Test: public Primitive{
public:
	Test(): Primitive(){
		Vertex a = {glm::vec3(0.f, 0.5f, 0.f),				glm::vec4(1.f, 0.f, 0.f, 1.f),		glm::vec2(0.5f, 1.f),		glm::vec3(0.f, 0.f, 1.f)};
		Vertex b = {glm::vec3(-0.5f, -0.5f, 0.f),			glm::vec4(0.f, 1.f, 0.f, 1.f),		glm::vec2(0.f, 0.f),		glm::vec3(0.f, 0.f, 1.f)};
		Vertex c = {glm::vec3(0.5f, -0.5f, 0.f),			glm::vec4(0.f, 0.f, 1.f, 1.f),		glm::vec2(1.f, 0.f),		glm::vec3(0.f, 0.f, 1.f)};
		std::vector<Vertex> vertices = {a, b, c};
		//unsigned nOfVertices = sizeof(vertices) / sizeof(Vertex);

		std::vector<GLuint> indices = {0, 1, 2};
		//unsigned nOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, 3, indices, 3);
	}
};