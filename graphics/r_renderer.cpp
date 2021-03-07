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
#include "time.h"
#include "stb_image.h"

#ifdef USE_OPENGL

#define TRANSITION_TIME 500

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

    glm::mat4 projection;
    ui8 projection_loc;
    ui8 model_loc;
    ui8 mvp_loc;

    ui32 palette_tex;
    ui16 previous_palette = 0;
    ui64 switch_palette_time = 0;
    float transition_percent = 0.0;

    Vec2 previous_window_size;
    Vec2 stretch_factor;
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
    
    //GAME FRAMEBUFFER
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
    
    //PROJECTION MATRIX
    projection = glm::ortho(0.0f, (float)(c.resolution.x), 0.0f, (float)(c.resolution.y));
    projection_loc = glGetUniformLocation(pid[0], "projection");
    model_loc = glGetUniformLocation(pid[0], "model");
    mvp_loc = glGetUniformLocation(pid[1], "mvp");
    
    //BLEND ALPHA
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //PALETTE TEX
    int p_w, p_h, p_ch;
    ui8* p_tex = stbi_load("res/graphics/palette_multi.png", &p_w, &p_h, &p_ch, STBI_rgb_alpha);
    palette_tex = (int)Graphics::Renderer::GL::createTexture(p_tex, p_w, p_h);
    
    //CATCH ERRORS
    GLenum e;
    while ((e = glGetError()) != GL_NO_ERROR) {
       log::error("OpenGL Error during Init: %d", e);
    }
}

ui32 Graphics::Renderer::GL::createTexture(Tex* tex, int w, int h) {
    ui32 tex_id;
    
    glGenTextures(1, &tex_id);
    
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                  
    return tex_id;
}

void Graphics::Renderer::GL::renderTexture(ui32 &tex_id, Rect &src, Rect &dst, ui16 frames, Config &c) {
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
    
    model = glm::translate(model, glm::vec3(stretch_factor.x * (dst.pos.x / c.render_scale), stretch_factor.y * (dst.pos.y / c.render_scale), 0.0f));
    model = glm::scale(model, glm::vec3(stretch_factor.x * (dst.size.x / c.render_scale), stretch_factor.y * (dst.size.y / c.render_scale), 1.0f));
    
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
    
    glBindTexture(GL_TEXTURE_2D, tex_id);
    
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::Renderer::GL::clear(Config &c) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.1, 0.1, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (previous_window_size.x != c.window_size.x or previous_window_size.y != c.window_size.y) {
        previous_window_size = c.window_size;
        stretch_factor = Vec2((c.resolution.x * c.render_scale)/c.window_size.x, (c.resolution.y * c.render_scale)/c.window_size.y);
        glViewport( 0, 0, c.window_size.x, c.window_size.y );
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::Renderer::GL::render(Config &c) {
    //RENDER TO WINDOW
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(pid[1]);
    
    //PALETTE
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, palette_tex);
    glUniform1i(glGetUniformLocation(pid[1], "palette"), 1);
    
    handlePaletteTransition(c);
    glUniform1f(glGetUniformLocation(pid[1], "palette_index"), ((float)c.palette_index / (float)c.num_palettes));
    glUniform1f(glGetUniformLocation(pid[1], "previous_palette_index"), ((float)previous_palette / (float)c.num_palettes) + 0.001);
    glUniform1f(glGetUniformLocation(pid[1], "transition_percent"), transition_percent);
    glUniform1i(glGetUniformLocation(pid[1], "use_grayscale"), c.use_grayscale);
    
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
    
    //MATRICES
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3((c.window_size.x), (c.window_size.y), 1.0f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(c.window_padding.x, c.window_padding.y, 0.0f));
    glm::mat4 proj = glm::ortho(0.0f, (float)(c.window_size.x), 0.0f, (float)(c.window_size.y));
    
    //BACKGROUND
    glUniform1i(glGetUniformLocation(pid[1], "is_background"), true);
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(proj * model));
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    //LEVEL
    glUniform1i(glGetUniformLocation(pid[1], "is_background"), false);
    model = glm::scale(glm::mat4(1.0f), glm::vec3((c.resolution.x * c.render_scale), (c.resolution.y * c.render_scale), 1.0f));
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(proj * view * model));
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

void Graphics::Renderer::GL::handlePaletteTransition(Config &c) {
    if (previous_palette != c.palette_index) {
        if (switch_palette_time == 0) {
            switch_palette_time = Time::current;
        } else {
            int delay = int(Time::current - switch_palette_time);
            
            if (delay < TRANSITION_TIME) {
                transition_percent = (float)delay / (float)TRANSITION_TIME;
            } else {
                switch_palette_time = 0; transition_percent = 0.0;
                previous_palette = c.palette_index;
            }
        }
    } else if (switch_palette_time != 0 or transition_percent != 0.0) {
        switch_palette_time = 0; transition_percent = 0.0;
    }
}

#endif
