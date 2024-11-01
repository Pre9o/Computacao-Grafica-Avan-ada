#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include "Scene.h"
#include <vector>
#include "glslprogram.h"

struct Light {
	vec4 position;
	float range;
};

class TessellatedQuad : public Scene
{
public:
	TessellatedQuad(GLFWwindow* window, int size = 1);

	void init();
	void update(double t);
	void render();
	void resize(int, int);

private:
	void genPlane();
	void genPatch(glm::vec2 offset);
	void genBuffers();
	void processInput();
	void processTessellation();
	void loadTextures();
	void updateCameraRotation(double deltatime);
	void updateLightRotation(double deltaTime);
	void updateLightPositions();
	void setupMatrices();
	void updateShaderUniforms();
	void updateInnerTessellation(std::vector<float>& distancias);
	void updateOuterTessellation(int centerIndex);
	void setCenterOuterTessellation(int centerIndex);
	void setVerticalOuterTessellation(int thisIndex, int centerIndex);
	void setHorizontalOuterTessellation(int thisIndex, int centerIndex);
	void setDiagonalOuterTessellation(int thisIndex, int topIndex, int rightIndex, int bottomIndex, int leftIndex, vec2 position);

	vec4 calculatePatchCenter(int patchIndex);
	
	std::vector<vec3> vertices;
	std::vector<vec2> texcoord;
	std::vector<unsigned char> outer;
	std::vector<unsigned char> inner;
	std::vector<unsigned int> indices;

	GLuint vaoID;
	GLSLProgram shader;
	GLFWwindow* window;

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat4 modelViewMatrix;
	glm::mat3 normalMatrix;

	Light principalLight;
	Light secondaryLight;

	vec3 cameraPos;
	vec3 planePos;

	int size;
	int numPatches = 3;
	int numVertices = 4;
	int numVerticesPerPatch = 16;
	bool rotateCamera = false;
	bool wireframe = false;
	float currentCameraRotation = 0.0f;
	float currentLightRotation = 0.0f;
	unsigned int handle[5];
	unsigned char centerLevel = 1;
};