#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//FUNCTION TEMPLATES
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//UNCHANGING INT TYPES THAT CAN ONLY STORE POSITIVE VALUES (large possible values than a signed int)
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//GLSL FUNCTION FOR VERTEX SHADER, STORED IN C CONST
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

//GLSL FUNCTION FOR FRAGMENT SHADER, STORED IN C CONST
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

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

	//CREATE SHADER OBJECT for VERTEX SHADER
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //STORE SHADER IN UNSIGNED INT, createShader takes type of shader as arg
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //ATTACH SHADER SOURCE TO SHADER OBJECT (takes shader obj, num strings, and source code for shader as args)
	glCompileShader(vertexShader); //FINALLY COMPILE THE SHADER

	//LOOK FOR SHADER ERRORS
	int success; //DEFINE SUCCESS
	char infoLog[512]; //STORE ERROR MESSAGE
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	//CREATE SHADER OBJECT for FRAGMENT SHADER
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //STORE SHADER IN UNSIGNED INT
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //ATTACH SHADER SOURCE TO SHADER OBJECT
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	//LINKING SHADERS
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); //CREATE PROGRAM OBJECT

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); //ATTACH THE TWO SHADERS THEN LINK THEM TO PROGRAM
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::SHADER::COMPILATION::FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader); //AFTER ATTACHING TO PROGRAM, SHADERS ARE NO LONGER NEEDED
	glDeleteShader(fragmentShader);

	//CREATE NORMALIZED COORDINATES FOR TRIANGLE
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	
	unsigned int VBO;
	glGenBuffers(1, &VBO); //CREATE VERTEX BUFFER OBJECT TO STORE LARGE AMOUNT OF VERTEX INFORMATION

	unsigned int VAO;
	glGenVertexArrays(1, &VAO); //CREATE VERTEX ARRAY OBJECT TO STORE VERTEX ATTRIBUTE CALLS

	//1. BIND VA OBJECT
	glBindVertexArray(VAO); //BIND VERTEX ARRAY OBJECT
	
	//2. COPY VERT ARRAY INTO A BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //BIND BUFFER ARRAY OBJECT
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //COPY AND SUBMIT USER-DEFINED DATA TO BUFFER

	//3. SET VERT ATTRIBUTE POINTERS
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //TELL OPENGL HOW TO INTERPRET VERTEX DATA. 1st arg is location of vertex shader, 2nd arg is size (vec3), 3rd arg specifies type, 4th arg bool for normalization, 5th arg is stride, last is offset for beginning of position data. 
	glEnableVertexAttribArray(0); //THEN ENABLE VERTEX ATTRIBUTE (THEY ARE DISABLED BY DEFAULT)

	//WHILE THE WINDOW IS OPEN (while bool WindowShouldClose is not true!)
	while (!glfwWindowShouldClose(window)) {
		
		//HANDLE INPUT
		processInput(window);
		

	//4. DRAW OBJECT
		//RENDERING
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//THEN PASS OPENGL DRAW FUNCTION
		glDrawArrays(GL_TRIANGLES, 0, 3); // 1st arg is primitive type to draw, 2nd arg tells starting index, 3rd arg specifies amt of verts


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