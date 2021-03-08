//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "r_renderer.h"
#include "r_opengl.h"
#include "r_shader.h"
#include "r_textures.h"
#include "r_palette.h"

#include "gui.h"
#include "time.h"
#include "stb_image.h"
#include "s_light.h"

using namespace Verse;
using namespace Graphics;

namespace {
    SDL_GLContext context;

    ui8 pid[2];

    ui32 fbo, fb_tex;
    ui32 vao;
    ui32 vbo;

    float vertices[] = {
         0.0,  1.0,  0.0,  1.0,
         1.0,  1.0,  1.0,  1.0,
         0.0,  0.0,  0.0,  0.0,
         1.0,  0.0,  1.0,  0.0,
    };

    float fb_vertices[] = {
         0.0,  1.0,  0.0,  0.0,
         1.0,  1.0,  1.0,  0.0,
         0.0,  0.0,  0.0,  1.0,
         1.0,  0.0,  1.0,  1.0,
    };

    glm::mat4 mat_proj;
    glm::mat4 fb_mat_proj, fb_mat_view;
    glm::mat4 fb_mat_model, fb_mat_model_bg;
    ui8 mat_loc;
    ui8 fb_mat_loc;

    ui32 palette_tex;

    Vec2 previous_window_size;
    Vec2 stretch_factor;
    float light_distortion;
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
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, c.resolution.x, c.resolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb_tex, 0);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        log::error("Error creating Game Framebuffer: %d", glGetError());
        SDL_Quit();
        exit(-1);
    }
    
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
    
    //MATRIX LOCATIONS
    mat_proj = glm::ortho(0.0f, (float)(c.resolution.x), 0.0f, (float)(c.resolution.y));
    mat_loc = glGetUniformLocation(pid[0], "mvp");
    fb_mat_loc = glGetUniformLocation(pid[1], "mvp");
    
    //BLEND ALPHA
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //PALETTE TEX
    Graphics::loadTexture("res/graphics/palette_multi.png", palette_tex);
    
    //CATCH ERRORS
    GLenum e;
    while ((e = glGetError()) != GL_NO_ERROR) {
       log::error("OpenGL Error during Init: %d", e);
    }
}

ui32 Graphics::Renderer::GL::createTexture(ui8* tex, int w, int h) {
    ui32 tex_id;
    
    glGenTextures(1, &tex_id);
    
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                  
    return tex_id;
}

void Graphics::Renderer::GL::renderTexture(ui32 &tex_id, Rect &src, Rect &dst, ui16 frames, Config &c, bool flip) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glUseProgram(pid[0]);
    
    if (frames > 1) {
        vertices[4*0 + 2] = (float)src.pos.x / (float)(src.size.x * (frames + 1));
        vertices[4*1 + 2] = vertices[4*0 + 2] + (float)src.size.x / (float)(src.size.x * (frames + 1));
        vertices[4*2 + 2] = vertices[4*0 + 2];
        vertices[4*3 + 2] = vertices[4*1 + 2];
    }
    
    if (flip) {
        vertices[4*0 + 2] = vertices[4*1 + 2];
        vertices[4*1 + 2] = vertices[4*2 + 2];
        vertices[4*2 + 2] = vertices[4*0 + 2];
        vertices[4*3 + 2] = vertices[4*1 + 2];
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glm::mat4 mat_model = glm::mat4(1.0f);
    
    mat_model = glm::translate(mat_model, glm::vec3(stretch_factor.x * (dst.pos.x / c.render_scale),
                                                    stretch_factor.y * (dst.pos.y / c.render_scale), 0.0f));
    mat_model = glm::scale(mat_model, glm::vec3(stretch_factor.x * (dst.size.x / c.render_scale),
                                                stretch_factor.y * (dst.size.y / c.render_scale), 1.0f));
    
    glUniformMatrix4fv(mat_loc, 1, GL_FALSE, glm::value_ptr(mat_proj * mat_model));
    
    glBindTexture(GL_TEXTURE_2D, tex_id);
    
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    vertices[4*0 + 2] = 0.0;
    vertices[4*1 + 2] = 1.0;
    vertices[4*2 + 2] = 0.0;
    vertices[4*3 + 2] = 1.0;
}

void Graphics::Renderer::GL::clear(Config &c) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(c.background_color[0], c.background_color[1], c.background_color[2], c.background_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(c.background_color[0], c.background_color[1], c.background_color[2], c.background_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (previous_window_size.x != c.window_size.x or previous_window_size.y != c.window_size.y) {
        previous_window_size = c.window_size;
        
        stretch_factor = Vec2((c.resolution.x * c.render_scale)/c.window_size.x, (c.resolution.y * c.render_scale)/c.window_size.y);
        light_distortion = c.resolution.x / c.resolution.y;
        
        fb_mat_model_bg = glm::scale(glm::mat4(1.0f), glm::vec3((c.window_size.x), (c.window_size.y), 1.0f));
        fb_mat_model = glm::scale(glm::mat4(1.0f), glm::vec3((c.resolution.x * c.render_scale), (c.resolution.y * c.render_scale), 1.0f));
        fb_mat_view = glm::translate(glm::mat4(1.0f), glm::vec3(c.window_padding.x, c.window_padding.y, 0.0f));
        fb_mat_proj = glm::ortho(0.0f, (float)(c.window_size.x), 0.0f, (float)(c.window_size.y));
        
        glViewport( 0, 0, c.window_size.x, c.window_size.y );
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::Renderer::GL::render(Config &c) {
    //RENDER TO WINDOW
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(pid[1]);
    
    //PALETTE
    Graphics::Palette::render(c, palette_tex, pid[1]);
    
    //LIGTH
    std::vector<glm::vec4> light_sources = System::Light::getLight();
    glUniform4fv(glGetUniformLocation(pid[1], "light"), (int)(light_sources.size()), reinterpret_cast<GLfloat *>(light_sources.data()));
    glUniform1i(glGetUniformLocation(pid[1], "light_size"), (int)(light_sources.size()));
    glUniform1f(glGetUniformLocation(pid[1], "light_distortion"), light_distortion);
    
    //FB TEXTURE
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fb_tex);
    glUniform1i(glGetUniformLocation(pid[1], "tex"), 0);
    
    //VERTEX DATA
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fb_vertices), fb_vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    //BACKGROUND
    glUniform1i(glGetUniformLocation(pid[1], "is_background"), true);
    glUniformMatrix4fv(fb_mat_loc, 1, GL_FALSE, glm::value_ptr(fb_mat_proj * fb_mat_model_bg));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    //LEVEL
    glUniform1i(glGetUniformLocation(pid[1], "is_background"), false);
    glUniformMatrix4fv(fb_mat_loc, 1, GL_FALSE, glm::value_ptr(fb_mat_proj * fb_mat_view * fb_mat_model));
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
