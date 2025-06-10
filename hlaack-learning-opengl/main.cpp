#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//FUNCTION TEMPLATES
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//UNCHANGING INT TYPES THAT CAN ONLY STORE POSITIVE VALUES (large possible values than a signed int)
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
	glfwInit();
	//SET UPPER AND LOWER VERSION LIMITS
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//CREATE WINDOW VARIABLE
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) { //if we have no window
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	//WHILE THE WINDOW IS OPEN (while bool WindowShouldClose is not true!)
	while (!glfwWindowShouldClose(window)) {
		
		//HANDLE INPUT
		processInput(window);
		
		//RENDERING
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//HANDLE EVENTS AND SWAP BUFFERS
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//CLEAN UP WINDOW
	glfwTerminate();
	return 0;
}

//THIS FUNCTION IS UTILIZED BY glfwSetFramebufferSizeCallback TO CHECK UPDATES TO WINDOW SIZE
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//THIS FUNCTION HANDLES ANY INPUT AND IS CALLED CONCURRENTLY WHILE THE WINDOW EXISTS
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}