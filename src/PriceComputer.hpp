#include <iostream>
#include "jlparser/parser.hpp"
#include "MonteCarlo.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_vector.h"


using namespace std;

class PriceComputer {
public:
	Param *P;
	double T, r, strike, rho;
	PnlVect* spot, * sigma, *dividends;
	string type;
	int size;
	int n_samples;
	int dates;
	int degree;

	PriceComputer(Param *P);

	double compute_price();

	~PriceComputer();

private:
	double compute_bestof(PnlVect *lambda);

	double compute_put_geom();

	double compute_basket(PnlVect *lamdbas);
	
};