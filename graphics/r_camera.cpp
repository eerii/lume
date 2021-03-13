//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "r_camera.h"

using namespace Verse;

namespace {
    Vec2 *camera;
}

void Graphics::Camera::bind(Vec2 *c) {
    camera = c;
}

void Graphics::Camera::move(int &x, int &y) {
    
}
