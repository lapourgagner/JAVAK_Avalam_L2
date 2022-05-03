// TODO: Utiliser les optimisation à la compilation (-O3) et la tester
#include "../include/avalam.h"
#include "../include/moteur.h"

#include <stdio.h>
#include <stdlib.h>

// Utile pour simuler un coup
void copierPlateau(T_Position currentPosition, T_Position currentPositionCopy)
{
	currentPositionCopy.trait	= currentPosition.trait;
	currentPositionCopy.numCoup = currentPosition.numCoup;
	for(int i = 0; i < NBCASES; i++)
		currentPositionCopy.cols[i] = currentPosition.cols[i];
	currentPositionCopy.evolution.bonusJ = currentPosition.evolution.bonusJ;
	currentPositionCopy.evolution.bonusR = currentPosition.evolution.bonusR;
	currentPositionCopy.evolution.malusJ = currentPosition.evolution.malusJ;
	currentPositionCopy.evolution.malusR = currentPosition.evolution.malusR;
	return;
}

// Recher l'index d'un coup avec un case origine et une case destiantion donnée
int rechercheCoup(T_ListeCoups listeCoups, octet origine, octet destination)
{
	int moy	  = 0;
	int debut = 0;
	int size  = 8 * NBCASES;
	while(debut <= size) {
		moy = (debut + size) / 2;
		if(listeCoups.coups[moy].origine == 0 && moy != 0) {
			size = moy - 1;
		}
		else if(listeCoups.coups[moy].origine == origine) {
			if(listeCoups.coups[moy].destination > destination) {
				while(listeCoups.coups[moy].origine == origine && listeCoups.coups[moy].destination > destination)
					moy--;
			}
			else if(listeCoups.coups[moy].destination < destination) {
				while(listeCoups.coups[moy].origine == origine && listeCoups.coups[moy].destination < destination)
					moy++;
			}
			if(listeCoups.coups[moy].origine == origine && listeCoups.coups[moy].destination == destination) {
				return moy;
			}
			return -1;
		}
		else if(listeCoups.coups[moy].origine < origine) {
			debut = moy + 1;
		}
		else {
			size = moy - 1;
		}
	}
	return -1;
}

int placerBonus(T_Position currentPosition, T_ListeCoups listeCoups)
// TODO: Adapater technique de la mante religieuse, pas adaptée avec l'ordre de placement des bonus
{
	int	  coup;
	octet tempo;
	switch(currentPosition.numCoup) {
		case 4: // Malus rouge
			//
			coup = 2;
			return coup;
			break;

		case 3: // Malus jaune
			//
			coup = 1;
			return coup;
			break;

		case 2:; // Bonus rouge
			//
			octet bonusJaune = currentPosition.evolution.bonusJ;
			for(int i = 0; i < 2; i++) {
				if(19 == bonusJaune || 28 == bonusJaune)
					tempo = bonusJaune;
			}
			if(tempo == 28) {
				currentPosition.evolution.bonusR = 22;
				coup							 = 22;
			}
			else if(tempo == 19) {
				currentPosition.evolution.bonusR = 25;
				coup							 = 25;
			}
			else {
				currentPosition.evolution.bonusR = 1;
				coup							 = 1;
			}
			return coup;
			break;

		case 1: // Bonus jaune
			//

			if(currentPosition.trait == JAU) {
				// Technique du cobra ancestral

				// octet tab[] = {11, 17, 18, 19, 25, 26};
				// octet tempo;
				// octet malusRouge = currentPosition.evolution.malusR;
				// for(int i = 0; i < 8; i++) {
				// 	if(tab[i] == malusRouge)
				// 		tempo = malusRouge;
				// }
				// if(currentPosition.evolution.malusR == tempo) {
				// 	currentPosition.evolution.bonusJ = 19;
				// 	coup							 = 19;
				// }
				// else {
				// 	currentPosition.evolution.bonusJ = 28;
				// 	coup							 = 28;
				// }
				currentPosition.evolution.bonusJ = 19;
				coup							 = 19;
			}

			return coup;
			break;

		default:

			return -1;
			break;
	}
}

int zonesafe(T_Position currentPosition)
{
	octet bonusJaune = currentPosition.evolution.bonusJ;
	if(currentPosition.cols[bonusJaune].couleur == ROU)
		return -1;
	else if(bonusJaune != 28 && bonusJaune != 20 && bonusJaune != 19 && bonusJaune != 27)
		return -1;
	return 1;
}

int ouverture(T_Position currentPosition, T_ListeCoups listeCoupsSoi)
{
	if(currentPosition.trait == JAU) {
		// Technique du cobra ancestral

		if(zonesafe(currentPosition) == -1)
			return -1;

		if(currentPosition.evolution.bonusJ == 28) {
			switch(currentPosition.numCoup) { // ??
				case 5:
					return rechercheCoup(listeCoupsSoi, 21, 29);
					break;

				case 7:
					if(estValide(currentPosition, 29, 20) == VRAI)
						return rechercheCoup(listeCoupsSoi, 29, 20);
					break;

				case 9:
					if(estValide(currentPosition, 20, 28) == VRAI)
						return rechercheCoup(listeCoupsSoi, 20, 28);
			}
		}

		else {
			switch(currentPosition.numCoup) {
				case 5:
					return rechercheCoup(listeCoupsSoi, 26, 18);
					break;

				case 7:
					if(estValide(currentPosition, 18, 27) == VRAI)
						return rechercheCoup(listeCoupsSoi, 18, 27);
					break;

				case 9:
					if(estValide(currentPosition, 27, 19) == VRAI)
						return rechercheCoup(listeCoupsSoi, 27, 19);
					break;
			}
		}
	}
	else if(currentPosition.trait == ROU) {
		// Technique de la mante religieuse

		if(zonesafe(currentPosition) == -1)
			return -1;

		if(currentPosition.evolution.bonusR == 22) {
			switch(currentPosition.numCoup) {
				case 6:
					if(estValide(currentPosition, 22, 29) == VRAI)
						return rechercheCoup(listeCoupsSoi, 22, 29);
					break;

				case 8:
					if(currentPosition.cols[21].nb == 1 && currentPosition.cols[20].couleur == ROU &&
					   estValide(currentPosition, 29, 28) == VRAI)
						return rechercheCoup(listeCoupsSoi, 29, 28);
					else if(estValide(currentPosition, 29, 20) == VRAI)
						return rechercheCoup(listeCoupsSoi, 29, 20);
					break;

				case 10:
					if(estValide(currentPosition, 20, 28) == VRAI)
						return rechercheCoup(listeCoupsSoi, 20, 28);
			}
		}
		else {
			switch(currentPosition.numCoup) {
				case 6:
					if(estValide(currentPosition, 25, 18) == VRAI)
						return rechercheCoup(listeCoupsSoi, 25, 18);
					break;

				case 8:
					if(currentPosition.cols[26].nb == 1 && currentPosition.cols[27].couleur == ROU &&
					   estValide(currentPosition, 18, 19) == VRAI)
						return rechercheCoup(listeCoupsSoi, 18, 19);
					else if(estValide(currentPosition, 29, 20) == VRAI)
						return rechercheCoup(listeCoupsSoi, 29, 20);
					break;

				case 10:
					if(currentPosition.cols[19].nb == 3 && currentPosition.cols[19].couleur == ROU &&
					   estValide(currentPosition, 26, 35) == VRAI)
						return rechercheCoup(listeCoupsSoi, 26, 35);
					else if(estValide(currentPosition, 20, 28) == VRAI)
						return rechercheCoup(listeCoupsSoi, 20, 28);
			}
		}
	}
	return -1;
}

float evaluerScorePlateau(T_Position currentPosition)
{
	float evaluation = 0;

	// Liste des paramètres
	int score_soi, score_adv, score5_soi, score5_adv;
	int score_soi_coeff	 = 1;
	int score_adv_coeff	 = 1;
	int score5_soi_coeff = 1;
	int score5_adv_coeff = 1;

	// On évalue le score
	T_Score score = evaluerScore(currentPosition);
	if(JAU == currentPosition.trait) {
		score_soi  = score.nbJ;
		score_adv  = score.nbR;
		score5_soi = score.nbJ5;
		score5_adv = score.nbR5;
	}
	else {
		score_soi  = score.nbR;
		score_adv  = score.nbJ;
		score5_soi = score.nbR5;
		score5_adv = score.nbJ5;
	}

	// TODO: mutiplier toutes lezs valeurs que l'on a obtenu par des coeffecicient à defeinir pour avoir un score final du
	// plateau, à comparer aux auutres mo ments score = ;

	printf1("Evaluation: %f\n", evaluation);
	return evaluation;
}

float evaluerScoreCoup(T_ListeCoups listeCoups, T_Position currentPosition, int origine, int destination)
{
	float evaluation = 0;
		octet o, d;
	int i;
	o = listeCoups.coups[i].origine;
    	d = listeCoups.coups[i].destination;
	octet myColor = currentPosition.trait;
	T_Voisins voisinOrigine;
	T_Voisins voisinDestination;
    	voisinOrigine = getVoisins(o);
	voisinDestination = getVoisins(d);
	//position de depart de ma couleur -> position d'arrivée couleur adverse et somme des deux tours = 5
	if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb+currentPosition.cols[d].nb==5) && (currentPosition.cols[d].couleur != myColor))
	    {
		evaluation = 100;
	    }

	//position de depart de ma couleur -> position d'arrivée de ma couleur et somme des deux tours = 5
	if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb+currentPosition.cols[d].nb == 5) && (currentPosition.cols[d].couleur == myColor))
	    {
		evaluation = 90;
	    }

	////position de depart couleur adverse -> position d'arrivée de couleur adverse et somme des deux tours = 5
	if ((currentPosition.cols[o].couleur != myColor) && (currentPosition.cols[o].nb+currentPosition.cols[d].nb==5) && (currentPosition.cols[d].couleur != myColor))
	    {
		evaluation = -100;
	    }
	
	//Isoler 1 voisin	80
	/*if ((currentPosition.cols[o].couleur == myColor) && (voisin.nb==1))
	    {
		evaluation = 80;
	    }*/

	//contre isoler 1 voisin	75
	if((currentPosition.cols[o].couleur != myColor) && (voisinOrigine.nb==1))
	    {
		evaluation=75;
	    } 
	
	//contre isoler 1 voisin	65
	if((currentPosition.cols[o].couleur != myColor) && (voisinOrigine.nb==2))
	    {
		evaluation=65;
	    } 

	//contre isoler 1 voisin	55
	if((currentPosition.cols[o].couleur != myColor) && (voisinOrigine.nb==3))
	    {
		evaluation=55;
	    } 

	for (int j=0; j<listeCoups.nb; j++) 
	{ 
		octet o2,d2;
		o2 = listeCoups.coups[j].origine; // Cette deuxième origine correspond à la destination du tour actuel
		d2 = listeCoups.coups[j].destination; // Cette deuxième destination correspond à la portée de la destination du tour actuel
		if ((currentPosition.cols[o].couleur == myColor))
		{
			if((currentPosition.cols[d].couleur == myColor))
			{
				if(currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[d2].nb!=5)
				{
					evaluation=40;
				}
				

			}

			if((currentPosition.cols[d].couleur != myColor))
				{
					if(currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[d2].nb!=5)
					{
						evaluation=58;
					}

				}

		}
		if ((currentPosition.cols[o].couleur != myColor))
		{
			if((currentPosition.cols[d].couleur == myColor))
			{
				evaluation=-100;
			}

			if((currentPosition.cols[d].couleur != myColor))
			{
				if(currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[d2].nb!=5)
				{
					evaluation=68;
				}

			}
		}
	}
	/*
	Tour adverse sur soi	-100
	isoler pion adverse	-100
	Tour 5 adverse	-100
	Tour de 3 si tour de 2 à côté de destination	-90
	Tour de 4 si tour de 1 à côté de destination	-90
	Isoler 4 voisins	50
	contre isoler 3 voisins	55
	soi sur adverse	58
	Isoler 3 voisins	60
	contre isoler 2 voisins	65
	averse dur adverse	68
	Isoler 2 voisins	70
	contre isoler 1 voisin	75
	Isoler 1 voisin	80
	Tour de 5 sur son propre pion	90
	Tour 5 sur pion adverse	100
	*/

	T_Voisins voisins = getVoisins(destination);
	for (int i = 0; i < voisins.nb; i++)
	{
		if (voisins.case[i] == 0 || voisins.case[i] == origine) // Si le voiin est une case vide ou que c'est notre place de départ, on ignore
		{
			break;
		}
		
		
	}
	
	return evaluation;
}

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups)
{
	printf("\033[0;31m");
	int result;

	// Pour le debug: Affiche tous les coups possibles, et même plus, ceux qui n'exitent pas !
	for(int i = 0; i < listeCoups.nb; i++) {
		printf("o:%d d: %d n:%d | ", listeCoups.coups[i].origine, listeCoups.coups[i].destination, i);
	}

	// Gestion des bonus/malus:
	if(11 > currentPosition.numCoup) // (bj, br, mj, mr)
	{
		// result = placerBonus(currentPosition, listeCoups);

		// if(result != -1)
		// 	printf("Erreur lors du placememnt des bonus (numCoup: %d)\n", currentPosition.numCoup);

		// ecrireIndexCoup(result);
		int coupOuverture = ouverture(currentPosition, listeCoups);
		if(coupOuverture != -1) {
			printf("On écrit le coup %d (o:%d, d:%d)\n", coupOuverture, listeCoups.coups[coupOuverture].origine,
				   listeCoups.coups[coupOuverture].destination);
			ecrireIndexCoup(coupOuverture);
		}
	}
	else if(currentPosition.numCoup >= 5 && currentPosition.numCoup <= 10) {
		result = ouverture(currentPosition, listeCoups);

		if(result == -1) {
			ecrireIndexCoup(0);
		}
		ecrireIndexCoup(result);
		return;
	}

	printf("Etat des bonus/malus: bj: %d, mj: %d, br: %d, mr:%d. Trait: %d\n", currentPosition.evolution.bonusJ,
		   currentPosition.evolution.malusJ, currentPosition.evolution.bonusR, currentPosition.evolution.malusR,
		   currentPosition.trait);

	printf("\033[0m");
}
