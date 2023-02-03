#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
class Option
{
public:
    double T_; /// maturité
    int dates_; /// nombre de dates d'exercice
    int size_; /// dimension du modèle, redondant avec BlackScholesModel::size_
    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (dates_+1) x size_
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlVect* spotPrices) = 0;

    virtual double payoff(const PnlMat *path, double t) = 0;
};


