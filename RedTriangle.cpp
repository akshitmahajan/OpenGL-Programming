#include <stdio.h>
#include <stdlib.h>
#include <GL\glew.h>
#include <GL\glfw3.h>
#include <glm\glm.hpp>

#include <string>
#include <fstream>

//using namespace glm;

// For Loading vertex files
std::string LoadFileToString(const char* filepath){

	std::string fileData;
	std::ifstream stream(filepath, std::ios::in);
	
	if(stream.is_open()){
	
		std::string line="";

		while(getline(stream, line)){
			fileData += "\n" + line;
		}
		fprintf(stdout, "\n ------------------ File Loaded -------------------\n");

		stream.close();
	}
	return fileData;
}


// Loading Shaders: There are 2 types of shaders: Vertex Shaders and Fragment Shaders
GLuint LoadShaders(const char* vertShaderPath, const char* fragShaderPath){
	
	fprintf( stdout, " -------------------------- Loading Shaders ----------------------\n" );

	GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;;
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertShaderSource = LoadFileToString(vertShaderPath);
	std::string fragShaderSource = LoadFileToString(fragShaderPath);

	const char* rawVertShaderSource = vertShaderSource.c_str();
	const char* rawFragShaderSource = fragShaderSource.c_str();

	glShaderSource(vertShader, 1, &rawVertShaderSource, NULL);
	glShaderSource(fragShader, 1, &rawFragShaderSource, NULL);

	glCompileShader(vertShader);
	
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compile_ok);
	if (0 == compile_ok)
	{
		fprintf(stderr, "Error in vertex shader\n");
		return 0;
	}

	glCompileShader(fragShader);
	
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compile_ok);
	if (!compile_ok) {
    fprintf(stderr, "Error in fragment shader\n");
    return 0;
	}
		
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		return 0;
	}

	return program;
}


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
	//glfwWindowHint(GLFW_OPENGL_PROFILE,e GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	// Open a window and create its OpenGL context
	GLFWwindow* window;
	window = glfwCreateWindow( 1024, 768, "First Triangle", NULL, NULL);
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

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Generate Vertex Array Object VAO
	/*GLuint vaoId=6;
	glGenVertexArrays(1, &vaoId);
	fprintf(stdout, "%d", vaoId);
	glBindVertexArray(vaoId);*/

	// Create and compile our GLSL program from the shaders
	GLuint program = LoadShaders("shader.vertshader","shader.fragshader"); 

	// Get a handle for our buffers
	const char* attribute_name = "vertexPosition_modelspace";
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(program, attribute_name);
	
	if (vertexPosition_modelspaceID == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
    }

	static const GLfloat verts[] = {
		//X, Y, Z
		-1.0, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, .5f, 0.0f
	};

	//Create vertex buffer object VBO
	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
 
	do{
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(program);

		// used for shaders, 1rst attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);	
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
 
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	// Cleanup VBO
	glDeleteBuffers(1, &vboID);

	return 1;

}