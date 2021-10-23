//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "config.h"
#include "serialization.h"

namespace Verse::System::Dialogue
{
    void parse(Config &c, Component::Dialogue* dialogue, str name);
    
    void load(EntityID eid, YAML::Node &entity, Scene *s, Config &c);
    void save(Component::Text* text, str path, std::vector<str> &key);
    void gui(Config &c, EntityID eid);
}
