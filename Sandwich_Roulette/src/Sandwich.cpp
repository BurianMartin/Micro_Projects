#include "Sandwich.hpp"

Sandwich::Sandwich(std::string data_in)
{
    decode_data_in(data_in);
}

void Sandwich::decode_data_in(std::string data_in)
{
    std::stringstream data(data_in);

    std::string token;

    std::getline(data, token, ';');

    name_ = token;

    while (std::getline(data, token, ';'))
    {
        if (Str_To_Ing.find(token) != Str_To_Ing.end())
        {
            ingredients_.push_back(Str_To_Ing.at(token));
        }
        else
        {
            err_log("Unknown ingredient: " << token << " in the sandwich: " << name_ << std::endl);
        }
    }
}

std::string Sandwich::Get_String_Data()
{
    std::string data = name_ + ";";

    for (Ingredient ing : ingredients_)
    {
        data += Ing_To_Str.at(ing);
        data += ";";
    }

    data[data.length() - 1] = '\n';

    return data;
}

void Sandwich::Set_Name(std::string name)
{
    name_ = name;
}

void Sandwich::Set_Ingredients(std::vector<Ingredient> ings)
{
    ingredients_ = ings;
}

void Sandwich::Add_Ingredient(Ingredient ing)
{
    ingredients_.push_back(ing);
}

std::vector<Ingredient> Sandwich::Get_Ingredients() const
{
    return ingredients_;
}

std::string Sandwich::Get_Name() const
{
    return name_;
}

std::ostream &operator<<(std::ostream &os, const Sandwich &s)
{
    os << s.name_ << ":\n    ";

    for (auto ing : s.ingredients_)
    {
        os << Ing_To_Str.at(ing) << " ";
    }
    os << "\n";

    return os;
}
