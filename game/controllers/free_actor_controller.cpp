//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "free_actor_controller.h"
#include "input.h"

using namespace Verse;

bool Controller::FreeActor::controller(Scene &s, Config &c, EntityID eid) {
    
    Component::Actor* actor = s.getComponent<Component::Actor>(eid);
    
    if (Input::down(Input::Key::J))
        actor->vel.x = -actor->max_move_speed;
    if (Input::down(Input::Key::L))
        actor->vel.x = actor->max_move_speed;
    if (Input::down(Input::Key::I))
        actor->vel.y = -actor->max_move_speed;
    if (Input::down(Input::Key::K))
        actor->vel.y = actor->max_move_speed;
    
    if (!Input::down(Input::Key::J) && !Input::down(Input::Key::L))
        actor->vel.x = 0;
    if (!Input::down(Input::Key::I) && !Input::down(Input::Key::K))
        actor->vel.y = 0;
    
    return actor->vel != Vec2f(0,0);
}
