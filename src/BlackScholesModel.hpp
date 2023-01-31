#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Modèle de Black Scholes
class BlackScholesModel
{
public:
    int size_; /// nombre d'actifs du modèle
    double r_; /// taux d'intérêt
    double rho_; /// paramètre de corrélation
    PnlVect *sigma_; /// vecteur de volatilités
    PnlVect *divid_; /// vecteur des dividendes
    PnlVect *spot_; /// valeurs initiales des sous-jacents
    PnlMat* cholMat_; /// Matrice L de la décomposition de Cholesky. L . L^T = M.


    /**
     * Génère un modèle de Black Scholes
     *
     * @param[in] size nombre d'actifs du modèle
     * @param[in] r taux d'intérêt
     * @param[in] rho paramètre de corrélation
     * @param[in] sigma vecteur de volatilité
     * @param[in] spot valeurs initiales des sous-jacents
     * @param[in] divid vecteur des dividendes
     */
    BlackScholesModel(int size, double r, double rho, PnlVect* sigma, PnlVect* spot, PnlVect* divid);

    ~BlackScholesModel();
    
    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     */
    void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

};
