#define DEV
// #define FULLSCREEN

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <gl/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// -----------------------------------------------------------------------------------------



struct LightVals {
	glm::vec3 pos;
	glm::vec3 color;
	unsigned int shadowMap;
};

struct TextureGroup {
	unsigned int diffusionMap;
	unsigned int specularMap;
	unsigned int normalMap;
};

struct Material {
	TextureGroup maps;
	float inverseRoughness;
};



// -----------------------------------------------------------------------------------------



#include <custom/mainFunctions.hpp>

#include <custom/objects/camera.hpp>
#include <custom/objects/object.hpp>
#include <custom/objects/light.hpp>

#include <custom/scripts/cameraController.hpp>

#include <iostream>

#ifndef DEV
	const char TITLE[] = "Portals Testing";
#else
	const char TITLE[] = "Portals Testing <DEV>";
#endif



// -----------------------------------------------------------------------------------------



int main() {
	
	glfwInit();


	#ifdef FULLSCREEN
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		const int WIDTH = mode -> width;
		const int HEIGHT = mode -> height;
	#else
		const int WIDTH = 1024;
		const int HEIGHT = 1024;
	#endif


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

	#ifdef FULLSCREEN
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, WIDTH, HEIGHT, mode -> refreshRate);
	#endif

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, WIDTH, HEIGHT);



	// -----------------------------------------------------------------------------------------



	Camera mainCamera; mainCamera.init(45.0f, WIDTH, HEIGHT, 0.1f, 100.0f);
	// mainCamera.set_loc(glm::vec3(0.0f, 1.5f, 0.0f));
	mainCamera.set_loc(glm::vec3(0.0f, 2.0f, 0.0f));



	std::vector<float> floorMesh = readCSV<float>("../resources/meshes/floor.vao");
	std::vector<float> wallMesh = readCSV<float>("../resources/meshes/walls.vao");
	std::vector<float> cubeMesh = readCSV<float>("../resources/meshes/cube.vao");
	
	unsigned int wallDiffusionMap = create_texture("../resources/textures/mainScene/wall/diffusionmap.jpg", GL_RGB);
	unsigned int wallSpecularMap = create_texture("../resources/textures/mainScene/wall/specularmap.jpg", GL_RGB);
	unsigned int wallNormalMap = create_texture("../resources/textures/mainScene/wall/normalmap.jpg", GL_RGB);
	TextureGroup wallMap {wallDiffusionMap, wallSpecularMap, wallNormalMap};

	unsigned int floorDiffusionMap = create_texture("../resources/textures/mainScene/floor/diffusionmap.jpg", GL_RGB);
	unsigned int floorSpecularMap = create_texture("../resources/textures/mainScene/floor/specularmap.jpg", GL_RGB);
	unsigned int floorNormalMap = create_texture("../resources/textures/mainScene/floor/normalmap.jpg", GL_RGB);
	TextureGroup floorMap {floorDiffusionMap, floorSpecularMap, floorNormalMap};

	unsigned int solidShader = create_shader("../resources/shaders/solid/vertexShader.vert", "../resources/shaders/solid/fragmentShader.frag"); 
	unsigned int shadowShader = create_shader("../resources/shaders/shadow/vertexShader.vert", "../resources/shaders/shadow/fragmentShader.frag");



	Object floor(floorMesh, solidShader, shadowShader, Material{floorMap, 512.0});
	floor.set_loc(glm::vec3(0.0, 0.0, 0.0));

	Object walls(wallMesh, solidShader, shadowShader, Material{wallMap, 8.0});
	walls.set_loc(glm::vec3(0.0, 0.0, 0.0));

	Object cube(cubeMesh, solidShader, shadowShader, Material{floorMap, 512.0});
	cube.set_loc(glm::vec3(2.0, 2.0, -2.0));



	std::vector<Light> lightVector {
		Light(glm::vec3(0.0, 2.0, 0.0), glm::vec3(1.0, 1.0, 1.0)),
	};

	std::vector<Object> objectVector {floor, walls, cube};



	// -----------------------------------------------------------------------------------------



	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	while(!glfwWindowShouldClose(window)) {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera_controller(&mainCamera, window);

		for (int i=0; i<lightVector.size(); i++) {lightVector[i].renderShadowPass(objectVector);}

		std::vector<LightVals> lightValsVector {};
		for (int i=0; i<lightVector.size(); i++) {lightValsVector.push_back(LightVals {lightVector[i].shadowCamera.location, lightVector[i].color, lightVector[i].depthCubemap});}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		for (int i=0; i<objectVector.size(); i++) {objectVector[i].draw(mainCamera, lightValsVector);}
		// for (int i=0; i<objectVector.size(); i++) {objectVector[i].draw_shadow(mainCamera, lightVector[0].shadowCamera.location);}

		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {glfwSetWindowShouldClose(window, true);}

		glfwSwapBuffers(window);
		glfwPollEvents();
	
	}



	// -----------------------------------------------------------------------------------------



	glfwTerminate();
	return 0;
}