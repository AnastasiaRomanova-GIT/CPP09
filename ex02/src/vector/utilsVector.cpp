#include "../../includes/PmergeMe.hpp"


std::string intToString(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string PmergeMe::valuesToStringVector(const std::vector<int>& values) const
{
    std::string result;

    for (size_t i = 0; i < values.size(); ++i)
    {
        if (i != 0)
            result += " ";
        result += intToString(values[i]);
    }
    return result;
}

std::vector<s_pair> PmergeMe::collectInitialPairsVector()
{
    std::vector<s_pair> pairs;
    size_t pairCount = p_initVectorExpr.size() / 2;
    for (size_t i = 0; i < pairCount; ++i)
    {
        s_pair pair;
        size_t index = i * 2;
        if (p_opTracking)
			p_operationCounterVector++;
        if (p_initVectorExpr[index] < p_initVectorExpr[index + 1])
        {
            pair.small = p_initVectorExpr[index];
            pair.large = p_initVectorExpr[index + 1];
        }
        else
        {
            pair.small = p_initVectorExpr[index + 1];
            pair.large = p_initVectorExpr[index];
        }
        pair.prev_small = NULL;
        pair.prev_large = NULL;
        pairs.push_back(pair);
    }

    if (p_initVectorExpr.size() % 2 != 0)
    {
        s_pair odd;

        odd.small = p_initVectorExpr.back();
        odd.large = p_initVectorExpr.back();
        odd.prev_small = NULL;
        odd.prev_large = NULL;
        pairs.push_back(odd);
    }
    return pairs;
}

std::vector<size_t> PmergeMe::getJacobsthalOrder(size_t pendingSize)
{
    std::vector<size_t> order;

    if (pendingSize == 0)
        return order;

    size_t previousJacobsthal = 1;
    size_t currentJacobsthal = 3;

    while (order.size() < pendingSize)
    {
        for (size_t loser = currentJacobsthal; loser > previousJacobsthal; --loser)
        {
            if (loser >= 2)
            {
                size_t index = loser - 2;

                if (index < pendingSize)
                    order.push_back(index);
            }
        }

        size_t nextJacobsthal = currentJacobsthal + 2 * previousJacobsthal;

        previousJacobsthal = currentJacobsthal;
        currentJacobsthal = nextJacobsthal;
    }

    return order;
}
