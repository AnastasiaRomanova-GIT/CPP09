#include "../../includes/PmergeMe.hpp"


///////////////////////////////////////////////
// Sorting functions for vector
////////////////////////////////////////////////
void PmergeMe::insertValueVector(std::vector<int>& chain, int value, size_t right)
{
    size_t left = 0;

    while (left < right)
    {
        size_t middle = left + (right - left) / 2;

        if (p_opTracking)
            p_operationCounterVector++;
        if (value < chain[middle])
            right = middle;
        else
            left = middle + 1;
    }
    chain.insert(chain.begin() + left, value);
}

std::vector<int> PmergeMe::FJsortValuesVector(const std::vector<int>& values)
{
    if (values.size() <= 1)
        return values;

    std::vector<int> largeValues;
    std::vector<int> smallValues;
    std::vector<int> partners;
    bool hasOdd = values.size() % 2 != 0;
    int oddValue = 0;

    for (size_t i = 0; i + 1 < values.size(); i += 2)
    {
        if (p_opTracking)
            p_operationCounterVector++;
        if (values[i] < values[i + 1])
        {
            smallValues.push_back(values[i]);
            largeValues.push_back(values[i + 1]);
            partners.push_back(values[i + 1]);
        }
        else
        {
            smallValues.push_back(values[i + 1]);
            largeValues.push_back(values[i]);
            partners.push_back(values[i]);
        }
    }
    if (hasOdd)
        oddValue = values.back();

    std::vector<int> sortedWinners = FJsortValuesVector(largeValues);
    std::vector<int> chain = sortedWinners;
    std::vector<int> pendingSmallValues;
    std::vector<int> pendingPartners;

    for (size_t winnerIndex = 0; winnerIndex < sortedWinners.size(); ++winnerIndex)
    {
        size_t pairIndex = 0;

        while (pairIndex < partners.size()
            && partners[pairIndex] != sortedWinners[winnerIndex])
            ++pairIndex;
        if (pairIndex < smallValues.size())
        {
            if (winnerIndex == 0)
                chain.insert(chain.begin(), smallValues[pairIndex]);
            else
            {
                pendingSmallValues.push_back(smallValues[pairIndex]);
                pendingPartners.push_back(partners[pairIndex]);
            }
        }
    }
    if (hasOdd)
    {
        pendingSmallValues.push_back(oddValue);
        pendingPartners.push_back(0);
    }

    std::vector<size_t> order = getJacobsthalOrder(pendingSmallValues.size());

    for (size_t i = 0; i < order.size(); ++i)
    {
        size_t index = order[i];
        size_t right = chain.size();

        if (pendingPartners[index] != 0)
        {
            right = 0;
            while (right < chain.size() && chain[right] != pendingPartners[index])
                ++right;
            if (right == chain.size())
                right = chain.size();
        }
        insertValueVector(chain, pendingSmallValues[index], right);
    }

    return chain;
}

static void constructChains(std::vector<s_pending>& pendingPairs, std::vector<s_pair>& upSortedPairs, std::vector<s_pair>& originalPairs, std::vector<s_pair*>& mainChain)
{
    // small pair from the smallest current
    mainChain.push_back(upSortedPairs[0].prev_small);

    //copy the large elements
    for (size_t i = 0; i < upSortedPairs.size(); ++i)
        mainChain.push_back(upSortedPairs[i].prev_large);

    // the rest of the loosers make own company
    for (size_t i = 1; i < upSortedPairs.size(); ++i)
    {
        s_pending item;

        item.value = upSortedPairs[i].prev_small;
        item.partner = upSortedPairs[i].prev_large;

        pendingPairs.push_back(item);
    }

    // odd element, if this level had one
    if (originalPairs.size() % 2 != 0)
    {
        s_pending odd;

        odd.value = &originalPairs.back();
        odd.partner = NULL;

        pendingPairs.push_back(odd);
    }
}

std::vector<s_pair> PmergeMe::insertJacobsthalVector(std::vector<s_pair>& upSortedPairs, std::vector<s_pair>& originalPairs)
{
    std::vector<s_pair*> mainChain;
    std::vector<s_pending> pendingPairs;

    constructChains(pendingPairs, upSortedPairs, originalPairs, mainChain);
    std::vector<size_t> order = getJacobsthalOrder(pendingPairs.size());

    // Insert pending elements in accordance with the Jacobsthal order.
    for (size_t i = 0; i < order.size(); ++i)
    {
        s_pending& pendingCurrentElement = pendingPairs[order[i]];
        size_t rightBoundary = mainChain.size();
        if (pendingCurrentElement.partner != NULL)
        {
            rightBoundary = 0;
            while (mainChain[rightBoundary] != pendingCurrentElement.partner)
                ++rightBoundary;
        }

        size_t leftBoundary = 0;

        while (leftBoundary < rightBoundary)
        {
            size_t middle = leftBoundary + (rightBoundary - leftBoundary) / 2;
            if (p_opTracking)
                p_operationCounterVector++;

            if (pendingCurrentElement.value->large < mainChain[middle]->large)
                rightBoundary = middle;
            else
                leftBoundary = middle + 1;
        }

        mainChain.insert(mainChain.begin() + leftBoundary, pendingCurrentElement.value);
    }

    std::vector<s_pair> result;

    for (size_t i = 0; i < mainChain.size(); ++i)
        result.push_back(*mainChain[i]);

    return result;
}

std::vector<s_pair> PmergeMe::FJsortVector(std::vector<s_pair>& pairs)
{
    if (pairs.size() <= 1)
    {
        return pairs;
    }

    std::vector<s_pair> nextPairs;

    size_t pairCount = pairs.size() / 2;
    for (size_t i = 0; i < pairCount; ++i)
    {
        s_pair currentPair;
        size_t index = i * 2;
        if (p_opTracking)
            p_operationCounterVector++;

        if (pairs[index].large > pairs[index + 1].large)
        {
            currentPair.large = pairs[index].large;
            currentPair.small = pairs[index + 1].large;

            currentPair.prev_large = &pairs[index];
            currentPair.prev_small = &pairs[index + 1];
        }
        else
        {
            currentPair.large = pairs[index + 1].large;
            currentPair.small = pairs[index].large;

            currentPair.prev_large = &pairs[index + 1];
            currentPair.prev_small = &pairs[index];
        }
        if (p_opTracking)
            p_operationCounterVector++;

        nextPairs.push_back(currentPair);
    }

    std::vector<s_pair> upSortedPairs = FJsortVector(nextPairs);
    std::vector<s_pair> downSortedPairs = insertJacobsthalVector(upSortedPairs, pairs);
    return downSortedPairs;
}

void PmergeMe::sortVector()
{
    if (p_opTracking)
		p_operationCounterVector = 0;
	clock_t startTime = clock();
    std::vector<int> sortedValues = FJsortValuesVector(p_initVectorExpr);
    p_resultVector = valuesToStringVector(sortedValues);
    clock_t endTime = clock();
    p_durationVector = static_cast<double>(endTime - startTime) * 1000000.0 / CLOCKS_PER_SEC;
}
