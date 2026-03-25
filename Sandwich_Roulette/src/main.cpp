#include "Fs_Interface.hpp"
#include "Roulette.hpp"

int main(int argc, char const *argv[])
{

    std::string filename;
    if (argc < 2)
    {
        filename = "tmp.txt";
        debug_log("No argument for filename, defaulting to \"tmp.txt\"");
    }
    else
    {
        filename = argv[1];
    }

    Fs_Interface interface(filename);

    Roulette roulette(interface.Release_Menu());

    roulette.start();

    interface.Receive_Menu(roulette.Release_Menu());

    interface.Save_Menu();

    return 0;
}
