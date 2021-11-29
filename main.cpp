#include "src/Config/FileParss.hpp"

int     main(int ac, char **av)
{
    FileParss parss(ac, av);

    std::cout << parss.server[0].locat["php"].getLocationExtention() << std::endl;
}

//