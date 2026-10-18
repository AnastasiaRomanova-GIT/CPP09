#include "../../includes/PmergeMe.hpp"


//////////////////////////////////////
// Sorting finctions for list
//////////////////////////////////////

void PmergeMe::insertValueList(std::list<int>& chain, int value, size_t right)
{
    size_t left = 0;

    while (left < right)
    {
        size_t middle = left + (right - left) / 2;
        std::list<int>::iterator middleIt = chain.begin();

        std::advance(middleIt, middle);
        if (p_opTracking)
            p_operationCounterList++;
        if (value < *middleIt)
            right = middle;
        else
            left = middle + 1;
    }

    std::list<int>::iterator insertIt = chain.begin();
    std::advance(insertIt, left);
    chain.insert(insertIt, value);
}

std::list<int> PmergeMe::FJsortValuesList(const std::list<int>& values)
{
    if (values.size() <= 1)
        return values;

    std::list<int> largeValues;
    std::list<int> smallValues;
    std::list<int> partners;
    bool hasOdd = values.size() % 2 != 0;
    int oddValue = 0;

    std::list<int>::const_iterator it = values.begin();
    while (it != values.end())
    {
        int first = *it;
        ++it;
        if (it == values.end())
        {
            oddValue = first;
            break;
        }

        int second = *it;
        ++it;

        if (p_opTracking)
            p_operationCounterList++;
        if (first < second)
        {
            smallValues.push_back(first);
            largeValues.push_back(second);
            partners.push_back(second);
        }
        else
        {
            smallValues.push_back(second);
            largeValues.push_back(first);
            partners.push_back(first);
        }
    }

    std::list<int> sortedWinners = FJsortValuesList(largeValues);
    std::list<int> chain = sortedWinners;
    std::list<int> pendingSmallValues;
    std::list<int> pendingPartners;

    size_t winnerIndex = 0;
    for (std::list<int>::iterator winnerIt = sortedWinners.begin();
        winnerIt != sortedWinners.end(); ++winnerIt, ++winnerIndex)
    {
        std::list<int>::iterator smallIt = smallValues.begin();
        std::list<int>::iterator partnerIt = partners.begin();

        for (; partnerIt != partners.end(); ++partnerIt, ++smallIt)
        {
            if (*partnerIt == *winnerIt)
                break;
        }
        if (smallIt != smallValues.end())
        {
            if (winnerIndex == 0)
                chain.insert(chain.begin(), *smallIt);
            else
            {
                pendingSmallValues.push_back(*smallIt);
                pendingPartners.push_back(*partnerIt);
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
        std::list<int>::iterator smallIt = pendingSmallValues.begin();
        std::list<int>::iterator partnerIt = pendingPartners.begin();
        size_t right = chain.size();

        std::advance(smallIt, order[i]);
        std::advance(partnerIt, order[i]);

        if (*partnerIt != 0)
        {
            std::list<int>::iterator chainIt = chain.begin();
            right = 0;
            while (chainIt != chain.end() && *chainIt != *partnerIt)
            {
                ++chainIt;
                ++right;
            }
            if (chainIt == chain.end())
                right = chain.size();
        }
        insertValueList(chain, *smallIt, right);
    }

    return chain;
}

static void constructChainsList(std::list<s_pending>& pendingPairs, std::list<s_pair>& upSortedPairs, std::list<s_pair>& originalPairs, 
    std::list<s_pair*>& mainChain)
{
    // b1: small partner of the smallest winner
    mainChain.push_back(upSortedPairs.front().prev_small);

    // Add all winners to the main chain
    std::list<s_pair>::iterator it = upSortedPairs.begin();

    for (; it != upSortedPairs.end(); ++it)
        mainChain.push_back(it->prev_large);

    // All remaining losers become pending elements.
    // Skip the first one because b1 is already in mainChain.
    it = upSortedPairs.begin();
    ++it;

    for (; it != upSortedPairs.end(); ++it)
    {
        s_pending item;

        item.value = it->prev_small;
        item.partner = it->prev_large;

        pendingPairs.push_back(item);
    }

    // Odd element has no partner
    if (originalPairs.size() % 2 != 0)
    {
        s_pending odd;

        odd.value = &originalPairs.back();
        odd.partner = NULL;

        pendingPairs.push_back(odd);
    }
}

std::list<s_pair> PmergeMe::insertJacobsthalList(std::list<s_pair>& upSortedPairs,  std::list<s_pair>& originalPairs)
{
    std::list<s_pair*> mainChain;
    std::list<s_pending> pendingPairs;

    constructChainsList(
        pendingPairs,
        upSortedPairs,
        originalPairs,
        mainChain);

    std::vector<size_t> order =
        getJacobsthalOrder(pendingPairs.size());

    for (size_t i = 0; i < order.size(); ++i)
    {
        // Find pendingPairs[order[i]] using an iterator
        std::list<s_pending>::iterator pendingIt =
            pendingPairs.begin();

        std::advance(pendingIt, order[i]);

        s_pending& pendingCurrentElement = *pendingIt;

        /*
         * Find the exclusive right boundary.
         *
         * If there is no partner (odd element),
         * search until mainChain.end().
         *
         * If there is a partner, search only before it.
         */
        std::list<s_pair*>::iterator rightBoundary =
            mainChain.end();

        if (pendingCurrentElement.partner != NULL)
        {
            rightBoundary = mainChain.begin();

            while (rightBoundary != mainChain.end()
                && *rightBoundary != pendingCurrentElement.partner)
            {
                ++rightBoundary;
            }
        }

        /*
         * Binary search on the range:
         *
         * [mainChain.begin(), rightBoundary)
         *
         * Lists have no indexes, so we work with:
         *   first = beginning of current search range
         *   count = number of elements in the range
         */
        std::list<s_pair*>::iterator leftBoundary =
            mainChain.begin();

        size_t count =
            std::distance(leftBoundary, rightBoundary);

        while (count > 0)
        {
            std::list<s_pair*>::iterator middle =
                leftBoundary;

            size_t step = count / 2;

            std::advance(middle, step);

            if (p_opTracking)
                p_operationCounterList++;

            if (pendingCurrentElement.value->large
                < (*middle)->large)
            {
                // Search the left half
                count = step;
            }
            else
            {
                // Search the right half
                ++middle;
                leftBoundary = middle;
                count -= step + 1;
            }
        }

        // leftBoundary is now the insertion position
        mainChain.insert(
            leftBoundary,
            pendingCurrentElement.value);
    }

    // Convert list<s_pair*> back to list<s_pair>
    std::list<s_pair> result;

    std::list<s_pair*>::iterator it = mainChain.begin();

    for (; it != mainChain.end(); ++it)
        result.push_back(**it);

    return result;
}

std::list<s_pair> PmergeMe::FJsortList(std::list<s_pair>& pairs)
{
    if (pairs.size() <= 1)
    {
        return pairs;
    }
    std::list<s_pair> nextPairs;
    std::list<s_pair>::iterator it = pairs.begin();
    while (it != pairs.end())
    {
        std::list<s_pair>::iterator current = it;
        ++it;
        if (it == pairs.end())
            break;
        std::list<s_pair>::iterator second = it;
        ++it;

        s_pair currentPair;
        if (p_opTracking)
            p_operationCounterList++;   
        
        if (current->large > second->large)
        {
            currentPair.large = current->large;
            currentPair.small = second->large;

            currentPair.prev_large = &(*current);
            currentPair.prev_small = &(*second);
        }
        else
        {
            currentPair.large = second->large;
            currentPair.small = current->large;

            currentPair.prev_large = &(*second);
            currentPair.prev_small = &(*current);
        }
        if (p_opTracking)
            p_operationCounterList++;
        nextPairs.push_back(currentPair);
    }

    std::list<s_pair> upSortedPairs = FJsortList(nextPairs);
    std::list<s_pair> downSortedpairs = insertJacobsthalList(upSortedPairs, pairs);
    return downSortedpairs;

}

void PmergeMe::sortList()
{
    if (p_opTracking)
        p_operationCounterList = 0;
    clock_t startTime = clock();
    std::list<int> sortedValues = FJsortValuesList(p_initListExpr);
    p_resultList = valuesToStringList(sortedValues);
    clock_t endTime = clock();
    p_durationList = static_cast<double>(endTime - startTime) * 1000000.0 / CLOCKS_PER_SEC;
}
