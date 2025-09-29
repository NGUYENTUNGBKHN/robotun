
#include <iostream>
#include <vector>
#include <cmath>

// !!! IMPORTANT: Include GLEW before other OpenGL/GLFW headers !!!
//#define GLEW_STATIC 
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

// Define PI
#define PI 3.14159265359f

// Window dimensions
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// --- SHADER CREATION FUNCTION ---
unsigned int createShader(const char* vertexShaderSource, const char* fragmentShaderSource) {
	// This function compiles and links the vertex and fragment shaders 
	unsigned int vertex, fragment;
	unsigned int program = glCreateProgram();

	// 1. Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex);

	// 2. Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment);

	// 3. Link Program
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}

// --- CIRCLE VERTEX DATA GENERATION FUNCTION ---
std::vector<float> createCircleVertices(float radius, int segments) {
	std::vector<float> vertices;

	// 1. Center point of the circle
	vertices.push_back(0.0f); // X
	vertices.push_back(0.0f); // Y
	vertices.push_back(0.0f); // Z

	// 2. Vertices along the circumference (using GL_TRIANGLE_FAN)
	for (int i = 0; i <= segments; i++) {
		// Current angle (from 0 to 2*PI radians)
		float angle = 2.0f * PI * ((float)i / (float)segments);

		// Convert polar coordinates (r, theta) to Cartesian (x, y)
		float x = radius * cos(angle);
		float y = radius * sin(angle);

		vertices.push_back(x); // X
		vertices.push_back(y); // Y
		vertices.push_back(0.0f); // Z
	}

	return vertices;
}
// -----------------------------------------------------------------

int main() {
	// 1. INITIALIZE AND CONFIGURE GLFW (Requesting Core Profile)
	if (!glfwInit())
		return -1;

	// Request OpenGL 3.3 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessary for macOS

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Circle with GLEW/VAO/VBO", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 2. INITIALIZE GLEW (THE LOADER)
	// glewExperimental = GL_TRUE; // Optional: Set flag for modern/experimental features

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
		return -1;
	}
	std::cout << "Using OpenGL Version: " << glGetString(GL_VERSION) << std::endl;


	// 3. SHADERS (Inline source for simplicity)
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.2f, 0.8f, 0.2f, 1.0f); // Green color\n"
		"}\0";

	unsigned int shaderProgram = createShader(vertexShaderSource, fragmentShaderSource);

	// 4. PREPARE VERTEX DATA AND VAO/VBO
	float radius = 0.5f;
	int segments = 60;
	std::vector<float> circleVertices = createCircleVertices(radius, segments);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO first, then configure VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy vertex data to VBO
	glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

	// Configure Vertex Attributes (Location 0: Position)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind (Good practice)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 5. MAIN RENDER LOOP
	while (!glfwWindowShouldClose(window)) {
		// Clear screen with a gray background
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the circle
		glUseProgram(shaderProgram); // Use our custom shader
		glBindVertexArray(VAO);

		// Draw using GL_TRIANGLE_FAN (segments + 2 vertices total: 1 center + circumference points)
		glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);

		// Swap buffers and process events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// 6. CLEANUP
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
