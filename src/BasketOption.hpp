#include "Option.hpp"


class BasketOption : public Option
{
public:
    double strike_; /// Strike de l'option
    PnlVect* lambdas_; /// Les lambdas des sous jacents

    BasketOption(double T, int dates, int size, double strike, PnlVect *lambda );

   
    double payoff(const PnlVect* spotPrices) override;

    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (dates_+1) x size_
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    double payoff(const PnlMat* path, double t) override;
};