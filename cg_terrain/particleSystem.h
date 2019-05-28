#pragma once
#include "particle.h"
#include "shader.h"
#include <vector>
#include "object.h"
#include "material.h"
#include "texture.h"

class ParticleSystem2D{
private:
	GLuint nParticles;
	std::vector<Particle> particles;
	GLuint lastUsedParticle = 0;
	glm::vec3 position;
	glm::vec3 velocity;
	Texture* texture;
	
	GLuint FirstUnusedParticle(){
		// search from last used particle
		for(int i = this->lastUsedParticle; i < this->nParticles; i++){
			if(this->particles[i].life <= 0.f){
				this->lastUsedParticle = i;
				return i;
			}
		}
		// otherwise, do linear search
		for(int i = 0; i < this->lastUsedParticle; i++){
			if(this->particles[i].life <= 0.f){
				this->lastUsedParticle = i;
				return i;
			}
		}
		// if all are alive, override first particle
		this->lastUsedParticle = 0;
		return 0;
	}

	void RespawnParticle(Particle &p){
		GLfloat rOffsetx= -10.f + rand() % 200 / 10.f;
		GLfloat rOffsetz = -5.f + rand() % 100 / 10.f;
		GLfloat rScale = .05f + rand() % 45 / 100.f;
		GLfloat rRotationx = rand() % 360;
		GLfloat rRotationy = rand() % 360;
		GLfloat rRotationz = rand() % 360;
		GLfloat rVelocityx = rand() % 10 / 100.f;
		GLfloat rVelocityy = rand() % 90 / 100.f;
		GLfloat rVelocityz = rand() % 10 / 100.f;
		GLfloat rAngularVelocityx = rand() % 10;
		GLfloat rAngularVelocityy = rand() % 10;
		GLfloat rAngularVelocityz = rand() % 10;
		GLfloat rColor = .5f + ((rand() % 100) / 100.f);
		p.mesh->setPosition(this->position);
		p.mesh->move(glm::vec3(rOffsetx, 0.f, rOffsetz));
		p.mesh->setScale(glm::vec3(rScale));
		p.mesh->setLocalRotation(glm::vec3(rRotationx, rRotationy, rRotationz));
		p.color = glm::vec4(rColor, rColor, rColor, 1.f);
		p.life = 40.f;
		p.velocity = this->velocity * .1f + glm::vec3(rVelocityx, -rVelocityy, rVelocityz);
		p.angularVelocity = glm::vec3(rAngularVelocityx, rAngularVelocityy, rAngularVelocityz);
	}

public:
	ParticleSystem2D(glm::vec3 position, glm::vec3 velocity, Texture* texture,  GLuint nParticles = 20000){
		this->position = position;
		this->velocity = velocity;
		this->texture = texture;
		this->nParticles = nParticles;
		for(int i = 0; i < this->nParticles; i++){
			this->particles.push_back(Particle());
		}
	}

	void Update(float dt, int nNew = 0){
		// add new particles
		for(int i = 0; i < nNew; i++){
			int unusedParticle = this->FirstUnusedParticle();
			RespawnParticle(particles[unusedParticle]);
		}

		// update all particles
		for(Particle &p : this->particles){
			p.life -= dt;
			if(p.life > 0.f){
				p.mesh->move(p.velocity * dt);
				p.mesh->rotateLocally(p.angularVelocity * dt);
				p.color.a -= dt * 2.5f;
			}
		}
	}

	void Render(Shader* shader){
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		for(Particle p : this->particles){
			if(p.life > 0.f){
				this->texture->bind(0);
				p.mesh->render(shader);
			}
		}
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
};