#pragma once

#include<iostream>
#include<string>

//#include<glew.h>
#include<GLFW/glfw3.h>

#include<SOIL.h>

class Texture{
private:
	GLuint id;
	int width;
	int height;
	unsigned int type;

public:
	Texture(){

	}

	Texture(const char* fileName, GLenum type, int wrapping = GL_REPEAT){
		this->type = type;

		// Load image
		unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

		// All upcoming operations now effect this texture object
		glGenTextures(1, &this->id);
		glBindTexture(type, this->id);

		// Set wrapping to repeat and filtering to linear
		glTexParameteri(type, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, wrapping);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Create texture and mipmaps
		if(image){
			glTexImage2D(type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(type);
		}else{
			std::cout << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
		}

		// Deactivate texture and unbind
		glActiveTexture(0);
		glBindTexture(type, 0);
		SOIL_free_image_data(image);
	}

	Texture(GLenum type, int color, int width, int height, unsigned char* image, int wrapping = GL_REPEAT){
		this->type = type;

		// All upcoming operations now effect this texture object
		glGenTextures(1, &this->id);
		glBindTexture(type, this->id);

		// Set wrapping to repeat and filtering to linear
		glTexParameteri(type, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, wrapping);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(type, 0, color, this->width, this->height, 0, color, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(type);

		// Deactivate texture and unbind
		glActiveTexture(0);
		glBindTexture(type, 0);
	}

	// constructor for empty textures, used for framebuffer textures
	Texture(GLenum type, int color, int width, int height, unsigned int valueType, int wrapping = GL_REPEAT){
		this->type = type;

		// All upcoming operations now effect this texture object
		glGenTextures(1, &this->id);
		glBindTexture(type, this->id);

		// Set wrapping to repeat and filtering to linear
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, wrapping);

		// create empty texture (last parameter 0)
		float pixels[] = {
			0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
		};
		glTexImage2D(type, 0, color, this->width, this->height, 0, color, valueType, pixels);
		glGenerateMipmap(type);

		// Deactivate texture and unbind
		//glActiveTexture(0);
		//glBindTexture(type, 0);
	}

	Texture(GLenum type, GLuint id){
		this->type = type;
		this->id = id;
	}

	~Texture(){
		glDeleteTextures(1, &this->id);
	}

	inline GLuint getID() const{return this->id;}

	glm::vec2 getSize(){
		return glm::vec2(this->width, this->height);
	}

	int getWidth(){
		return this->width;
	}

	int getHeight(){
		return this->height;
	}

	void bind(const GLint texture_unit){
		glActiveTexture(GL_TEXTURE0 + texture_unit);
		glBindTexture(this->type, this->id);
	}

	void unbind(){
		glActiveTexture(0);
		glBindTexture(this->type, 0);
	}

	void loadFromFile(const char* fileName){
		if(this->id){
			glDeleteTextures(1, &this->id);
		}

		unsigned char* image = SOIL_load_image(fileName, &this->width, &this->height, NULL, SOIL_LOAD_RGBA);

		glGenTextures(1, &this->id);
		glBindTexture(this->type, this->id);

		glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if(image){
			glTexImage2D(this->type, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(this->type);
		}else{
			std::cout << "ERROR::TEXTURE::LOADFROMFILE::TEXTURE_LOADING_FAILED: " << fileName << "\n";
		}

		glActiveTexture(0);
		glBindTexture(this->type, 0);
		SOIL_free_image_data(image);
	}
};