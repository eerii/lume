//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "shader.h"

using namespace Verse;

#ifndef __APPLE__
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLVALIDATEPROGRAMPROC glValidateProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLUSEPROGRAMPROC glUseProgram;

bool Graphics::Shader::initGLExtensions() {
    glCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
    glDeleteShader = (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");
    glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
    glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)SDL_GL_GetProcAddress("glValidateProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
    glUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");

    return glCreateShader && glShaderSource && glCompileShader && glGetShaderiv &&
        glGetShaderInfoLog && glDeleteShader && glAttachShader && glCreateProgram &&
        glLinkProgram && glValidateProgram && glGetProgramiv && glGetProgramInfoLog &&
        glUseProgram;
}
#endif

ui8 Graphics::Shader::compileShader(const char* source, ui32 shaderType) {
    //log::info("Compiling Shader...");
    
    //CREATE SHADER FROM SOURCE
    ui8 id = glCreateShader(shaderType);
    glShaderSource(id, 1, &source, NULL);
    //COMPILE SHADER
    glCompileShader(id);

    //ERROR HANDLING
    int shaderCompiled = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &shaderCompiled);
    if(shaderCompiled != GL_TRUE) {
        log::error("Shader Compilation Error, ID: %d", id);
        int logLen;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char *log = (char*)malloc(logLen);
            glGetShaderInfoLog(id, logLen, &logLen, log);
            
            log::error("Shader compile log: %s", log);
            free(log);
        }
        glDeleteShader(id);
        id = 0;
    } else {
        log::info("Shader Compiled Correctly, ID: %d", id);
    }
    
    return id;
}

ui8 Graphics::Shader::compileProgram(str vertexFile, str fragmentFile) {
    //log::info("Compiling Program...");
    
    ui8 program = 0;
    ui8 vertexShader, fragmentShader;

    program = glCreateProgram();

    //COMPILE VERTEX SHADER
    std::ifstream f(vertexFile.c_str());
    std::string source((std::istreambuf_iterator<char>(f)),
                        std::istreambuf_iterator<char>());
    vertexShader = compileShader(source.c_str(), GL_VERTEX_SHADER);
    
    //COMPILE FRAGMENT SHADER
    f=std::ifstream(fragmentFile.c_str());
    source=std::string((std::istreambuf_iterator<char>(f)),
                        std::istreambuf_iterator<char>());
    fragmentShader = compileShader(source.c_str(), GL_FRAGMENT_SHADER);

    //ATTACH TO PROGRAM
    if(vertexShader && fragmentShader) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        glValidateProgram(program);

        int logLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        if(logLen > 0) {
            char* log = (char*)malloc(logLen * sizeof(char));
            glGetProgramInfoLog(program, logLen, &logLen, log);
            
            log::error("Program compile log: %s", log);
            free(log);
        }
    }
    
    //COULD DELETE THE SHADERS
    /*if(vertexShader)
        glDeleteShader(vertexShader);
    if(fragmentShader)
        glDeleteShader(fragmentShader);*/
    
    return program;
}
