//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#include "config.h"
#include "serialization.h"

namespace Fresa::System::Dialogue
{
    void parse(Config &c, Component::Dialogue* dialogue, str name);
    
    void load(EntityID eid, YAML::Node &entity, Scene *s, Config &c);
    void save(Component::Text* text, str path, std::vector<str> &key);
    void gui(Config &c, EntityID eid);
}
