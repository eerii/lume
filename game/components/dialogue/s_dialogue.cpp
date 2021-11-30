//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#include "s_dialogue.h"

#include <filesystem>
#include <iostream>
#include <fstream>

using namespace Fresa;

enum ReadStatus {
    WAITING_NAME,
    WAITING_START,
    WAITING_END,
};

bool hasValidCharacters(str s) {
    return s.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") == std::string::npos;
}

void System::Dialogue::parse(Config &c, Component::Dialogue *dialogue, str name) {
    str path = "res/" + name + ".dlg";
    
    //Check if the file exists
    std::filesystem::path f{path};
    
    if (not std::filesystem::exists(f)) {
        log::error("The dialog file that you attempted to read (" + path + ") does not exist");
        return;
    }
    
    //Read the file
    std::ifstream file(path);
    str line;
    
    if (not file.is_open()) {
        log::error("The dialog file (" + path + ") couldn't be opened");
        return;
    }
    
    //File analysis
    ReadStatus rs = WAITING_NAME;
    dialogue->fragments = {};
    str curr_fragment_name = "";
    int i = 0;
    
    while (std::getline(file, line)) {
        i++;
        
        if (line.empty())
            continue;
        
        if (rs == WAITING_NAME) {
            if (not hasValidCharacters(line))
                log::error("Line " + std::to_string(i) + " has invalid characters");
            
            curr_fragment_name = line;
            rs = WAITING_START;
        } else if (rs == WAITING_START) {
            if (line.find_first_of("#") != std::string::npos) {
                if (dialogue->fragments.count(curr_fragment_name) > 0)
                    log::error("Duplicated fragment in dialogue (" + path + "), " + curr_fragment_name);
                
                dialogue->fragments[curr_fragment_name] = Fresa::Dialogue::Fragment();
                rs = WAITING_END;
            }
        } else if (rs == WAITING_END) {
            if (line.find_first_of("#") != std::string::npos)
                rs = WAITING_NAME;
            
            auto it = line.find(":");
            if (it == std::string::npos)
                continue;
            
            str speaker = line.substr(0, it);
            str message = line.substr(it + 2);
            Fresa::Dialogue::Statement statement(speaker, message);
            dialogue->fragments[curr_fragment_name].statements.push_back(statement);
            
            
        }
    }
    
    file.close();
}

void System::Dialogue::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    
}

void System::Dialogue::save(Component::Text* text, str path, std::vector<str> &key) {
    
}

void System::Dialogue::gui(Config &c, EntityID eid) {
    
}
