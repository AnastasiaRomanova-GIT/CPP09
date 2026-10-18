#include "../includes/BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() :
    p_dataFile("data.csv"), 
    p_inputFile("input.txt")
{
    loadDB();
}

BitcoinExchange::BitcoinExchange(const std::string& inputFilename) :
    p_dataFile("data.csv"), 
    p_inputFile(inputFilename)
{
    loadDB();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) :
    p_dataFile(other.p_dataFile), 
    p_inputFile(other.p_inputFile),
    p_dataMap(other.p_dataMap)
{}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) 
{
    if (this != &other) {
        p_dataFile = other.p_dataFile;
        p_inputFile = other.p_inputFile;
        p_dataMap = other.p_dataMap;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}


//////////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////////

void BitcoinExchange::processInput()
{
    std::ifstream inputFile(this->p_inputFile.c_str());
    if (!inputFile.is_open()) 
    {
        throw InputError(this->p_inputFile, "could not open file");
    }
    std::string line;    
    while (std::getline(inputFile, line)) 
    {
        if (this->trim(line) == "date | value") {
            continue;
        }
        if (!line.empty())
        {
            this->processLine(line);
        }
    }
}

std::string BitcoinExchange::trim(const std::string& str) const
{
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    
    if (start == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

int BitcoinExchange::parseAmount(const std::string& valueStr, double& amount) const
{
    char *endptr;
    amount = std::strtod(valueStr.c_str(), &endptr);
    
    if (*endptr != '\0') {
        return 1; // Error: Bad input format (garbage characters)
    }
    if (amount < 0) {
        return 2; // Error: Negative number
    }
    if (amount > 1000) {
        return 3; // Error: Value exceeds bound
    }
    return 0; // Success
}

void BitcoinExchange::lookupAndPrint(const std::string& date, double amount)
{
    std::map<std::string, double>::iterator it = p_dataMap.lower_bound(date);
    
    if (it == p_dataMap.begin() && it->first != date) {
        std::cerr << "Error: no historical data found for " << date << std::endl;
        return;
    }
    if (it == p_dataMap.end() || it->first != date) {
        --it;
    }
    
    double result = amount * it->second;
    std::cout << date << " => " << amount << " = " << result << std::endl;
}

void BitcoinExchange::processLine(const std::string& line)
{
    size_t pipePos = line.find('|');
    if (pipePos == std::string::npos) 
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return; 
    }

    std::string date = this->trim(line.substr(0, pipePos));
    std::string valueStr = this->trim(line.substr(pipePos + 1));

    if (date.empty() || valueStr.empty()) 
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        return;
    }

    if (!this->isValidDate(date)) 
    {
        std::cerr << "Error: bad input => " << date << std::endl;
        return;
    }

    double amount;
    int errorStatus = this->parseAmount(valueStr, amount);
    
    if (errorStatus == 1) {
        std::cerr << "Error: bad input => " << line << std::endl;
        return;
    } 
    else if (errorStatus == 2) {
        std::cerr << "Error: not a positive number." << std::endl;
        return;
    } 
    else if (errorStatus == 3) {
        std::cerr << "Error: too large a number." << std::endl;
        return;
    }

    this->lookupAndPrint(date, amount);
}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(date[i])) return false;
    }

    int year, month, day;
    std::string yearStr = date.substr(0, 4);
    std::string monthStr = date.substr(5, 2);
    std::string dayStr = date.substr(8, 2);

    std::stringstream ss(yearStr + " " + monthStr + " " + dayStr);
    ss >> year >> month >> day;

    if (month < 1 || month > 12 || day < 1 || day > 31) return false;

    // Handle days in a month and leap years
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[2] = 29;
    }

    if (day > daysInMonth[month]) return false;

    return true;
}

// private
void BitcoinExchange::loadDB() 
{
    std::ifstream dataFile(this->p_dataFile.c_str());
    if (!dataFile.is_open()) 
    {
        throw InputError(this->p_dataFile, "could not open file");
    }
    
    std::string line;
    std::getline(dataFile, line); // Skip the header line
    
    while (std::getline(dataFile, line)) 
    {
        size_t commaPosition = line.find(',');
        if (commaPosition == std::string::npos) 
        {
            throw InputError(this->p_dataFile, "invalid line format");
        }
        
        std::string date = line.substr(0, commaPosition);
        std::string valueStr = line.substr(commaPosition + 1);
        
        std::stringstream ss(valueStr);
        double value;
        char c;
        
        if (!(ss >> value) || (ss >> c)) 
        {
            throw InputError(this->p_dataFile, "invalid value format");
        }
        
        this->p_dataMap[date] = value;
    }
}
