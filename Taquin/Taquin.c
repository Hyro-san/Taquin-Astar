// Inclusion des biblioth�ques utiles
#include "taquin.h"
#include "AStar.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <assert.h>



// Fonction pour copier un plateau de taquin pSrc vers pDest
int copyTaquin(Taquin * _pSrc, Taquin * _pDest)
{ 
	memcpy(_pDest, _pSrc, sizeof(Taquin));

	_pDest->plateau = (Uint8**)calloc(_pDest->largeur, sizeof(Uint8*));
	if(!_pDest->plateau)
		return 1;
	for (int i = 0; i < _pDest->largeur; i++)
	{
		(_pDest->plateau)[i] = (Uint8*)calloc(_pDest->hauteur, sizeof(Uint8));
		if (!(_pDest->plateau)[i])
			return 1;
		memcpy(_pDest->plateau[i], _pSrc->plateau[i], sizeof(Uint8) * _pSrc->hauteur);
	}

	return 0;
}

// fonction qui renvoie 1 si les 2 taquins sont identiques
// 0 sinon
// -1 si il y a une erreur au passage des param�tres
int equalTaquin(Taquin * _pTaquin1, Taquin * _pTaquin2)
{
	if (_pTaquin1->hauteur != _pTaquin2->hauteur || _pTaquin1->largeur != _pTaquin2->largeur)
		return -1;

	for (int x = 0; x < _pTaquin1->largeur; x++)
			if(!memcmp(_pTaquin1->plateau[x], _pTaquin1->plateau[x], sizeof(Uint8*)*_pTaquin1->hauteur))
				return -1;

	return 0;
}

// Fonction qui cr�e un plateau de taquin 
// ATTENTION le plateau doit �tre NULL avant l'appel � cette fonction 
// Si le plateau n'est pas NULL la fonction essayera de lib�rer la zone m�moire occup�e par le plateau et cela peut donc aboutir � un crash si le plateau n'a pas �t� initialis�
int createTaquin(Taquin * _pTaquin, Uint8 _hauteur, Uint8 _largeur)
{
	// Test pour v�rifier que les donn�es pass�es ne sont pas corrompues
	if(!_pTaquin) return 0;
	
	// Tests pour v�rifier les param�tres de taille de taquin
	if(_hauteur<SIZE_MINI) _hauteur = SIZE_MINI;
	if(_hauteur>SIZE_MAXI) _hauteur = SIZE_MAXI;
	if(_largeur<SIZE_MINI) _largeur = SIZE_MINI;
	if(_largeur>SIZE_MAXI) _largeur = SIZE_MAXI;
	
	// On v�rifie que le plateau n'existe pas
	// S'il existe on lib�re la m�moire avant de recr�er le plateau
	freeTaquin(_pTaquin);

	_pTaquin->hauteur = _hauteur;
	_pTaquin->largeur = _largeur;

	// on alloue la zone m�moire pour stocker les adresses des lignes du tableau
	_pTaquin->plateau = (Uint8**) calloc(_hauteur, sizeof(Uint8*));
		
	// si on a pas r�ussi � allouer la zone m�moire on retourne 0
	if(!_pTaquin->plateau) return 0;

	for(int i=0; i < _hauteur; i++)
	{
		// On alloue la zone m�moire pour contenir la ligne i
		_pTaquin->plateau[i] = (Uint8*) calloc(_largeur, sizeof(Uint8));
		// S'il y a eu un souci � l'allocation on lib�re tout ce qui a d�j�� �t� allou� et on retourne 0
		if(!_pTaquin->plateau[i])
		{
			freeTaquin(_pTaquin);
			return 1;
		}
	}
	// On initialise le taquin
	if(initTaquin(_pTaquin)) return 1;

	return 0;
}


// Fonction qui initialise le taquin
int initTaquin(Taquin * _pTaquin)
{
	for (int x = 0; x < _pTaquin->largeur; x++)
		for (int y = 0; y < _pTaquin->hauteur; y++)
			_pTaquin->plateau[x][y] = (Uint8)(x + y * _pTaquin->hauteur);

	return 0;
}

// Fonction qui m�lange le taquin en effectuant entre minRandom et maxRandom coups al�atoires
int mixTaquin(Taquin * _pTaquin, int _minRandom, int _maxRandom)
{
	int alea = 1;//rand() % (_maxRandom - _minRandom + 1) + _minRandom;
	int d = AUCUN;
	int rand_d = AUCUN;
	for (int i = 0; i < alea; i++)
	{
		rand_d = rand() % 4 + 1;
		if ((d == HAUT && rand_d == BAS) || (d == BAS && rand_d == HAUT) || (d == DROITE && rand_d == GAUCHE) || (d == GAUCHE && rand_d == DROITE)) {
			--i;
			continue;
		}
		if (moveTaquin(_pTaquin, rand_d)) --i;;
		//displayTaquin(_pTaquin, 0);
		d = rand_d;
	}

	return 1;
}

// Fonction qui permet de bouger une pi�ce du taquin (en bougeant la case vide)
int moveTaquin(Taquin* _pTaquin, deplacement _d)
{
	if (!_d) return 1;

	int x = 0, y = 0;
	for (x = 0; x < _pTaquin->largeur; ++x) {
		for (y = 0; y < _pTaquin->hauteur; ++y)
			if (!_pTaquin->plateau[x][y])
				break;
		if (!_pTaquin->plateau[x][y])
			break;
	}
		

	switch (_d) {
		case GAUCHE:
			if (x > 0) {
				_pTaquin->plateau[x][y] = _pTaquin->plateau[x - 1][y];
				_pTaquin->plateau[x - 1][y] = 0;
				return 0;
			}
			return 1;
		case DROITE:
			if (x < _pTaquin->largeur - 1) {
				_pTaquin->plateau[x][y] = _pTaquin->plateau[x + 1][y];
				_pTaquin->plateau[x + 1][y] = 0;
				return 0;
			}
			return 1;
		case BAS:
			if (y < _pTaquin->hauteur - 1) {
				_pTaquin->plateau[x][y] = _pTaquin->plateau[x][y + 1];
				_pTaquin->plateau[x][y + 1] = 0;
				return 0;
			}
			return 1;
		case HAUT:
			if (y > 0) {
				_pTaquin->plateau[x][y] = _pTaquin->plateau[x][y - 1];
				_pTaquin->plateau[x][y - 1] = 0;
				return 0;
			}
			return 1;
		}
	
	return 1;
}

// Fonction qui renvoie le d�placement � effectuer pour annuler le d�placement donn� en param�tre
deplacement cancelMove(deplacement _d)
{
	return (_d == GAUCHE) ? DROITE : (_d == DROITE) ? GAUCHE : (_d == HAUT) ? BAS : (_d == BAS) ? HAUT : AUCUN;
}

// Fonction qui renvoie 1 si le taquin es r�solu, 0 sinon
int endTaquin(Taquin * _pTaquin)
{
	for (int x = 0; x < _pTaquin->largeur; x++)
		for (int y = 0; y < _pTaquin->hauteur; y++)
			if (_pTaquin->plateau[x][y] != (Uint8)(x + y * _pTaquin->hauteur))
				return 0;

	return 1;
}

// fonction d'affichage du taquin
int displayTaquin(Taquin * _pTaquin, int _offset)
{
	for (int i = 0; i < _pTaquin->largeur; i++)
		printf("________");
	printf("\n");
	for (int y = 0; y < _pTaquin->hauteur; y++)
	{
		//printf("|");
		for (int x = 0; x < _pTaquin->largeur; x++)
			printf("%hhd\t|", _pTaquin->plateau[x][y]);
		printf("\n");

		for (int i = 0; i < _pTaquin->largeur; i++)
			printf("________");
		printf("\n");
	}
	printf("\n\n\n");
	return 0;
}

// Fonction pour lib�rer les zones m�moires occup�es par un taquin
int freeTaquin(Taquin * _pTaquin)
{
	if (_pTaquin->plateau)
	{
		// On lib�re le plateau ligne par ligne
		for (int i = 0; i < _pTaquin->hauteur; i++) 
			if (_pTaquin->plateau[i]) 
				free(_pTaquin->plateau[i]);
		// On lib�re le tableau qui stockait les lignes
		free(_pTaquin->plateau);
		_pTaquin->plateau = NULL;
	}	
		
	return 0;
}
// Boucle de jeu 
int gameLoop(int hauteur,int largeur,int minRandom, int maxRandom)
{

	// BOUCLE DE JEU ! A DEVELOPPER
	while(!_kbhit())
	{
		printf(".");
	}
	return 1;
}

