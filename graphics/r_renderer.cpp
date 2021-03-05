//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "r_renderer.h"
#include "r_opengl.h"
#include "r_shader.h"

#ifdef USE_OPENGL

using namespace Verse;
using namespace Graphics;

namespace {
    SDL_GLContext context;

    ui32 vao;
    ui32 vbo[2];

    float vertices[] = {
       -0.5,    0.5,
        0.5,    0.5,
       -0.5,   -0.5,
        0.5,   -0.5
    };

    float colors[] = {
        1.0, 0.2, 0.2,
        0.2, 1.0, 0.2,
        0.2, 0.2, 1.0,
        0.9, 1.0, 0.8,
    };
}

void Graphics::Renderer::GL::create(Config &c, SDL_Window* window, ui8 &pid) {
    //CONTEXT
    context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        log::error("Error creating OpenGL Context: %s", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }
    if (SDL_GL_MakeCurrent(window, context) != 0) {
        log::error("Error making OpenGL Context current: %s", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }
    
    //EXTENSIONS
    #ifndef __APPLE__
    //Initialize Extensions
    if (!initGLExtensions()) {
        log::error("Couldn't init GL extensions!");
        SDL_Quit();
        exit(-1);
    }
    #endif
    
    //V-SYNC
    SDL_GL_SetSwapInterval(1);
    
    //INFORMATION
    log::graphics("---");
    log::graphics("Vendor:          %s", glGetString(GL_VENDOR));
    log::graphics("Renderer:        %s", glGetString(GL_RENDERER));
    log::graphics("OpenGL Version:  %s", glGetString(GL_VERSION));
    log::graphics("GLSL Version:    %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    log::graphics("---");
    
    //SHADERS
    //TODO: Port shaders
    pid = Graphics::Shader::compileProgram("res/shaders/opengl.vertex", "res/shaders/opengl.frag");
    //Graphics::Shader::validateProgram(pid);
    log::graphics("Program ID: %d", pid);
    log::graphics("---");
    
    //VAO & VBO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    Graphics::Shader::validateProgram(pid);
    glUseProgram(pid);
    
    glGenBuffers(2, vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    ui8 pos_loc = glGetAttribLocation(pid, "pos");
    glEnableVertexAttribArray(pos_loc);
    glVertexAttribPointer(pos_loc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    
    ui8 col_loc = glGetAttribLocation(pid, "vertex_color");
    glEnableVertexAttribArray(col_loc);
    glVertexAttribPointer(col_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    
    //CATCH ERRORS
    GLenum e;
    while ((e = glGetError()) != GL_NO_ERROR) {
       log::error("OpenGL Error during Init: %d", e);
    }
}

void Graphics::Renderer::GL::clear() {
    glClearColor(0.1, 0.1, 0.2, 1.0); //Change for background color
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Resize Window
}

void Graphics::Renderer::GL::present(SDL_Window* window, ui8 &pid) {
    
    //Test
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    SDL_GL_SwapWindow(window);
}

void Graphics::Renderer::GL::destroy() {
    //OPENGL
    
    //SDL
    SDL_GL_DeleteContext(context);
}

#endif
