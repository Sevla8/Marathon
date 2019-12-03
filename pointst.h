/*  INF3105 | Structures de données et algorithmes
    UQAM | Département d'informatique
    http://ericbeaudry.uqam.ca/INF3105/

    Fichier pointst.h © Éric Beaudry.

    SVP, n'imprimez pas ce fichier, car il est déjà fourni. Pénalité de 0.5 si imprimé inutilement.
*/
#if !defined(_POINTST__H_)
#define _POINTST__H_

#include <iostream>

// Cette classe représente un point sur la surface de la Terre.
class PointST {

  public:
    PointST(){}
    PointST(double latitude_, double longitude_); // angles en radians
    PointST(const PointST&);

    double distance(const PointST&) const;

  private:
    double latitude;  // angle en radians
    double longitude; // angle en radians

  // Les opérateurs << et >> écrit et lit du texte en degrés et font la conversion en radians.
  friend std::ostream& operator << (std::ostream&, const PointST&);
  friend std::istream& operator >> (std::istream&, PointST&);
};

#endif

