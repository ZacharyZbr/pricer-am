#include <iostream>
#include "jlparser/parser.hpp"
#include "MonteCarlo.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_vector.h"


using namespace std;

class PriceComputer {
public:
	Param *P; /// the parser
	double T, r, strike, rho;
	PnlVect* spot, * sigma, *dividends;
	string type;
	int size;
	int n_samples;
	int dates;
	int degree;

	/**
	* Constructor of the pricer.
	* Extracts all the crucial informations in *P and sets the
	* public attributes with it.
	*/
	PriceComputer(Param *P);

	/**
	* Compute the price of the option given in the Param *P
	* @return the price of the american option
	*/
	double compute_price();

	~PriceComputer();

private:
	/**
	* Computes the price of the best of option
	* @param[in] *labdas vector containing the weights of the underlying assets
	* to take into account when computing the payoff of the option.
	* @return the price
	*/
	double compute_bestof(PnlVect *lambda);

	/**
	* Computes the price of the best of option
	* @return the price
	*/
	double compute_put_geom();

	/**
	* Computes the price of the basket option
	* @param[in] *labdas vector containing the weights of the underlying assets
	* to take into account when computing the payoff of the option.
	* @return the price
	*/
	double compute_basket(PnlVect *lamdbas);
	
};