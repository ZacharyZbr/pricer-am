#pragma once
#include <string>

class PricingResults
{
private:
    double price;
public:
    PricingResults(double price)
        : price(price)
    { }

    ~PricingResults() { }

    friend std::ostream& operator<<(std::ostream &stm, const PricingResults &res);
};
