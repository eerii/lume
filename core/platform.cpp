//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "platform.h"
#include "input.h"
#include "imgui.h"

using namespace Verse;

bool Platform::init() {
    #if _WIN32
    SetProcessDPIAware();
    #endif

    SDL_version version;
    SDL_GetVersion(&version);
    log::info("SDL v%d.%d.%d", version.major, version.minor, version.patch);
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) {
        log::error("SDL_Init has failed!!", SDL_GetError());
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG))) {
        log::error("IMG_Init has failed!!", SDL_GetError());
        return false;
    }
    
    return true;
}

bool Platform::handle_events(SDL_Event* event) {
    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYDOWN:
                if (event->key.repeat == 0)
                    Input::on_key_down((Input::Key)event->key.keysym.scancode);
                break;
            case SDL_KEYUP:
                if (event->key.repeat == 0)
                    Input::on_key_up((Input::Key)event->key.keysym.scancode);
                break;
            case SDL_MOUSEMOTION:
                Input::on_mouse_move(event->motion.x, event->motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                Input::on_mouse_down(event->button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                Input::on_mouse_up(event->button.button);
                break;
            case SDL_MOUSEWHEEL:
                Input::on_mouse_wheel(event->wheel.y);
                break;
            case SDL_WINDOWEVENT:
                if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    ImGuiIO& imgui_io = ImGui::GetIO();
                    imgui_io.DisplaySize.x = static_cast<float>(event->window.data1);
                    imgui_io.DisplaySize.y = static_cast<float>(event->window.data2);
                }
                break;
        }
    }
    return true;
}
