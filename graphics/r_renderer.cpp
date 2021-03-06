//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "r_renderer.h"
#include "r_opengl.h"
#include "r_shader.h"
#include "r_textures.h"

#include "gui.h"

#ifdef USE_OPENGL

using namespace Verse;
using namespace Graphics;

namespace {
    SDL_GLContext context;

    ui8 pid[2];

    ui32 fbo;
    ui32 fb_tex;
    ui32 vao;
    ui32 vbo;

    float vertices[] = {
         0.0,  1.0,  0.0,  1.0,
         1.0,  1.0,  1.0,  1.0,
         0.0,  0.0,  0.0,  0.0,
         1.0,  0.0,  1.0,  0.0,
    };

    float fb_vertices[] = {
        -1.0,  1.0,  0.0,  1.0,
         1.0,  1.0,  1.0,  1.0,
        -1.0, -1.0,  0.0,  0.0,
         1.0, -1.0,  1.0,  0.0,
    };

    glm::mat4 projection;
    ui8 projection_loc;
    ui8 model_loc;
}

void Graphics::Renderer::GL::create(Config &c, SDL_Window* window) {
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
    
    //IMGUI
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init();
    
    //SHADERS
    pid[0] = Graphics::Shader::compileProgram("res/shaders/render.vertex", "res/shaders/render.frag");
    log::graphics("Program (Render) ID: %d", pid[0]);
    pid[1] = Graphics::Shader::compileProgram("res/shaders/post.vertex", "res/shaders/post.frag");
    log::graphics("Program (Post) ID: %d", pid[1]);
    log::graphics("---");
    
    //FRAMEBUFFER
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    glGenTextures(1, &fb_tex);
    glBindTexture(GL_TEXTURE_2D, fb_tex);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, c.window_size.x, c.window_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); //TODO: Update with window
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        log::error("Error creating Framebuffer: %d", glGetError());
        SDL_Quit();
        exit(-1);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    //GENERATE VERTEX
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    //ATTRIBUTES
    //0 - v_transform
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //VALIDATE PROGRAMS
    for (ui8 p : pid) {
        Graphics::Shader::validateProgram(p);
    }
    
    //VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //PROJECTION MATRIX
    projection = glm::ortho(0.0f, (float)c.window_size.x, (float)c.window_size.y, 0.0f);
    projection_loc = glGetUniformLocation(pid[0], "projection");
    model_loc = glGetUniformLocation(pid[0], "model");
    
    //BLEND ALPHA
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //CATCH ERRORS
    GLenum e;
    while ((e = glGetError()) != GL_NO_ERROR) {
       log::error("OpenGL Error during Init: %d", e);
    }
}

ui32 Graphics::Renderer::GL::create_texture(Tex* tex, int w, int h) {
    ui32 tex_id;
    
    glGenTextures(1, &tex_id);
    
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                  
    return tex_id;
}

void Graphics::Renderer::GL::render_texture(ui32 &tex_id, Rect &src, Rect &dst, ui16 frames) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glUseProgram(pid[0]);
    
    if (frames > 1) {
        vertices[4*0 + 2] = (float)src.pos.x / (float)(src.size.x * (frames + 1));
        vertices[4*1 + 2] = vertices[4*0 + 2] + (float)src.size.x / (float)(src.size.x * (frames + 1));
        vertices[4*2 + 2] = vertices[4*0 + 2];
        vertices[4*3 + 2] = vertices[4*1 + 2];
    } else {
        vertices[4*0 + 2] = 0.0;
        vertices[4*1 + 2] = 1.0;
        vertices[4*2 + 2] = 0.0;
        vertices[4*3 + 2] = 1.0;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(dst.pos.x, dst.pos.y, 0.0f));
    model = glm::scale(model, glm::vec3(dst.size.x, dst.size.y, 1.0f));
    
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
    
    glBindTexture(GL_TEXTURE_2D, tex_id);
    
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::Renderer::GL::clear() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.1, 0.1, 0.2, 1.0); //Change for background color
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::Renderer::GL::render() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fb_vertices), fb_vertices, GL_STATIC_DRAW);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(pid[1]);
    
    glBindTexture(GL_TEXTURE_2D, fb_tex);
    
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Graphics::Renderer::GL::present(SDL_Window* window) {
    SDL_GL_SwapWindow(window);
}

void Graphics::Renderer::GL::destroy() {
    //OPENGL
    
    //IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    //SDL
    SDL_GL_DeleteContext(context);
}

#endif
