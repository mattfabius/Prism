#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb/stb_image.h"
#include <assimp/Importer.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Utils.h"
#include "Collider.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void resolveCollisions(Camera &cam, Collider colliders[10]);
unsigned int loadTexture(const char* filePath);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

float mixPercent = 0.2f;

Camera camera;
glm::vec3 cameraStartPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

bool firstMouse = true;
float mouseLastX = 400, mouseLastY = 300;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// initilize GLFW and set to OpenGL version 3.3 using OpenGL's core profile (not backward compatible)
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// try to create a window that's 800x600px named "Prism" and set as current context or error if failed
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Prism", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// register function to be called when the user changes the window's size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// load OS-specific addresses of OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// enable depth testing -- show objects closer to camera occluding objects further from camera
	glEnable(GL_DEPTH_TEST);

	Shader defaultShader("default.vert", "default.frag");
	Shader lightingShader("phong.vert", "phong.frag");
	//Shader waterShader("test.vert", "test.frag");
	Shader lightSourceShader("phong.vert", "lightSource.frag");
	camera = Camera(cameraStartPos, true);

	/*
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f, -0.5f,  -0.5f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // back right	0
		 0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // front right	1
		-0.5f, -0.5f,  -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // back left	2
		-0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 2.0f,   // front left	3
		 0.0f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f	// top			4
	};
	unsigned int indices[] = {
		0, 1, 2, // first triangle
		1, 2, 3,  // second triagle
		3, 4, 1,
		0, 4, 1,
		2, 4, 0,
		3, 4, 2
	};
	*/

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 2, // first triangle
		1, 2, 3  // second triangle
	};

	float groundVerts[] = {
		// position			//normals			// texture coords
		-1.0f, 0.0f, -1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f, //back left	0
		-1.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f, //front left	1
		1.0f, 0.0f, -1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f, //back right	2
		1.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // front right	3
	};
	
	// generate new buffer and store id in VBO, vertex buffer object
	unsigned int VBOs[2], EBO, VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAOs[0]);

	// set VBO as active buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	// copy vertex data to active array buffer (VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// main cube
	// vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal vector attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ground plane
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVerts), groundVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind VAO to not accidentally modify
	glBindVertexArray(0);

	// we can unbind VBO and EBO as active buffers since it's stored as the bound buffer in VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	unsigned int diffuseMap = loadTexture("container2.png");
	glActiveTexture(GL_TEXTURE1);
	unsigned int specularMap = loadTexture("container2_specular.png");

	lightingShader.Activate();
	//lightingShader.SetVec3("objectColor", 0.75, 0.25, 0.1);
	//lightingShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	lightingShader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
	lightingShader.SetFloat("material.shininess", 32.0f);
	lightingShader.SetInt("material.diffuse", 0); // 0 corresponds to GL_TEXTURE0 - diffuseMap
	lightingShader.SetInt("material.specular", 1); // 1 corresponds to GL_TEXTURE1 - specularMap

	lightingShader.SetFloat("light.constant", 1.0f);
	lightingShader.SetFloat("light.linear", 0.045f);
	lightingShader.SetFloat("light.quadratic", 0.0075f);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	Collider boxColliders[10];

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	glm::vec3 pointLightColor(0.5f);
	for (int i = 0; i < 4; i++)
	{
		std::string attribute = "pointLights[" + std::to_string(i);
		attribute += "].";
		lightingShader.SetFloat(attribute + "constant", 1.0f);
		lightingShader.SetFloat(attribute + "linear", 0.22f);
		lightingShader.SetFloat(attribute + "quadratic", 0.2f);

		lightingShader.SetVec3(attribute + "position", pointLightPositions[i]);

		lightingShader.SetVec3(attribute + "ambient", pointLightColor * 0.2f);
		lightingShader.SetVec3(attribute + "diffuse", pointLightColor * 0.5f);
		lightingShader.SetVec3(attribute + "specular", pointLightColor);
	}

	glm::vec3 dirLightColor(0.25f);
	lightingShader.SetVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
	lightingShader.SetVec3("directionalLight.ambient", dirLightColor * 0.2f);
	lightingShader.SetVec3("directionalLight.diffuse", dirLightColor * 0.5f);
	lightingShader.SetVec3("directionalLight.specular", dirLightColor);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);
		resolveCollisions(camera, boxColliders);

		// rendering
		//glClearColor(0.2f, 0.35f, 0.65f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.Activate();

		// camera
		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();

		glm::mat4 projection = glm::perspective(glm::radians(camera.fieldOfView), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		lightingShader.SetMat4("view", view);
		lightingShader.SetMat4("projection", projection); //this rarely changes so could be set outside the render loop
		lightingShader.SetVec3("viewPos", camera.position);

		lightingShader.SetVec3("light.position", camera.position);
		lightingShader.SetVec3("light.direction", camera.forward);
		lightingShader.SetFloat("light.innerCutoff", glm::cos(glm::radians(12.5f)));
		lightingShader.SetFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)));

		glm::vec3 lightColor(1.0f, 1.0f, 0.85f);
		lightingShader.SetVec3("light.ambient", lightColor * 0.2f);
		lightingShader.SetVec3("light.diffuse", lightColor * 0.5f);
		lightingShader.SetVec3("light.specular", lightColor);

		glBindVertexArray(VAOs[0]);
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// cubes
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			boxColliders[i] = Collider(cubePositions[i], glm::vec3(1.0f));
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.SetMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// ground plane
		/*glBindVertexArray(VAOs[1]);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		lightingShader.SetMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

		// light source
		lightSourceShader.Activate();
		lightSourceShader.SetVec3("sourceColor", pointLightColor);
		glBindVertexArray(lightVAO);
		
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 lightModel = glm::mat4(1.0f);
			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			lightSourceShader.SetMat4("model", lightModel);
			lightSourceShader.SetMat4("view", view);
			lightSourceShader.SetMat4("projection", projection);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glBindVertexArray(0);

		// swap buffers and poll input events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteBuffers(1, &EBO);
	defaultShader.Delete();

	// destory windows, free up allocated memory, and revert GLFW to uninitialized state
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//set the OpenGL viewport size to the size of the GLFW window whenever the user resizes
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		mouseLastX = xpos;
		mouseLastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - mouseLastX;
	float yOffset = mouseLastY - ypos;
	mouseLastX = xpos;
	mouseLastY = ypos;
	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	camera.Rotate(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.fieldOfView -= (float)yoffset;
	camera.fieldOfView = clamp(camera.fieldOfView, 1.0f, 45.0f);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// for debugging, toggle between fill and wireframe mode with 1 and 2 keys
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
	{
		mixPercent = clamp(mixPercent + 0.01f, 0.0f, 1.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
	{
		mixPercent = clamp(mixPercent - 0.01f, 0.0f, 1.0f);
	}

	const float cameraSpeed = 3.5f * deltaTime;
	glm::vec3 moveDirection(0.0f);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveDirection += glm::vec3(0.0f, 0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveDirection += glm::vec3(0.0f, 0.0f, -1.0f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveDirection += glm::vec3(1.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveDirection += glm::vec3(-1.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && camera.isFlightCam)
		moveDirection += glm::vec3(0.0f, -1.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && camera.isFlightCam)
		moveDirection += glm::vec3(0.0f, 1.0f, 0.0f);

	if (moveDirection != glm::vec3(0.0f))
		camera.Move(glm::normalize(moveDirection) * cameraSpeed);
}

void resolveCollisions(Camera &cam, Collider colliders[10]) 
{
	for (int i = 0; i < 10; i++) {
		if (cam.collider.IsColliding(colliders[i]))
		{
			glm::vec3 overlapAmount = cam.collider.GetOverlapAmount(colliders[i]);
			cam.MoveWorldPosition(-overlapAmount);
		}
	}
}

unsigned int loadTexture(const char* filePath)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nChannels;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath, &width, &height, &nChannels, 0);

	if (data)
	{
		GLenum format = GL_RED;
		if (nChannels == 3)
			format = GL_RGB;
		else if (nChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture: " << filePath << std::endl;
	}

	stbi_image_free(data);
	return textureID;
}