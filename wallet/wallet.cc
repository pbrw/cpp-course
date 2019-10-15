#include <exception>
#include <regex>
#include <stdexcept>
#include <cstdlib>
#include <utility>
#include <iomanip>
#include <iterator>

#include "wallet.h"

namespace {

    class TooManyBException: public std::exception
    {
        virtual const char* what() const throw()
        {
            return "BajtekCoins upper limit exceeded (21e6 B)";
        }
    } tooManyB;

    class TooLittleBException: public std::exception
    {
        virtual const char* what() const throw()
        {
            return "BajtekCoins lower limit exceeded (0 B)";
        }
    } tooLittleB;

    class OperationIndexOutOfBoundsException: public std::exception {
        virtual const char* what() const throw() {
            return "Operation index out of bounds";
        }
    } invalidOperationIndex;

    // It is guaranteed by regex that exp <= 8
    unsigned long long pow10(unsigned long exp){
        unsigned long long x = 1;
        for(unsigned long i = 0; i < exp; i++){
            x *= 10;
        }
        return x;
    }

    unsigned long long convertToUll(const std::string &str, unsigned long long units){
        static const std::regex regex(
                // integer part
                R"(^(?:\s*)(0*[1-9]?[0-9]{0,7}))"

                // mantissa
                R"(([,.]([0-9]{0,8}))?(?:\s*)$)"
                );

        std::smatch parsedData;
        auto regexSuccess = std::regex_match(str, parsedData, regex);

        try{
            if(regexSuccess){
                std::string::size_type idx = 0;
                // res = integer part + mantissa
                unsigned long long res = std::stoull(parsedData.str(1), &idx, 10) * units;

                if(parsedData.str(3).size() != 0){
                    res += std::stoull(parsedData.str(3), &idx, 10) * (units / pow10(parsedData.str(3).size()));
                }

                return res;
            }
            else{
                throw std::invalid_argument("");
            }
        } catch (...){
            throw std::invalid_argument("Invalid argument passed to Wallet(str) constuctor");
        }
    }

    unsigned long long convertBinToUll(const std::string &str){
        static const std::regex regexBin(
                R"(^([0-1]{1,})$)"
                );

        std::smatch parsedData;
        auto regexSuccess = std::regex_match(str, parsedData, regexBin);

        unsigned long long x;

        try{
            if(regexSuccess){
                std::string::size_type idx = 0;
                x = std::stoull(parsedData.str(0), &idx, 2);
            }
            else{
                throw std::invalid_argument("");
            }
        } catch (...){
            throw std::invalid_argument("Invalid argument passed to Wallet::fromBinary(str)");
        }

        return x;
    }
}

void Wallet::increaseBalance(unsigned long long delta) {
    if (delta > B_NOT_IN_CIRCULATION) {
        throw tooManyB;
    }

    balance += delta;
    B_NOT_IN_CIRCULATION -= delta;
}

void Wallet::decreaseBalance(unsigned long long delta) {
    if (delta > balance) {
        throw tooLittleB;
    }

    balance -= delta;
    B_NOT_IN_CIRCULATION += delta;
}

void Wallet::updateHistory() {
    operationsHistory.push_back(Operation(balance));
}

Wallet::~Wallet() {
    B_NOT_IN_CIRCULATION += this->balance;
}

Wallet::Wallet() {
    operationsHistory.push_back(Operation(0));
}

Wallet::Wallet(int n) : Wallet((long long) n){}

Wallet::Wallet(unsigned int n) : Wallet((unsigned long long) n) {}

Wallet::Wallet(long long n) {
    if (n < 0) {
        throw tooLittleB;
    }

    unsigned long long initialBalance = n;
    initialBalance *= UNITS_IN_B;

    if (initialBalance > B_NOT_IN_CIRCULATION){
        throw tooManyB;
    }

    increaseBalance(initialBalance);
    updateHistory();
}

Wallet::Wallet(unsigned long long n) {
    unsigned long long initialBalance = n;
    initialBalance *= UNITS_IN_B;

    if (initialBalance > B_NOT_IN_CIRCULATION){
        throw tooManyB;
    }

    increaseBalance(initialBalance);
    updateHistory();
}

Wallet::Wallet(Wallet&& w1, Wallet&& w2)
        : balance(w1.balance + w2.balance) {

    std::vector<Operation> w;
    w.reserve(w1.opSize() + w2.opSize() + 1);

    std::merge(w1.operationsHistory.begin(), w1.operationsHistory.end(),
               w2.operationsHistory.begin(), w2.operationsHistory.end(),
               std::back_inserter(w));

    w1.balance = 0;
    w2.balance = 0;

    this->operationsHistory = w;
    operationsHistory.push_back(Operation(balance));
}

Wallet Wallet::fromBinary(const std::string &str) {
    unsigned long long x = convertBinToUll(str);

    return Wallet(x);
}

Wallet::Wallet(const std::string &str) {
    unsigned long long initialBalance = convertToUll(str, UNITS_IN_B);

    increaseBalance(initialBalance);
    updateHistory();
}

Wallet::Wallet(const char* str) : Wallet(std::string(str)) {}

Wallet::Wallet(Wallet &&wallet)
    : balance(wallet.balance)
    , operationsHistory(std::move(wallet.operationsHistory)) {

    updateHistory();
}

Wallet& Wallet::operator= (Wallet&& wallet) {
    if (this == &wallet) return *this;

    balance = wallet.balance;
    operationsHistory = std::move(wallet.operationsHistory);
    updateHistory();
    
    return *this;
}

Wallet& Wallet::operator+= (Wallet& wallet) {
    increaseBalance(wallet.balance);
    wallet.decreaseBalance(wallet.balance);
    
    updateHistory();
    wallet.updateHistory();
    
    return *this;
}

Wallet& Wallet::operator+= (Wallet&& wallet) {
    return *this += wallet;
}

Wallet operator+ (Wallet&& wallet, Wallet& wallet2) {
    Wallet result = Wallet(std::move(wallet));
    result += wallet2;
    return result;
}

Wallet operator+ (Wallet&& wallet, Wallet&& wallet2) {
    Wallet result = std::move(wallet);
    result.increaseBalance(wallet2.balance);
    
    return result;
}

Wallet& Wallet::operator-= (Wallet& wallet) {
    decreaseBalance(wallet.balance);
    wallet.increaseBalance(wallet.balance);
    
    updateHistory();
    wallet.updateHistory();
    
    return *this;
}

Wallet& Wallet::operator-= (Wallet&& wallet) {
    return *this -= wallet;
}

Wallet operator- (Wallet&& wallet, Wallet& wallet2) {
    Wallet result = Wallet(std::move(wallet));
    result -= wallet2;
    return result;
}

Wallet operator- (Wallet&& wallet, Wallet&& wallet2) {
    Wallet result = std::move(wallet);
    result.decreaseBalance(wallet2.balance);

    return result;
}

Wallet& Wallet::operator*= (int n) {
    if (n == 0) decreaseBalance(balance);
    else increaseBalance(balance * (n - 1));

    updateHistory();
    
    return *this;
}

Wallet operator* (Wallet& wallet, int n) {
    Wallet result = Wallet(std::move(wallet));
    result *= n;
    return result;
}

Wallet operator* (Wallet&& wallet, int n) {
    return wallet * n;
}

Wallet operator* (int n, Wallet& wallet) {
    return wallet * n;
}

Wallet operator* (int n, Wallet&& wallet) {
    return wallet * n;
}

unsigned long long Wallet::getUnits() const {
    return balance;
}

size_t Wallet::opSize() const {
    return operationsHistory.size();
}

const Wallet& Empty() {
    static const Wallet wallet;
    return wallet;
}

bool operator== (const Wallet& wallet, const Wallet& wallet2) {
    return wallet.balance == wallet2.balance;
}

bool operator< (const Wallet& wallet, const Wallet& wallet2) {
    return wallet.balance < wallet2.balance;
}

bool operator<= (const Wallet& wallet, const Wallet& wallet2) {
    return wallet.balance <= wallet2.balance;
}

bool operator> (const Wallet& wallet, const Wallet& wallet2) {
    return wallet.balance > wallet2.balance;
}

bool operator>= (const Wallet& wallet, const Wallet& wallet2) {
    return wallet.balance >= wallet2.balance;
}

bool operator!= (const Wallet& wallet, const Wallet& wallet2) {
    return wallet.balance != wallet2.balance;
} 

Operation::Operation(unsigned long long finalBalance) {
    this->finalBalance = finalBalance;
    this->time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
}

unsigned long long Operation::getUnits() const {
    return finalBalance;
}

const Operation Wallet::operator[] (int i) const {
    if ((size_t)i >= operationsHistory.size()) {
        throw invalidOperationIndex;
    } 
    return operationsHistory[i];
}

std::ostream& operator<< (std::ostream& os, const Operation& op)
{
    time_t t = std::chrono::system_clock::to_time_t(op.time);
    std::tm tm = *std::localtime(&t);
    os << "Wallet balance is " << op.finalBalance << std::put_time(&tm, " B after operation made at day %F");
    return os;
}

std::ostream& operator<< (std::ostream& os, const Wallet& wallet)
{
    unsigned long long integerPart = wallet.balance / wallet.UNITS_IN_B;
    unsigned long long decimalPart = wallet.balance % wallet.UNITS_IN_B;

    os << "Wallet[" << integerPart;
    if (decimalPart) {
        while (decimalPart % 10 == 0) decimalPart /= 10;
        os << "," << decimalPart;
    }
    os << " B]";

    return os;
}
