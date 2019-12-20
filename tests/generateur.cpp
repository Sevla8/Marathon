/* UQAM / INF3105 / Automne 2019 / TP3
   Générateur de fichiers test.
*/
#include <iostream>
#include <cassert>
#include <fstream>
#include <random>
#include <set>
#include <vector>

using namespace std;

int main(int argc, const char** argv)
{
    int nbRequetes = 1000;
    double distanceCibleMoyenne = 5000;
    double probDirectiveDispoRoute = 0.1;
    double probDirectiveNonDispoRoute = 0.05;
    
    if(argc<2 || argc>6){
        cout << "./generateur carte.txt [nbRequetes [moyenneDistCible [p+ [p-]]]]" << endl;
        return 1;
    }
    
    const char* fichiercarte=argv[1];
    if(argc>2)
        nbRequetes = stoi(argv[2]);
    if(argc>3)
        distanceCibleMoyenne = stod(argv[3]);
    if(argc>4)
        probDirectiveDispoRoute = stod(argv[4]);
    if(argc>5)
        probDirectiveNonDispoRoute = stod(argv[5]);

    vector<long> ids;
    vector<string> nomsroute;
    {
        ifstream ifcarte(fichiercarte);
        if(ifcarte.fail()){
            cout << "Échec ouverture fichier carte '" << fichiercarte << "'." << endl;
            return 2;
        }
    
        // Lire les IDs de la carte
        {
            
            string temp;
            ifcarte >> temp;
            while(ifcarte && temp!="---"){
                assert(temp[0]=='n');
                long id = stol(temp.substr(1));
                //cout << id << endl;
                ids.push_back(id);
                while(ifcarte && temp != ";")
                    ifcarte >> temp;
                ifcarte >> temp;
            }
        }
    
        // Lire les noms de route de la carte
        {
            set<string> setnomsroute;
            setnomsroute.insert("?"); // ne pas piger les routes sans nom (avec ?)
            while(ifcarte){
                string nom, temp;
                ifcarte >> nom;
                if(!nom.empty() && setnomsroute.find(nom)==setnomsroute.end()){
                    setnomsroute.insert(nom);
                    nomsroute.push_back(nom);
                    //cout << nom << endl;
                }
                do{
                    ifcarte >> temp;
                }while(ifcarte && temp != ";");
            }
        }
    }
    
    // Générer des requêtes
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> distribution_ids(0, ids.size()-1);
    std::uniform_int_distribution<int> distribution_routes(0, nomsroute.size()-1);
    std::exponential_distribution<double> distribution_distcible(1.0/distanceCibleMoyenne);
    std::uniform_real_distribution<> distribution_01(0.0, 1.0);
    
    double sum_distancecible=0;
    for(int i=0;i<nbRequetes;i++){
        if(distribution_01(e1) <= probDirectiveDispoRoute){
            cout << "+ " << nomsroute[distribution_routes(e1)] << " ;\n";
        }
        if(distribution_01(e1) <= probDirectiveNonDispoRoute){
            cout << "- " << nomsroute[distribution_routes(e1)] << " ;\n";
        }
    
        long id = ids[distribution_ids(e1)];
        double distancecible = distribution_distcible(e1);
        sum_distancecible += distancecible;
        cout << 'n' << id << '\t' << (int) distancecible << " ;\n";
    }
    return 0;
}
