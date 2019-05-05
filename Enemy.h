#pragma once

#include "Model.h"
#include <string>

class Enemy: public Model {
public:
    Enemy(std::string dir): Model(dir) {}
private:
};

