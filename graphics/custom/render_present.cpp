//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <SDL2/SDL.h>

#include "render_present.h"
#include "time.h"
#include "s_light.h"

#define TRANSITION_TIME 500

using namespace Verse;

namespace {
    unsigned int textures[2];

    int previous_palette = 0;
    ui64 switch_palette_time = 0;
    float transition_percent = 0.0;
}

void Graphics::present(Config &c, SDL_Renderer *renderer, SDL_Window *window,
                       SDL_Texture *render_target, SDL_Texture *palette_tex, ui8 pid) {
    int previous_pid = 0;
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
    
    if(pid != 0) {
        glGetIntegerv(GL_CURRENT_PROGRAM, &previous_pid);
        glUseProgram(pid);
    }
    
    //GENERATE AND BIND TEXTURES
    glGenTextures(2, textures);
    
    glActiveTexture(GL_TEXTURE0);
    SDL_GL_BindTexture(render_target, NULL, NULL);
    glUniform1i(glGetUniformLocation(pid, "tex"), 0);
    
    glActiveTexture(GL_TEXTURE1);
    SDL_GL_BindTexture(palette_tex, NULL, NULL);
    glUniform1i(glGetUniformLocation(pid, "palette"), 1);
    
    //PALETTE TRANSITION
    handlePaletteTransition(c);
    
    glUniform1f(glGetUniformLocation(pid, "palette_index"), ((float)c.palette_index / (float)c.num_palettes) + 0.001);
    glUniform1f(glGetUniformLocation(pid, "previous_palette_index"), ((float)previous_palette / (float)c.num_palettes) + 0.001);
    glUniform1f(glGetUniformLocation(pid, "transition_percent"), transition_percent);
    
    //LIGHT
    System::Light::render(pid, c);
    glUniform2f(glGetUniformLocation(pid, "window_size"), c.window_size.x, c.window_size.y);
    glUniform1i(glGetUniformLocation(pid, "use_grayscale"), c.use_grayscale ? 1 : 0);
    
    //COORDINATES FOR DRAWING
    float minx, miny, maxx, maxy;
    float minu, maxu, minv, maxv;
    
    //Background
    minx = 0.0f;
    miny = 0.0f;
    maxx = c.window_size.x;
    maxy = c.window_size.y;
    
    glUniform1i(glGetUniformLocation(pid, "is_background"), 1);
    
    glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(minx, miny);
        glVertex2f(maxx, miny);
        glVertex2f(minx, maxy);
        glVertex2f(maxx, maxy);
    glEnd();
    
    //Texture
    float w = c.render_scale * c.resolution.x;
    float h = c.render_scale * c.resolution.y;
    
    minx = c.window_padding.x;
    miny = c.window_padding.y;
    maxx = w + c.window_padding.x;
    maxy = h + c.window_padding.y;
    
    minu = 0.0f;
    maxu = 1.0f - (2.0f * c.window_padding.x / c.window_size.x);
    minv = 1.0f - (2.0f * c.window_padding.y / c.window_size.y);
    maxv = 0.0f;
    
    glUniform1i(glGetUniformLocation(pid, "is_background"), 0);
    
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(minu, minv); glVertex2f(minx, miny);
        glTexCoord2f(maxu, minv); glVertex2f(maxx, miny);
        glTexCoord2f(minu, maxv); glVertex2f(minx, maxy);
        glTexCoord2f(maxu, maxv); glVertex2f(maxx, maxy);
    glEnd();
    
    SDL_GL_SwapWindow(window);

    if(pid != 0) {
        glUseProgram(previous_pid);
    }
}

void Graphics::handlePaletteTransition(Config &c) {
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
