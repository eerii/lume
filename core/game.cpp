//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "game.h"
#include "time.h"
#include "input.h"
#include "file.h"
#include "events.h"
#include "gui.h"
#include "s_actor.h"

#define ENABLE_GUI

using namespace Verse;

namespace
{
    Config* config;
    ui32 accumulator;
}

bool Game::init(Config &c) {
    log::debug("Starting the game...");
    
    //CONFIGURATION
    config = &c;
    
    //INITIALIZE FILE SYSTEM
    File::init();
    
    #if _WIN32
    SetProcessDPIAware();
    #endif

    //INITIALIZE SDL
    SDL_version version;
    SDL_GetVersion(&version);
    log::debug("SDL v%d.%d.%d", version.major, version.minor, version.patch);
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) {
        log::error("SDL_Init has failed!!", SDL_GetError());
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG))) {
        log::error("IMG_Init has failed!!", SDL_GetError());
        return false;
    }
    
    //INITIALIZE GRAPHICS
    Graphics::init(*config);
    
    return true;
}

bool Game::update(Scene &scene) {
    bool running = true;
    
    Game::time_frame();
    
    while (accumulator >= TIMESTEP) {
        accumulator -= TIMESTEP;
        
        //Event Update
        running &= Events::handleEvents();
        
        //Gui Update
#ifdef ENABLE_GUI
        Gui::update(1.0f / 60.0f);
#endif
        
        //Entity Update
        System::Actor::update(scene);
        
        //Input Update
        Input::frame();
    }
    
    //const float alpha = accumulator / TIMESTEP; //Used to linearly interpolate
    
    //Render Update
#ifdef ENABLE_GUI
    Gui::prerender(scene, *config);
#endif
    
    Graphics::clear(*config);
    
    Graphics::render(scene, *config);
    
#ifdef ENABLE_GUI
    Gui::render();
#endif
    
    Graphics::display(*config);
    
    //Prevent the game from rendering faster than the refresh speed
    ui32 frameTicks = (ui32)(time() - Time::current);
    if (frameTicks < 1000 / Graphics::getRefreshRate())
        SDL_Delay(1000 / Graphics::getRefreshRate() - frameTicks);
    
    return running;
}

void Game::time_frame() {
    Time::previous = Time::current;
    Time::current = time();
    Time::delta = Time::current - Time::previous;
    accumulator += Time::delta;
}

void Game::stop() {
    log::debug("Closing the game...");
    
    Graphics::destroy();
    SDL_Quit();
}
