#include <iostream>
#include <ctime>
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "PricingResults.hpp"
#include "jlparser/parser.hpp"
#include "BasketOption.hpp"
#include "MonteCarlo.hpp"
#include "BlackScholesModel.hpp"


using namespace std;

int main(int argc, char** argv)
{
    double T, r, strike, rho;
    PnlVect* spot, * sigma, * divid, * lambda;
    string type;
    int size;
    int n_samples;
    int dates;
    int degree;

    char* infile = argv[1];
    Param* P = new Parser(infile);

    P->extract("option type", type);
    P->extract("maturity", T);
    P->extract("model size", size);
    P->extract("spot", spot, size);
    P->extract("volatility", sigma, size);
    P->extract("interest rate", r);
    P->extract("correlation", rho);
    P->extract("payoff coefficients", lambda, size);
    P->extract("dates", dates);
    P->extract("degree for polynomial regression", degree);
    if (P->extract("dividend rate", divid, size, true) == false)
    {
        divid = pnl_vect_create_from_zero(size);
    }
    P->extract("strike", strike);
    P->extract("MC iterations", n_samples);

    BasketOption* optiond5 = new BasketOption(T, dates, size, strike, lambda);
    BlackScholesModel* mod = new BlackScholesModel(size, r, rho, sigma, spot, divid);
    MonteCarlo* myMC = new MonteCarlo(mod, optiond5);
    double price = myMC->price(dates, n_samples, degree);

    std::cout << "le prix : " << price << std::endl;

    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_vect_free(&divid);
    delete P;

    return 0;
}
