#pragma once

#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include <time.h>
#include "pnl/pnl_basis.h"

class MonteCarlo
{
public:
    BlackScholesModel *mod_; /*! pointeur vers le modèle */
    Option *opt_; /*! pointeur sur l'option */


    /**
    * Instanciate a MonteCarlo object that will allow to compute the price. 
    * 
    * @param[in] *mod the pointer towards the model to use to simulate the path
    * of the underlying assets (here it will be Black-Scholes model)
    * @param[in] *opt the pointer towards the option to price
    */
    MonteCarlo(BlackScholesModel* mod, Option* opt);

    ~MonteCarlo();

    /**
     * Computes the price of the american option at time 0
     * 
     * @param[in] nbTimeStep the number of time steps pour the subdivision
     * @param[in] nbSimul the number of MonteCarlo simulations
     * @param[in] degree the degree for the polynomial regression in the Longstoff algo
     * @return valeur de l'estimateur Monte Carlo
     */
    double price(int nbTimeSteps, int nbSimul, int degree);


    /**
    * Simulates the nbSamples trajectories of the underlying asset and store
    * them in the *path PnlMat
    * 
    * @param[out] *path the PnlMat of size (nbTimesSteps * nbSample) \times nbAssets
    * @param[in] nbTimeStep the number of time steps pour the subdivision
    * @param[in] nbSimul the number of MonteCarlo simulations 
    * @param[in] nbAssets the size of the asset
    */
    void simulateAllPaths(PnlMat *path, int nbSample, int nbTimeSteps, int nbAssets);

};


