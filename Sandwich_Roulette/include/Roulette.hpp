#pragma once

#include <memory>
#include <unistd.h>

#include "Menu.hpp"
#include "Utils.hpp"

class Roulette
{
private:
    std::unique_ptr<Menu> menu_;
    int Roll_Roulette();

public:
    Roulette(std::unique_ptr<Menu> menu);

    void start();

    std::unique_ptr<Menu> Release_Menu();
};