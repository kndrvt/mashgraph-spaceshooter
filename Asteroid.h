#pragma once

#include "Model.h"
#include <string>

class Asteroid: public Model {
public:
    Asteroid(std::string dir): Model(dir) {}
private:
};
