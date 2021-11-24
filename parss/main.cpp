#include "../inc/ConfigFilePars.hpp"

int     main(int ac, char **av)
{
    ConfigFilePars parss(ac, av);

    std::cout << parss.server[0].locat["php"].getLocationExtention() << std::endl;
}

//