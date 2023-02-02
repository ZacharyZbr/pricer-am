#include "PriceComputer.hpp"
#include "Performance.hpp"
#include "GeometricPut.hpp"
#include "BasketOption.hpp"

PriceComputer::PriceComputer(Param *P) {
	this->P = P;
    P->extract("option type", this->type);
    P->extract("maturity", this->T);
    P->extract("model size", this->size);
    P->extract("spot", this->spot, this->size);
    P->extract("volatility", this->sigma, this->size);
    P->extract("interest rate", this->r);
    P->extract("correlation", this->rho);
    P->extract("dates", this->dates);
    P->extract("degree for polynomial regression", this->degree);
    if (P->extract("dividend rate", this->dividends, this->size, true) == false)
    {
        this->dividends = pnl_vect_create_from_zero(this->size);
    }
    P->extract("strike", this->strike);
    P->extract("MC iterations", this->n_samples);

}

PriceComputer::~PriceComputer() {
    pnl_vect_free(&spot);
    pnl_vect_free(&sigma);
    pnl_vect_free(&dividends);
}

double PriceComputer::compute_bestof(PnlVect* lambda) {
    Performance* option = new Performance(T, dates, size, strike, lambda);
    BlackScholesModel* mod = new BlackScholesModel(size, r, rho, sigma, spot, dividends);
    MonteCarlo* myMC = new MonteCarlo(mod, option);
    double price = myMC->price(dates, n_samples, degree);
    return price;
}

double PriceComputer::compute_basket(PnlVect* lambda) {
    BasketOption* option = new BasketOption(T, dates, size, strike, lambda);
    BlackScholesModel* mod = new BlackScholesModel(size, r, rho, sigma, spot, dividends);
    MonteCarlo* myMC = new MonteCarlo(mod, option);
    double price = myMC->price(dates, n_samples, degree);
    return price;
}

double PriceComputer::compute_put_geom() {
    GeometricPut* option = new GeometricPut(T, dates, size, strike);
    BlackScholesModel* mod = new BlackScholesModel(size, r, rho, sigma, spot, dividends);
    MonteCarlo* myMC = new MonteCarlo(mod, option);
    double price = myMC->price(dates, n_samples, degree);
    return price;
}

double PriceComputer::compute_price() {
    
        if (type._Equal("bestof")) {
            PnlVect* lambdas;
            P->extract("payoff coefficients", lambdas, size);
            return compute_bestof(lambdas);
        }
        if (type._Equal("geometric_put")) {
            return compute_put_geom();
        }
        if (type._Equal("exchange")) {
            PnlVect* lambdas;
            P->extract("payoff coefficients", lambdas, size);
            return compute_basket(lambdas);
        }
        else {
            std::cout<<"Sorry this code does not price this option type, the price will be 0"<<std::endl;
        }
    


	return 0;
}

////P->extract("payoff coefficients", lambda, size);