#include "Performance.hpp"

Performance::Performance(double T, int dates, int size, double strike, PnlVect* lambda) {
	this->T_ = T;
	this->dates_ = dates;
	this->size_ = size;
	this->strike_ = strike;
	this->lambdas_ = lambda;
}

double Performance::payoff(const PnlVect* spot) {
	PnlVect* buffer = pnl_vect_create(size_);
	pnl_vect_clone(buffer, spot);
	pnl_vect_mult_vect_term(buffer, lambdas_);
	double max = pnl_vect_max(buffer);
	double payoff = max - strike_;
	if (payoff < 0) {
		payoff = 0;
	}
	pnl_vect_free(&buffer);
	return payoff;
}

double Performance::payoff(const PnlMat* path, double t) {
	int index = t * dates_ / T_;
	PnlVect* buffer = pnl_vect_create(size_);
	pnl_mat_get_row(buffer, path, index);
	pnl_vect_mult_vect_term(buffer, lambdas_);
	double max = pnl_vect_max(buffer);
	double payoff = max - strike_;
	if (payoff < 0) {
		payoff = 0;
	}
	pnl_vect_free(&buffer);
	return payoff;
}