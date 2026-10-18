#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <map>
#include <sstream> /* strtod */
#include <stdlib.h>
#include <cctype>

typedef struct s_result
{
    std::string date;
    double value;
}   t_result;

class InputError : public std::exception {
private:
    // variables
    std::string p_errMessage;

public:
    InputError(const std::string& filename, const std::string& reason) {
        p_errMessage = "Error: " + reason + " [" + filename + "]";
    }
    virtual ~InputError() throw() {}
    virtual const char* what() const throw() {
        return p_errMessage.c_str();
    }
};

// • Your program must take a file as an argument.
// • Each line in this file must use the following format: "date | value".
// • A valid date will always be in the following format: Year-Month-Day.
// • A valid value must be either a float or a positive integer, between 0 and 1000.

class BitcoinExchange
{
    private:
        // variables
        std::string p_dataFile;
        std::string p_inputFile;
        std::map<std::string, double> p_dataMap;

        // methods
        void loadDB();
        bool isValidDate(const std::string& date) const;
        // helpers
        void        processLine(const std::string& line);
        std::string trim(const std::string& str) const;
        int         parseAmount(const std::string& valueStr, double& amount) const;
        void        lookupAndPrint(const std::string& date, double amount);

    public:
        // constructors and operators
        BitcoinExchange();
        BitcoinExchange(const std::string& inputFilename);
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();

        // methods
        void processInput();
};

#endif
