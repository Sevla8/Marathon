#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "pointst.h"
#include "carte.h"

using namespace std;

int tp3(Carte& carte, istream& isrequetes) {
	while (isrequetes) {
		// forcer la lecture des prochains espace blancs
		isrequetes >> ws;
		// si fin de fichier, sortir
		if (isrequetes.eof()) break;

		switch (isrequetes.peek()) { // peek retourne le prochain caractère sans le consommer
			case '-':
			case '+':
			{
				// Directive d'exclusion de routes
				char plusoumoins;
				isrequetes >> plusoumoins;
				bool disponible = plusoumoins == '+';
				list<string> nomsroute;
				string nomroute;
				isrequetes >> nomroute;
				while (!nomroute.empty() && nomroute != ";") {
					nomsroute.push_back(nomroute);
					isrequetes >> nomroute >> ws;
				}
				carte.changerDisponibilites(nomsroute, disponible);
				break;
			}
			default:
			{
				// Requête
				long noeudorigine = -1;
				double distancecible = -1;
				char lettren, pointvirgule;
				isrequetes >> lettren >> noeudorigine >> distancecible >> pointvirgule;
				if (lettren != 'n') {
					cout << "Erreur de lecture d'une requête: '" << lettren << "' != 'n' !" << endl;
					return 11;
				}
				if (pointvirgule != ';') {
					cout << "Erreur de lecture d'une requête: '" << pointvirgule << "' != ';' !" << endl;
					return 12;
				}
				list<long> trajet;
				double distance;
				long noeudideal = carte.trouverDestIdeale(noeudorigine, distancecible, distance, trajet);
				if (noeudideal >= 0) {
					cout << 'n' << noeudideal << '\n';
					cout << round(distance) << '\n';
					for (long n : trajet) cout << 'n' << n << ' ';
					cout << endl;
				}
				else cout << "IMPOSSIBLE!" << endl;
			}
		}
	}

	// carte.test();

	return 0;
}

int main(int argc, const char** argv) {
	if (argc<2 || argc>3) {
		cout << "Syntaxe d'appel: ./tp3 carte.txt [requetes.txt]" << endl;
		return 1;
	}

	const char* nomfichiercarte = argv[1];
	Carte carte;
	{
		ifstream ifcarte(nomfichiercarte);
		if (ifcarte.fail()) {
			cout << "Erreur ouverture fichier carte '" << nomfichiercarte << "' !" << endl;
			return 2;
		}
		ifcarte >> carte;
	}

	// si aucun fichier requêtes, lire depuis l'entrée standard
	if (argc == 2) return tp3(carte, cin);

	const char* nomfichierrequetes = argv[2];
	ifstream isrequetes(nomfichierrequetes);
	if (isrequetes.fail()) {
		cout << "Erreur ouverture fichier requêtes '" << nomfichierrequetes << "' !" << endl;
		return 3;
	}
	return tp3(carte, isrequetes);
}
