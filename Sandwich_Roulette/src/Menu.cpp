#include "Menu.hpp"

Menu::Menu(std::vector<Sandwich> eaten, std::vector<Sandwich> to_try) : eaten_(eaten), to_try_(to_try)
{
}

std::vector<Sandwich> Menu::Get_Eaten_Sandwiches() const
{
    return eaten_;
}

std::vector<Sandwich> Menu::Get_ToTry_Sandwiches() const
{
    return to_try_;
}

void Menu::Eat_Sandwich(int index)
{
    Sandwich removed = to_try_[index];
    to_try_.erase(to_try_.begin() + index);
    eaten_.push_back(removed);
}
