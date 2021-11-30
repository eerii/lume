//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <functional>

namespace Fresa::Dialogue
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

namespace Fresa::Component
{
    struct Dialogue {
        str path;
        std::map<str, Fresa::Dialogue::Fragment> fragments;
        std::map<str, EntityID> characters;
    };
}
