#include "../includes/RPN.hpp"

RPN::RPN() {}
RPN::RPN(const std::string& expression) : p_expression(expression) {}
RPN::RPN(const RPN& other) : p_stack(other.p_stack), p_expression(other.p_expression) {}
RPN::~RPN() {}


//operators
RPN& RPN::operator=(const RPN& other)
{
    if (this != &other)
    {
        p_stack = other.p_stack;
        p_expression = other.p_expression;
    }
    return *this;
} 

//functions
void RPN::performOperation(const std::string& operation)
{
    if (p_stack.size() < 2)
    {
        throw std::runtime_error("Not enough operands for operation '" + operation + "'");
    }

    int second = p_stack.top();
    p_stack.pop();
    int first = p_stack.top();
    p_stack.pop();

    int result;
    if (operation == "+")
        result = first + second;
    else if (operation == "-")
        result = first - second;
    else if (operation == "*")
        result = first * second;
    else if (operation == "/")
    {
        if (second == 0)
            throw std::runtime_error("Division by zero");
        result = first / second;
    }
    else
        throw std::runtime_error("Unknown operation '" + operation + "'");

    p_stack.push(result);
}

void RPN::processToken(const std::string& token)
{
    if (token.size() == 1 && std::isdigit(token[0]))
    {
        p_stack.push(token[0] - '0');
    }
    else if (token == "+" || token == "-" || token == "*" || token == "/")
    {
        performOperation(token);
    }
    else
    {
        throw std::runtime_error("Invalid token '" + token + "'");
    }
}


void RPN::evaluate(void)
{
    std::istringstream stream(p_expression);
    std::string token;
    while (stream >> token)
    {
        processToken(token);
    }
    if (p_stack.size() != 1)
    {
        throw std::runtime_error("");
    }
    p_result = p_stack.top();
}

int RPN::getResult(void) const
{
    return p_result;
}
