/*
_____________________________________________________________________________________________

                                    fonctions.h
_____________________________________________________________________________________________

****************        ******************************************************        *******

                ******* Par Hidekela Fanaperantsoa, facebook.com/404.dont.find *******

****************        ******************************************************        *******




---------------------------------------------------------------------------------------------

            Rôle: Prototype de quelques fonctions utiles

---------------------------------------------------------------------------------------------

                        Date de création: 16/11/2021 à 17:00
                        Date de dernière modification: 16/11/2021 à 17:00

                                ~~~~~~~~~~~~~~~~~~~~~

*/
#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <FMOD/fmod.h>
#include "constants.h"
#include "joueurMusique.h"

void tracerParPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void clickGauche(bool* pleinEcran, bool* listeAudio, int nombreMusique, Uint16 x, Uint16 y,int reperage_x, int* pContinuer,FMOD_SYSTEM *systeme,FMOD_CHANNEL **canal,FMOD_SOUND *musique[PLAYLIST_MAX],FMOD_DSP **dsp,FMOD_DSP_PARAMETER_FFT **spectre,int *musiqueJoueeActuelle,unsigned int longueurMusique[PLAYLIST_MAX], bool* play, bool* stop, float* volume,SDL_Surface* surfaceMusique[PLAYLIST_MAX],char liste_basename_surfaceMusique[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX],SDL_Color couleurBleu,SDL_Color couleurNoire,SDL_Color couleurBlanc,TTF_Font *arial);
FOCUS focus(Uint16 x,Uint16 y,int reperage_x,bool play,bool stop);
char* strtolower(char *chaine);
void getNameParentDir(char dirname[]);
void UTF8(char chaine[]);
void ReblitScrolle(Uint16 x,int musiqueJoueeActuelle,int* Pdebut_blit_surfaceMusique,int debut_blit_dir_file,int nombreFichierLu,TYPE_FICHIER typeFichier[],SDL_Surface* ecran,SDL_Surface* iconDossier,SDL_Surface* iconPrecedent,SDL_Surface* iconMusique,SDL_Surface* iconPlay,SDL_Surface** surfaceMusique,SDL_Surface** liste_dossier_ou_fichier);
void doubleclik_event(bool *stop,FMOD_SYSTEM *systeme,FMOD_CHANNEL **canal,FMOD_SOUND *musique[PLAYLIST_MAX],FMOD_DSP **dsp,FMOD_DSP_PARAMETER_FFT **spectre,float volume,unsigned int longueurMusique[PLAYLIST_MAX],SDL_Surface* ecran,int* musiqueJoueeActuelle,SDL_Surface** dossierActuel,char format[19][6],char* dossierOuvert,TTF_Font* razorface_regular,TTF_Font* gannister,TTF_Font* arial,SDL_Color couleurVert,SDL_Color couleurBleu,SDL_Color couleurBlanc,SDL_Color couleurNoire,Uint16 x,Uint16 y,TYPE_FICHIER* typeFichier,SDL_Surface** liste_dossier_ou_fichier,SDL_Surface* surfaceMusique[PLAYLIST_MAX],SDL_Surface* iconPrecedent,SDL_Surface* iconDossier,SDL_Surface* iconMusique,SDL_Surface* iconPlay,int* POINTEUR_debut_blit_dir_file,int* POINTEUR_debut_blit_surfaceMusique,int* PnombreFichierLu,int* PnombreMusique,char liste_nom[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX],char liste_path_surfaceMusique[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX],char liste_basename_surfaceMusique[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX]);


#endif // FONCTIONS_H_INCLUDED
