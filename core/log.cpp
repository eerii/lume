//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <string>
#include <memory>

#include "log.h"

using namespace Verse;

int lenght = 1024;

void log::info(str p_info, ...)
{
    char msg[lenght];
    va_list ap;
    va_start(ap, p_info);
    vsnprintf(msg, sizeof(char) * lenght, p_info.c_str(), ap);
    va_end(ap);

    std::clog << "[INFO] " << msg << std::endl;
}

void log::warn(str p_info, ...)
{
    char msg[lenght];
    va_list ap;
    va_start(ap, p_info);
    vsnprintf(msg, sizeof(char) * lenght, p_info.c_str(), ap);
    va_end(ap);

    std::clog << "[WARNING] " << msg << std::endl;
}

void log::error(str p_info, ...)
{
    char msg[lenght];
    va_list ap;
    va_start(ap, p_info);
    vsnprintf(msg, sizeof(char) * lenght, p_info.c_str(), ap);
    va_end(ap);

    std::cerr << "[ERROR] " << msg << std::endl;
}

void log::debug(bool p_debug, str p_info, ...)
{
    if (p_debug) {
        char msg[lenght];
        va_list ap;
        va_start(ap, p_info);
        vsnprintf(msg, sizeof(char) * lenght, p_info.c_str(), ap);
        va_end(ap);

        std::cout << "[DEBUG] " << msg << std::endl;
    }
}

void log::vec2(Vec2 p_vector, str p_name) {
    std::clog << "[VECTOR] " << p_name << ((p_name == "") ? "" : " ") << "x: " << p_vector.x << " | y: " << p_vector.y << std::endl;
}

void log::num(int p_number, str p_name) {
    std::clog << "[NUMBER]: " << p_name << ((p_name == "") ? "" : ": ") << p_number << std::endl;
}
void log::num(ui32 p_number, str p_name){
    std::clog << "[NUMBER]: " << p_name << ((p_name == "") ? "" : ": ") << p_number << std::endl;
}
void log::num(ui64 p_number, str p_name){
    std::clog << "[NUMBER]: " << p_name << ((p_name == "") ? "" : ": ") << p_number << std::endl;
}
void log::num(float p_number, str p_name){
    std::clog << "[NUMBER]: " << p_name << ((p_name == "") ? "" : ": ") << p_number << std::endl;
}
void log::num(double p_number, str p_name){
    std::clog << "[NUMBER]: " << p_name << ((p_name == "") ? "" : ": ") << p_number << std::endl;
}
