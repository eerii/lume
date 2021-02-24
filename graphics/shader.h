//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu
//this specific code is based on https://github.com/AugustoRuiz/sdl2glsl under the MIT License
//thank you so so much to augusto ruiz for the help with this part c:

#pragma once

#include "dtypes.h"
#include "log.h"

#include <fstream>
#include <streambuf>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __APPLE__ //MACOS
#define GL_SILENCE_DEPRECATION

#include "CoreFoundation/CoreFoundation.h"
#include <OpenGL/OpenGL.h>

#if ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
#include <OpenGL/gl3.h>
#else
#include <OpenGL/gl.h>
#endif //!ESSENTIAL_GL_PRACTICES_SUPPORT_GL3

#else //OTHERS (Not tested)

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#endif

namespace Verse::Graphics::Shader
{
    bool initGLExtensions();
    GLuint compileShader(const char* source, GLuint shaderType);
    GLuint compileProgram(str vtxFile, str fragFile);
}
