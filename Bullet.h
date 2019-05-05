#pragma once

#include "Model.h"
#include <string>

class Bullet: public Model {
public:
    Bullet(std::string dir): Model(dir) {}
private:
};