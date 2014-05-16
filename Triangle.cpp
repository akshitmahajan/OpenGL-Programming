#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h>
#include <GL\glfw3.h>
#include <glm\glm.hpp>

using namespace glm;

int main(int argc, char** argv){

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	// Open a window and create its OpenGL context
	GLFWwindow* window; // (In the accompanying source code, this variable is global)
	window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
	if( window == NULL ){

		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
 
	// Initialize GLEW
	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	//Generate Vertex Array Object VAO
	GLuint vaoId=6;
	glGenVertexArrays(1, &vaoId);
	fprintf(stdout, "%d", vaoId);
	glBindVertexArray(vaoId);

	static const GLfloat verts[] = {
		//X, Y, Z
		-1.0, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	//Create vertex buffer object VBO

	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
 
	do{
		glEnableVertexAttribArray(0);	//used for shaders
		//glBindBuffer(GL_ARRAY_BUFFER, vboID);		//used for shaders
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
 
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0 );

	return 1;

}