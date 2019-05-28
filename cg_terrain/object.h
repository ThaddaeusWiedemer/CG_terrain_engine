#pragma once

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "Material.h"
#include "particleSystem.h"

class Object{
private:
	Material* material;
	Texture* overrideTextureDiffuse;
	Texture* overrideTextureSpecular;
	Texture* overrideTextureDisplace;
	std::vector<Mesh*> meshes;
	glm::vec3 origin;

	void updateUniforms(){

	}

	void init(glm::vec3 origin, Material* material, Texture* orTexDif, Texture* orTexSpec, Texture* orTexDisp, std::vector<Mesh*>& meshes){
		this->origin = origin;
		this->material = material;
		this->overrideTextureDiffuse = orTexDif;
		this->overrideTextureSpecular = orTexSpec;
		this->overrideTextureDisplace = orTexDisp;

		for(auto* i : meshes){
			this->meshes.push_back(new Mesh(*i));
		}

		for(auto& i : this->meshes){
			i->move(this->origin);
			i->setOrigin(this->origin);
		}
	}

public:
	Object(glm::vec3 origin, Material* material, Texture* orTexDif, std::vector<Mesh*>& meshes){
		init(origin, material, orTexDif, nullptr, nullptr, meshes);
	}

	Object(glm::vec3 origin, Material* material, Texture* orTexDif, Texture* orTexSpec, std::vector<Mesh*>& meshes){
		init(origin, material, orTexDif, orTexSpec, nullptr, meshes);
	}

	Object(glm::vec3 origin, Material* material, Texture* orTexDif, Texture* orTexSpec, Texture* orTexDisp, std::vector<Mesh*>& meshes){
		init(origin, material, orTexDif, orTexSpec, orTexDisp, meshes);
	}

	~Object(){
		for(auto*& i : this->meshes){
			delete i;
		}
	}

	//Functions
	glm::vec3 getPosition(){
		std::vector<glm::vec3> positions;
		for(auto& i : this->meshes){
			positions.push_back(i->getPosition());
		}
		return positions[0];
	}

	glm::mat4 getModelMatrix(){
		std::vector<glm::mat4> matrices;
		for(auto& i : this->meshes){
			matrices.push_back(i->getModelMatrix());
		}
		return matrices[0];
	}

	void rotateLocally(const glm::vec3 rotation){
		for(auto& i : this->meshes){
			i->rotateLocally(rotation);
		}
	}

	void rotate(const glm::quat rotation){
		for(auto& i : this->meshes){
			i->rotate(rotation);
		}
	}

	void scaleUp(const glm::vec3 scale){
		for(auto& i : this->meshes){
			i->scaleUp(scale);
		}
	}

	void move(const glm::vec3 position){
		for(auto& i : this->meshes){
			i->move(position);
		}
	}
 
	void setMaterial(Material* material){
		this->material = material;
	}

	void setDiffuseTexture(Texture* texture){
		this->overrideTextureDiffuse = texture;
	}

	void setSpecularTexture(Texture* texture){
		this->overrideTextureSpecular = texture;
	}

	void update(){

	}

	void render(Shader* shader){
		//Update the uniforms
		this->updateUniforms();

		//Update uniforms
		this->material->sendToShader(*shader);

		//Activate texture
		this->overrideTextureDiffuse->bind(0);
		if(this->overrideTextureSpecular != nullptr){
			this->overrideTextureSpecular->bind(1);
		}
		if(this->overrideTextureDisplace != nullptr){
			this->overrideTextureDisplace->bind(2);
		}

		//Draw
		for(auto& i : this->meshes){
			i->render(shader, GL_TRIANGLES);
		}
	}

	void renderPoints(Shader* shader, float size = 10.f){
		//Update the uniforms
		this->updateUniforms();

		//Update uniforms
		this->material->sendToShader(*shader);

		//Activate texture
		this->overrideTextureDiffuse->bind(0);
		this->overrideTextureSpecular->bind(1);
		this->overrideTextureDisplace->bind(2);

		// Set point size
		glPointSize(size);

		//Draw
		for(auto& i : this->meshes){
			i->render(shader, GL_POINTS);
		}
	}

	void renderPatches(Shader* shader){
		shader->Use();
		//Update the uniforms
		this->updateUniforms();

		//Update uniforms
		this->material->sendToShader(*shader);

		//Activate texture
		this->overrideTextureDiffuse->bind(0);
		this->overrideTextureSpecular->bind(1);
		this->overrideTextureDisplace->bind(2);

		//Draw
		for(auto& i : this->meshes){
			i->render(shader, GL_PATCHES);
		}
	}
};