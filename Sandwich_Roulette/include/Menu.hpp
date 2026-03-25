#pragma once
#include <vector>

#include "Sandwich.hpp"

class Menu
{
private:
    std::vector<Sandwich> eaten_;
    std::vector<Sandwich> to_try_;

public:
    Menu(std::vector<Sandwich> eaten, std::vector<Sandwich> to_try);

    std::vector<Sandwich> Get_Eaten_Sandwiches() const;
    std::vector<Sandwich> Get_ToTry_Sandwiches() const;

    void Eat_Sandwich(int index);
};