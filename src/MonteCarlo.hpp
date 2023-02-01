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

    MonteCarlo(BlackScholesModel* mod, Option* opt);

    ~MonteCarlo();

    /**
     * Calcule le prix de l'option à la date 0
     *
     * @return valeur de l'estimateur Monte Carlo
     */
    double price(int nbTimeSteps, int nbSimul, int degree);

    void simulateAllPaths(PnlMat *path, int nbSample, int nbTimeSteps, int nbAssets);

};


