#include "GeometricPut.hpp"
#include <iostream>
#include < valarray >
#include <cmath>

GeometricPut::GeometricPut(double T, int dates, int size, double strike) {
	T_ = T;
	dates_ = dates;
	size_ = size;
	strike_ = strike;
}

double GeometricPut::payoff(const PnlVect* spot) {
	double product = pnl_vect_prod(spot);
	double inverse = 1 / (double)size_;
	double payoff = strike_ - pow(product, inverse);
	if (payoff < 0) {
		payoff = 0;
	}
	return payoff;
}


double GeometricPut::payoff(const PnlMat* path, double t) {
	int index = t * dates_ / T_;
	PnlVect* row = pnl_vect_create(size_);
	double inverse = 1 / (double)size_;
	pnl_mat_get_row(row, path, index);
	double product = pnl_vect_prod(row);
	double p = strike_ - pow(product, inverse);

	if (p < 0) {
		p = 0;
	}
	pnl_vect_free(&row);
	return p;
}