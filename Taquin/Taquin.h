//////////////////////////////////////////////////////////////////////
//
//	JEU DE TAQUIN
//
//	Structures de donn�es utiles � la repr�sentation du jeu
//	Fonctions utiles pour jouer au taquin en mode console
//
//////////////////////////////////////////////////////////////////////

#ifndef _TAQUIN_

#define _TAQUIN_


// Nombre de cases maximum en largeur ou hauteur
#define SIZE_MAXI	5
// Nombre de cases minimum en largeur ou hauteur
#define SIZE_MINI	3

// Case vide, on d�finit la case vide comme �tant la valeur 0, elle doit �tre en haut � gauche du taquin quand il est termin�
#define VIDE 0


// Type red�fini pour s�lectionner le d�placement � effectuer
typedef enum{AUCUN, GAUCHE, DROITE, HAUT, BAS} deplacement;

// Structure de donn�es pour stocker le taquin et ses possibilit�s de jeu (position de la case vide)
typedef struct sTaquin {
	Uint8 ** plateau;			// Tableau contenant le taquin (
	Uint8 hauteur,largeur;		// Taille du plateau
	Uint8 x,y;					// Position de la case vide � d�placer
} Taquin;


// Fonction pour copier un plateau de taquin pSrc vers pDest
int copyTaquin(Taquin * _pSrc, Taquin * _pDest);

// fonction qui renvoie 1 si les 2 taquins sont identiques
// 0 sinon
// -1 si il y a une erreur au passage des param�tres
int equalTaquin(Taquin * _pTaquin1, Taquin * _pTaquin2);

// Fonction qui cr�e un plateau de taquin 
// ATTENTION le plateau doit �tre NULL avant l'appel � cette fonction 
// Si le plateau n'est pas NULL la fonction essayera de lib�rer la zone m�moire occup�e par le plateau et cela peut donc aboutir � un crash si le plateau n'a pas �t� initialis�
int createTaquin(Taquin * _pTaquin, Uint8 _hauteur, Uint8 _largeur);

// Fonction qui initialise le taquin
int initTaquin(Taquin * _pTaquin);

// Fonction qui m�lange le taquin en effectuant entre minRandom et maxRandom coups al�atoires
int mixTaquin(Taquin * _pTaquin, int _minRandom, int _maxRandom);

// Fonction qui permet de bouger une pi�ce du taquin (en bougeant la case vide)
int moveTaquin(Taquin * _pTaquin, deplacement _d);

// Fonction qui renvoie le d�placement � effectuer pour annuler le d�placement donn� en param�tre
deplacement cancelMove(deplacement _d);

// Fonction qui renvoie 1 si le taquin est r�solu, 0 sinon
int endTaquin(Taquin * _pTaquin);

// fonction d'affichage du taquin
int displayTaquin(Taquin * _pTaquin, int _offset);

// Fonction pour lib�rer les zones m�moires occup�es par un taquin
int freeTaquin(Taquin * _pTaquin);

// Boucle de jeu 
int gameLoop(int _hauteur,int _largeur,int _minRandom, int _maxRandom);



#endif