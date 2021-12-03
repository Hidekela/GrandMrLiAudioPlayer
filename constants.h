/*
_____________________________________________________________________________________________

                                    constants.h
_____________________________________________________________________________________________

****************        ******************************************************        *******

                ******* Par Hidekela Fanaperantsoa, facebook.com/404.dont.find *******

****************        ******************************************************        *******




---------------------------------------------------------------------------------------------

            Rôle: Listes des contantes utiles, des énumérations et des structures

---------------------------------------------------------------------------------------------

                        Date de création: 16/11/2021 à 17:00
                        Date de dernière modification: 16/11/2021 à 17:00

                                ~~~~~~~~~~~~~~~~~~~~~

*/
#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED


#define LARGEUR_FENETRE 522
#define HAUTEUR_FENETRE 405
#define POSITION_Y_POSITION 335 //Ordonnée de la position actuelle de la musique (la barre evolutive)
#define LARGEUR_FENETRE_LISTE 548
#define HAUTEUR_FENETRE_LISTE 350
#define NOMBRE_DOSSIER_MAX 100
#define NOM_DOSSIER_MAX 245
#define BLIT_MUSIQUE_MAX 12
#define BLIT_DIR_FILE_MAX 12
#define POSITION_Y_dossierActuel 52
#define POSITION_X_MUSIQUE 15
#define POSITION_Y_MUSIQUE 81 //Ordonnée du debut de blitage des surfaceMusique
#define POSITION_X_DOSSIER 288
#define POSITION_Y_DOSSIER 81 //Ordonnée du debut de blitage des surfaceMusique
#define HAUTEUR_DOSSIER_FICHIER 22
#define WIDTH_MAX_SURFACE_MUSIQUE 230 //Pour ne pas dépasser la barre qui sépare la playlit des dossiers

enum elementEnum {ICON,CAPTION,REDUIRE,AGRANDIR,QUITTER,LISTE,STOP,PRECEDENT,PLAY,PAUSE,SUIVANT,BAISSEVOLUME,AUGMENTEVOLUME,_NOMBRE_ELEMENT_ENUM_};
typedef enum bouttonFocusEnum {REDUIRE_FOCUS,AGRANDIR_FOCUS,QUITTER_FOCUS,LISTE_FOCUS,STOP_FOCUS,PRECEDENT_FOCUS,PLAY_FOCUS,PAUSE_FOCUS,SUIVANT_FOCUS,BAISSEVOLUME_FOCUS,AUGMENTEVOLUME_FOCUS,_NOMBRE_BOUTTON_FOCUS_ENUM_,SANS_FOCUS} bouttonFocusEnum;
typedef struct {SDL_Rect position; bouttonFocusEnum focusRetour;} FOCUS;
typedef struct dirent dirent;
typedef enum {DOSSIER,DOSSIER_PARENT,MUSIQUE} TYPE_FICHIER;


#endif // CONSTANTS_H_INCLUDED
