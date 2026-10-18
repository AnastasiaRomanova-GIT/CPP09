#include "../includes/PmergeMe.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Error: No input provided." << std::endl;
        return 1;
    }

    try
    {
		PmergeMe pmergeMe(argc, argv);
        pmergeMe.sort();
        pmergeMe.printResults();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
