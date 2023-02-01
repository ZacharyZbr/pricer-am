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

    //std::cout << "caracteristiques du polynome " << std::endl;
    //pnl_basis_print(hermitePolynom);
    PnlVect *tau_l = pnl_vect_create_from_scalar(nbSample, opt_->T_);

    PnlVect *alpha = pnl_vect_create_from_zero(nbAssets);

    //std::cout << "On commence a simuler path" << std::endl;

    PnlMat* path = pnl_mat_create((nbTimeSteps + 1 ) * nbSample, nbAssets);

    simulateAllPaths(path, nbSample, nbTimeSteps, nbAssets);

    //std::cout << "on a simule path" << std::endl;

    //pnl_mat_print(path);

    PnlMat *X = pnl_mat_create(nbSample, nbAssets);

    PnlMat *subBlock = pnl_mat_create(nbTimeSteps + 1, nbAssets);

    PnlVect* res = pnl_vect_create(nbSample);
    
    PnlVect* Y = pnl_vect_create(nbSample);
    

    double price = 0;

    for (int k = nbTimeSteps; k > 0; k--) {

        // std::cout << "Boucle : " << k << std::endl;

        for (int i = 0; i < nbSample; i++) {
            pnl_mat_get_row(res, path, k + i * (nbTimeSteps + 1));
            
            pnl_mat_set_row(X, res, i);
            
        }

        //pnl_mat_print(X);

        //std::cout << "ca marche" << std::endl;

        for (int row = 0; row < nbSample; row++) {
            pnl_mat_extract_subblock(subBlock, path, row * (nbTimeSteps + 1), nbTimeSteps + 1, 0, nbAssets);
            double payoff = opt_->payoff(subBlock, pnl_vect_get(tau_l, row));
            //pnl_mat_print(subBlock);

            //std::cout << "payoff du block : " << row << " evalue au temps " << pnl_vect_get(tau_l, row) << " qui correspond a l'index " << pnl_vect_get(tau_l, row) * nbTimeSteps / opt_->T_ << " vaut : " << payoff << std::endl;
            pnl_vect_set(Y, row, payoff);
        }
        
        //std::cout << "On solve" << std::endl;

        // std::cout << "La valeur de alpha apr�s le solveur de taille  " << alpha->size << std::endl;

        pnl_basis_fit_ls(hermitePolynom, alpha, X, Y);
        //std::cout << "La valeur de alpha apr�s le solveur de taille  " << alpha->size << std::endl;
        //pnl_vect_print(alpha);
       

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
        price += opt_->payoff(subBlock, pnl_vect_get(tau_l, l));
    }
    price = price / nbSample;
    pnl_mat_free(&subBlock);
    pnl_mat_free(&path);


    double price_zero = opt_->payoff(mod_->spot_);
    mod_->~BlackScholesModel();

   /* std::cout << price << std::endl;
    std::cout << price_zero << std::endl;*/

    if (price > price_zero) {
        return price;
    }
    else {
        return price_zero;
    }


    
}