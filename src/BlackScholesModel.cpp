#include "BlackScholesModel.hpp"
#include <iostream>
#include <assert.h>
using namespace std;

BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect *sigma, PnlVect *spot, PnlVect *divid) {
	this->size_ = size;
	this->r_ = r;
	this->rho_ = rho;
	this->sigma_ = sigma;
	this->spot_ = spot;
	this->divid_ = divid;
	this->cholMat_ = pnl_mat_create_from_scalar(size, size, rho);
	pnl_mat_set_diag(cholMat_, 1, 0);
	pnl_mat_chol(cholMat_);
}

BlackScholesModel::~BlackScholesModel() {
	pnl_mat_free(&cholMat_);
}

void BlackScholesModel::asset(PnlMat* path, double T, int nbTimeSteps, PnlRng *rng) {
	
	// On initialise en temps 0 la valeur des sous-jacents
	pnl_mat_set_row(path, spot_, 0);

	int nb_dates = path->m;

	assert(nb_dates == nbTimeSteps + 1);

	double step = T / nbTimeSteps;

	PnlVect *cholRow = pnl_vect_create(size_);

	PnlVect *G = pnl_vect_create(size_);

	for (int row = 1; row <= nbTimeSteps; row++) {

		pnl_vect_rng_normal(G, size_, rng);
		
		for (int asset = 0; asset < size_; asset++) {

			pnl_mat_get_row(cholRow, cholMat_, asset);
			double assetDivid = pnl_vect_get(divid_, asset);
			double mu = r_ - assetDivid;
			double assetSigma = pnl_vect_get(sigma_, asset);

			double assetPrice = pnl_mat_get(path, row - 1, asset) * exp((mu - 0.5 * assetSigma * assetSigma) * step + sqrt(step) * assetSigma * pnl_vect_scalar_prod(cholRow, G));

			pnl_mat_set(path, row, asset, assetPrice);
		}
	}
	pnl_vect_free(&G);
	pnl_vect_free(&cholRow);
}