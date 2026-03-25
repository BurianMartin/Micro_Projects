#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <utility>
#include <memory>

#include "Menu.hpp"

namespace fs = std::filesystem;

class Fs_Interface
{
private:
    std::unique_ptr<Menu> menu_;
    std::string menu_filename_;

    std::pair<std::vector<Sandwich>, std::vector<Sandwich>> Load_Menu();

public:
    Fs_Interface(std::string filename);

    std::unique_ptr<Menu> Release_Menu();
    void Receive_Menu(std::unique_ptr<Menu>);

    void Save_Menu();
};