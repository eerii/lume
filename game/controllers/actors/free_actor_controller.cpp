//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "free_actor_controller.h"

#include "input.h"
#include "s_actor.h"

using namespace Verse;

bool Controller::FreeActor::controller(Config &c, EntityID eid) {
    
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    
    if (Input::down(Input::Key::A))
        actor->vel.x = -actor->max_move_speed;
    if (Input::down(Input::Key::D))
        actor->vel.x = actor->max_move_speed;
    if (Input::down(Input::Key::W))
        actor->vel.y = -actor->max_move_speed;
    if (Input::down(Input::Key::S))
        actor->vel.y = actor->max_move_speed;
    
    if (!Input::down(Input::Key::A) && !Input::down(Input::Key::D))
        actor->vel.x = 0;
    if (!Input::down(Input::Key::W) && !Input::down(Input::Key::S))
        actor->vel.y = 0;
    
    System::Actor::move(c, eid);
    
    return actor->vel != Vec2(0.0f,0.0f);
}
