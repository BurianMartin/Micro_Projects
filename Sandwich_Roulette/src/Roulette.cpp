#include "Roulette.hpp"

Roulette::Roulette(std::unique_ptr<Menu> menu)
{
    menu_ = std::move(menu);
}

void Roulette::start()
{
    std::string input = "";

    while (true)
    {

        if (menu_->Get_ToTry_Sandwiches().size() == 0)
        {
            std_log("Congratulations !! You have tried all 100 sandwiches from that spanish place !!\n");
            return;
        }

        std_log("Rolling roulette .");

        for (int i = 0; i < 10; i++)
        {
            std_log(".");

            std::cout.flush();

            usleep(250000);
        }

        int index = Roll_Roulette();

        std_log("Would you like to eat this sandwich ? Y/N\n");

        while (true)
        {
            std::cin >> input;

            if (input == "y" || input == "Y")
            {
                menu_->Eat_Sandwich(index);
                break;
            }
            else if (input == "n" || input == "N")
            {
                break;
            }
            else
            {
                std_log("Please answer with Y/N :\n");
            }
        }

        std_log("Would you like to roll another sandwich ? Y/N\n");

        while (true)
        {
            std::cin >> input;

            if (input == "y" || input == "Y")
            {
                break;
            }
            else if (input == "n" || input == "N")
            {
                return;
            }
            else
            {
                std_log("Please answer with Y/N :\n");
            }
        }
    }
}

std::unique_ptr<Menu> Roulette::Release_Menu()
{
    if (!menu_)
    {
        err_log("Tried to move menu from Roulette while it is not the property or Roulette");
        return std::unique_ptr<Menu>();
    }

    return std::move(menu_);
}

int Roulette::Roll_Roulette()
{
    auto sandwiches = menu_->Get_ToTry_Sandwiches();

    int index = rand() % sandwiches.size();

    std_log("\nRullette rolled a sandwich for you:\n"
            << sandwiches[index] << std::endl);

    return index;
}
