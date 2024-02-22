#include <stdio.h>
#include <string.h>
#include<GL/glew.h>
#include<GLFW/glfw3.h>


//window dimensions
const GLint WIDTH = 800, HEIGHT = 600l;

GLuint VAO, VBO, shader;

//Vertex Shader
static const char* vShader = "															\n\
#version 330																			\n\
layout(location = 0)in vec3 pos;														\n\
																						\n\
void main(){																			\n\
		gl_Position= vec4(0.2* pos.x,0.2* pos.y,pos.z,1.0);								\n\
}																						\n\
";
//fragment shader
static const char* fShader =  "															\n\
#version 330																			\n\
out  vec4 color;																		\n\
																						\n\
void main(){																		    \n\
		color= vec4(1.0,0.0,0.0,1.0);													\n\
}																						 \n\
";
void CreateTriangle() 
{
	GLfloat vertices[] = {
		-1.0f,-1.0f,0.0f, //vertice 1 / x -1.0f , y -1.0f , 0.0f
		1.0f,-1.0f,0.0f,//vertice 2 
		0.0f,1.0f,0.0f//vertice 3
	};

	glGenVertexArrays(1, &VAO);// o graphics card ta criando um vertex array e rervando o espaco
	glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);  
			glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void AddShader(GLuint TheProgram,const char*shaderCode,GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codelenght[1];
	codelenght[0] = strlen(shaderCode);


	glShaderSource(theShader, 1, theCode, codelenght);
	glCompileShader(theShader);	


	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog (theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: ' %s' ",shaderType, eLog);
		return;
	}
	glAttachShader(TheProgram, theShader);
}

void CompileShader() {
	shader = glCreateProgram();
	if (!shader) {
		printf("error creating shader");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: ' %s' ",eLog);
			return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error Validating program: ' %s' ", eLog);
		return;
	}
}
int main() {

	if (!glfwInit()) {

		printf("Glfw initialization failde");
		glfwTerminate();
		return 1;
	}

	//SETUP GLFW WINDOW PROPRIETIES
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	//seting versio = 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	//core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//ALLow forward compatiblity
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "TRIANGLE", NULL, NULL);
	if (!mainWindow) {
		printf("GLFWwindow Creation failed");
		glfwTerminate();
		return 1;
	}


	//buffer size information ( area in the middle of the window)
	int bufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow,&bufferWidth,&BufferHeight);

	//set context for glew to use ( mostrar para o glew q o opengl ta ligado)

	glfwMakeContextCurrent(mainWindow); // Vai botar o contexto para o mainWindow

	//Allow modern extension features 
	glewExperimental = GL_TRUE;

	if (glewInit()) {
		printf("Glew initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	};

	//setup viewport size

	glViewport(0, 0, bufferWidth, BufferHeight);

	//loop until window close

	CreateTriangle();
	CompileShader();


	while (!glfwWindowShouldClose(mainWindow)) {
		//get+ handle user input events
		glfwPollEvents();

		//clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);

			glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);

		glUseProgram(0);
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}

