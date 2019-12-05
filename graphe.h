#ifndef _GRAPHE__H_
#define _GRAPHE__H_

#include <istream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

template <class S, class A, class E>
class Graphe {
	public:
		void ajouter_sommet(E, const S&);
		void ajouter_arete(const A&, const std::list<E>& noeuds);
		// lister sommets
		// lister aretes
		// lister aretes sortant d'un sommet
		// lister aretes entrantes d'un sommet
		// verifier existe arete entre 2 sommets
	private:
		struct Sommet {
			Sommet(const S&, const E&);
			S objet;
			std::map<int, A> aretesSortantes;
			std::map<int, A> aretesEntrantes;
			E etiquette;
			bool estDisponible;
		};
		std::map<S, int> indices;
		std::vector<Sommet> sommets;
};

template <class S, class A, class E>
Graphe<S, A, E>::Sommet::Sommet(const S& s, const E& e) : objet(s), etiquette(e), estDisponible(true) {}

template <class S, class A, class E>
void Graphe<S, A, E>::ajouter_sommet(E e, const S& s) {
	// assert(!this->indices.find(s) == this->indices.end());
	// int indice = this->indices.size();
	// this->indices[s] = indice;
	// this->sommets.push_back(Sommet(s, e));
}

template <class S, class A, class E>
void Graphe<S, A, E>::ajouter_arete(const A& nomroute, const std::list<E>& etiquettes) {
	// for (std::list<E>::const_iterator iter = etiquettes.cbegin(); iter != etiquettes.cend(); ) {
	// 	E tmp1 = *iter;
	// 	++iter;
	// 	if (iter == etiquettes.end()) break;
	// 	E tmp2 = *iter;
	// 	this->noeuds[tmp1].voisins.insert(tmp2);
	// }
}

#endif
