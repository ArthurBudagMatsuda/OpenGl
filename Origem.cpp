#include <stdio.h>
#include <string.h>
#include <cmath>
#include<GL/glew.h>
#include<GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//window dimensions
const GLint WIDTH = 1920, HEIGHT = 1080;


GLuint VAO[2], VBO[2], shader, uniformMovel;
bool direction = true;
float trioOffSet = 0.0f;
float triMaxOffSet = 0.7f;
float triIncreament = 0.0005f;
 /* variable will be a variable which is constant over the whole flow.

We pass in one variable and that variable will be the same on every single instance or every single

time that shader runs for that set of coordinates. */
 /*//atribite variable  is  is distinct for each point that's passed through.

So each coordinate that gets this shader here will be run for each coordinate that's passed through

each of the points on the triangle.*/

//Vertex Shader

static const char* vShader = "															\n\
#version 330																			\n\
layout(location = 0)in vec3 pos;														\n\
uniform mat4 Model	;																    \n\
																						\n\
																						\n\
void main(){																			\n\
		gl_Position= Model * vec4(0.2* pos.x ,0.2* pos.y,pos.z,1.0);						\n\
}																						\n\
";
//fragment shader
static const char* fShader =  "															\n\
#version 330																			\n\
out  vec4 color;																		\n\
																						\n\
void main(){																		    \n\
		color= vec4(1.0,1.0,1.0,1.0);													\n\
}																						 \n\
";
void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f,-1.0f,0.0f, //vertice 1 / x -1.0f , y -1.0f , 0.0f
		1.0f,-1.0f,0.0f,//vertice 2 
		0.0f,1.0f,0.0f,//vertice 3
	};
	GLfloat secondTriangle[] = {
	-1.0f,-1.0f,0.0f, //vertice 1 / x -1.0f , y -1.0f , 0.0f
	1.0f,-1.0f,0.0f,//vertice 2 
	0.0f,1.0f,0.0f,//vertice 3
	};


	glGenVertexArrays(1, &VAO);// o graphics card ta criando um vertex array e rervando o espaco
	glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 9, vertices, GL_STATIC_DRAW);
			

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
		printf("Error linking program: ' %s' ", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error Validating program: ' %s' ", eLog);
		return;
	}

	uniformMovel = glGetUniformLocation(shader, "Model");
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
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

		if (direction) {
			trioOffSet += triIncreament;
		}
		else {
			trioOffSet -= triIncreament;
		}

		if (trioOffSet >= triMaxOffSet || trioOffSet <= -triMaxOffSet) {
			direction = !direction;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
			
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(trioOffSet, 0.0f, 0.0f));

		glUniformMatrix4fv(uniformMovel, 1, GL_FALSE, glm::value_ptr(model));
			
		glBindVertexArray(VAO);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		glUseProgram(0);
		glfwSwapBuffers(mainWindow);
		glfwPollEvents();

	}

	return 0;
}

