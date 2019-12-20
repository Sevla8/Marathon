/*
  Valideur pour INF3105 / 2019A / TP2
  g++ valideur.cpp -o valideur -O
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <string> 
#include <list>
#include <map>

using namespace std;

struct Reponse{
    long noeudIdeal=-2;
    int distance;
    list<long> chemin;
};

bool lireReponse(istream& is, Reponse& rep){
    char c=0;
    if(is.eof()){
        return false;
    }
    switch(is.peek()){
        case 'n':
            {
                is >> c;
                is >> rep.noeudIdeal >> rep.distance >> ws;
                string ligne;
                getline(is, ligne);
                stringstream ss(ligne);
                while(ss){
                    long id=-1;
                    char cn=0;
                    ss >> cn >> id >> ws;
                    if(cn=='n' && id>=0)
                        rep.chemin.push_back(id);
                }
            }
            break;
        case 'I':
            {
                string ligne;
                getline(is, ligne);
                if(ligne=="IMPOSSIBLE!")
                    rep.noeudIdeal = -1; // -1 pour IMPOSSIBLE
                else{
                    //cout << "ERREUR" << endl;
                    rep.noeudIdeal = -2; // pas correct
                }
            }
            break;
        default:
            return false;
    }
    return true;
}


int main(int argc, const char** argv)
{
    string nomfichier_test, nomfichier_solution, nomfichier_resultat;
    bool silence=false;
    bool corrige=false;
    for(int i=1,j=0;i<argc;i++){
        if(strcmp(argv[i],"-q")==0 || strcmp(argv[i],"-quiet")==0 || strcmp(argv[i],"-s")==0 || strcmp(argv[i],"-silence")==0)
            silence=true;
        else if(strcmp(argv[i],"-c")==0 || strcmp(argv[i],"-corrige")==0)
            corrige=true;
        else
        switch(j++){
            case 0:
                nomfichier_test = argv[i];
                break;
            case 1:
                nomfichier_solution = argv[i];
                break;
            case 2:
                nomfichier_resultat = argv[i];
                break;
        }
    }
    if(nomfichier_resultat.empty()){
        cerr << "Valideur pour INF3105/2019A/TP2" << endl;
        cerr << "Syntaxe : valideur text.txt solution+.txt votreresultat+.txt" << endl;
        return 1;
    }
    
    ifstream ifs_test(nomfichier_test.c_str());
    ifstream ifs_solution(nomfichier_solution.c_str());
    ifstream ifs_resultat(nomfichier_resultat.c_str());
    
    if(ifs_test.fail()){
        cerr << "Erreur ouverture test " << nomfichier_test << endl;
        return 1;
    }
    if(ifs_solution.fail()){
        cerr << "Erreur ouverture sol " << nomfichier_solution << endl;
        return 2;
    }
    if(ifs_resultat.fail()){
        cerr << "Erreur ouverture res " << nomfichier_resultat << endl;
        return 3;
    }
    
    int nbtests = 0;
    int nbNoeudOK = 0;
    int nbDistanceOK = 0;
    int nbCheminOK = 0;
    
    if(!silence)
        cout << "#Ligne\tRequête\t:\tValidation" << endl;
    int numeroligne=0;
    while(ifs_test){
        string ligne;
        getline(ifs_test, ligne);
        if(ligne.empty()) continue;
        numeroligne++;
        if(!silence)
            cout << numeroligne << ".\t";
        switch(ligne[0]){
            case '+':
            case '-':
                if(!silence)
                    cout << "\n";
                //rien à faire
                break;
            case 'n':
                stringstream ss(ligne);
                char c=0;
                long origine=-1;
                double distancecible=0;
                ss >> c >> origine >> distancecible;
                if(!silence)
                    cout << "n" << origine << " " << distancecible << "\t:";
                nbtests++;
                Reponse rsolution, rresultat;
                if(!lireReponse(ifs_solution, rsolution)){
                    cout << "\tERREUR: fichier solution tronqué!" << endl;
                    continue;
                }
                if(!lireReponse(ifs_resultat, rresultat)){
                    if(!silence)
                        cout << "\tFichier résultat tronqué!" << endl;
                    continue;
                }
                if(rsolution.noeudIdeal>=0 && rsolution.chemin.front()!=origine){
                    cout << "\tErreur: le chemin ne part pas de l'origine!" << endl;
                    continue;
                }
                
                bool noeudok = rsolution.noeudIdeal == rresultat.noeudIdeal;
                bool distanceok = rsolution.distance == rresultat.distance;
                bool cheminok = rsolution.chemin == rresultat.chemin;
                
                //cout << "\t" << (noeudok ? "OK" : "Echec_Noeud");
                //cout << "\t" << (distanceok ? "OK" : "Echec_Distance");
                //cout << "\t" << (cheminok ? "OK" : "Echec_Chemin");
                
                if(noeudok) nbNoeudOK++;
                if(!noeudok && !silence) cout << "\tEchec_Noeud";
                if(!noeudok && !silence && rsolution.noeudIdeal==-1)  cout << "\tImpossible";
                if(!noeudok && !silence && rresultat.noeudIdeal==-1)  cout << "\tNon_Impossible";
                if(distanceok) nbDistanceOK++;
                if(!distanceok && !silence) cout << "\tEchec_Distance";
                if(cheminok) nbCheminOK++;
                if(!cheminok && !silence) cout << "\tEchec_Chemin";
                
                if(noeudok && distanceok && cheminok && !silence)
                    cout << "\tOK";
                if(!silence)
                    cout << endl;
                break;
        }
    }
    if(!silence)
        cout << "-----------------------------\n"
             << "DI_OK\tDIST_OK\tChem_OK\tNbReq" << endl;
         
    cout << nbNoeudOK << "\t" << nbDistanceOK << "\t" << nbCheminOK;
    if(!corrige)
        cout << "\t/" << nbtests;

    cout << endl;

    return 0;
}

