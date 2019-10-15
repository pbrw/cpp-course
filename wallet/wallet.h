#ifndef _WALLET_H_
#define _WALLET_H_

#include <vector>
#include <ctime>
#include <chrono>
#include <iostream>

using namespace std::rel_ops;

class Operation {
    private:
        unsigned long long finalBalance;
        std::chrono::time_point<std::chrono::system_clock> time;

    public:
        Operation(unsigned long long finalBalance);
        unsigned long long getUnits() const;
        bool operator==(const Operation& op) const {return this->time == op.time;}
        bool operator< (const Operation& op) const {return this->time < op.time;}
        friend std::ostream& operator<< (std::ostream& os, const Operation& op);

};

class Wallet {
    private: 
        static const unsigned long long UNITS_IN_B = 100'000'000;
        inline static unsigned long long B_NOT_IN_CIRCULATION = static_cast<unsigned long long>(21e14);
        unsigned long long balance = 0;
        
        std::vector<Operation> operationsHistory;
    
        void updateHistory();
        void increaseBalance(unsigned long long delta);
        void decreaseBalance(unsigned long long delta);
    
    public:
        ~Wallet();
        Wallet();
        Wallet(int n);
        Wallet(unsigned int n);
        Wallet(long long n);
        Wallet(unsigned long long n);
        explicit Wallet(const std::string &str);
        explicit Wallet(const char* str);
        // copy constructor explicitly forbidden
        Wallet(const Wallet& wallet) = delete;
        // copy assignment explicitly forbidden
        Wallet& operator= (const Wallet& wallet) = delete;
        // move assignment allowed
        Wallet& operator= (Wallet&& wallet);
        Wallet(Wallet&& wallet);

        Wallet(Wallet&& w1, Wallet&& w2);
        static Wallet fromBinary(const std::string &str);
        
        friend Wallet operator* (Wallet& wallet, int n);
        friend Wallet operator* (Wallet&& wallet, int n);
        friend Wallet operator* (int n, Wallet& wallet);
        friend Wallet operator* (int n, Wallet&& wallet);
        
        friend Wallet operator+ (Wallet&& wallet, Wallet& wallet2);
        friend Wallet operator+ (Wallet&& wallet, Wallet&& wallet2);
        
        friend Wallet operator- (Wallet&& wallet, Wallet& wallet2);
        friend Wallet operator- (Wallet&& wallet, Wallet&& wallet2);

        Wallet& operator+= (Wallet& wallet);
        Wallet& operator+= (Wallet&& wallet);
                
        Wallet& operator-= (Wallet& wallet);
        Wallet& operator-= (Wallet&& wallet);
        
        Wallet& operator*= (int n);

        friend bool operator== (const Wallet& wallet, const Wallet& wallet2); 
        friend bool operator< (const Wallet& wallet, const Wallet& wallet2);
        friend bool operator<= (const Wallet& wallet, const Wallet& wallet2);
        friend bool operator> (const Wallet& wallet, const Wallet& wallet2);
        friend bool operator>= (const Wallet& wallet, const Wallet& wallet2);
        friend bool operator!= (const Wallet& wallet, const Wallet& wallet2);

        const Operation operator[] (int i) const;

        unsigned long long getUnits() const;
        size_t opSize() const;

        friend std::ostream& operator<< (std::ostream& os, const Wallet& op);


        // preventing implicit conversions
        template<typename T> Wallet(T arg) = delete;
        
        template<typename T> friend Wallet operator* (Wallet&, T) = delete;
        template<typename T> friend Wallet operator* (T, Wallet&) = delete;    
        template<typename T> friend Wallet operator* (Wallet&&, T) = delete;
        template<typename T> friend Wallet operator* (T, Wallet&&) = delete;
        
        template<typename T> Wallet& operator*= (T) = delete;

};

const Wallet& Empty();



#endif // _WALLET_H_
