#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <iostream>
#include <sstream>

class RPN
{
private:
    std::stack<int> p_stack;
    std::string p_expression;
    int p_result;

    RPN();

public:
    RPN(const std::string& expression);
    RPN(const RPN& other);
    ~RPN();

    //operators 
    RPN& operator=(const RPN& other);

    //functions
    void evaluate(void);
    void processToken(const std::string& token);
    void performOperation(const std::string& operation);
    int getResult(void) const;
};

#endif // RPN_HPP
