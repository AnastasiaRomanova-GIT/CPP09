#include "../includes/PmergeMe.hpp"

PmergeMe::PmergeMe() : p_expression(""), p_opTracking(false), p_durationVector(0), p_durationList(0),
    p_operationCounterVector(0), p_operationCounterList(0)
{
	#if defined COUNTCOMPARISONS
		p_opTracking = true;
	#else 
		p_opTracking = false;
	#endif
}

PmergeMe::PmergeMe(const std::string &expression) : p_expression(expression), p_opTracking(false),
    p_durationVector(0), p_durationList(0), p_operationCounterVector(0), p_operationCounterList(0)
{
	#if defined COUNTCOMPARISONS
		p_opTracking = true;
	#else 
		p_opTracking = false;
	#endif
}

PmergeMe::PmergeMe(int argc, char** argv)
    : p_expression(""), p_opTracking(false), p_durationVector(0), p_durationList(0),
      p_operationCounterVector(0), p_operationCounterList(0)
{
    if (argc < 2)
        throw std::invalid_argument("No input provided.");

    for (int i = 1; i < argc; ++i)
    {
        if (!p_expression.empty())
            p_expression += " ";
        p_expression += argv[i];

        std::istringstream stream(argv[i]);
        int value;

        while (stream >> value)
        {
            if (value < 0)
                throw std::invalid_argument(
                    "Only positive integers are allowed.");

            p_initVectorExpr.push_back(value);
            p_initListExpr.push_back(value);
            p_initialListExpr.push_back(value);
        }

        if (!stream.eof())
            throw std::invalid_argument("Invalid input.");
    }

    if (p_initVectorExpr.empty())
        throw std::invalid_argument("No valid numbers provided.");

#if defined COUNTCOMPARISONS
    p_opTracking = true;
#else
    p_opTracking = false;
#endif
}

PmergeMe::PmergeMe(const PmergeMe& other) : p_expression(other.p_expression),
    p_initVectorExpr(other.p_initVectorExpr), 
	p_initListExpr(other.p_initListExpr),
    p_initialListExpr(other.p_initialListExpr),
	p_opTracking(other.p_opTracking),
    p_resultVector(other.p_resultVector), 
	p_resultList(other.p_resultList),
    p_durationVector(other.p_durationVector),
	p_durationList(other.p_durationList),
    p_operationCounterVector(other.p_operationCounterVector),
    p_operationCounterList(other.p_operationCounterList)
{}
PmergeMe::~PmergeMe() {}

//operators
PmergeMe& PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        p_expression = other.p_expression;
        p_initVectorExpr = other.p_initVectorExpr;
        p_initListExpr = other.p_initListExpr;
        p_initialListExpr = other.p_initialListExpr;
        p_resultVector = other.p_resultVector;
        p_resultList = other.p_resultList;
        p_durationVector = other.p_durationVector;
        p_durationList = other.p_durationList;
		p_opTracking = other.p_opTracking;
        p_operationCounterVector = other.p_operationCounterVector;
        p_operationCounterList = other.p_operationCounterList;
    }
    return *this;
}


void PmergeMe::sort(void)
{
    sortVector();
    sortList();
}

void PmergeMe::printResults(void)
{
    std::cout << "Before: " << p_expression << "\n";
    std::cout << "After: "  << p_resultVector << "\n";
    std::cout << "Time to process a range of " << p_initVectorExpr.size() << " elements with std::vector : " << p_durationVector << "us\n";
	if (p_opTracking)
		std::cout << "Comparisons Number: " << p_operationCounterVector << std::endl;
    std::cout << "Time to process a range of " << p_initListExpr.size() << " elements with std::list : " << p_durationList << "us\n";
	if (p_opTracking)
		std::cout << "Comparisons Number: " << p_operationCounterList << std::endl;

}
