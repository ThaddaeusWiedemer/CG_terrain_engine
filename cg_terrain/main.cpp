#include <iostream>
#include <cmath>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// SOIL
#include <SOIL.h>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

// Other includes
#include "camera.h"
#include "shader.h"
#include "vertex.h"
#include "primitives.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "object.h"
#include "framebuffer.h"

// Function prototypes
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void DoMovement(GLfloat &cooldown);

// Window dimensions
GLuint WIDTH = 1800, HEIGHT = 1000;

// Camera
Camera camera(glm::vec3(0.f, 10.f, 0.f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
GLfloat max_speed = 9.f;
GLfloat camera_x = 0.f;
GLfloat camera_y = 0.f;
GLfloat camera_z = 0.f;
GLfloat acceleration = 200.f;

// Deltatime
GLfloat deltaTime = 0.f;    // Time between current frame and last frame
GLfloat lastFrame = 0.f;      // Time of last frame

// Rotation
GLfloat rot_speed = 3.f;
glm::quat rot_quat = glm::angleAxis(0.f, glm::vec3(1, 0, 0));

// Tesselation
GLfloat waveSize = 10.f;
bool simpleModel = false;
bool showGui = false;
bool highlights = true;
GLuint fresnel = 1;
GLuint edge = 1;

// The MAIN function, from here we start the application and run the game loop
int main(){
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Terrain Engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// Initialize GLAD to setup the OpenGL Function pointers
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Build and compile shader program
	Shader shader(4, 1, "main.vert.glsl", "simple.frag.glsl");
	Shader waterShaderSimple(4, 1, "watersimple.vert.glsl", "watersimple.frag.glsl");
	Shader waterShader(4, 1, "water.vert.glsl", "water.frag.glsl");
	Shader terrainShader(4, 1, "main.vert.glsl", "terrain.frag.glsl");
	Shader guiShader(4, 1, "gui.vert.glsl", "gui.frag.glsl");

	// Load and create textures
	std::vector<Texture*> textures;
	textures.push_back(new Texture("white.jpg", GL_TEXTURE_2D));
	textures.push_back(new Texture("sky0.bmp", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
	textures.push_back(new Texture("sky1.bmp", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
	textures.push_back(new Texture("sky2.bmp", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
	textures.push_back(new Texture("sky3.bmp", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
	textures.push_back(new Texture("sky4.bmp", GL_TEXTURE_2D, GL_CLAMP_TO_EDGE));
	textures.push_back(new Texture("water.bmp", GL_TEXTURE_2D));
	textures.push_back(new Texture("terrain.bmp", GL_TEXTURE_2D));
	textures.push_back(new Texture("terrainDetail.bmp", GL_TEXTURE_2D));
	textures.push_back(new Texture("waterDUDV.png", GL_TEXTURE_2D));
	textures.push_back(new Texture("waterNormals.png", GL_TEXTURE_2D));

	// Create materials
	Material* materialOnlyAmbient = new Material(glm::vec3(1.f), glm::vec3(0.f), glm::vec3(0.f));
	Material* materialWater = new Material(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f));
	Material* materialTerrain = new Material(glm::vec3(.5f), glm::vec3(.8f), glm::vec3(0.f));

	// Framebuffer
	// TODO why does the position of this snippet before or after the terrain creation matter?
	// was related to the non-proper handling of detail texture, should be fine now
	Framebuffer fboTexture;
	fboTexture.GenerateFBO(WIDTH, HEIGHT);
	Framebuffer fboDepth;
	fboDepth.GenerateFBO(WIDTH, HEIGHT);

	// Set up vertex data (and buffer(s)) and attribute pointers
	// SKY
	GLfloat skyboxScale = 200.f;
	std::vector<Object*> skybox;
	// back
	std::vector<Mesh*> meshSky0 = {new Mesh(new Quad(), glm::vec3(0.f, .5f, 1.f) * skyboxScale, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(2.f, 1.f, 1.f) * skyboxScale)};
	skybox.push_back(new Object(glm::vec3(0.f), materialOnlyAmbient, textures[1], meshSky0));
	// right
	std::vector<Mesh*> meshSky1 = {new Mesh(new Quad(), glm::vec3(1.f, .5f, 0.f) * skyboxScale, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f, 90.f, 0.f), glm::vec3(2.f, 1.f, 1.f) * skyboxScale)};
	skybox.push_back(new Object(glm::vec3(0.f), materialOnlyAmbient, textures[2], meshSky1));
	// front
	std::vector<Mesh*> meshSky2 = {new Mesh(new Quad(), glm::vec3(0.f, .5f, -1.f) * skyboxScale, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f, 180.f, 0.f), glm::vec3(2.f, 1.f, 1.f) * skyboxScale)};
	skybox.push_back(new Object(glm::vec3(0.f), materialOnlyAmbient, textures[3], meshSky2));
	// left
	std::vector<Mesh*> meshSky3 = {new Mesh(new Quad(), glm::vec3(-1.f, .5f, 0.f) * skyboxScale, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f, 270.f, 0.f), glm::vec3(2.f, 1.f, 1.f) * skyboxScale)};
	skybox.push_back(new Object(glm::vec3(0.f), materialOnlyAmbient, textures[4], meshSky3));
	// top
	std::vector<Mesh*> meshSky4 = {new Mesh(new Quad(), glm::vec3(0.f, 1.f, 0.f) * skyboxScale, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(270.f, 0.f, 0.f), glm::vec3(2.f, 2.f, 1.f) * skyboxScale)};
	skybox.push_back(new Object(glm::vec3(0.f), materialOnlyAmbient, textures[5], meshSky4));
	
	// WATER
	std::vector<Mesh*> meshWater = {new Mesh(new Quad(glm::vec4(1.f, 1.f, 1.f, .2f)), glm::vec3(0.f, 0.f, 0.f) * skyboxScale, glm::vec3(0.f), glm::vec3(0.f), glm::vec3(270.f, 0.f, 0.f), glm::vec3(2.f, 2.f, 1.f) * skyboxScale)};
	Object water(glm::vec3(0.f), materialWater, textures[6], textures[10], textures[9], meshWater);
	waterShaderSimple.setVec2f(glm::vec2(skyboxScale / 6.f), "texCoordScale");
	GLfloat waterSpeed = .5f / skyboxScale;
	GLfloat moveFactor = 0.f;
	GLfloat waveSpeed = .1f;

	// TERRAIN
	std::vector<Mesh*> meshTerrain = {Mesh::fromTopography("terrainHeight.bmp")};
	meshTerrain[0]->move(glm::vec3(-50.f, -5.9f, -50.f));
	meshTerrain[0]->scaleUp(glm::vec3(100.f, 20.f, 100.f));
	Object terrain(glm::vec3(0.f), materialTerrain, textures[7], textures[8], meshTerrain);
	terrainShader.setVec2f(glm::vec2(30.f), "detailTexScale");

	// REFLECTIONS
	glm::quat flipRotation = glm::angleAxis(glm::radians(180.f), glm::vec3(1, 0, 0));// *glm::angleAxis(glm::radians(180.f), glm::vec3(0, 1, 0));
	std::vector<Object*> reflectionSky;
	reflectionSky.push_back(new Object(glm::vec3(0.f), materialOnlyAmbient, textures[1], meshSky0));
	reflectionSky.push_back(new Object(glm::vec3(0.f), materialOnlyAmbient, textures[2], meshSky1));
	reflectionSky.push_back(new Object(glm::vec3(0.f), materialOnlyAmbient, textures[3], meshSky2));
	reflectionSky.push_back(new Object(glm::vec3(0.f), materialOnlyAmbient, textures[4], meshSky3));
	reflectionSky.push_back(new Object(glm::vec3(0.f), materialOnlyAmbient, textures[5], meshSky4));
	for(Object* obj : reflectionSky){
		obj->scaleUp(glm::vec3(1.f, -1.f, 1.f));
		obj->move(glm::vec3(0.f, -1.f, 0.f) * skyboxScale);
	}
	reflectionSky[4]->move(glm::vec3(0.f, -1.f, 0.f) * skyboxScale);
	reflectionSky[4]->rotate(glm::vec3(0.f, glm::radians(180.f), 0.f));
	reflectionSky[4]->scaleUp(glm::vec3(-1.f, 1.f, 1.f));
	Object reflectionTerrain(glm::vec3(0.f), materialTerrain, textures[7], textures[8], meshTerrain);
	reflectionTerrain.scaleUp(glm::vec3(1.f, -1.f, 1.f));
	reflectionTerrain.move(glm::vec3(0.f, 11.8f, 0.f));

	// GUI output
	std::vector<Mesh*> meshGui = {new Mesh(new Quad())};
	Object guiTexture(glm::vec3(0.f), materialWater, fboTexture.getColorTexture(), meshGui);
	Object guiDepth(glm::vec3(0.f), materialWater, fboDepth.getDepthTexture(), meshGui);

	// Lights
	glm::vec3 light(-1.f, .6f, -.8f);
	light *= skyboxScale;
	terrainShader.setVec3f(light, "lightPos0");
	waterShader.setVec3f(light, "lightPos0");

	// enable depth testing and clipping planes
	glEnable(GL_CLIP_DISTANCE0);
	glEnable(GL_DEPTH_TEST);

	GLfloat cooldown = 0.f;

	// Game loop
	while(!glfwWindowShouldClose(window)){
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		cooldown -= deltaTime;
		if(cooldown < 0)
			cooldown = 0;
		DoMovement(cooldown);

		// Apply keyboard rotation
		for(Object* obj : skybox){
			obj->rotate(rot_quat);
		}
		for(Object* obj : reflectionSky){
			obj->rotate(rot_quat);
		}
		water.rotate(rot_quat);
		terrain.rotate(rot_quat);
		reflectionTerrain.rotate(rot_quat);
		
		// transformation matrices
		glm::mat4 view(1);
		glm::mat4 projection(1);

		// RENDER PASS 1
		// generate reflection texture by rendering the skybox (with its reflection to avoid blackspace in 
		// the texture) and the island, as viewed from a camera mirrored underneath the water surface
		fboTexture.bind();
		// clear buffers
		glClearColor(.0f, .0f, .0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		// invert camera for reflection
		camera.invert();
		// send camera matrices to shaders
		view = camera.getViewMatrix();		
		projection = camera.getProjectionMatrix((GLfloat)WIDTH, (GLfloat)HEIGHT);
		shader.setMat4fv(view, "view");
		shader.setMat4fv(projection, "projection");
		terrainShader.setMat4fv(view, "view");
		terrainShader.setMat4fv(projection, "projection");
		// undo camera inversion
		camera.invert();

		// render skybox
		for(Object* obj : skybox){
			obj->render(&shader);
		}

		// render skybox reflection
		for(Object* obj : reflectionSky){
			obj->render(&shader);
		}

		// render terrain
		terrainShader.setVec4f(glm::vec4(0.f, 1.f, 0.f, 0.f), "clipPlane");
		terrain.render(&terrainShader);
		fboTexture.unbind();
		// END RENDER PASS 1

		// RENDER PASS 2
		// generate the depth texture by rendering only the (unclipped) terrain
		fboDepth.bind();
		// clear buffers
		glClearColor(.0f, .0f, .0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		// send camera matrices to shaders
		view = camera.getViewMatrix();
		projection = camera.getProjectionMatrix((GLfloat)WIDTH, (GLfloat)HEIGHT);
		terrainShader.setMat4fv(view, "view");
		terrainShader.setMat4fv(projection, "projection");

		// render terrain
		terrainShader.setVec4f(glm::vec4(0.f), "clipPlane");
		terrain.render(&terrainShader);
		fboDepth.unbind();
		// END RENDER PASS 2

		// RENDER PASS 3
		// render the final result
		// clear buffers
		glClearColor(.0f, .0f, .0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		// send camera matrices to shaders
		view = camera.getViewMatrix();
		projection = camera.getProjectionMatrix((GLfloat)WIDTH, (GLfloat)HEIGHT);
		shader.setMat4fv(view, "view");
		shader.setMat4fv(projection, "projection");
		waterShader.setMat4fv(view, "view");
		waterShader.setMat4fv(projection, "projection");
		waterShaderSimple.setMat4fv(view, "view");
		waterShaderSimple.setMat4fv(projection, "projection");
		terrainShader.setMat4fv(view, "view");
		terrainShader.setMat4fv(projection, "projection");
		guiShader.setMat4fv(projection, "projection");

		// render skybox
		for(Object* obj : skybox){
			obj->render(&shader);
		}

		// render terrain
		terrainShader.setVec4f(glm::vec4(0.f, 1.f, 0.f, 0.f), "clipPlane");
		terrain.render(&terrainShader);

		// render water with reflection
		if(simpleModel){
			// render skybox reflection
			for(Object* obj : reflectionSky){
				obj->render(&shader);
			}

			// render terrain reflection
			terrainShader.setVec4f(glm::vec4(0.f, -1.f, 0.f, 0.f), "clipPlane");
			reflectionTerrain.render(&terrainShader);

			// terrain without shade
			terrain.setMaterial(materialOnlyAmbient);

			// apply water movement and render
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			water.setDiffuseTexture(textures[6]);
			water.setMaterial(materialOnlyAmbient);
			waterShaderSimple.setVec2f(glm::vec2(currentFrame * waterSpeed), "texCoordOffset");
			water.render(&waterShaderSimple);
			glDisable(GL_BLEND);
		} else{
			// terrain with shade
			terrain.setMaterial(materialTerrain);

			// render water
			water.setDiffuseTexture(fboTexture.getColorTexture());
			if(highlights){
				water.setMaterial(materialWater);
			} else{
				water.setMaterial(materialOnlyAmbient);
			}
			waterShader.set1f(skyboxScale / waveSize, "tiling");
			moveFactor += waveSpeed * deltaTime;
			waterShader.set1f(moveFactor, "moveFactor");
			waterShader.setVec3f(camera.getPosition(), "cameraPos");
			fboDepth.getDepthTexture()->bind(3);
			waterShader.set1i(3, "depthMap");
			waterShader.set1i(fresnel, "doFresnel");
			waterShader.set1i(edge, "doEdge");
			water.render(&waterShader);

			if(showGui){
				// render GUI
				guiShader.setVec2f(glm::vec2(-.75f, .75f), "offset");
				guiShader.set1i(-1, "flip");
				guiTexture.render(&guiShader);
				guiShader.setVec2f(glm::vec2(-.25f, .75f), "offset");
				guiShader.set1i(1, "flip");
				guiDepth.render(&guiShader);
			}
		}
		// END RENDER PASS 3

		// reset variables and swap the screen buffers
		rot_quat = glm::angleAxis(0.f, glm::vec3(1, 0, 0));
		glfwSwapBuffers(window);
	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height){
	WIDTH = width;
	HEIGHT = height;
	glViewport(0, 0, width, height);
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if(key >= 0 && key < 1024){
		if(action == GLFW_PRESS)
			keys[key] = true;
		else if(action == GLFW_RELEASE)
			keys[key] = false;
	}
}

// Move through the scene
void DoMovement(GLfloat &cooldown){
	// Camera lateral movement
	if(camera_x > 0){
		camera_x = std::fmax(camera_x - deltaTime * acceleration / 2, 0);
	} else{
		camera_x = std::fmin(camera_x + deltaTime * acceleration / 2, 0);
	}
	if(camera_y > 0){
		camera_y = std::fmax(camera_x - deltaTime * acceleration / 2, 0);
	} else{
		camera_y = std::fmin(camera_x + deltaTime * acceleration / 2, 0);
	}
	if(camera_z > 0){
		camera_z = std::fmax(camera_z - deltaTime * acceleration / 2, 0);
	} else{
		camera_z = std::fmin(camera_z + deltaTime * acceleration / 2, 0);
	}
	if(keys[GLFW_KEY_W]){
		camera_x = fmin(camera_x + deltaTime * acceleration, max_speed);
	}
	if(keys[GLFW_KEY_S]){
		camera_x = fmax(camera_x - deltaTime * acceleration, -max_speed);
	}
	if(keys[GLFW_KEY_E]){
		camera_y = fmin(camera_y + deltaTime * acceleration * 4, max_speed * 4);
	}
	if(keys[GLFW_KEY_Q]){
		camera_y = fmax(camera_y - deltaTime * acceleration * 4, -max_speed * 4);
	}
	if(keys[GLFW_KEY_A]){
		camera_z = fmin(camera_z + deltaTime * acceleration, max_speed * 1.5f);
	}
	if(keys[GLFW_KEY_D]){
		camera_z = fmax(camera_z - deltaTime * acceleration, -max_speed * 1.5f);
	}
	camera.Move(FORWARD, deltaTime * camera_x);
	camera.Move(UP, deltaTime * camera_y);
	camera.Move(LEFT, deltaTime * camera_z);
	// Camera rotations
	if(keys[GLFW_KEY_R]){
		rot_quat = glm::rotate(rot_quat, rot_speed * deltaTime, glm::vec3(1, 0, 0));
	}
	if(keys[GLFW_KEY_F]){
		rot_quat = glm::rotate(rot_quat, -rot_speed * deltaTime, glm::vec3(1, 0, 0));
	}
	if(keys[GLFW_KEY_T]){
		rot_quat = glm::rotate(rot_quat, rot_speed * deltaTime, glm::vec3(0, 1, 0));
	}
	if(keys[GLFW_KEY_G]){
		rot_quat = glm::rotate(rot_quat, -rot_speed * deltaTime, glm::vec3(0, 1, 0));
	}
	if(keys[GLFW_KEY_Y]){
		rot_quat = glm::rotate(rot_quat, rot_speed * deltaTime, glm::vec3(0, 0, 1));
	}
	if(keys[GLFW_KEY_H]){
		rot_quat = glm::rotate(rot_quat, -rot_speed * deltaTime, glm::vec3(0, 0, 1));
	}
	// reset
	if(keys[GLFW_KEY_Z]){
		camera.ResetPosition();
		rot_quat = glm::angleAxis(0.f, glm::vec3(1, 0, 0));
	}
	// toggle line mode
	if(keys[GLFW_KEY_X] && cooldown == 0){
		cooldown = .2f;
		simpleModel = !simpleModel;
	}
	// toggle GUI
	if(keys[GLFW_KEY_C] && cooldown == 0){
		cooldown = .2f;
		showGui = !showGui;
	}
	// control wave size
	if(keys[GLFW_KEY_V] && cooldown == 0){
		cooldown = .2f;
		waveSize++;
	}
	if(keys[GLFW_KEY_B] && cooldown == 0){
		cooldown = .2f;
		waveSize--;
	}
	// toggle highlights
	if(keys[GLFW_KEY_1] && cooldown == 0){
		cooldown = .2f;
		highlights = !highlights;
	}
	// toggle Fresnel Effect
	if(keys[GLFW_KEY_2] && cooldown == 0){
		cooldown = .2f;
		fresnel += std::pow(-1, fresnel);
		std::cout << fresnel << std::endl;
	}
	// toggle edge effect
	if(keys[GLFW_KEY_3] && cooldown == 0){
		cooldown = .2f;
		edge += std::pow(-1, edge);
	}
}

// Camera tilt via mouse
void MouseCallback(GLFWwindow* window, double xpos, double ypos){
	if(firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// Zoom
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	camera.ProcessMouseScroll(yoffset);
}