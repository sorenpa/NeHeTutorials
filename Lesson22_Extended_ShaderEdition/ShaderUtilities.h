/*******************************************************************
Written by: Soren Peter Andersen
email: soren.p.andersen@gmail.com

Description: Header file for simple shader loading functions
*******************************************************************/

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <stdio.h>

//This funtion reads a file and returns a pointer to a char array of its contents
static char* ReadFile(const char*filename);

//Prints the shader log
//TODO print the logs to a file
void printShaderInfoLog(GLuint obj);

//Prints the program log
//TODO print the logs to a file
void printProgramInfoLog(GLuint obj);

//This function takes the filename of a shader program, and generates a
//shader of type shaderType. The shader ID handle is returned in param shaderId.
//The funtion returns true if the generation of the shader succeeds
bool GenerateShader(const char* shaderFileName, GLenum shaderType, GLuint &shaderId);


//Function that links a vertex and fragmentshader together in a single
//shader progra. The program ID handle is returned in param programId.
//The funtion returns true if the generation of the program succeeds.
bool GenerateShaderProgram(GLuint vertexShaderId, GLuint FragmentShaderId, GLuint &programID);

