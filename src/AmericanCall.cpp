#include "AmericanCall.hpp"

AmericanCall::AmericanCall(double T, int dates, int size, double strike) {
	T_ = T;
	dates_ = dates;
	size_ = size;
	strike_ = strike;
}


double AmericanCall::payoff(const PnlVect* spotPrices) {
	double priceSpot = pnl_vect_get(spotPrices, 0);
	if (priceSpot - strike_ > 0) {
		return priceSpot - strike_ ;
	}
	else {
		return 0;
	}
}

/**
 * Calcule la valeur du payoff sur la trajectoire
 *
 * @param[in] path est une matrice de taille (dates_+1) x size_
 * contenant une trajectoire du mod?le telle que cr??e
 * par la fonction asset.
 * @return phi(trajectoire)
 */
double AmericanCall::payoff(const PnlMat* path, double t) {
	int index = t * dates_ / T_;
	double assetPrice = pnl_mat_get(path, index, 0);
	if (assetPrice - strike_ > 0) {
		return assetPrice - strike_;
	}
	else {
		return 0;
	}
}