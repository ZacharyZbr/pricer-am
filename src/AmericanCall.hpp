#include "Option.hpp"


class AmericanCall:Option
{
public:
    double strike_; /// Strike de l'option


    AmericanCall(double T, int dates, int size, double strike);

    double payoff(const PnlVect* spotPrices);

    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (dates_+1) x size_
     * contenant une trajectoire du mod?le telle que cr??e
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    double payoff(const PnlMat* path, double t) override;
};