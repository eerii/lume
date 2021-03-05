//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "r_textures.h"
#include "r_renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Verse;

#ifdef USE_OPENGL

Tex* Graphics::loadTexture(str path, ui32 &tex_id) {
    int w, h, ch;
    ui8* tex = stbi_load(path.c_str(), &w, &h, &ch, STBI_rgb_alpha);
    
    tex_id = (int)Graphics::Renderer::GL::create_texture(tex, w, h);
    
    return tex;
}

#else

namespace {
    SDL_Renderer *renderer;
}

void Graphics::linkRendererToTexture(SDL_Renderer* r) {
    renderer = r;
}

Tex* Graphics::loadTexture(str path, ui32 &tex_id) {
    if (renderer == nullptr) {
        log::error("Renderer doesn't exist and you tried to load a texture");
        return nullptr;
    }
    
    SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
    if (tex == nullptr) {
        log::error("There was an error loading the texture: %s", path.c_str());
    }
    return tex;
}

SDL_Surface* Graphics::loadSurface(str path) {
    if (renderer == nullptr) {
        log::error("Renderer doesn't exist and you tried to load a texture");
        return nullptr;
    }
    
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        log::error("There was an error loading the surface: %s", path.c_str());
    }
    return surface;
}

#endif
