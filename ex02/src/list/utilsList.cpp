#include "../../includes/PmergeMe.hpp"

static std::string intToStringList(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::list<s_pair> PmergeMe::collectInitialPairsList()
{
    std::list<s_pair> pairs;
    std::list<int>::iterator iterPairs = p_initListExpr.begin();

    while (iterPairs != p_initListExpr.end())
    {
        int first = *iterPairs;
        ++iterPairs;

        // first is the odd element
        if (iterPairs == p_initListExpr.end())
        {
            s_pair odd;
            odd.small = first;
            odd.large = first;
            odd.prev_small = NULL;
            odd.prev_large = NULL;
            pairs.push_back(odd);
            break;
        }

        int second = *iterPairs;
        ++iterPairs;

        s_pair pair;

        if (p_opTracking)
            p_operationCounterList++;

        if (first < second)
        {
            pair.small = first;
            pair.large = second;
        }
        else
        {
            pair.small = second;
            pair.large = first;
        }
        pair.prev_small = NULL;
        pair.prev_large = NULL;

        pairs.push_back(pair);
    }

    return pairs;
}

std::string PmergeMe::valuesToStringList(const std::list<int>& values) const
{
    std::string result;

    for (std::list<int>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        if (!result.empty())
            result += " ";
        result += intToStringList(*it);
    }
    return result;
}
