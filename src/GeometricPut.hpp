#include "Option.hpp"


class GeometricPut : public Option
{
public:
    double strike_; /// Strike de l'option

    /**
    * A constructor for an American geometric put
    * 
    * @param[in] T the maturity of the option
    * @param[in] dates
    * @param[in] size the size/dimension of the option
    * @param[in] strike the strike
    */
    GeometricPut(double T, int dates, int size, double strike);

    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (dates_+1) x size_
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @param t the time at which the payoff is computed
     * @return phi(trajectoire)
     */
    double payoff(const PnlMat* path, double t) override;

    /**
    * Computes the payoff at time t = 0
    * @param[in] *spot vector that contains the spot prices
    */
    double payoff(const PnlVect *spot) override;
};