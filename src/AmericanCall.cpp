#include "AmericanCall.hpp"

AmericanCall::AmericanCall(double T, int dates, int size, double strike) {
	T_ = T;
	dates_ = dates;
	size_ = size;
	strike_ = strike;
}


/**
 * Calcule la valeur du payoff sur la trajectoire
 *
 * @param[in] path est une matrice de taille (dates_+1) x size_
 * contenant une trajectoire du modèle telle que créée
 * par la fonction asset.
 * @return phi(trajectoire)
 */
double AmericanCall::payoff(const PnlMat* path, double t) {
	int index = t * dates_ / T_;
	double assetPrice = pnl_mat_get(path, index, 0);
	return assetPrice - strike_;
}