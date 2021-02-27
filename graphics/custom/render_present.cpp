//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "render_present.h"

using namespace Verse;

namespace {
    unsigned int textures[2];
}

void Graphics::present(Config &c, SDL_Renderer *renderer, SDL_Window *window,
                       SDL_Texture *render_target, SDL_Texture *palette_tex,
                       int pid, int width, int height) {
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
    
    glUniform1f(glGetUniformLocation(pid, "palette_index"), ((float)c.palette_index / (float)c.num_palettes) + 0.001);

    //COORDINATES FOR DRAWING
    float minx, miny, maxx, maxy;
    float minu, maxu, minv, maxv;

    minx = 0.0f; miny = 0.0f; maxx = width; maxy = height;
    minu = 0.0f; maxu = 1.0f; minv = 1.0f; maxv = 0.0f;
    
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
