#include <iostream>
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "PricingResults.hpp"
#include "jlparser/parser.hpp"
#include "PriceComputer.hpp"


using namespace std;

int main(int argc, char** argv)
{
    
    char* infile = argv[1];
    Param* P = new Parser(infile);

    PriceComputer* pricer = new PriceComputer(P);
    double price = pricer->compute_price();
    
    std::cout << PricingResults(price) << std::endl;

    delete pricer;
    delete P;

    return 0;
}
