/****
	Project created by Immortality (6/14/20)
	skybox: https://opengameart.org/content/galaxy-skybox
	Textures: https://www.solarsystemscope.com/textures/,
	https://www.universetoday.com/wp-content/uploads/2013/10/milky_way.jpg
****/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "OrbitCamera.h"
#include "Model.h"

#define PI 3.14159
#define CONFIG_FILE "config.ini"

int WIDTH = 0;
int HEIGHT = 0;
bool isFullscreen;

void load_config(bool& fullscreen, int& width, int& height);
void setCallbacks(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint loadCubemap(std::vector<std::string> faces);

// camera
OrbitCamera camera;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
#pragma region INITIALIZATION

	load_config(isFullscreen, WIDTH, HEIGHT);

	if (!glfwInit())
	{
		std::cerr << "GLFW Init failed!" << std::endl;
		glfwTerminate();
		return 0;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window;
	if (isFullscreen == true)
	{
		window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System",
			glfwGetPrimaryMonitor(), nullptr);
	}
	else
	{
		window = glfwCreateWindow(WIDTH, HEIGHT, "Solar System",
			nullptr, nullptr);
	}

	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);
	setCallbacks(window); //setup glfw window callbacks

	GLFWimage images[1];
	images[0].pixels = stbi_load("res/img/milky_way.jpg", &images[0].width, &images[0].height, 0, 4); //rgba channels 
	if (images[0].pixels == nullptr)
	{
		std::cerr << "Failed to load icon" << std::endl;
		stbi_image_free(images[0].pixels);
	}
	else
	{
		glfwSetWindowIcon(window, 1, images);
		stbi_image_free(images[0].pixels);
	}
	
	GLenum err = glewInit(); //glewInit returns glenum
	if (GLEW_OK != err)
	{
		std::cerr << "GLEW ERROR: " << glewGetErrorString(err) << "\n";
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);
#pragma endregion INITIALIZATION

	std::vector<std::string> faces =
	{
		"res/skybox/right.jpg",
		"res/skybox/left.jpg",
		"res/skybox/top.jpg",
		"res/skybox/bottom.jpg",
		"res/skybox/front.jpg",
		"res/skybox/back.jpg"
	};

	GLuint skyboxTexture = loadCubemap(faces);
	glEnable(GL_DEPTH_TEST);

#pragma region SPHERE

	
#pragma endregion SPHERE


	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	float triangleVertices[] = 
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
	};
	// triangle placeholder
	GLuint planetVAO, planetVBO;
	glGenVertexArrays(1, &planetVAO);
	glGenBuffers(1, &planetVBO);

	glBindVertexArray(planetVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planetVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// end
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBindVertexArray(0);

	Shader skyboxShader("res/shader/skybox.vs", "res/shader/skybox.fs");
	Shader planetShader("res/shader/planetVertex.glsl", "res/shader/planetFragment.glsl");
	
	Model sun("res/planets/sun/sphere.obj");
	Model mercury("res/planets/mercury/sphere.obj");
	Model venus("res/planets/venus/sphere.obj");
	Model earth("res/planets/earth/sphere.obj");
	Model moon("res/planets/moon/sphere.obj");
	Model mars("res/planets/mars/sphere.obj");
	Model jupiter("res/planets/jupiter/sphere.obj");
	Model saturn("res/planets/saturn/sphere.obj");
	Model uranus("res/planets/uranus/sphere.obj");
	Model neptune("res/planets/neptune/sphere.obj");
	
	

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		
		planetShader.use();

		//sun
		glm::mat4 model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(5, 5, 5));
		model = glm::translate(model, glm::vec3(0,0,0));
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		sun.Draw(planetShader);
		//mercury
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1, 1, 1));
		model = glm::translate(model, glm::vec3(0 + 7, 0, 0));
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		mercury.Draw(planetShader);

		//venus
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
		model = glm::translate(model, glm::vec3(0 + 8.5, 0, 0));
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		venus.Draw(planetShader);

		//earth
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
		model = glm::translate(model, glm::vec3(0 + 11, 0, 0));
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		earth.Draw(planetShader);

		//moon
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(0 + 32, 0, 0));
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		moon.Draw(planetShader);

		//mars
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(1, 1, 1));
		model = glm::translate(model, glm::vec3(0 + 18, 0, 0));
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		mars.Draw(planetShader);

		//jupiter
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(3, 3, 3));
		model = glm::translate(model, glm::vec3(0 + 8, 0, 0));
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		jupiter.Draw(planetShader);

		//saturn
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(3, 3, 3));
		model = glm::translate(model, glm::vec3(0 + 11, 0, 0));
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		saturn.Draw(planetShader);

		//uranus
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2, 2, 2));
		model = glm::translate(model, glm::vec3(0 + 21, 0, 0));
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		uranus.Draw(planetShader);

		//neptune
		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2, 2, 2));
		model = glm::translate(model, glm::vec3(0 + 24, 0, 0));
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		neptune.Draw(planetShader);

		/*planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		glBindVertexArray(planetVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);*/

		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	glDeleteVertexArrays(1, &planetVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &planetVBO);
	glDeleteBuffers(1, &skyboxVBO);

	return 0;
}

void load_config(bool& fullscreen, int& width, int& height)
{
	std::ifstream file;
	std::string line;

	try
	{
		file.open(CONFIG_FILE);
		if (!file.is_open())
			throw std::ios::failure("The config.ini file has been misplaced. Please reinstall the application.");
		
		int index = 1;
		while (std::getline(file, line))
		{
			switch (index)
			{
			case 2:
			{
				int boolInt = std::stoi(line);
				if (boolInt == 0)
					fullscreen = false;
				else
					fullscreen = true;
			}
				break;
			case 4:
				width = std::stoi(line);
				break;
			case 6:
				height = std::stoi(line);
				break;
			}
			index++;
		}
		file.close();
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (std::invalid_argument const& e)
	{
		std::cerr << "(INFILE)Bad input: std::invalid_argument thrown" << '\n';
	}
	catch (std::out_of_range const& e)
	{
		std::cerr << "(INFILE_Integer overflow: std::out_of_range thrown" << '\n';
	}
}

void setCallbacks(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

GLuint loadCubemap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		//stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cerr << "Cubemap tex failed to load, path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//stbi_set_flip_vertically_on_load(false);
	return textureID;
}
