#include <iostream>
#include <vector>


int     main(void)
{
    std::vector<int> test(5,5);

    test.clear();
    test.push_back(10);
    for(size_t i = 0; i< test.size(); i++)
    {
        std::cout << test[i] << std::endl;
    }
}