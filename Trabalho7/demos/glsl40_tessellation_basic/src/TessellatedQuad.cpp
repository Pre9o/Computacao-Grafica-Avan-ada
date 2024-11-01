#include "TessellatedQuad.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "TextureManager.h"
#include "GLUtils.h"

using namespace std;

char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY)             \
    (glfwGetKey(WINDOW, KEY) ?              \
     (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :   \
     (keyOnce[KEY] = false))

TextureManager* texManager;

TessellatedQuad::TessellatedQuad(GLFWwindow* window, int size)
	: window(window), size(size) {}

float scaleValue(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
	if (inputMin == inputMax) {
		return outputMin;
	}
	float inputRange = inputMax - inputMin;
	float outputRange = outputMax - outputMin;
	return outputMin + ((value - inputMin) * outputRange) / inputRange;
}

void TessellatedQuad::loadTextures() {
	glActiveTexture(GL_TEXTURE0);
	if (!texManager->LoadTexture("..\\..\\resources\\old_bricks_sharp_diff_COLOR.png", 0)) {
		cout << "Failed to load texture." << endl;
	}

	glActiveTexture(GL_TEXTURE1);
	if (!texManager->LoadTexture("..\\..\\resources\\old_bricks_sharp_diff_DISP.png", 1)) {
		cout << "Failed to load texture." << endl;
	}
}

void TessellatedQuad::init() {
	genPlane();
	genBuffers();

	modelMatrix = glm::mat4(1.0f);

	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	projectionMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(w) / h, 0.5f, 50.0f);

	try {
		shader.compileShader("shader/glsl40_basic_tess.frag", GLSLShader::FRAGMENT);
		shader.compileShader("shader/glsl40_basic_tess.geom", GLSLShader::GEOMETRY);
		shader.compileShader("shader/glsl40_basic_tess.tcs", GLSLShader::TESS_CONTROL);
		shader.compileShader("shader/glsl40_basic_tess.tes", GLSLShader::TESS_EVALUATION);
		shader.compileShader("shader/glsl40_basic_tess.vert", GLSLShader::VERTEX);

		shader.link();
		shader.use();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	shader.printActiveAttribs();

	texManager = TextureManager::Inst();

	loadTextures();
}

void TessellatedQuad::processInput()
{
	if (glfwGetKeyOnce(window, 'R')) {
		rotateCamera = !rotateCamera;
	}

	if (glfwGetKeyOnce(window, 'W')) {
		wireframe = !wireframe;
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (glfwGetKeyOnce(window, GLFW_KEY_UP)) {
		int maxTessLevel;
		glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &maxTessLevel);

		centerLevel++;
		if (centerLevel > maxTessLevel) {
			centerLevel = maxTessLevel;
		}
		if (centerLevel > 256) {
			centerLevel = 256;
		}
	}

	if (glfwGetKeyOnce(window, GLFW_KEY_DOWN)) {
		centerLevel--;
		if (centerLevel < 1) {
			centerLevel = 1;
		}
	}
}

void TessellatedQuad::processTessellation() {
	int centerIndex = numPatches * numVertices + numVertices;

	inner[centerIndex] = centerLevel;
	inner[centerIndex + 1] = centerLevel;
	std::vector<float> distancias;

	updateInnerTessellation(distancias);
	updateOuterTessellation(centerIndex);

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, outer.size() * sizeof(unsigned char), outer.data());
	glBindBuffer(GL_ARRAY_BUFFER, handle[3]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, inner.size() * sizeof(unsigned char), inner.data());
}

void TessellatedQuad::updateCameraRotation(double deltaTime)
{
	if (rotateCamera) {
		currentCameraRotation += static_cast<float>(deltaTime);
	}
}

void TessellatedQuad::updateLightRotation(double deltaTime)
{
	currentLightRotation += static_cast<float>(deltaTime);
}

void TessellatedQuad::updateLightPositions()
{
	principalLight.range = 3;
	secondaryLight.range = 2;

	principalLight.position = vec4(size * (0.5 + cos(currentLightRotation) * 0.1), 1, size * (0.5 + sin(currentLightRotation) * 0.1), 1);
	secondaryLight.position = vec4(size * (0.5 + cos(currentLightRotation * 2) * 0.3), 1, size * (0.5 + sin(currentLightRotation * 1) * 0.3), 1);
}


void TessellatedQuad::setupMatrices()
{
	const float camRotSpeed = 0.5;
	const float camRadius = 2.5;

	cameraPos = vec3(cos(currentCameraRotation * camRotSpeed) * camRadius, 1, sin(currentCameraRotation * camRotSpeed) * camRadius);
	viewMatrix = glm::lookAt(cameraPos, vec3(0, 1, 0), vec3(0, 2, 0));

	modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, vec3(-size * 0.5, 0, -size * 0.5));

	modelViewMatrix = viewMatrix * modelMatrix;
	modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
	normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelViewMatrix)));

	principalLight.position = modelViewProjectionMatrix * principalLight.position;
	secondaryLight.position = modelViewProjectionMatrix * secondaryLight.position;
}

void TessellatedQuad::update(double deltaTime)
{
	processInput();
	processTessellation();
	updateCameraRotation(deltaTime);
	updateLightRotation(deltaTime);
	updateLightPositions();
	setupMatrices();
	updateShaderUniforms();
}


void TessellatedQuad::updateShaderUniforms()
{
	shader.setUniform("displacementmapSampler", 1);
	shader.setUniform("colorTextureSampler", 0);

	shader.setUniform("MVP", modelViewProjectionMatrix); 
	shader.setUniform("NormalMatrix", normalMatrix); 

	shader.setUniform("principalLight.pos", principalLight.position);
	shader.setUniform("principalLight.range", principalLight.range);
	shader.setUniform("secondaryLight.pos", secondaryLight.position);
	shader.setUniform("secondaryLight.range", secondaryLight.range);
}


void TessellatedQuad::updateInnerTessellation(std::vector<float>& distancias) {
	for (int i = 0; i < numPatches; i++) {
		for (int j = 0; j < numPatches; j++) {
			if (i == 1 && j == 1) continue;

			int patchIndex = i * numPatches * numVertices + j * numVertices;
			auto p = calculatePatchCenter(patchIndex);
			p = modelViewMatrix * p;
			distancias.push_back(glm::sqrt(p.x * p.x + p.y * p.y + p.z * p.z));
			int tessLevel = (int)scaleValue(glm::length(p), 1, 10, 128, 64);
			int innerTessIndex = i * numPatches * numVertices + j * numVertices;
			inner[innerTessIndex] = (unsigned char)tessLevel;
			inner[innerTessIndex + 1] = (unsigned char)tessLevel;
		}
	}
}

vec4 TessellatedQuad::calculatePatchCenter(int patchIndex) {
	auto p1 = vec4(vertices[patchIndex + 0], 1);
	auto p2 = vec4(vertices[patchIndex + 1], 1);
	auto p3 = vec4(vertices[patchIndex + 2], 1);
	auto p4 = vec4(vertices[patchIndex + 3], 1);
	return (p1 + p2 + p3 + p4) * (float)(1 / numVertices);
}

void TessellatedQuad::updateOuterTessellation(int centerIndex) {
	for (int i = 0; i < numPatches; i++) {
		for (int j = 0; j < numPatches; j++) {
			if (i == 1 && j == 1) {
				setCenterOuterTessellation(centerIndex);
				continue;
			}
			int thisIndex = i * numPatches * numVertices + j * numVertices;
			int rightIndex = i * numPatches * numVertices + (j + 1) * numVertices;
			int leftIndex = i * numPatches * numVertices + (j - 1) * numVertices;
			int topIndex = (i + 1) * numPatches * numVertices + j * numVertices;
			int bottomIndex = (i - 1) * numPatches * numVertices + j * numVertices;

			if (j == 1) {
				setVerticalOuterTessellation(thisIndex, centerIndex);
			}
			else if (i == 1) {
				setHorizontalOuterTessellation(thisIndex, centerIndex);
			}
			else {
				setDiagonalOuterTessellation(thisIndex, topIndex, rightIndex, bottomIndex, leftIndex, vec2(i, j));
			}
		}
	}
}

void TessellatedQuad::setCenterOuterTessellation(int centerIndex) {
	outer[centerIndex + 0] = inner[centerIndex];
	outer[centerIndex + 1] = inner[centerIndex];
	outer[centerIndex + 2] = inner[centerIndex];
	outer[centerIndex + 3] = inner[centerIndex];
}

void TessellatedQuad::setVerticalOuterTessellation(int thisIndex, int centerIndex) {
	outer[thisIndex + 0] = inner[centerIndex];
	outer[thisIndex + 2] = inner[centerIndex];
	outer[thisIndex + 1] = inner[thisIndex];
	outer[thisIndex + 3] = inner[thisIndex];
}

void TessellatedQuad::setHorizontalOuterTessellation(int thisIndex, int centerIndex) {
	outer[thisIndex + 1] = inner[centerIndex];
	outer[thisIndex + 3] = inner[centerIndex];
	outer[thisIndex + 0] = inner[thisIndex];
	outer[thisIndex + 2] = inner[thisIndex];
}

void TessellatedQuad::setDiagonalOuterTessellation(int thisIndex, int topIndex, int rightIndex, int bottomIndex, int leftIndex, vec2 position) {
	outer[thisIndex + 2] = (position.x == 2) ? inner[thisIndex] : inner[topIndex];
	outer[thisIndex + 3] = (position.y == 2) ? inner[thisIndex] : inner[rightIndex];
	outer[thisIndex + 0] = (position.x == 0) ? inner[thisIndex] : inner[bottomIndex];
	outer[thisIndex + 1] = (position.y == 0) ? inner[thisIndex] : inner[leftIndex];
}


void TessellatedQuad::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	TextureManager::Inst()->BindTexture(0);
	glActiveTexture(GL_TEXTURE1);
	TextureManager::Inst()->BindTexture(1);

	glBindVertexArray(vaoID);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawElements(GL_PATCHES, indices.size(), GL_UNSIGNED_INT, (GLubyte*)NULL);
	glBindVertexArray(0);
}

void TessellatedQuad::genBuffers()
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(5, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), (GLvoid*)&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(0);  

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, texcoord.size() * sizeof(vec2), (GLvoid*)&texcoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(1);  

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, outer.size() * sizeof(unsigned char), outer.data(), GL_DYNAMIC_DRAW);
	glVertexAttribIPointer((GLuint)2, 1, GL_UNSIGNED_BYTE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(2);  

	glBindBuffer(GL_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ARRAY_BUFFER, inner.size() * sizeof(unsigned char), inner.data(), GL_DYNAMIC_DRAW);
	glVertexAttribIPointer((GLuint)3, 1, GL_UNSIGNED_BYTE, 0, (GLubyte*)NULL);
	glEnableVertexAttribArray(3);  

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), (GLvoid*)&indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void TessellatedQuad::resize(int x, int y)
{
	glViewport(0, 0, x, y);
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	projectionMatrix = glm::perspective(glm::radians(75.0f), static_cast<float>(w) / h, 0.1f, 100.0f);

}

void TessellatedQuad::genPatch(vec2 offset) {
	float patchSize = static_cast<float>(size) / numPatches;
	const float onethird = 1.0f / numPatches;

	auto addVertex = [&](float x, float y) {
		vertices.push_back(vec3(offset.x + x, 0, offset.y + y) * patchSize);
		texcoord.push_back(vec2(offset.x + x, offset.y + y) * onethird);
		};

	addVertex(0, 0); 
	addVertex(1, 0); 
	addVertex(1, 1); 
	addVertex(0, 1); 

	indices.push_back(vertices.size() - 4);
	indices.push_back(vertices.size() - 3);
	indices.push_back(vertices.size() - 2);
	indices.push_back(vertices.size() - 1);
}

void TessellatedQuad::genPlane() {
	for (int i = 0; i < numPatches; i++) {
		for (int j = 0; j < numPatches; j++) {
			genPatch(vec2(i, j));

			inner.push_back(0);
			inner.push_back(0);
			inner.push_back(0);
			inner.push_back(0);

			outer.push_back(0);
			outer.push_back(0);
			outer.push_back(0);
			outer.push_back(0);
		}
	}
}
