/*******************************************************************
Written by: Soren Peter Andersen
email: soren.p.andersen@gmail.com

Description: Implementation of the shader utility functions from
ShaderUtilities.h
*******************************************************************/

#include "ShaderUtilities.h"

static char* ReadFile(const char*fileName)
{
    //Create a filepointer fp, and open the file "fileName" for reading
    FILE * fp;
    
    

    //try to open the file for reading
    if(fopen_s(&fp,fileName,"r") == 0)
    {
        //find the length of the file
        fseek(fp,0,SEEK_END);
        long length = ftell(fp); 
        fseek(fp,0,SEEK_SET);

        char* contents = new char[length+1];

        //initialize the conents array
        for(int i=0;i<length+1;i++)
        {
            contents[i] = 0;
        }

        //Reed the file into the conents array
        fread(contents,1,length,fp);

        contents[length+1] = '\0'; //End of file char

        fclose(fp);

        return contents;
    }

    //if we cannot open the file return 0
    return 0;
}

bool GenerateShader(const char* shaderFileName, GLenum shaderType,GLuint &shaderId)
{
    //Create a shader of shaderType and store the ID in shaderId
    shaderId = glCreateShader(shaderType);
    //Read the shader source file
    char* shaderSource = ReadFile(shaderFileName);
    //Bind the contents of the shader source file to the shader
    if(shaderSource != 0)
    {
        glShaderSource(shaderId, 1, (const GLchar **)&shaderSource, NULL);
    
        glCompileShader(shaderId);

        //Check if compilation went well
        int status;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

        if(status == GL_TRUE)
        {
            return true;
        }
        else
        {
            printShaderInfoLog(shaderId);
        }
    }
    return false;
}


bool GenerateShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId, GLuint &programId)
{
    //Create a shader program and store the ID in programId
    programId = glCreateProgram();
    //Attach the vertex shader to the program
    glAttachShader(programId,vertexShaderId);
    //Attach the fragment shader to the program
    glAttachShader(programId,fragmentShaderId);
    //link the shaders in one program
    glLinkProgram(programId);

    //Check if program linking went well
    int status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if(status == GL_TRUE)
    {
        return true;
    }
    else
    {
        printProgramInfoLog(programId);
    }

    return false;
}

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
    printf("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}