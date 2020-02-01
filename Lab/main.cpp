#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <cstdlib> // для system
using namespace std;

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/controls.hpp>

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		//getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1280, 960, "2_Treugolnika", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		//getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		//getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Dark blue background
	glClearColor(0.2f, 0.2f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around


	static const GLfloat g_vertex_buffer_data[] = {
		 0.0f, 0.0f, 0.0f,
		 1.0f, 0.0f, 1.0f,
		 0.0f, 0.0f, 1.0f,
		 0.0f, -0.0f, 0.0f,
		 1.0f, -0.0f, 1.0f,
		 1.0f, -0.0f, 0.0f,
		  0.0f, -0.9f, 0.0f,
		 1.0f, -0.9f, 1.0f,
		 0.0f, -0.9f, 1.0f,
		 0.0f, 0.8f, 0.0f,
		 1.0f, 0.8f, 1.0f,
		 1.0f, 0.8f, 0.0f,
		 0.0f, 0.81f, 0.0f,
		1.0f, 0.81f, 0.0f,
		 0.0f, 1.5f, 0.0f,
		
	};
	// Это будет идентификатором нашего буфера вершин
	GLuint vertexbuffer;
	// Создадим 1 буфер и поместим в переменную vertexbuffer его идентификатор
	glGenBuffers(1, &vertexbuffer);
	// Сделаем только что созданный буфер текущим
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Передадим информацию о вершинах в OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	int xw, yw;
	glfwGetWindowSize(window, &xw, &yw);
	cout << xw << " "<< yw << endl;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //скрыть курсор 
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
	glUseProgram(programID);

	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs();


	// Send our transformation to the currently bound shader, Отправить трансвофрмацию в шейдер 
		// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Указываем, что первым буфером атрибутов будут вершины
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // Атрибут 0. Подробнее об этом будет рассказано в части, посвященной шейдерам.
			3,                  // Размер
			GL_FLOAT,           // Тип
			GL_FALSE,           // Указывает, что значения не нормализованы
			0,                  // Шаг
			(void*)0            // Смещение массива в буфере
		);

		// Вывести треугольник!
		glDrawArrays(GL_TRIANGLES, 0, 15); // Начиная с вершины 0, всего 3 вершины -> один треугольник

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		//glfwGetCursorPos(window, &xpos, &ypos);
		//cout << xpos << " " << ypos << endl;

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}