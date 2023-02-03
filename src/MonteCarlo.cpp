#include "MonteCarlo.hpp"
#include <iostream>

using namespace std;

MonteCarlo::MonteCarlo(BlackScholesModel* mod, Option* opt) {
    this->mod_ = mod;
    this->opt_ = opt;
}

MonteCarlo::~MonteCarlo() {
    mod_->~BlackScholesModel();
}


void MonteCarlo::simulateAllPaths(PnlMat* path, int nbSample, int nbTimeSteps, int nbAssets) {
    PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    PnlMat* res = pnl_mat_create(nbTimeSteps + 1, nbAssets);
    for (int i = 0; i < nbSample; i++) {
        mod_->asset(res, opt_->T_, nbTimeSteps, rng);
        pnl_mat_set_subblock(path, res, i * (nbTimeSteps + 1), 0);
    }
    pnl_mat_free(&res);
    pnl_rng_free(&rng);
}


double MonteCarlo::price(int nbTimeSteps, int nbSample, int degree) {


    int nbAssets = mod_->size_;

    // Params simulation
    PnlBasis *hermitePolynom = pnl_basis_create_from_degree(PNL_BASIS_HERMITE, degree, nbAssets);

    PnlVect *tau_l = pnl_vect_create_from_scalar(nbSample, opt_->T_);

    PnlVect *alpha = pnl_vect_create_from_zero(nbAssets);

    PnlMat* path = pnl_mat_create((nbTimeSteps + 1 ) * nbSample, nbAssets);

    simulateAllPaths(path, nbSample, nbTimeSteps, nbAssets);

    PnlMat *X = pnl_mat_create(nbSample, nbAssets);

    PnlMat *subBlock = pnl_mat_create(nbTimeSteps + 1, nbAssets);

    PnlVect* res = pnl_vect_create(nbSample);
    
    PnlVect* Y = pnl_vect_create(nbSample);
    
    double price = 0;

    for (int k = nbTimeSteps; k > 0; k--) {

        for (int i = 0; i < nbSample; i++) {
            pnl_mat_get_row(res, path, k + i * (nbTimeSteps + 1));
            
            pnl_mat_set_row(X, res, i);

            pnl_mat_extract_subblock(subBlock, path, i * (nbTimeSteps + 1), nbTimeSteps + 1, 0, nbAssets);
            
            double payoff = opt_->payoff(subBlock, pnl_vect_get(tau_l, i));

            pnl_vect_set(Y, i, payoff);
            
        }

        pnl_basis_fit_ls(hermitePolynom, alpha, X, Y);
        
        for (int l = 0; l < nbSample; l++) {

            pnl_mat_extract_subblock(subBlock, path, l * (nbTimeSteps + 1), nbTimeSteps + 1, 0, nbAssets);
            double payoff_tk = opt_->payoff(subBlock, k * opt_->T_ / nbTimeSteps);
            pnl_mat_get_row(res, X, l);
            double evaluation = pnl_basis_eval_vect(hermitePolynom, alpha, res);
            if ((payoff_tk >= evaluation) && (evaluation > 0)) {
                pnl_vect_set(tau_l, l, k * opt_->T_ / nbTimeSteps);
            }
        }
    }

    pnl_vect_free(&Y);
    pnl_vect_free(&res);
    pnl_vect_free(&alpha);
    pnl_mat_free(&X);
    pnl_basis_free(&hermitePolynom);
    
    for (int l = 0; l < nbSample; l++) {
        pnl_mat_extract_subblock(subBlock, path, l * (nbTimeSteps + 1), nbTimeSteps + 1, 0, nbAssets);
        double tau = pnl_vect_get(tau_l, l);
        price += opt_->payoff(subBlock, tau) * exp( - mod_->r_ * tau) ;
    }
    price = price / nbSample;
    pnl_mat_free(&subBlock);
    pnl_mat_free(&path);


    double price_zero = opt_->payoff(mod_->spot_);
    mod_->~BlackScholesModel();

    if (price > price_zero) {
        return price;
    }
    else {
        return price_zero;
    }
}