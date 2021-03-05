//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "r_textures.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Verse;

#ifdef USE_OPENGL

Tex* Graphics::loadTexture(str path) {
    int w, h, ch;
    ui8* tex = stbi_load(path.c_str(), &w, &h, &ch, 0);
    //log::info("Image W: %d, H: %d, CH: %d", w, h, ch);
    
    return tex;
}

#else

namespace {
    SDL_Renderer *renderer;
}

void Graphics::linkRendererToTexture(SDL_Renderer* r) {
    renderer = r;
}

Tex* Graphics::loadTexture(str path) {
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
