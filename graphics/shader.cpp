//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "shader.h"

using namespace Verse;

#ifndef __APPLE__

// I'm avoiding the use of GLEW or some extensions handler, but that
// doesn't mean you should...
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

GLuint Graphics::Shader::compileShader(const char* source, GLuint shaderType) {
    log::info("Compiling Shader...");
    //std::cout << "Compilando shader:" << std::endl << source << std::endl;
    // Create ID for shader
    GLuint result = glCreateShader(shaderType);
    // Define shader text
    glShaderSource(result, 1, &source, NULL);
    // Compile shader
    glCompileShader(result);

    //Check vertex shader for errors
    GLint shaderCompiled = GL_FALSE;
    glGetShaderiv( result, GL_COMPILE_STATUS, &shaderCompiled );
    if( shaderCompiled != GL_TRUE ) {
        std::cout << "Error en la compilación: " << result << "!" << std::endl;
        GLint logLength;
        glGetShaderiv(result, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar *log = (GLchar*)malloc(logLength);
            glGetShaderInfoLog(result, logLength, &logLength, log);
            std::cout << "Shader compile log:" << log << std::endl;
            free(log);
        }
        glDeleteShader(result);
        result = 0;
    } else {
        log::info("Shader Compiled Correctly, ID: %d", result);
        //std::cout << "Shader compilado correctamente. Id = " << result << std::endl;
    }
    return result;
}

GLuint Graphics::Shader::compileProgram(str vtxFile, str fragFile) {
    log::info("Compiling Program...");
    
    GLuint programId = 0;
    GLuint vtxShaderId, fragShaderId;

    programId = glCreateProgram();

    std::ifstream f(vtxFile.c_str());
    std::string source((std::istreambuf_iterator<char>(f)),
                        std::istreambuf_iterator<char>());
    vtxShaderId = compileShader(source.c_str(), GL_VERTEX_SHADER);
    
    f=std::ifstream(fragFile.c_str());
    source=std::string((std::istreambuf_iterator<char>(f)),
                        std::istreambuf_iterator<char>());
    fragShaderId = compileShader(source.c_str(), GL_FRAGMENT_SHADER);

    if(vtxShaderId && fragShaderId) {
        // Associate shader with program
        glAttachShader(programId, vtxShaderId);
        glAttachShader(programId, fragShaderId);
        glLinkProgram(programId);
        glValidateProgram(programId);

        // Check the status of the compile/link
        GLint logLen;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);
        if(logLen > 0) {
            char* log = (char*) malloc(logLen * sizeof(char));
            // Show any errors as appropriate
            glGetProgramInfoLog(programId, logLen, &logLen, log);
            std::cout << "Prog Info Log: " << std::endl << log << std::endl;
            free(log);
        }
    }
    if(vtxShaderId) {
        glDeleteShader(vtxShaderId);
    }
    if(fragShaderId) {
        glDeleteShader(fragShaderId);
    }
    return programId;
}
