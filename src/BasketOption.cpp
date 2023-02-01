#include "BasketOption.hpp"

BasketOption::BasketOption(double T, int dates, int size, double strike, PnlVect* lambda) {
	this->T_ = T;
	this->dates_ = dates;
	this->size_ = size;
	this->strike_ = strike;
	this->lambdas_ = lambda;

}

double BasketOption::payoff(const PnlVect* spotPrices) {
	double somme = pnl_vect_scalar_prod(spotPrices, lambdas_);
	if (somme - strike_ > 0) {
		return somme - strike_;
	}
	else {
		return 0;
	}
}

double BasketOption::payoff(const PnlMat* path, double t) {
	int index = t * dates_ / T_;
	double somme = 0;
	for (int i = 0; i < size_; i++) {
		somme += pnl_vect_get(lambdas_, i) * pnl_mat_get(path, index, i);
	}
	if (somme - strike_ > 0) {
		return somme - strike_;
	}
	else {
		return 0;
	}
}