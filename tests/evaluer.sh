#!/bin/bash
##########################################################################
# UQAM - Département d'informatique
# INF3105 - Structures de données et algorithmes
# Automne 2019
# TP3
# http://ericbeaudry.uqam.ca/INF3105/tp3/
# beaudry.eric@uqam.ca
#
# Script d'évaluation
#
# Instructions:
# 1. Déposer ce script avec les fichiers problèmes dans un répertoire
#    distinct (ex: tests).
# 2. Se placer dans le répertoire contenant votre programme ou contenant
#    la liste des soumissions Oto (*.tp_oto).
# 3. Lancer ce script (ex: ../tests/evaluer.sh).
##########################################################################

echo "UQAM | Département d'informatique"
echo "INF3105 | Structures de données et algorithmes"
echo "Évaluation du TP3 ..."
echo


# Obtenir le chemin du répertoire contenant le présent script et les fichiers tests
pushd `dirname $0`
repertoire_tests=`pwd`
popd
# Cartes à tester
cartes="uqam stdamas sthilaire montreal quebec canada"
if [ ! -z "$1" ]; then cartes="$1"; fi;


if [ `pwd` -ef $repertoire_tests ];
then
    echo "Ce script doit être dans un répertoire différent de celui contenant votre tp3."
    echo "Ce script a été arrêté afin de ne pas écraser les fichiers resultat carte-req+?.txt!"
    exit -2;
fi

########### Détection du valideur de résultats #######
# Exécutable du programme de validation
valideur="${repertoire_tests}/valideur"
if [ -x "${valideur}-`uname`-`uname -p`" ]
then
    valideur="${valideur}-`uname`-`uname -p`"
else
    if [ -x "${valideur}-`uname`" ]
    then
        valideur="${valideur}-`uname`"
    fi
fi
######################################################



# Détection si l'utilitaire time sous Linux est disponible (peut afficher 0.00)
echo "Détection de /usr/bin/time..."
/usr/bin/time -f %U echo 2>&1 > /dev/null
souslinux=$?

# Fixer les limites : 300s CPU, 4Go mémoire et 64Mo fichier (suffisant pour quebec, mais insuffisant pour canada)
ulimit -t 300 -v 4194304 -f 65536
echo "ulimit (limites courantes des ressources) :"
ulimit -t -v -f
echo "-----"

# Détection du CPU
if [ -e /proc/cpuinfo ] ; then
    cpuinfo=`grep "model name" /proc/cpuinfo | sort -u | cut -d ":" -f 2`
else
    cpuinfo="?"
fi

function Nettoyer
{
    echo "Nettoyage"
    #make clean

    # Au cas où un Makefile soumis ne ferait pas un «make clean» correctement.
    rm -f *.o* *.gch tp[1-3] *-req*+.txt
    # Au besoin, nettoyer les précédents fichiers logs
    rm -f log*.txt
}

##### Fonction d'évaluation d'un TP ######
function EvaluerTP
{
    date2=`date +%Y%m%d_%H%M%S`
    logfile="./logs/${date2}.log"
    echo "Les résultats seront déposés dans $logfile..."

    echo "Machine : " `hostname`
    echo "#Machine : " `hostname`  > $logfile
    echo "CPU :$cpuinfo"
    echo "#CPU :$cpuinfo"  >> $logfile
    echo "Date début : ${date2}"
    echo "#Date début : ${date2}"  >> $logfile
    echo "Limite de `ulimit -t` secondes par test"
    echo "#Limite de `ulimit -t` secondes par test"  >> $logfile
    echo
    echo "#" >> $logfile

    # Pour statistiques : nombre de lignes de code...
    echo "Taille des fichiers source :" >> ${logfile}
    wc *.{c*,h*}  >> ${logfile}

    signature=`grep -o "_CARTE__H_" carte.h | tail -n 1`

    taille=`wc *.{c*,h*} | grep total`
    sommaire="${signature}\t${taille}"

    ## Forcer la recompilation.
    #rm tp3 *.o
    #make clean
    #make tp3

    # Vérification de l'existance du programme.
    if [ ! -e ${programme} ]
	    then
	    echo "  Erreur : le fichier ${programme} est inexistant!"
	    return
    fi
    if [ ! -x ${programme} ]
	    then
	    echo "  Erreur : le fichier ${programme} n'est pas exécutable!"
	    return
    fi

    echo -e "#FichierTest\tNoeuds\tRequêtes\tCPU\tMém(k)\tNbDest\tNbDist\tNbChem" >> $logfile
    echo -e "FichierTest\tNoeuds\tRequêtes\tCPU\tMém(k)\tValidation"

    # Itérer sur tous les réseaux
    for carte in $cartes; do
        fcarte="${repertoire_tests}/${carte}-carte.txt"
        nbnoeuds=`grep -E "\([0-9.]+,[ ]*-[0-9.]+\)?.;" $fcarte | wc -l`

        # Lister les fichiers requêtes
        tests="`cd $repertoire_tests && ls ${carte}-req[0-9].txt`"

        # Itérer sur tous les fichiers tests
        for test in $tests;
        do
            nbreqs=`grep "^n[0-9]\+" $repertoire_tests/$test | wc -l`

            if [ $souslinux -eq 0 ]; then
                t=`(/usr/bin/time -f "%U\t%Mk" ./tp3 $fcarte $repertoire_tests/$test > ${test%.txt}+.txt) 2>&1 | tail -n 1`
            else
                t=`(time -p ./tp1 $fcarte $repertoire_tests/$test > ${test%.txt}+.txt) 2>&1 | egrep user | cut -f 2 -d " "`
            fi


            if [ -e "${repertoire_tests}/${test%.txt}+.txt" ]; then
                if [ -x ${valideur} ]; then
                    verif=`$valideur -s $repertoire_tests/$test ${repertoire_tests}/${test%.txt}+.txt ${test%.txt}+.txt | tail -n 1`
                else
                    diff -qtibw ${test%.txt}+.txt ${repertoire_tests}/${test%.txt}+.txt 2>&1 > /dev/null
                    if [ $? -eq 0 ]; then
                        verif="OK"
                    else
                        verif="Échec"
                    fi
                fi
            else
                verif="?"
            fi

            echo -e "${test}\t${nbnoeuds}\t${nbreqs}\t${t}\t${verif}"
            echo -e "${test}\t${nbnoeuds}\t${nbreqs}\t${t}\t${verif}" >> $logfile
            sommaire="${sommaire}\t\t${t}\t${verif}"

        done
    done
}




# Lister les soumissions Oto (répertoires terminant par .tp_oto)
tps=`ls -1 | grep .tp_oto`

# Si aucun répertoire .tp_oto n'existe, essayer le répertoire courant (auto-évaluation)
if [ ! -n "$tps" ]; then
    tps="."
fi

# Génération de l'entête du rapport
date=`date +%Y%m%d_%H%M%S`
echo "#Rapport de correction INF3105 / $programme" > "rapport-${date}.txt"
echo -e "#Date:\t${date}" >> "rapport-${date}.txt"
echo -e "#Machine :\t" `hostname` >> "rapport-${date}.txt"
echo -e "#CPU :\t$cpuinfo" >> "rapport-${date}.txt"
echo >> "rapport-${date}.txt"

# Génération des titres des colonnes
echo -e -n "#\t\t\t\t" >> "rapport-${date}.txt"
for carte in ${cartes};
do
    tests="`cd $repertoire_tests && ls ${carte}-req[0-9].txt`"
    for test in $tests;
    do
        echo -e -n "$test\t\t\t\t\t\t" >> "rapport-${date}.txt"
    done
done
echo >> "rapport-${date}.txt"

echo -e -n "#Soumission\tCheck1\tTaille sources" >> "rapport-${date}.txt"
for carte in ${cartes};
do
    tests="`cd $repertoire_tests && ls ${carte}-req[0-9].txt`"
    for test in $tests;
    do
        echo -e -n "\t\tCPU(s)\tMémoire(k)\tNbDest\tNbDist\tNbChem" >> "rapport-${date}.txt"
    done
done
echo >> "rapport-${date}.txt"

# Itération sur chaque TP
for tp in $tps; do
    sommaire=""
    echo "## CORRECTION : $tp"
    pushd $tp
    	EvaluerTP
#       Nettoyer
    popd
    #echo -e ">> ${sommaire}"
    echo -e "${tp}\t${sommaire}" >> "rapport-${date}.txt"
done

