#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <list>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>

struct s_pair
{
    int small;
    int large;
    s_pair* prev_small;
    s_pair* prev_large;
};

struct s_pending
{
    s_pair* value;    // looser
    s_pair* partner;  // corresponding winner
};

class PmergeMe
{
    private:
        PmergeMe();
        std::string p_expression;
        // original expressions
        std::vector<int> p_initVectorExpr;
        std::list<int> p_initListExpr;
        std::list<int> p_initialListExpr;   
		bool p_opTracking;

        // results
        std::string p_resultVector;
        std::string p_resultList;

        double p_durationVector;
        double p_durationList;

		size_t p_operationCounterVector;
		size_t p_operationCounterList;

        void sortVector();
        void sortList();

        //functions for sorting vector
        std::vector<s_pair> collectInitialPairsVector();
        std::vector<s_pair> FJsortVector(std::vector<s_pair>& pairs);
        std::vector<s_pair> recursivelySortVector(std::vector<s_pair>& pairs);
        std::vector<s_pair> insertJacobsthalVector(std::vector<s_pair>& upSortedPairs, std::vector<s_pair>& originalPairs);
        std::vector<size_t> getJacobsthalOrder(size_t pendingSize);
        std::vector<int> FJsortValuesVector(const std::vector<int>& values);
        void insertValueVector(std::vector<int>& chain, int value, size_t right);
        std::string valuesToStringVector(const std::vector<int>& values) const;
        void printResultsVector() const;
        
        //list functions
        std::list<s_pair> FJsortList(std::list<s_pair>& pairs);
        std::list<s_pair> insertJacobsthalList(std::list<s_pair>& upSortedPairs, std::list<s_pair>& originalPairs);
        std::list<s_pair> collectInitialPairsList();
        std::list<int> FJsortValuesList(const std::list<int>& values);
        void insertValueList(std::list<int>& chain, int value, size_t right);

        std::string valuesToStringList(const std::list<int>& values) const;
    public:
        PmergeMe(int argc, char** argv);
        PmergeMe(const std::string &expression);
        PmergeMe(const PmergeMe& other);
        ~PmergeMe();


        PmergeMe &operator=(const PmergeMe &other);

        void sort(void);
		void printResults(void);
};

#endif
