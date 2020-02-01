#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

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
	window = glfwCreateWindow(1900, 900, "2_Treugolnika", NULL, NULL);
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

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	static const GLfloat g_vertex_buffer_data[] = {
		-0.7f, -0.9f, 0.0f,
		 0.9f, -0.9f, 0.0f,
		 0.1f,  0.94f, 0.0f,
		 -0.7f, -0.9f, 0.0f,
		  -0.01f,  0.8f, 0.0f,
		 0.08f,  0.9f, 0.0f,
		
	};
	// Это будет идентификатором нашего буфера вершин
	GLuint vertexbuffer;
	// Создадим 1 буфер и поместим в переменную vertexbuffer его идентификатор
	glGenBuffers(1, &vertexbuffer);
	// Сделаем только что созданный буфер текущим
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Передадим информацию о вершинах в OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
	glUseProgram(programID);

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
		glDrawArrays(GL_TRIANGLES, 0, 6); // Начиная с вершины 0, всего 3 вершины -> один треугольник

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}