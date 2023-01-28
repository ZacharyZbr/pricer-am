#include <iostream>
#include "PricingResults.hpp"

std::ostream& operator<<(std::ostream &stm, const PricingResults &res)
{
    stm << '{' << "\"price\": " << res.price << '}';
    return stm;
}