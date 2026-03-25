#include "Fs_Interface.hpp"

Fs_Interface::Fs_Interface(std::string filename) : menu_filename_(filename)
{
    std::pair<std::vector<Sandwich>, std::vector<Sandwich>> result = Load_Menu();

    if (result.first.empty() && result.second.empty())
    {
        err_log("Could not load sandwiches from file" << std::endl);
        return;
    }

    menu_ = std::make_unique<Menu>(result.first, result.second);
}

std::pair<std::vector<Sandwich>, std::vector<Sandwich>> Fs_Interface::Load_Menu()
{

    if (!fs::exists(menu_filename_))
    {
        err_log("Error opening file: The file does not exist --- filename :: " << menu_filename_ << std::endl);
        return std::pair<std::vector<Sandwich>, std::vector<Sandwich>>();
    }

    std::ifstream file(menu_filename_);

    bool is_eaten = false;
    std::vector<Sandwich> eaten;
    std::vector<Sandwich> to_try;

    std::string line = "";

    while (std::getline(file, line))
    {
        if (line.find("Eaten:") != std::string::npos)
        {
            is_eaten = true;
        }
        else if (line.find("To Try:") != std::string::npos)
        {
            is_eaten = false;
        }
        else if (is_eaten)
        {
            eaten.emplace_back(line);
        }
        else if (!is_eaten)
        {
            to_try.emplace_back(line);
        }
    }
    file.close();

    std_log("Menu sucessfully loaded from " << menu_filename_ << std::endl);

    for (auto sndw : eaten)
    {
        debug_log(sndw);
    }
    for (auto sndw : to_try)
    {
        debug_log(sndw);
    }

    return std::pair<std::vector<Sandwich>, std::vector<Sandwich>>(eaten, to_try);
}

void Fs_Interface::Save_Menu()
{

    if (!fs::exists(menu_filename_))
    {
        err_log("Error opening file: The file does not exist --- filename :: " << menu_filename_ << std::endl);
        return;
    }
    else if (!menu_)
    {
        err_log("The menu_ member imn Fs_Interface is empty!!" << std::endl);
        return;
    }

    std::ofstream file(menu_filename_);

    file << "Eaten:\n";

    auto eaten = menu_->Get_Eaten_Sandwiches();
    auto to_try = menu_->Get_ToTry_Sandwiches();

    for (Sandwich s : eaten)
    {
        file << s.Get_String_Data();
    }

    file << "To Try:\n";

    for (Sandwich s : to_try)
    {
        file << s.Get_String_Data();
    }

    file.close();

    std_log("Menu sucessfully saved no " << menu_filename_ << std::endl);

    return;
}

std::unique_ptr<Menu> Fs_Interface::Release_Menu()
{
    return std::move(menu_);
}

void Fs_Interface::Receive_Menu(std::unique_ptr<Menu> menu)
{
    menu_ = std::move(menu);
}
