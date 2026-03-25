#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

#include "Utils.hpp"

enum class Ingredient
{
    Wheat_Bread = 0,
    Whole_Bread,
    Letucce,
    Mustard,
    Chicken,
    Ketchup,
    Tomato,
    Olives,
    Cheese,
    Salmon,
    Onion,
    Ham
};

inline const std::map<Ingredient, std::string> Ing_To_Str = {{Ingredient::Wheat_Bread, "Wheat Bread"},
                                                             {Ingredient::Whole_Bread, "Whole Bread"},
                                                             {Ingredient::Letucce, "Letucce"},
                                                             {Ingredient::Mustard, "Mustard"},
                                                             {Ingredient::Ketchup, "Ketchup"},
                                                             {Ingredient::Chicken, "Chicken"},
                                                             {Ingredient::Tomato, "Tomato"},
                                                             {Ingredient::Olives, "Olives"},
                                                             {Ingredient::Salmon, "Salmon"},
                                                             {Ingredient::Cheese, "Cheese"},
                                                             {Ingredient::Onion, "Onion"},
                                                             {Ingredient::Ham, "Ham"}};

inline const std::map<std::string, Ingredient> Str_To_Ing = {{"Wheat Bread", Ingredient::Wheat_Bread},
                                                             {"Whole Bread", Ingredient::Whole_Bread},
                                                             {"Letucce", Ingredient::Letucce},
                                                             {"Mustard", Ingredient::Mustard},
                                                             {"Ketchup", Ingredient::Ketchup},
                                                             {"Chicken", Ingredient::Chicken},
                                                             {"Tomato", Ingredient::Tomato},
                                                             {"Olives", Ingredient::Olives},
                                                             {"Salmon", Ingredient::Salmon},
                                                             {"Cheese", Ingredient::Cheese},
                                                             {"Onion", Ingredient::Onion},
                                                             {"Ham", Ingredient::Ham}};

class Sandwich
{
private:
    std::string name_;
    std::vector<Ingredient> ingredients_;

    void decode_data_in(std::string data_in);

public:
    Sandwich(std::string name);

    std::string Get_String_Data();

    void Set_Name(std::string data_in);
    void Set_Ingredients(std::vector<Ingredient> ings);
    void Add_Ingredient(Ingredient ing);

    std::vector<Ingredient> Get_Ingredients() const;
    std::string Get_Name() const;

    friend std::ostream &operator<<(std::ostream &os, const Sandwich &s);
};