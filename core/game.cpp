//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "game.h"
#include "time.h"
#include "input.h"
#include "platform.h"
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
    log::info("Starting the game...");
    
    config = &c;
    
    bool running = Platform::init();
    Graphics::init();
    
    System::Actor::init();
    
    return running;
}

bool Game::update(Scene &scene) {
    bool running = true;
    
    Game::time_frame();
    
    while (accumulator >= TIMESTEP) {
        accumulator -= TIMESTEP;
        
        //Event Update
        running &= Platform::handle_events(&Platform::event);
        
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
    
    Graphics::clear();
    
    Graphics::render(scene, *config);
    
#ifdef ENABLE_GUI
    Gui::render();
#endif
    
    Graphics::display();
    
    //Prevent the game from rendering faster than the refresh speed
    ui32 frameTicks = (ui32)(time() - Time::current);
    if (frameTicks < 1000 / Graphics::refreshRate)
        SDL_Delay(1000 / Graphics::refreshRate - frameTicks);
    
    return running;
}

void Game::time_frame() {
    Time::previous = Time::current;
    Time::current = time();
    Time::delta = Time::current - Time::previous;
    accumulator += Time::delta;
}

void Game::stop() {
    log::info("Closing the game...");
    
    SDL_DestroyWindow(Graphics::window);
    SDL_Quit();
}
