/*
_____________________________________________________________________________________________

                                    main.c
_____________________________________________________________________________________________

****************        ******************************************************        *******

                ******* Par Hidekela Fanaperantsoa, facebook.com/404.dont.find *******

****************        ******************************************************        *******




---------------------------------------------------------------------------------------------

            Rôle: Fonction principale, affiche le controleur et la liste de musique

---------------------------------------------------------------------------------------------

                        Date de création: 25/04/2020 à 22:41
                        Date de dernière modification: 16/11/2021 à 17:00

                                ~~~~~~~~~~~~~~~~~~~~~

*/
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
#include "fonctions.h"

int main(int argc, char *argv[])
{
    int continuer = 1, idDossierOuvert = 0, musiqueJoueeActuelle = -1, barreSpectrum = 0, reperage_x = 0, i = 0, j = 0, k = 0, nombreMusique = 0, nombreFichierLu = 0, debut_blit_surfaceMusique = 0, debut_blit_dir_file = 0;
    long tempsPrecedent = 0, tempsActuel = 0, tempsClickPrecedent = 0, tempsClickActuel = 0;
    unsigned int longueurMusique[PLAYLIST_MAX] = {0}, positionMusique = 0;
    bool pleinEcran = false, pleinEcran2 = false, listeAudio = false, alt = false, play = false, stop = true, click = false, tabButton = false;
    float volume = 0.1;
    char charTemp, *stringTemp, *TestFormat, disqueExistant[26][5] = {{0}}, liste_nom[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX], liste_path_surfaceMusique[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX], liste_basename_surfaceMusique[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX], format[19][6] = {".mp3",".wav",".wma",".dls",".m3u",".asx",".wax",".pls",".aiff",".asf",".flac",".fsb",".it",".mod",".mp2",".ogg",".raw",".s3m",".xm"}, dossierOuvert[NOM_DOSSIER_MAX] = {0};

    TYPE_FICHIER  typeFichier[NOMBRE_DOSSIER_MAX] = {DOSSIER};
    DIR *dossier = NULL, *TestDossier = NULL;
    dirent *dossier_ou_fichier = NULL;

    SDL_Surface *ecran = NULL, *titre1 = NULL, *titre2 = NULL, *dossierActuel = NULL, *surfaceMusique[PLAYLIST_MAX] = {NULL}, *liste_dossier_ou_fichier[NOMBRE_DOSSIER_MAX] = {NULL}, *barreVerticale = NULL, *iconDossier = NULL, *iconPrecedent = NULL, *iconMusique = NULL, *iconPlay = NULL, *element[_NOMBRE_ELEMENT_ENUM_] = {NULL}, *boutton_focus[_NOMBRE_BOUTTON_FOCUS_ENUM_] = {NULL};
    SDL_Event event;
    SDL_Rect position, BlitMusiqueUnderMax;
    SDL_Color couleurBlanc = {255,255,255}, couleurNoire = {0,0,0}, couleurVert = {50,255,50}, couleurBleu = {0,162,232};

    TTF_Font *revamped, *arial, *razorface_regular, *gannister;

    FOCUS testfocus;

    FMOD_SYSTEM *systeme = NULL;
    FMOD_RESULT resultat;
    FMOD_CHANNEL *canal = NULL;
    FMOD_SOUND *musique[PLAYLIST_MAX] = {NULL};
    FMOD_DSP *dsp = NULL;
    FMOD_DSP_PARAMETER_FFT *spectre = NULL;

    //Initialisation de FMOD

    if((resultat = FMOD_System_Create(&systeme)) != FMOD_OK || (resultat = FMOD_System_Init(systeme,1,FMOD_INIT_NORMAL,NULL)) != FMOD_OK)
    {
        fprintf(stderr,"Erreur de chargement de FMOD: %d\n",resultat);
        exit(EXIT_FAILURE);
    }

    //Initialisation de SDL_ttf

    TTF_Init();

    revamped = TTF_OpenFont("font\\revamped.ttf",20);
    arial = TTF_OpenFont("font\\arial.ttf",12);
    razorface_regular = TTF_OpenFont("font\\razorface_regular.ttf",17);
    gannister = TTF_OpenFont("font\\gannister.ttf",10);

    //Initialisation de SDL

    SDL_Init(SDL_INIT_VIDEO);

    SDL_putenv("SDL_VIDEO_CENTERED=center");

    SDL_WM_SetIcon(IMG_Load("sprite\\setIcon.png"),NULL);
    SDL_WM_SetCaption("Grand Myster Li audioPlayer",NULL);
    ecran = SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32,SDL_HWSURFACE|SDL_NOFRAME|SDL_DOUBLEBUF);

    if(ecran == NULL)
    {
        fprintf(stderr,"Erreur de chargement video: %s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

/*
___________________________________________________________________________________________________

                **************Chargemenet des sprites***************
___________________________________________________________________________________________________

*/

    element[ICON] = IMG_Load("sprite\\icon.png");
    element[CAPTION] = TTF_RenderText_Shaded(arial,"Grand Myster Li audioPlayer",couleurBlanc,couleurNoire);
    element[REDUIRE] = IMG_Load("sprite\\reduire.png");
    element[AGRANDIR] = IMG_Load("sprite\\agrandir.png");
    element[QUITTER] = IMG_Load("sprite\\quitter.png");
    element[LISTE] = IMG_Load("sprite\\liste.png");
    element[STOP] = IMG_Load("sprite\\stop.png");
    element[PRECEDENT] = IMG_Load("sprite\\precedent.png");
    element[PLAY] = IMG_Load("sprite\\play.png");
    element[PAUSE] = IMG_Load("sprite\\pause.png");
    element[SUIVANT] = IMG_Load("sprite\\suivant.png");
    element[BAISSEVOLUME] = IMG_Load("sprite\\baissevolume.png");
    element[AUGMENTEVOLUME] = IMG_Load("sprite\\augmentevolume.png");

    boutton_focus[REDUIRE_FOCUS] = IMG_Load("sprite\\reduire-focus.png");
    boutton_focus[AGRANDIR_FOCUS] = IMG_Load("sprite\\agrandir-focus.png");
    boutton_focus[QUITTER_FOCUS] = IMG_Load("sprite\\quitter-focus.png");
    boutton_focus[LISTE_FOCUS] = IMG_Load("sprite\\liste-focus.png");
    boutton_focus[STOP_FOCUS] = IMG_Load("sprite\\stop-focus.png");
    boutton_focus[PRECEDENT_FOCUS] = IMG_Load("sprite\\precedent-focus.png");
    boutton_focus[PLAY_FOCUS] = IMG_Load("sprite\\play-focus.png");
    boutton_focus[PAUSE_FOCUS] = IMG_Load("sprite\\pause-focus.png");
    boutton_focus[SUIVANT_FOCUS] = IMG_Load("sprite\\suivant-focus.png");
    boutton_focus[BAISSEVOLUME_FOCUS] = IMG_Load("sprite\\baissevolume-focus.png");
    boutton_focus[AUGMENTEVOLUME_FOCUS] = IMG_Load("sprite\\augmentevolume-focus.png");

/*
____________________________________________________________________________________________

            Chargement des icons
____________________________________________________________________________________________

*/

    iconDossier = IMG_Load("sprite\\dossier.png");
    iconPrecedent = IMG_Load("sprite\\dossierParent.png");
    iconMusique = IMG_Load("sprite\\musique.png");
    iconPlay = IMG_Load("sprite\\playMusic.png");

/*
____________________________________________________________________________________________

            Chargement des Surfaces constantes
____________________________________________________________________________________________

*/

    barreVerticale = SDL_CreateRGBSurface(SDL_HWSURFACE,2,257,32,0,0,0,0);
    SDL_FillRect(barreVerticale,NULL,SDL_MapRGB(ecran->format,100,100,100));
    titre1 = TTF_RenderText_Shaded(revamped,"Playlist",couleurBlanc,couleurNoire);
    titre2 = TTF_RenderText_Shaded(revamped,"Dossier",couleurBlanc,couleurNoire);

/*
__________________________________________________________________________________________________


        Affichage des éléments sur SDL_Surface *ecran


        NB: i) L'affichage est fait avant les évènements pour éviter la création de plusieur
          SDL_Surface* actuelle (voir TP MarioSokoban M@teo21)

            ii) element[CAPTION] avant tout car les position en y sont pareilles après,
          pareil pour element[PLAY]

            iii) Tester si play ou pause: element[PLAY/PAUSE]?

            iv) Les autres après element[PLAY] se positionnent relativement par rapport à ce
          dernier

            v) reperage_x, représentant l'abscisse de element[LISTE], est utilisé pour
          se repérer dans la fonction focus()
__________________________________________________________________________________________________
*/

    //Souris initialement au centre de l'écran
    SDL_WarpMouse(ecran->w/2,ecran->h/2);

    while(continuer)
    {
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

        position.y = 8;
        position.x = 28;
        SDL_BlitSurface(element[CAPTION],NULL,ecran,&position);

        position.y = 0;

        position.x = 3;
        SDL_BlitSurface(element[ICON],NULL,ecran,&position);

        position.x = LARGEUR_FENETRE - (element[REDUIRE]->w * 3);
        SDL_BlitSurface(element[REDUIRE],NULL,ecran,&position);

        position.x = LARGEUR_FENETRE - (element[AGRANDIR]->w * 2);
        SDL_BlitSurface(element[AGRANDIR],NULL,ecran,&position);

        position.x = LARGEUR_FENETRE - element[QUITTER]->w;
        SDL_BlitSurface(element[QUITTER],NULL,ecran,&position);

        position.y = HAUTEUR_FENETRE - element[PLAY]->h - 10;
        position.x = (LARGEUR_FENETRE - element[PLAY]->w)/2;
        play||stop?SDL_BlitSurface(element[PLAY],NULL,ecran,&position):SDL_BlitSurface(element[PAUSE],NULL,ecran,&position);

        position.y = HAUTEUR_FENETRE - 48;

        position.x -= 55;
        SDL_BlitSurface(element[PRECEDENT],NULL,ecran,&position);

        position.x -= 30;
        SDL_BlitSurface(element[STOP],NULL,ecran,&position);

        position.x -= 30;
        SDL_BlitSurface(element[LISTE],NULL,ecran,&position);

        reperage_x = position.x;

        position.x = LARGEUR_FENETRE - position.x - 25;
        SDL_BlitSurface(element[AUGMENTEVOLUME],NULL,ecran,&position);

        position.x -= 30;
        SDL_BlitSurface(element[BAISSEVOLUME],NULL,ecran,&position);

        position.x -= 55;
        SDL_BlitSurface(element[SUIVANT],NULL,ecran,&position);

/*
_____________________________________________________________________________________________________

        Evénements :
            i) Teste de focus en avance car on blit la focus après clickGauche si il y en a
            ii) Affectation de position.x/y pour le blitage du seul boutton_focus courrant
_____________________________________________________________________________________________________

*/
        testfocus = focus(event.motion.x,event.motion.y,reperage_x,play,stop);
        position.x = testfocus.position.x;
        position.y = testfocus.position.y;

        SDL_PollEvent(&event);
        switch(event.type)
        {
            case SDL_KEYUP:
                alt = false;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_LALT:
                        alt = true;
                        break;
                    case SDLK_F4:
                        if(alt)
                        {
                            continuer = 0;
                        }
                        continue;
                    case SDLK_UP: //Augmentation de la volume de 1%
                        if(volume < 1.0)
                        {
                            volume += 0.01;
                            FMOD_Channel_SetVolume(canal,volume);
                        }
                        break;
                    case SDLK_DOWN: //Baissé de la volume de 1%
                        if(volume > 0.01)
                        {
                            volume -= 0.01;
                            FMOD_Channel_SetVolume(canal,volume);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                click = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if(click)
                {
                    switch(event.button.button)
                    {
                        case SDL_BUTTON_LEFT:

                            clickGauche(&pleinEcran,&listeAudio,nombreMusique,event.button.x,event.button.y,reperage_x,&continuer,systeme,&canal,musique,&dsp,&spectre,&musiqueJoueeActuelle,longueurMusique,&play,&stop,&volume,surfaceMusique,liste_basename_surfaceMusique,couleurBleu,couleurNoire,couleurBlanc,arial);

                            if(listeAudio)
                            {
/*
______________________________________________________________________________________________________________________

                                Affichage de la fenêtre de la liste des dossiers et des playlist

                                        jusqu'à la fin de la boucle while. (!listeAudio)
______________________________________________________________________________________________________________________

*/

                                click = false;

                                //Chargement des diques éxistant

                                idDossierOuvert = 0;
                                stringTemp = (char*) malloc(5*sizeof(char));
                                for(charTemp = 'A'; charTemp <= 'Z'; charTemp++)
                                {
                                    sprintf(stringTemp,"%c:\\",charTemp);
                                    dossier = opendir(stringTemp);
                                    if(dossier != NULL)
                                    {
                                        strcpy(disqueExistant[idDossierOuvert],stringTemp);
                                        idDossierOuvert++;
                                    }
                                }
                                free(stringTemp);

                                //Ouvre le premier disque trouvé
                                idDossierOuvert = 0;
                                strcpy(dossierOuvert,disqueExistant[idDossierOuvert]);
                                dossier = opendir(dossierOuvert);
                                i = 0;

                                //Charge les dossiers et les musiques du premier disque trouvé

                                while((dossier_ou_fichier = readdir(dossier)) != NULL && i < NOMBRE_DOSSIER_MAX)
                                {
                                    stringTemp = (char*) malloc(sizeof(dossierOuvert)+sizeof(dossier_ou_fichier->d_name));
                                    sprintf(stringTemp,"%s%s",dossierOuvert,dossier_ou_fichier->d_name);
                                    if((TestDossier = opendir(stringTemp)) != NULL)
                                    {
                                        closedir(TestDossier);
                                        if(dossier_ou_fichier->d_namlen == 1 && strcmp(dossier_ou_fichier->d_name,".") == 0);
                                        else if(dossier_ou_fichier->d_namlen == 2 && strcmp(dossier_ou_fichier->d_name,"..") == 0)
                                        {
                                            typeFichier[i] = DOSSIER_PARENT;
                                            strcpy(liste_nom[i],dossier_ou_fichier->d_name);
                                            liste_dossier_ou_fichier[i] = TTF_RenderText_Shaded(gannister," DOSSIER_PARENT",couleurBlanc,couleurNoire);
                                            i++;
                                        }
                                        else
                                        {
                                            typeFichier[i] = DOSSIER;
                                            strcpy(liste_nom[i],dossier_ou_fichier->d_name);
                                            liste_dossier_ou_fichier[i] = TTF_RenderText_Shaded(arial,liste_nom[i],couleurBlanc,couleurNoire);
                                            i++;
                                        }
                                    }
                                    else if((TestFormat = strrchr(dossier_ou_fichier->d_name,'.')) != NULL)
                                    {
                                        for(j = 0; j < 19 && strcmp(format[j],strtolower(TestFormat)) != 0; j++);
                                        if(j==19) continue;
                                        typeFichier[i] = MUSIQUE;
                                        strcpy(liste_nom[i],dossier_ou_fichier->d_name);
                                        liste_dossier_ou_fichier[i] = TTF_RenderText_Shaded(arial,liste_nom[i],couleurBlanc,couleurNoire);
                                        i++;
                                    }
                                    free(stringTemp);
                                }
                                closedir(dossier);
                                nombreFichierLu = i;
                                debut_blit_dir_file = 0;

                                //Chargement de la surface du dossier actuel
                                dossierActuel = TTF_RenderText_Shaded(razorface_regular,dossierOuvert,couleurVert,couleurNoire);

                                //Chargement d'écran

                                ecran = SDL_SetVideoMode(LARGEUR_FENETRE_LISTE,HAUTEUR_FENETRE_LISTE,32,SDL_HWSURFACE|SDL_DOUBLEBUF);

                                if(ecran == NULL)
                                {
                                    fprintf(stderr,"Erreur de chargement video: %s\n",SDL_GetError());
                                    exit(EXIT_FAILURE);
                                }

                                //Affichage des surfaces chargés

                                position.y = 15;
                                position.x = (ecran->w/2 - titre1->w)/2;
                                SDL_BlitSurface(titre1,NULL,ecran,&position);

                                position.x = (3*ecran->w/2 - titre2->w)/2;
                                SDL_BlitSurface(titre2,NULL,ecran,&position);


                                position.y = POSITION_Y_dossierActuel;
                                position.x = (ecran->w - dossierActuel->w)/2;
                                if(position.x < 0) position.x = 0;
                                SDL_BlitSurface(dossierActuel,NULL,ecran,&position);

                                position.y = POSITION_Y_MUSIQUE;
                                position.x = POSITION_X_MUSIQUE;
                                BlitMusiqueUnderMax.x = 0;
                                BlitMusiqueUnderMax.y = 0;

                                for(i = debut_blit_surfaceMusique; surfaceMusique[i] != NULL && i < PLAYLIST_MAX && i < (BLIT_MUSIQUE_MAX+debut_blit_surfaceMusique); i++)
                                {
                                    i != musiqueJoueeActuelle? SDL_BlitSurface(iconMusique,NULL,ecran,&position) : SDL_BlitSurface(iconPlay,NULL,ecran,&position);
                                    position.x += 18;
                                    position.y += 3;
                                    if(surfaceMusique[i]->w > WIDTH_MAX_SURFACE_MUSIQUE)
                                    {
                                        BlitMusiqueUnderMax.w = WIDTH_MAX_SURFACE_MUSIQUE;
                                    }
                                    else
                                    {
                                        BlitMusiqueUnderMax.w = surfaceMusique[i]->w;
                                    }
                                    BlitMusiqueUnderMax.h = surfaceMusique[i]->h;

                                    SDL_BlitSurface(surfaceMusique[i],&BlitMusiqueUnderMax,ecran,&position);
                                    position.y += surfaceMusique[i]->h + 5;
                                    position.x -= 18;
                                }

                                position.y = POSITION_Y_DOSSIER;
                                position.x = ecran->w/2 - 1;
                                SDL_BlitSurface(barreVerticale,NULL,ecran,&position);


                                position.x = POSITION_X_DOSSIER;
                                for(i = debut_blit_dir_file; i < (BLIT_DIR_FILE_MAX+debut_blit_dir_file) && i < nombreFichierLu; i++)
                                {
                                    if(typeFichier[i] == DOSSIER_PARENT)
                                        SDL_BlitSurface(iconPrecedent,NULL,ecran,&position);
                                    else if(typeFichier[i] == DOSSIER)
                                        SDL_BlitSurface(iconDossier,NULL,ecran,&position);
                                    else
                                        SDL_BlitSurface(iconMusique,NULL,ecran,&position);

                                    position.x += 18;
                                    if(typeFichier[i] != DOSSIER_PARENT) position.y += 3;
                                    SDL_BlitSurface(liste_dossier_ou_fichier[i],NULL,ecran,&position);
                                    position.y += liste_dossier_ou_fichier[i]->h + 5;
                                    position.x -= 18;
                                }

                                SDL_Flip(ecran);

/*
________________________________________________________________________________________________________________

                                Evènements dans la liste des dossier et playlist
________________________________________________________________________________________________________________

*/

                                while(listeAudio)
                                {
                                    SDL_PollEvent(&event);
                                    switch(event.type)
                                    {
                                        case SDL_QUIT: //Quitte tout
                                            continuer = 0;
                                            listeAudio = false;
                                            continue;
                                        case SDL_KEYUP:
                                            tabButton = false;
                                            break;
                                        case SDL_KEYDOWN:
                                            switch(event.key.keysym.sym)
                                            {
                                                case SDLK_ESCAPE: //Quitte le fenêtre de la liste des dossiers et playlist
                                                    listeAudio = false;
                                                    continue;
                                                case SDLK_TAB:
                                                    if(!tabButton)
                                                    {
                                                        //Ouverture du disque suivant

                                                        idDossierOuvert++;
                                                        if(strcmp(disqueExistant[idDossierOuvert],"") == 0)
                                                            idDossierOuvert = 0;

                                                        strcpy(dossierOuvert,disqueExistant[idDossierOuvert]);
                                                        dossier = opendir(dossierOuvert);

                                                        //Vide la liste des noms des dossiers et musiques

                                                        for(i = 0; i < nombreFichierLu; i++)
                                                            strcpy(liste_nom[i],"");

                                                        //Chargement des dossiers et musiques du disque trouvé

                                                        i = 0;

                                                        while((dossier_ou_fichier = readdir(dossier)) != NULL && i < NOMBRE_DOSSIER_MAX)
                                                        {
                                                            stringTemp = (char*) malloc(sizeof(dossierOuvert)+sizeof(dossier_ou_fichier->d_name));
                                                            sprintf(stringTemp,"%s%s",dossierOuvert,dossier_ou_fichier->d_name);
                                                            if((TestDossier = opendir(stringTemp)) != NULL)
                                                            {
                                                                closedir(TestDossier);
                                                                if(dossier_ou_fichier->d_namlen == 1 && strcmp(dossier_ou_fichier->d_name,".") == 0);
                                                                else if(dossier_ou_fichier->d_namlen == 2 && strcmp(dossier_ou_fichier->d_name,"..") == 0)
                                                                {
                                                                    typeFichier[i] = DOSSIER_PARENT;
                                                                    strcpy(liste_nom[i],dossier_ou_fichier->d_name);
                                                                    liste_dossier_ou_fichier[i] = TTF_RenderText_Shaded(gannister," DOSSIER_PARENT",couleurBlanc,couleurNoire);
                                                                    i++;
                                                                }
                                                                else
                                                                {
                                                                    typeFichier[i] = DOSSIER;
                                                                    strcpy(liste_nom[i],dossier_ou_fichier->d_name);
                                                                    liste_dossier_ou_fichier[i] = TTF_RenderText_Shaded(arial,liste_nom[i],couleurBlanc,couleurNoire);
                                                                    i++;
                                                                }
                                                            }
                                                            else if((TestFormat = strrchr(dossier_ou_fichier->d_name,'.')) != NULL)
                                                            {
                                                                for(j = 0; j < 19 && strcmp(format[j],strtolower(TestFormat)) != 0; j++);
                                                                if(j==19) continue;
                                                                typeFichier[i] = MUSIQUE;
                                                                strcpy(liste_nom[i],dossier_ou_fichier->d_name);
                                                                liste_dossier_ou_fichier[i] = TTF_RenderText_Shaded(arial,liste_nom[i],couleurBlanc,couleurNoire);
                                                                i++;
                                                            }
                                                            free(stringTemp);
                                                        }
                                                        closedir(dossier);
                                                        nombreFichierLu = i;

                                                        //Charge la surface du disque ouvert
                                                        dossierActuel = TTF_RenderText_Shaded(razorface_regular,dossierOuvert,couleurVert,couleurNoire);

                                                        //Affiche les surfaces chargés

                                                        position.h = dossierActuel->h;
                                                        position.w = LARGEUR_FENETRE_LISTE;
                                                        position.y = POSITION_Y_dossierActuel;
                                                        position.x = 0;
                                                        SDL_FillRect(ecran,&position,SDL_MapRGB(ecran->format,0,0,0));

                                                        position.x = (ecran->w - dossierActuel->w)/2;
                                                        if(position.x < 0) position.x = 0;
                                                        SDL_BlitSurface(dossierActuel,NULL,ecran,&position);

                                                        position.h = HAUTEUR_FENETRE_LISTE - POSITION_Y_DOSSIER;
                                                        position.w = LARGEUR_FENETRE_LISTE - POSITION_X_DOSSIER;
                                                        position.y = POSITION_Y_DOSSIER;
                                                        position.x = POSITION_X_DOSSIER;
                                                        SDL_FillRect(ecran,&position,SDL_MapRGB(ecran->format,0,0,0));

                                                        //Pour éviter des erreurs causées par la différence de nombre de dossier dans chaque disque, on blit à partir de 0
                                                        debut_blit_dir_file = 0;

                                                        for(i = debut_blit_dir_file; i < (BLIT_DIR_FILE_MAX+debut_blit_dir_file) && i < nombreFichierLu; i++)
                                                        {
                                                            typeFichier[i] == DOSSIER? SDL_BlitSurface(iconDossier,NULL,ecran,&position) : (typeFichier[i] == DOSSIER_PARENT?SDL_BlitSurface(iconPrecedent,NULL,ecran,&position) : SDL_BlitSurface(iconMusique,NULL,ecran,&position));
                                                            position.x += 18;
                                                            if(typeFichier[i] != DOSSIER_PARENT) position.y += 3;
                                                            SDL_BlitSurface(liste_dossier_ou_fichier[i],NULL,ecran,&position);
                                                            position.y += liste_dossier_ou_fichier[i]->h + 5;
                                                            position.x -= 18;
                                                        }

                                                        SDL_Flip(ecran);

                                                        tabButton = true;
                                                    }
                                                    break;
                                                default:
                                                    break;
                                            }
                                            break;
                                        case SDL_MOUSEBUTTONDOWN:
                                            if(event.button.button == SDL_BUTTON_WHEELDOWN)
                                            {
                                                if(event.button.x > 274 && (nombreFichierLu - 1 - debut_blit_dir_file)/BLIT_DIR_FILE_MAX > 0)
                                                {
                                                    //Scroll vers le bas des dossiers et musiques
                                                    debut_blit_dir_file += 12;
                                                    ReblitScrolle(event.button.x,musiqueJoueeActuelle,&debut_blit_surfaceMusique,debut_blit_dir_file,nombreFichierLu,typeFichier,ecran,iconDossier,iconPrecedent,iconMusique,iconPlay,surfaceMusique,liste_dossier_ou_fichier);
                                                }
                                                else if((nombreMusique - 1 - debut_blit_surfaceMusique)/BLIT_MUSIQUE_MAX > 0)
                                                {
                                                    //Scroll vers le bas du playlist
                                                    debut_blit_surfaceMusique += 12;
                                                    ReblitScrolle(event.button.x,musiqueJoueeActuelle,&debut_blit_surfaceMusique,debut_blit_dir_file,nombreFichierLu,typeFichier,ecran,iconDossier,iconPrecedent,iconMusique,iconPlay,surfaceMusique,liste_dossier_ou_fichier);
                                                }
                                            }
                                            else if(event.button.button == SDL_BUTTON_WHEELUP)
                                            {
                                                if(event.button.x > 274 && debut_blit_dir_file > 0)
                                                {
                                                    //Scroll vers le haut des dossiers et musiques
                                                    debut_blit_dir_file -= 12;
                                                    ReblitScrolle(event.button.x,musiqueJoueeActuelle,&debut_blit_surfaceMusique,debut_blit_dir_file,nombreFichierLu,typeFichier,ecran,iconDossier,iconPrecedent,iconMusique,iconPlay,surfaceMusique,liste_dossier_ou_fichier);
                                                }
                                                else if(debut_blit_surfaceMusique > 0)
                                                {
                                                    //Scroll vers le haut du playlist
                                                    debut_blit_surfaceMusique -= 12;
                                                    ReblitScrolle(event.button.x,musiqueJoueeActuelle,&debut_blit_surfaceMusique,debut_blit_dir_file,nombreFichierLu,typeFichier,ecran,iconDossier,iconPrecedent,iconMusique,iconPlay,surfaceMusique,liste_dossier_ou_fichier);
                                                }
                                            }
                                            else if(!click)
                                            {
                                                tempsClickActuel = SDL_GetTicks();
                                                if (tempsClickActuel - tempsClickPrecedent < 300)
                                                {
                                                    //Activation du double clique et teste si c'est en bon endroit
                                                    if(event.button.y > POSITION_Y_DOSSIER && event.button.y < (position.y - 4))
                                                        doubleclik_event(&stop,systeme,&canal,musique,&dsp,&spectre,volume,longueurMusique,ecran,&musiqueJoueeActuelle,&dossierActuel,format,dossierOuvert,razorface_regular,gannister,arial,couleurVert,couleurBleu,couleurBlanc,couleurNoire,event.button.x,event.button.y,typeFichier,liste_dossier_ou_fichier,surfaceMusique,iconPrecedent,iconDossier,iconMusique,iconPlay,&debut_blit_dir_file,&debut_blit_surfaceMusique,&nombreFichierLu,&nombreMusique,liste_nom,liste_path_surfaceMusique,liste_basename_surfaceMusique);
                                                }
                                                tempsClickPrecedent = SDL_GetTicks();
                                                click = true;
                                            }
                                            break;
                                        case SDL_MOUSEBUTTONUP:
                                            click = false;
                                            break;
                                        default:
                                            break;
                                    }

                                    tempsActuel = SDL_GetTicks();
                                    if (tempsActuel - tempsPrecedent < 25)
                                    {
                                        SDL_Delay(25 - (tempsActuel - tempsPrecedent));
                                    }
                                    tempsPrecedent = SDL_GetTicks();

                                    //Si il y a une musique jouée, on teste la position actuelle de la musique
                                    if(!stop)
                                    {
                                        if((resultat = FMOD_Channel_GetPosition(canal,&positionMusique,FMOD_TIMEUNIT_MS)) != FMOD_OK)
                                        {
                                            fprintf(stderr,"Erreur de chargement de la position actuelle de la musique: %d\n",resultat);
                                            exit(EXIT_FAILURE);
                                        }

                                        //À la fin de la musique, on joue la musique valide suivante ou on stop
                                        if(positionMusique>=longueurMusique[musiqueJoueeActuelle])
                                        {
                                            surfaceMusique[musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[musiqueJoueeActuelle],couleurBlanc,couleurNoire);
                                            musiqueJoueeActuelle++;
                                            while(musiqueJoueeActuelle < PLAYLIST_MAX && !(longueurMusique[musiqueJoueeActuelle] > 0))
                                            {
                                                musiqueJoueeActuelle++;
                                            }

                                            if(musiqueJoueeActuelle < PLAYLIST_MAX && musique[musiqueJoueeActuelle] != NULL)
                                            {
                                                MusicPlay(systeme,&canal,musique,&dsp,&spectre,musiqueJoueeActuelle,volume);
                                                surfaceMusique[musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[musiqueJoueeActuelle],couleurBleu,couleurNoire);
                                            }
                                            else
                                            {
                                                stop = true;
                                                musiqueJoueeActuelle = 0;
                                            }

                                            //Mis à jour de l'affichage de la musique jouée (colorée en bleu et l'ancienne en blanc)

                                            position.y = POSITION_Y_MUSIQUE;
                                            position.x = POSITION_X_MUSIQUE;
                                            BlitMusiqueUnderMax.x = 0;
                                            BlitMusiqueUnderMax.y = 0;

                                            for(i = debut_blit_surfaceMusique; surfaceMusique[i] != NULL && i < PLAYLIST_MAX && i < (BLIT_MUSIQUE_MAX+debut_blit_surfaceMusique); i++)
                                            {
                                                i != musiqueJoueeActuelle? SDL_BlitSurface(iconMusique,NULL,ecran,&position) : SDL_BlitSurface(iconPlay,NULL,ecran,&position);
                                                position.x += 18;
                                                position.y += 3;
                                                if(surfaceMusique[i]->w > WIDTH_MAX_SURFACE_MUSIQUE)
                                                {
                                                    BlitMusiqueUnderMax.w = WIDTH_MAX_SURFACE_MUSIQUE;
                                                }
                                                else
                                                {
                                                    BlitMusiqueUnderMax.w = surfaceMusique[i]->w;
                                                }
                                                BlitMusiqueUnderMax.h = surfaceMusique[i]->h;

                                                SDL_BlitSurface(surfaceMusique[i],&BlitMusiqueUnderMax,ecran,&position);
                                                position.y += surfaceMusique[i]->h + 5;
                                                position.x -= 18;
                                            }

                                            SDL_Flip(ecran);
                                        }
                                    }
                                }

/*
_______________________________________________________________________________________________________________________

                                Sortie de la fenêtre de la liste des dossiers et playlist

                                Recharge l'écran pour l'affichage du controleur
_______________________________________________________________________________________________________________________

*/

                                ecran = SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32,SDL_HWSURFACE|SDL_NOFRAME|SDL_DOUBLEBUF);

                                if(ecran == NULL)
                                {
                                    fprintf(stderr,"Erreur de chargement video: %s\n",SDL_GetError());
                                    exit(EXIT_FAILURE);
                                }
                            }
                            else if(pleinEcran)
                            {
                                click = false;
/*
______________________________________________________________________________________________________________________

                                Affichage de la fenêtre de la liste des dossiers et des playlist

                                        jusqu'à la fin de la boucle while. (!listeAudio)
______________________________________________________________________________________________________________________

*/
                                ecran = SDL_SetVideoMode(1024,760,32,SDL_HWSURFACE|SDL_NOFRAME|SDL_DOUBLEBUF|SDL_FULLSCREEN);

                                if(ecran == NULL)
                                {
                                    pleinEcran2 = true;
                                    ecran = SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32,SDL_HWSURFACE|SDL_NOFRAME|SDL_DOUBLEBUF|SDL_FULLSCREEN);
                                    if(ecran == NULL)
                                    {
                                        pleinEcran = false;
                                        continue;
                                    }
                                }

                                SDL_ShowCursor(SDL_DISABLE);

                                while(pleinEcran)
                                {
                                    SDL_PollEvent(&event);
                                    switch(event.type)
                                    {
                                        case SDL_KEYUP:
                                            click = false;
                                            break;
                                        case SDL_KEYDOWN:
                                            switch(event.key.keysym.sym)
                                            {
                                                case SDLK_ESCAPE:
                                                    pleinEcran2 = false;
                                                    pleinEcran = false;
                                                    continue;
                                                case SDLK_SPACE: //play ou pause
                                                    if(!click)
                                                    {
                                                        if(play)
                                                        {
                                                            FMOD_Channel_SetPaused(canal,0);
                                                            play = false;
                                                        }
                                                        else
                                                        {
                                                            FMOD_Channel_SetPaused(canal,1);
                                                            play = true;
                                                        }
                                                        click = true;
                                                    }
                                                    break;
                                                case SDLK_UP: //Augmentation de la volume de 1%
                                                    if(volume < 1.0)
                                                    {
                                                        volume += 0.01;
                                                        FMOD_Channel_SetVolume(canal,volume);
                                                    }
                                                    break;
                                                case SDLK_DOWN: //Baissé de la volume de 1%
                                                    if(volume > 0.01)
                                                    {
                                                        volume -= 0.01;
                                                        FMOD_Channel_SetVolume(canal,volume);
                                                    }
                                                    break;
                                                default:
                                                    break;
                                            }
                                            break;
                                        default:
                                            break;
                                    }

                                    tempsActuel = SDL_GetTicks();
                                    if (tempsActuel - tempsPrecedent < 25)
                                    {
                                        SDL_Delay(25 - (tempsActuel - tempsPrecedent));
                                    }
                                    tempsPrecedent = SDL_GetTicks();

                                    if((resultat = FMOD_Channel_GetPosition(canal,&positionMusique,FMOD_TIMEUNIT_MS)) != FMOD_OK)
                                    {
                                        fprintf(stderr,"Erreur de chargement de la position actuelle de la musique: %d\n",resultat);
                                        exit(EXIT_FAILURE);
                                    }

                                    //Si la position actuelle de la musique atteint la longueur de la musique ou surpasse en cas d'erreur
                                    //On joue la musique valide suivante sinon on stop et on pointe vers la première musique
                                    if(positionMusique>=longueurMusique[musiqueJoueeActuelle])
                                    {
                                        surfaceMusique[musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[musiqueJoueeActuelle],couleurBlanc,couleurNoire);
                                        musiqueJoueeActuelle++;
                                        while(musiqueJoueeActuelle < PLAYLIST_MAX && !(longueurMusique[musiqueJoueeActuelle] > 0))
                                        {
                                            musiqueJoueeActuelle++;
                                        }

                                        if(musiqueJoueeActuelle < PLAYLIST_MAX && musique[musiqueJoueeActuelle] != NULL && longueurMusique[musiqueJoueeActuelle] > 0)
                                        {
                                            MusicPlay(systeme,&canal,musique,&dsp,&spectre,musiqueJoueeActuelle,volume);
                                            surfaceMusique[musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[musiqueJoueeActuelle],couleurBleu,couleurNoire);
                                        }
                                        else
                                        {
                                            stop = true;
                                            pleinEcran2 = false;
                                            pleinEcran = false;
                                            musiqueJoueeActuelle = 0;
                                            continue;
                                        }
                                    }

                                    SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,0,0,0));

                                    SDL_LockSurface(ecran);

                                    //Tracé du spectre
                                    k = 0;
                                    if(!pleinEcran2)
                                    {
                                        for (i = 0; i < spectre->length; i++)
                                        {
                                            barreSpectrum = spectre->spectrum[0][i] * 15000 * volume;
                                            if(barreSpectrum>760) barreSpectrum = 760;
                                            for(j = 760 - barreSpectrum; j < 760; j++)
                                            {
                                                tracerParPixel(ecran,k,j,SDL_MapRGB(ecran->format,255 - j/(760/255.0),0,j/(760/255.0)));
                                            }
                                            k += 2;
                                        }
                                    }
                                    else
                                    {
                                        for (i = 0; i < spectre->length; i++)
                                        {
                                            barreSpectrum = spectre->spectrum[0][i] * 15000 * volume;
                                            if(barreSpectrum>HAUTEUR_FENETRE) barreSpectrum = HAUTEUR_FENETRE;
                                            for(j = HAUTEUR_FENETRE - barreSpectrum; j < HAUTEUR_FENETRE; j++)
                                            {
                                                tracerParPixel(ecran,i,j,SDL_MapRGB(ecran->format,255 - j/(HAUTEUR_FENETRE/255.0),0,j/(HAUTEUR_FENETRE/255.0)));
                                            }

                                        }
                                    }

                                    SDL_UnlockSurface(ecran);

                                    SDL_Flip(ecran);

                                }
/*
_______________________________________________________________________________________________________________________

                                Sortie du mode plein écran

                                Recharge l'écran pour l'affichage du controleur
_______________________________________________________________________________________________________________________

*/

                                ecran = SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32,SDL_HWSURFACE|SDL_NOFRAME|SDL_DOUBLEBUF);

                                if(ecran == NULL)
                                {
                                    fprintf(stderr,"Erreur de chargement video: %s\n",SDL_GetError());
                                    exit(EXIT_FAILURE);
                                }

                                SDL_ShowCursor(SDL_ENABLE);

                            }
                            else if(musique[musiqueJoueeActuelle] != NULL && play && position.y == HAUTEUR_FENETRE-60 && position.x == LARGEUR_FENETRE-reperage_x-165)
                            {
                                //On pause
                                SDL_BlitSurface(boutton_focus[PAUSE_FOCUS],NULL,ecran,&position);
                                FMOD_Channel_SetPaused(canal,1);
                            }
                            else if(musique[musiqueJoueeActuelle] != NULL && !play && position.y == HAUTEUR_FENETRE-60 && position.x == LARGEUR_FENETRE-reperage_x-165)
                            {
                                //On play si pause ou stoppé
                                SDL_BlitSurface(boutton_focus[PLAY_FOCUS],NULL,ecran,&position);
                                if(!stop)
                                {
                                    FMOD_Channel_SetPaused(canal,0);
                                }
                                else
                                {
                                    stop = false;
                                    MusicPlay(systeme,&canal,musique,&dsp,&spectre,musiqueJoueeActuelle,volume);
                                    surfaceMusique[musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[musiqueJoueeActuelle],couleurBleu,couleurNoire);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    click = false;
                }
            case SDL_MOUSEMOTION:
                switch(testfocus.focusRetour)
                {
                    case REDUIRE_FOCUS:
                        SDL_BlitSurface(boutton_focus[REDUIRE_FOCUS],NULL,ecran,&position);
                        break;
                    case AGRANDIR_FOCUS:
                        SDL_BlitSurface(boutton_focus[AGRANDIR_FOCUS],NULL,ecran,&position);
                        break;
                    case QUITTER_FOCUS:
                        SDL_BlitSurface(boutton_focus[QUITTER_FOCUS],NULL,ecran,&position);
                        break;
                    case LISTE_FOCUS:
                        SDL_BlitSurface(boutton_focus[LISTE_FOCUS],NULL,ecran,&position);
                        break;
                    case STOP_FOCUS:
                        SDL_BlitSurface(boutton_focus[STOP_FOCUS],NULL,ecran,&position);
                        break;
                    case PRECEDENT_FOCUS:
                        SDL_BlitSurface(boutton_focus[PRECEDENT_FOCUS],NULL,ecran,&position);
                        break;
                    case PLAY_FOCUS:
                        SDL_BlitSurface(boutton_focus[PLAY_FOCUS],NULL,ecran,&position);
                        break;
                    case PAUSE_FOCUS:
                        SDL_BlitSurface(boutton_focus[PAUSE_FOCUS],NULL,ecran,&position);
                        break;
                    case SUIVANT_FOCUS:
                        SDL_BlitSurface(boutton_focus[SUIVANT_FOCUS],NULL,ecran,&position);
                        break;
                    case BAISSEVOLUME_FOCUS:
                        SDL_BlitSurface(boutton_focus[BAISSEVOLUME_FOCUS],NULL,ecran,&position);
                        break;
                    case AUGMENTEVOLUME_FOCUS:
                        SDL_BlitSurface(boutton_focus[AUGMENTEVOLUME_FOCUS],NULL,ecran,&position);
                        break;
                    case SANS_FOCUS:
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }

        tempsActuel = SDL_GetTicks();
        if (tempsActuel - tempsPrecedent < 25)
        {
            SDL_Delay(25 - (tempsActuel - tempsPrecedent));
        }
        tempsPrecedent = SDL_GetTicks();

        //Si il y a une musique jouée, on affiche la position actuelle de la musique et on affiche les spectres
        if(!stop)
        {
            if((resultat = FMOD_Channel_GetPosition(canal,&positionMusique,FMOD_TIMEUNIT_MS)) != FMOD_OK)
            {
                fprintf(stderr,"Erreur de chargement de la position actuelle de la musique: %d\n",resultat);
                exit(EXIT_FAILURE);
            }

            //Si la position actuelle de la musique atteint la longueur de la musique ou surpasse en cas d'erreur
            //On joue la musique valide suivante sinon on stop et on pointe vers la première musique
            if(positionMusique>=longueurMusique[musiqueJoueeActuelle])
            {
                surfaceMusique[musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[musiqueJoueeActuelle],couleurBlanc,couleurNoire);
                musiqueJoueeActuelle++;
                while(musiqueJoueeActuelle < PLAYLIST_MAX && !(longueurMusique[musiqueJoueeActuelle] > 0))
                {
                    musiqueJoueeActuelle++;
                }

                if(musiqueJoueeActuelle < PLAYLIST_MAX && musique[musiqueJoueeActuelle] != NULL && longueurMusique[musiqueJoueeActuelle] > 0)
                {
                    MusicPlay(systeme,&canal,musique,&dsp,&spectre,musiqueJoueeActuelle,volume);
                    surfaceMusique[musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[musiqueJoueeActuelle],couleurBleu,couleurNoire);
                }
                else
                {
                    stop = true;
                    musiqueJoueeActuelle = 0;
                }
            }

            SDL_LockSurface(ecran);

            //Barre blanche évolutive de la position actuelle de la musique
            for(j = 5; j < (positionMusique*512/longueurMusique[musiqueJoueeActuelle])+5; j++)
                tracerParPixel(ecran,j,POSITION_Y_POSITION,SDL_MapRGB(ecran->format,255,255,255));

            //Barre gris rétrécie du reste de la longueur de la musique par différence de la position actuelle
            for(j = (positionMusique*512/longueurMusique[musiqueJoueeActuelle])+5; j < 517; j++)
                tracerParPixel(ecran,j,POSITION_Y_POSITION,SDL_MapRGB(ecran->format,100,100,100));

            //Tracé du spectre
            for (i = 0; i < spectre->length; i++)
            {
                barreSpectrum = spectre->spectrum[0][i] * 8000 * volume;
                if(barreSpectrum>300) barreSpectrum = 300;
                for(j = 330 - barreSpectrum; j < 330; j++)
                {
                    tracerParPixel(ecran,i+5,j,SDL_MapRGB(ecran->format,255 - j/(330/255.0),0,j/(330/255.0)));
                }
            }

            SDL_UnlockSurface(ecran);
        }

        SDL_Flip(ecran);
    }

    //On libère les musiques chargés et quitte FMOD
    for(i = 0; i < PLAYLIST_MAX && musique[i] != NULL; i++)
        FMOD_Sound_Release(musique[i]);

    FMOD_System_Release(systeme);

    //On libère les fonts chargés et quitte TTF
    TTF_CloseFont(gannister);
    TTF_CloseFont(razorface_regular);
    TTF_CloseFont(arial);
    TTF_CloseFont(revamped);
    TTF_Quit();

    //On libère les surfaces chargés et quitte SDL
    for(i = 0; i < _NOMBRE_BOUTTON_FOCUS_ENUM_; i++)
    {
        SDL_FreeSurface(boutton_focus[i]);
    }
    for(i = 0; i < _NOMBRE_ELEMENT_ENUM_; i++)
    {
        SDL_FreeSurface(element[i]);
    }
    for(i = 0; i < PLAYLIST_MAX && surfaceMusique[i] != NULL; i++)
    {
        SDL_FreeSurface(surfaceMusique[i]);
    }
    for(i = 0; i < nombreFichierLu; i++)
    {
        SDL_FreeSurface(liste_dossier_ou_fichier[i]);
    }

    SDL_FreeSurface(iconDossier);
    SDL_FreeSurface(iconPrecedent);
    SDL_FreeSurface(iconMusique);
    SDL_FreeSurface(iconPlay);
    SDL_FreeSurface(dossierActuel);
    SDL_FreeSurface(barreVerticale);
    SDL_FreeSurface(titre1);
    SDL_FreeSurface(titre2);

    SDL_Quit();

    return EXIT_SUCCESS;
}
