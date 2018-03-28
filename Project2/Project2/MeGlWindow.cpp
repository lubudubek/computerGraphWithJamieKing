


//#include "stdafx.h"
#include "GL/glew.h"
#include "MeGlWindow.h"
#include <iostream>
#include <fstream>
using namespace std;

const float X_DELTA = 0.1f;
const uint NUM_VERTCES_PER_TRI = 3;
const uint FLOAT_PER_VERTICE = 6;
const uint TRANGLE_BYTE_SIZE = FLOAT_PER_VERTICE * NUM_VERTCES_PER_TRI * sizeof(float) ;
const uint MAX_TRIS = 16;
uint numTris = 0;

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "file failed to load!";
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

bool checkStatus(GLuint objectId,
	PFNGLGETSHADERIVPROC objectGeter,
	PFNGLGETSHADERINFOLOGPROC getnfoLogFunc,
	GLenum statusType)
{
	GLint linkStatus;
	objectGeter(objectId, statusType, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint infoLogLength;
		objectGeter(objectId, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getnfoLogFunc(objectId, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderId)
{
	return checkStatus(shaderId, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProagramStatus(GLuint programId)
{
	return checkStatus(programId, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}



void sendDataToOpenGl()
{
	const float RED_TRIANGLE_Z = +0.5f;
	const float GREEN_TRIANGLE_Z = -0.5f;
	GLfloat verts[] =
	{
		+0.0f, +1.0f, RED_TRIANGLE_Z,
		+1.0f, +0.0f, +0.0f,
		+1.0f, -1.0f, RED_TRIANGLE_Z,
		+1.0f, +0.0f, +0.0f,
		-1.0f, -1.0f, RED_TRIANGLE_Z,
		+1.0f, +0.0f, +0.0f,

		+0.0f, -1.0f, GREEN_TRIANGLE_Z,
		+0.0f, +1.0f, +0.0f,
		+1.0f, +1.0f, GREEN_TRIANGLE_Z,
		+0.0f, +1.0f, +0.0f,
		-1.0f, +1.0f, GREEN_TRIANGLE_Z,
		+0.0f, +1.0f, +0.0f,
	};
	GLuint myBufferId;
	glGenBuffers(1, &myBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, myBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	GLushort indexes[] = { 0,1,2, 3, 4, 5 };
	GLuint myIndexBufferId;
	glGenBuffers(1, &myIndexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIndexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
}

void sendAnotherTriToOpenGl()
{
	if (numTris == MAX_TRIS)
	{
		return;
	}
	const GLfloat THIS_TRI_X = -1.0f +numTris *X_DELTA;
	GLfloat thisTri[] =
	{
		THIS_TRI_X, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		THIS_TRI_X + X_DELTA, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		THIS_TRI_X, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};
	//GLfloat thisTri[] =
	//{
	//	-1.0f, 1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,

	//	-0.9f, 1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,

	//	-1.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//};
	glBufferSubData(GL_ARRAY_BUFFER,
		numTris * TRANGLE_BYTE_SIZE, TRANGLE_BYTE_SIZE, thisTri);
	numTris++;
}

void MeGlWindow::paintGL()
{
	//	glClearColor(1, 0, 0, 1);
	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height());
		glDrawArrays(GL_TRIANGLES, 0, 6);
	//sendAnotherTriToOpenGl();
	//glDrawArrays(GL_TRIANGLES, (numTris - 1) * NUM_VERTCES_PER_TRI, NUM_VERTCES_PER_TRI);
}

void installShaders()
{
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	string vertexShaderCode = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = vertexShaderCode.c_str();//vertexShaderCode;
	glShaderSource(vertexShaderId, 1, adapter, 0);

	string fragmentShaderCode = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderId, 1, adapter, 0);

	glCompileShader(vertexShaderId);
	glCompileShader(fragmentShaderId);
	
	if (!checkShaderStatus(vertexShaderId) || !checkShaderStatus(fragmentShaderId))
		return;

	GLuint glProgramId = glCreateProgram();
	glAttachShader(glProgramId, vertexShaderId);
	glAttachShader(glProgramId, fragmentShaderId);

	glLinkProgram(glProgramId);

	if (!checkProagramStatus(glProgramId))
		return;
		
	glUseProgram(glProgramId);
}



void MeGlWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGl();
	installShaders();
}
