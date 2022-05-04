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
	printf("RC: o:%d d:%d\n", origine, destination);
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
		case 3:; // Malus rouge
			coup = rechercheCoup(listeCoups, 2, 2);
			return coup;
			break;

		case 2:; // Malus jaune
			coup = rechercheCoup(listeCoups, 1, 1);
			return coup;
			break;

		case 1:; // Bonus rouge
			octet bonusJaune = currentPosition.evolution.bonusJ;
			for(int i = 0; i < 2; i++) {
				if(19 == bonusJaune || 28 == bonusJaune)
					tempo = bonusJaune;
			}
			if(tempo == 28) {
				coup = 22;
			}
			else if(tempo == 19) {
				coup = rechercheCoup(listeCoups, 25, 25);
			}
			else {
				coup = rechercheCoup(listeCoups, 1, 1);
			}
			return coup;
			break;

		case 0:; // Bonus jaune
			if(currentPosition.trait == JAU) {
				// Technique du cobra ancestral
				coup = rechercheCoup(listeCoups, 19, 19);
			}
			return coup;
			break;

		default:;
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
	if(zonesafe(currentPosition) == -1) {
		printf("Erreur zonesafe == -1 (?)\n");
		return -1;
	}

	if(currentPosition.trait == JAU) {
		// Technique du cobra ancestral

		if(currentPosition.evolution.bonusJ == 28) {
			switch(currentPosition.numCoup) {
				case 4:;
					return rechercheCoup(listeCoupsSoi, 21, 29);
					break;

				case 6:;
					return rechercheCoup(listeCoupsSoi, 29, 20);
					break;

				case 8:;
					return rechercheCoup(listeCoupsSoi, 20, 28);
			}
		}

		else {
			switch(currentPosition.numCoup) {
				case 4:
					return rechercheCoup(listeCoupsSoi, 26, 18);
					break;

				case 6:;
					return rechercheCoup(listeCoupsSoi, 18, 27);
					break;

				case 8:;
					return rechercheCoup(
						listeCoupsSoi, 27,
						19); // TODO: C'est un coup inutile, car c'est la seule possibilité de l'ilot, à faire en fin de partie
					break;
			}
		}
	}
	else if(currentPosition.trait == ROU) {
		// Technique de la mante religieuse

		if(currentPosition.evolution.bonusR == 22) {
			switch(currentPosition.numCoup) {
				case 5:;
						return rechercheCoup(listeCoupsSoi, 22, 29);
					break;

				case 7:;
					if(currentPosition.cols[21].nb == 1 && currentPosition.cols[20].couleur == ROU &&
					   estValide(currentPosition, 29, 28) == VRAI)
						return rechercheCoup(listeCoupsSoi, 29, 28);
						return rechercheCoup(listeCoupsSoi, 29, 20); // else implicite
					break;

				case 9:;
						return rechercheCoup(listeCoupsSoi, 20, 28);
			}
		}
		else {
			switch(currentPosition.numCoup) {
				case 5:;
						return rechercheCoup(listeCoupsSoi, 25, 18);
					break;

				case 7:;
					if(currentPosition.cols[26].nb == 1 && currentPosition.cols[27].couleur == ROU &&
					   estValide(currentPosition, 18, 19) == VRAI)
						return rechercheCoup(listeCoupsSoi, 18, 19);
						return rechercheCoup(listeCoupsSoi, 29, 20); // else implicite
					break;

				case 9:;
					if(currentPosition.cols[19].nb == 3 && currentPosition.cols[19].couleur == ROU &&
					   estValide(currentPosition, 26, 35) == VRAI)
						return rechercheCoup(listeCoupsSoi, 26, 35);
						return rechercheCoup(listeCoupsSoi, 20, 28); // else implicite
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

	evaluation =
		score_soi * score_soi_coeff + score_adv * score_adv_coeff + score5_soi * score5_soi_coeff + score5_adv * score5_adv_coeff;

	printf1("Evaluation: %f\n", evaluation);
	return evaluation;
}

float evaluerScoreCoup(T_ListeCoups listeCoups, T_Position currentPosition, int origine, int destination)
{
	float	  evaluation = 0;
	octet	  traitPerso = currentPosition.trait;
	T_Voisins voisinOrigine;
	T_Voisins voisinDestination;
	voisinOrigine	  = getVoisins(origine);
	voisinDestination = getVoisins(destination);

	// Tour adverse sur soi	-100
	if(currentPosition.cols[origine].couleur != traitPerso && currentPosition.cols[destination].couleur == traitPerso) {
		evaluation = evaluation - 100;
	}

	// TODO: isoler pion adverse	-100

	// Tour 5 adverse	-100
	if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb) == 5 &&
	   currentPosition.cols[origine].couleur != traitPerso) {
		evaluation = evaluation - 100;
	}

	// Tour de 3 si tour de 2 à côté de destination	-90
	if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb == 3)) {
		for(int i = 0; i < voisinDestination.nb; i++) {
			if(currentPosition.cols[voisinDestination.cases[i]].nb == 2 && voisinDestination.cases[i] != origine) {
				evaluation = evaluation - 90;
				break;
			}
		}
	}

	// Tour de 4 si tour de 1 à côté de destination	-90
	if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb == 4)) {
		for(int i = 0; i < voisinDestination.nb; i++) {
			if(currentPosition.cols[voisinDestination.cases[i]].nb == 1 && voisinDestination.cases[i] != origine) {
				evaluation = evaluation - 90;
				break;
			}
		}
	}

	// TODO: Isoler 4 voisins	50

	// TODO: contre isoler 3 voisins	55

	// soi sur adverse	58
	if(currentPosition.cols[origine].couleur == traitPerso && currentPosition.cols[destination].couleur != traitPerso) {
		evaluation = evaluation + 58;
	}

	// TODO: Isoler 3 voisins	60

	// TODO: contre isoler 2 voisins	65

	// adverse sur adverse	68
	if(currentPosition.cols[origine].couleur != traitPerso && currentPosition.cols[destination].couleur != traitPerso) {
		evaluation = evaluation + 68;
	}

	// TODO: Isoler 2 voisins	70

	// TODO: contre isoler 1 voisin	75

	// if((currentPosition.cols[origine].couleur != traitPerso) && (voisinOrigine.nb==1))
	// {
	//     evaluation=75;
	// }

	// Isoler 1 voisin	80

	// Tour de 4 si aucune tour de 1 à côté de destination
	if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb) == 4 &&
	   currentPosition.cols[origine].couleur == traitPerso) {
		int i = 0;
		for(i = 0; i < voisinDestination.nb; i++) {
			if(currentPosition.cols[voisinDestination.cases[i]].nb == 1 && voisinDestination.cases[i] != origine) {
				break;
			}
		}
		if(currentPosition.cols[voisinDestination.cases[i]].nb != 1) {
			evaluation = evaluation + 80;
		}
	}

	// Tour de 5 sur son propre pion	90
	if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb) == 5 &&
	   (currentPosition.cols[origine].couleur == traitPerso && currentPosition.cols[destination].couleur == traitPerso)) {
		evaluation = evaluation + 90;
	}
	// Tour 5 sur pion adverse	100
	if((currentPosition.cols[origine].nb + currentPosition.cols[destination].nb) == 5 &&
	   (currentPosition.cols[origine].couleur == traitPerso && currentPosition.cols[destination].couleur != traitPerso)) {
		evaluation = evaluation + 100;
	}

	// T_Voisins voisins = getVoisins(destination);
	// for(int i = 0; i < voisins.nb; i++) {
	// 	if(voisins.case[i] == 0 ||
	// 	   voisins.case[i] == origine) // Si le voisin est une case vide ou que c'est notre place de départ, on ignore
	// 	{
	// 		break;
	// 	}
	// }

	return evaluation;
}

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups)
{
	printf("\033[0;31m\n"); // On passe la couleur de la sortie interne du bot en rouge pour plus de lisibilité
	int result;

	// Pour le debug: Affiche tous les coups possibles
	for(int i = 0; i < listeCoups.nb; i++) {
		printf("o:%d d:%d n:%d | ", listeCoups.coups[i].origine, listeCoups.coups[i].destination, i);
	}

	// Gestion des bonus/malus:
	if(4 > currentPosition.numCoup) // (bj, br, mj, mr)
	{
		result = placerBonus(currentPosition, listeCoups);

		if(result == -1)
			printf("IMPOSSIBLE DE PLACER LE BONUS/MALUS: -1 (numCoup: %d)\n", currentPosition.numCoup);
		printf("On place le le bonus, coup %d (o:%d, d:%d)\n", result, listeCoups.coups[result].origine,
			   listeCoups.coups[result].destination);
		ecrireIndexCoup(result);
		printf("\033[0m\n");
		return; // C'est la fin du programme pour cette action, évite un else pour les ouvertures
	}

	// Gestion des ouvertures
	if(currentPosition.numCoup < 10) // Valeur de fin des ouvertures à déterminer
	{
		result = ouverture(currentPosition, listeCoups);
		if(result == -1)
			printf("IMPOSSIBLE DE PLACER L'OUVERTURE: -1 (numCoup: %d)\n", currentPosition.numCoup);
		printf("On place une ouverture, coup %d (o:%d, d:%d)\n", result, listeCoups.coups[result].origine,
			   listeCoups.coups[result].destination);
		ecrireIndexCoup(result);
		if(result != -1) {
			printf("\033[0m\n");
			return; // C'est la fin du programme pour cette action, évite un else pour la partie jeu
		}
		printf("Coup non satisfaisant, situation possiblement problématique; tentative de placememnt de coup classique");
	}

	// Partie de jeu
	result = 0;

	if(result == -1)
		printf("IMPOSSIBLE DE PLACER LE COUP: -1 (numCoup: %d)\n", currentPosition.numCoup);
	printf("On place le le coup %d (o:%d, d:%d)\n", result, listeCoups.coups[result].origine,
		   listeCoups.coups[result].destination);
	ecrireIndexCoup(result);

	printf("Etat des bonus/malus: bj: %d, mj: %d, br: %d, mr:%d. Trait: %d\n", currentPosition.evolution.bonusJ,
		   currentPosition.evolution.malusJ, currentPosition.evolution.bonusR, currentPosition.evolution.malusR,
		   currentPosition.trait);

	printf("\033[0m\n"); // On repasse en police par déaut pour les messages de gestion
	return;
}