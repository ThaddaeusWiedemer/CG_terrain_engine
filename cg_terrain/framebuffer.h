#pragma once
#include <iostream>
#include <string>
#include <vector>

class Framebuffer{
private:
	unsigned int FBO;                   //framebuffer object
	GLuint texture_id;
	GLuint depthTexture_id;
	Texture* texture;
	Texture* depthTexture;
	//std::vector<GLenum> drawbuffer;     //add texture attachements

		// delete objects
	void cleanUp(){
		glDeleteFramebuffers(1, &FBO);
		//glDeleteTextures(1, &texture_);
		//glDeleteTextures(1, &depthBuffer_);
	}

	// generate an empty color texture with 4 channels (RGBA8) using bilinear filtering
	void createTextureAttachment(unsigned int width, unsigned int height){
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glGenerateMipmap(GL_TEXTURE_2D);
		// create the texture and the framebuffer texture attachment
		// the attachment uses mipmap level 0 (last parameter)
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_id, 0);
		texture = new Texture(GL_TEXTURE_2D, texture_id);
	}

	//generate an empty depth texture with 1 depth channel using bilinear filtering
	void createDepthTextureAttachment(unsigned int width, unsigned int height){
		glGenTextures(1, &depthTexture_id);
		glBindTexture(GL_TEXTURE_2D, depthTexture_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		// create the texture and the framebuffer texture attachment
		// the attachment uses mipmap level 0 (last parameter)
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture_id, 0);
		depthTexture = new Texture(GL_TEXTURE_2D, depthTexture_id);
	}

public:
	Framebuffer(){
	}

	~Framebuffer(){
		cleanUp();
	}

	//Generate FBO and two empty textures
	void GenerateFBO(unsigned int width, unsigned int height){
		//Generate a framebuffer object and attachments and bind it to the pipeline
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		createTextureAttachment(width, height);
		createDepthTextureAttachment(width, height);

		// draw the color buffer attachment
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		//Check for FBO completeness
		unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(fboStatus != GL_FRAMEBUFFER_COMPLETE){
			std::cout << "Error! Framebuffer (status " << fboStatus << ") not complete." << std::endl;
			//std::cin.get();
			//std::terminate();
		}

		//unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//return color texture from the framebuffer
	Texture* getColorTexture(){
		return texture;
	}

	//return depth texture from the framebuffer
	Texture* getDepthTexture(){
		return depthTexture;
	}

	// resize framebuffer
	void resize(unsigned int width, unsigned int height){
		cleanUp();
		GenerateFBO(width, height);
	}

	// bind framebuffer (all render operations will target this buffer)
	void bind(){
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	}

	// unbind framebuffer (by binding default framebuffer)
	void unbind(){
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
};