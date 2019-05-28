#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class Shader{
private:
	GLuint program;
	const int versionMajor;
	const int versionMinor;

	// Retrieves shader source code from file
	std::string loadShaderSource(const GLchar* fileName){
		std::string temp = "";
		std::string src = "";
		std::ifstream in_file;
		// Ensure ifstream object can throw exceptions:
		in_file.exceptions(std::ifstream::badbit);

		// Vertex
		try{
			in_file.open(fileName);
			while(std::getline(in_file, temp)){
				src += temp + "\n";
			}
			in_file.close();
		}catch (std::ifstream::failure e){
			std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << fileName << "\n";
		}

		// Update version number if used in shader
		std::string versionNr =
			std::to_string(this->versionMajor) +
			std::to_string(this->versionMinor) +
			"0";
		src.replace(src.find("#version"), 12, ("#version " + versionNr));
		return src;
	}

	// Compile shaders
	GLuint loadShader(GLenum type, const GLchar* fileName){
		char infoLog[512];
		GLint success;

		GLuint shader = glCreateShader(type);
		std::string str_src = this->loadShaderSource(fileName);
		const GLchar* src = str_src.c_str();
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
			std::cout << infoLog << "\n";
		}
		return shader;
	}

	// Link Shaders
	void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint tessctrlShader, GLuint tessevalShader, GLuint fragmentShader){
		char infoLog[512];
		GLint success;

		this->program = glCreateProgram();

		glAttachShader(this->program, vertexShader);
		if(geometryShader){
			glAttachShader(this->program, geometryShader);
		}
		if(tessctrlShader){
			glAttachShader(this->program, tessctrlShader);
		}
		if(tessevalShader){
			glAttachShader(this->program, tessevalShader);
		}
		glAttachShader(this->program, fragmentShader);

		glLinkProgram(this->program);

		glGetProgramiv(this->program, GL_LINK_STATUS, &success);
		if(!success){
			glGetProgramInfoLog(this->program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
			std::cout << infoLog << "\n";
		}
		this->Use();
	}

public:
	// Constructor generates the shader on the fly
	Shader(const int versionMajor, const int versionMinor, const GLchar* vertexPath, const GLchar* fragmentPath,
		const GLchar* geometryPath = "", const GLchar* tessctrlPath = "", const GLchar* tessevalPath = "") : versionMajor(versionMajor), versionMinor(versionMinor){
		GLuint vertexShader = 0;
		GLuint geometryShader = 0;
		GLuint tessctrlShader = 0;
		GLuint tessevalShader = 0;
		GLuint fragmentShader = 0;

		vertexShader = loadShader(GL_VERTEX_SHADER, vertexPath);
		if(geometryPath != ""){
			geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryPath);
		}
		if(tessctrlPath != ""){
			tessctrlShader = loadShader(GL_TESS_CONTROL_SHADER, tessctrlPath);
		}
		if(tessevalPath != ""){
			tessevalShader = loadShader(GL_TESS_EVALUATION_SHADER, tessevalPath);
		}
		fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentPath);

		this->linkProgram(vertexShader, geometryShader, tessctrlShader, tessevalShader, fragmentShader);

		// Delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(tessctrlShader);
		glDeleteShader(tessevalShader);
		glDeleteShader(fragmentShader);
	}

	~Shader(){
		glDeleteProgram(this->program);
	}

	// Uses the current shader
	void Use(){
		glUseProgram(this->program);
	}

	void Unuse(){
		glUseProgram(0);
	}

	// Accessors
	GLuint getProgram(){
		return this->program;
	}

	// Set uniforms
	void set1i(GLint value, const GLchar* name){
		this->Use();
		glUniform1i(glGetUniformLocation(this->program, name), value);
		this->Unuse();
	}

	void set1f(GLfloat value, const GLchar* name){
		this->Use();
		glUniform1f(glGetUniformLocation(this->program, name), value);
		this->Unuse();
	}

	void setVec2f(glm::fvec2 value, const GLchar* name){
		this->Use();
		glUniform2fv(glGetUniformLocation(this->program, name), 1, glm::value_ptr(value));
		this->Unuse();
	}

	void setVec3f(glm::fvec3 value, const GLchar* name){
		this->Use();
		glUniform3fv(glGetUniformLocation(this->program, name), 1, glm::value_ptr(value));
		this->Unuse();
	}

	void setVec4f(glm::fvec4 value, const GLchar* name){
		this->Use();
		glUniform4fv(glGetUniformLocation(this->program, name), 1, glm::value_ptr(value));
		this->Unuse();
	}

	void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE){
		this->Use();
		glUniformMatrix3fv(glGetUniformLocation(this->program, name), 1, transpose, glm::value_ptr(value));
		this->Unuse();
	}

	void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE){
		this->Use();
		glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, transpose, glm::value_ptr(value));
		this->Unuse();
	}
};
#endif