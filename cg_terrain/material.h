#pragma once

#include<GLFW/glfw3.h>

//#include<glm/glm.hpp>
//#include<vec2.hpp>
//#include<vec3.hpp>
//#include<vec4.hpp>
//#include<mat4x4.hpp>
//#include<gtc\type_ptr.hpp>

#include"Shader.h"

class Material{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLint diffuseTex;
	GLint specularTex;
	GLint displaceTex;

public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		GLint diffuseTex, GLint specularTex, GLint displaceTex){
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
		this->displaceTex = displaceTex;
	}

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->diffuseTex = 0;
		this->specularTex = 1;
		this->displaceTex = 2;
	}

	~Material(){}

	//Function
	void sendToShader(Shader& program){
		program.setVec3f(this->ambient, "material.ambient");
		program.setVec3f(this->diffuse, "material.diffuse");
		program.setVec3f(this->specular, "material.specular");
		program.set1i(this->diffuseTex, "material.diffuseTex");
		program.set1i(this->specularTex, "material.specularTex");
		program.set1i(this->displaceTex, "material.displaceTex");
	}
};