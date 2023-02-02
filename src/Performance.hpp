#include "Option.hpp"


class Performance : public Option
{
public:
    double strike_; /// Strike de l'option
    PnlVect* lambdas_; /// Les lambdas des sous jacents

    Performance(double T, int dates, int size, double strike, PnlVect* lambda);

    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (dates_+1) x size_
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    double payoff(const PnlMat* path, double t) override;

    double payoff(const PnlVect* spot) override;
};