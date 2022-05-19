#include "TaquinSDL.h"
#include "AStar.h"
#include <time.h>
#include "SDL_gfxprimitives.h"




// Fonction permettant de cr�er le taquin en SDL � partir de sa taille et du fichier BMP � utiliser
// Cette fonction cr�e aussi la fen�tre SDL et la stocke dans la structure TaquinSDL
int createTaquinSDL(TaquinSDL * pTaquinSDL,int hauteur, int largeur, char * pathBMPfile)
{
	// Test pour v�rifier que les donn�es pass�es ne sont pas corrompues
	if(!pTaquinSDL) return 0;

	// initialisation de la fen�tre SDL
	pTaquinSDL->pWindow = NULL;
	// initialisation de l'image � NULL
	pTaquinSDL->pFond = NULL;
	// On cr�e le taquin qui sera utilis� pour jouer
	pTaquinSDL->taquin.plateau = NULL;
	createTaquin(&(pTaquinSDL->taquin),hauteur,largeur);

	// On met � jour la taille du taquin
	largeur = pTaquinSDL->taquin.largeur;
	hauteur = pTaquinSDL->taquin.hauteur;

	// On initialise la SDL
	// On initialise non seulement le mode vid�o mais aussi la gestion du temps pour pouvoir utiliser SDL_GetTicks()
	if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ))
	{
		freeTaquinSDL(pTaquinSDL);
		return 0;
	}

	// On charge le BMP qui servira de fond au taquin
	pTaquinSDL->pFond= SDL_LoadBMP(pathBMPfile);
	if(!pTaquinSDL->pFond) 
	{
		freeTaquinSDL(pTaquinSDL);
		return 0;
	}

	// Initialisation de la fen�tre

	// On r�cup�re la taille d'une case
	pTaquinSDL->resX = pTaquinSDL->pFond->w/largeur;
	pTaquinSDL->resY = pTaquinSDL->pFond->h/hauteur;

	// On cr�e la fen�tre en fonction de la r�solution de l'image d'entr�e
	pTaquinSDL->pWindow = SDL_SetVideoMode(pTaquinSDL->resX*largeur, pTaquinSDL->resY*hauteur, 32, SDL_DOUBLEBUF|SDL_HWSURFACE);
	if(!pTaquinSDL->pWindow)
	{
		freeTaquinSDL(pTaquinSDL);
		return 0;
	}


	SDL_WM_SetCaption("TAQUIN","TAQUIN" );

	SDL_WM_SetIcon(NULL,NULL);

	return 1;
	
}


int displayCaseTaquin(TaquinSDL * _pTaquinSDL,Uint8 _caseTaquin, SDL_Rect * _pDest, int _x, int _y, int _refresh)
{
	// on d�finit o� on veut la dessiner
	{
		SDL_Rect destination = {
			_x * _pTaquinSDL->resX,
			_y * _pTaquinSDL->resY,
			_pTaquinSDL->resX,
			_pTaquinSDL->resY 
		};

		*_pDest = destination;
	}

	int posX = _caseTaquin % _pTaquinSDL->taquin.largeur;
	int posY = _caseTaquin / _pTaquinSDL->taquin.largeur;

	SDL_Rect imgToPaste = {
		posX * _pTaquinSDL->resX,
		posY * _pTaquinSDL->resY,
		_pTaquinSDL->resX,
		_pTaquinSDL->resY 
	};

	// Si la case n'est pas vide ...
	if (_pTaquinSDL->taquin.plateau[_x][_y] != 0)
	{
		// on calcule o� est la case "caseTaquin" dans l'image initiale pour -par la suite - venir d�couper la zone qui correspond � la case

		// On dessine la case dans la fen�tre (en d�coupant dans l'image initiale avec la zone d�finie ci-dessus)
		SDL_LowerBlit(_pTaquinSDL->pFond, &imgToPaste, _pTaquinSDL->pWindow, _pDest);

	}
	else
	{
		SDL_Surface* black_sruface = SDL_CreateRGBSurface(0, _pTaquinSDL->resX, _pTaquinSDL->resY, 32, 0, 0, 0, 0);
		SDL_MapRGB(black_sruface->format, 0, 0, 0);
		SDL_LowerBlit(black_sruface, &imgToPaste, _pTaquinSDL->pWindow, _pDest);
		SDL_FreeSurface(black_sruface);
	}
	

	if(_refresh) SDL_UpdateRect(_pTaquinSDL->pWindow,_pDest->x,_pDest->y,_pDest->w,_pDest->h);

	return 1;
}

// fonction pour rendre le taquin dans son �tat actuel
int displayTaquinSDL(TaquinSDL * _pTaquinSDL)
{
	// Test pour v�rifier que les donn�es pass�es ne sont pas corrompues
	if(!_pTaquinSDL || !_pTaquinSDL->taquin.plateau || !_pTaquinSDL->pWindow ) return 1;

	// On dessine les cases une par une en allant d�couper l'image de fond avec un SDL_Rect
	{

		SDL_Rect dest;
		for (int x = 0; x < _pTaquinSDL->taquin.largeur; x++)
		{
			for (int y = 0; y < _pTaquinSDL->taquin.hauteur; y++)
			{
				displayCaseTaquin(_pTaquinSDL, _pTaquinSDL->taquin.plateau[x][y], &dest, x, y, 0);
			}
		}


		// On met � jour la fen�tre compl�te
		SDL_UpdateRect(_pTaquinSDL->pWindow,0,0,0,0);

	}


	return 0;
}

// fonction permettant de faire le rendu SDL du taquin et de jouer (gestion des �v�nements � l'int�rieur de la fonction)
int gameLoopSDL(int hauteur,int largeur, char * pathBMPfile, int minRandom, int maxRandom)
{
	int end = 0;
	SDL_Event e;
	TaquinSDL t;

	// On cr�e le taquin et la fen�tre pour le dessiner
	if(!createTaquinSDL(&t,hauteur,largeur,pathBMPfile)) return 0;


	// On boucle sur le jeu tant qu'on a pas demand� de quitter
	while(end>=0)
	{
		int tick = SDL_GetTicks();

		// On m�lange le taquin
		mixTaquin(&(t.taquin), minRandom, maxRandom);

		// On affiche le taquin m�lang�
		displayTaquin(&(t.taquin), 0);
		displayTaquinSDL(&t);

		end = 0;

		// On boucle tant que la solution n'a pas �t� trouv�e
		while(!end )
		{
			// Gestion des �v�nements
			

			int dt = SDL_GetTicks()-tick;
			tick +=dt;

			while(!end && SDL_PollEvent(&e))
			{
				switch(e.type)
				{
					case SDL_KEYDOWN:
						{
							deplacement d = AUCUN;
							
							switch(e.key.keysym.sym)
							{
								// On d�finit le mouvement � effectuer
								case SDLK_q:		;
								case SDLK_LEFT:		d = GAUCHE;
									break;
								case SDLK_d:
								case SDLK_RIGHT:	d = DROITE;
									break;
								case SDLK_z:
								case SDLK_UP:		d = HAUT;
									break;
								case SDLK_s:
								case SDLK_DOWN:		d = BAS;
									break;
								case SDLK_ESCAPE:	
									// On quitte l'application
									end = -1;
									break;
								case SDLK_F1:
									// On d�clenche la r�solution du taquin
									// r�solution SDL � faire par la suite pour laisser la main � l'utilisateur :
									// - Arr�ter la r�solution (appui sur n'importe qu'elle touche
									// - Quitter l'application (ECHAP ou CROIX en haut � droite)
									{
										//on initialise les variables permettant de r�cup�rer les informations issues de la r�solution
										deplacement * tabDeplacements = NULL;
										unsigned long nbDeplacements = 0;
										unsigned long nbSommetsParcourus = 0;
										unsigned long timeElapsed = 0;

										// On demande la r�solution du taquin � l'ordinateur
										if(solveTaquin(&(t.taquin),&tabDeplacements,&nbDeplacements, &nbSommetsParcourus, &timeElapsed, 0, t.pWindow))
										{
											// Si on a trouv� une solution on affiche les informations issues de la r�solution
											unsigned long i;
											int res = 0;
											printf("Nombre de deplacements = %d\n",nbDeplacements);
											printf("Nombre de sommets parcourus = %d\n",nbSommetsParcourus);
											printf("Temps ecoule = %d ms\n",timeElapsed);
											displayTaquin(&(t.taquin), 0);
											displayTaquinSDL(&t);

											// On affiche la solution �tape par �tape
											for(i=0; i < nbDeplacements; i++)
											{
												// On effectue le d�placement, on affiche le nouveau plateau et on attend un appui sur une touche pour continuer
												if(tabDeplacements[i]!=AUCUN)
												{
													if (!moveTaquin(&(t.taquin), tabDeplacements[i]))
													{
														displayTaquinSDL(&t);
														displayTaquin(&(t.taquin), 0);
													}
													else break;
												}
											}
										}
										// Si la r�solution n'a pas fonctionn�, on affiche le taquin tel qu'il �tait avant r�solution (on efface l'icone de "progression" si elle avait �t� dessin�e)
										else
										{
											displayTaquinSDL(&t);
											displayTaquin(&(t.taquin), 0);
										}

										if(tabDeplacements) 
										{
											free(tabDeplacements);
											tabDeplacements = NULL;
										}
									}
									break;
							}
							if (d != AUCUN && !moveTaquin(&(t.taquin), d)) {
								displayTaquinSDL(&t);
								displayTaquin(&(t.taquin), 0);
							}
						}
						break;

					case SDL_MOUSEBUTTONDOWN:
						{
							// On r�cup�re la position et l'�tat des boutons de la souris
							int x,y;
							Uint8 state = SDL_GetMouseState(&x,&y);

							if(state & SDL_BUTTON_LEFT)
							{
								deplacement d = AUCUN;

								// On r�cup�re la position de la case dans le taquin
								x = x/t.resX;
								y = y/t.resY;

								if (t.taquin.x == x)
								{
									if (t.taquin.y + 1 == y)
										d = BAS;
									else if (t.taquin.y - 1 == y)
										d = HAUT;
								}
								if (t.taquin.y == y)
								{
									if (t.taquin.x + 1 == x)
										d = DROITE;
									else if (t.taquin.x - 1 == x)
										d = GAUCHE;
								}
								// On applique le d�placement
								if (!moveTaquin(&(t.taquin), d))
								{
									displayTaquinSDL(&t);
									displayTaquin(&(t.taquin), 0); 
								}
							}
						}
						break;

					case SDL_QUIT:
						// On quitte
						end = -1;
						break;
				}
			}
			// On relache du temps processeur pour les autres processus
			SDL_Delay(1);

			if(!end) 
				end = endTaquin(&(t.taquin));
		}

		// Si on n'a pas demand� � quitter c'est qu'on a termin� le taquin
		if(end>0)
		{
			// Affichage de l'image compl�te par dessus le tout
			SDL_Rect rect = { 0, 0, t.pWindow->w, t.pWindow->h};
			SDL_Rect windo = { 0, 0, t.pWindow->w, t.pWindow->h};
			SDL_LowerBlit(t.pFond, &rect, t.pWindow, &windo);
			SDL_UpdateRect(t.pWindow, 0, 0, 0, 0);
			while (e.key.keysym.sym != SDLK_RETURN)
			{
				SDL_PollEvent(&e);
			}
			//SDL_Delay(1000);
		}
		// On r�initialise le taquin pour le prochain tour de jeu
		initTaquin(&(t.taquin));
	}
	// On lib�re le taquin et les surfaces SDL
	freeTaquinSDL(&t);

	/* Shut them both down */
	SDL_Quit();

	return 1;
}

// fonction pour lib�rer le taquins et les surfaces SDL utilis�es
int freeTaquinSDL(TaquinSDL * pTaquinSDL)
{
	// test pour savoir si les donn�es pass�es ne sont pas corrompues
	if(!pTaquinSDL) return 0;

	// On lib�re la fen�tre SDL
	if(pTaquinSDL->pWindow) SDL_FreeSurface(pTaquinSDL->pWindow);

	// On lib�re l'image de fond
	if(pTaquinSDL->pFond) SDL_FreeSurface(pTaquinSDL->pFond);

	// On lib�re le taquin (mode console)
	freeTaquin(&(pTaquinSDL->taquin));

	return 1;
}