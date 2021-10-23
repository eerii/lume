//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <map>
#include <functional>

namespace Verse::Dialogue
{
    using Statement = std::pair<str, str>;

    enum Style {
        REGULAR,
        FAST,
        SLOW
    };

    struct Fragment {
        std::vector<Statement> statements;
        std::map<str, Fragment*> options;
        std::function<void()> callback;
        bool* finished;
        Style style = REGULAR;
    };
}

namespace Verse::Component
{
    struct Dialogue {
        str path;
        std::map<str, Verse::Dialogue::Fragment> fragments;
        std::map<str, EntityID> characters;
    };
}
