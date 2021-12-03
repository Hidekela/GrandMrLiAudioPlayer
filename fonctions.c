/*
_____________________________________________________________________________________________

                                    fonctions.c
_____________________________________________________________________________________________

****************        ******************************************************        *******

                ******* Par Hidekela Fanaperantsoa, facebook.com/404.dont.find *******

****************        ******************************************************        *******




---------------------------------------------------------------------------------------------

            Rôle: Quelques fonctions utiles

---------------------------------------------------------------------------------------------

                        Date de création: 16/11/2021 à 17:00
                        Date de dernière modification: 16/11/2021 à 17:00

                                ~~~~~~~~~~~~~~~~~~~~~

*/
#include "fonctions.h"

char* strtolower(char *chaine)
{
    unsigned int i;
    for(i=0;i<strlen(chaine);i++)
    {
        chaine[i] = tolower(chaine[i]);
    }
    return chaine;
}

void getNameParentDir(char dirname[])
{
    int i;

    for(i = strlen(dirname)-1; dirname[i] != '\\'; i--);
    dirname[i] = '\0';
    for(i = strlen(dirname)-1; dirname[i] != '\\'; i--);
    dirname[i+1] = '\0';
}
void UTF8(char chaine[])
{
    wchar_t wbuff[1024]; //Pour encoder la chaine en utf-8 pour fmod_createstream
    MultiByteToWideChar(CP_ACP,0,chaine,-1,wbuff,1024);
    WideCharToMultiByte(CP_UTF8,0,wbuff,-1,chaine,1024,NULL,NULL);
    return;
}
void ReblitScrolle(Uint16 x,int musiqueJoueeActuelle,int* Pdebut_blit_surfaceMusique,int debut_blit_dir_file,int nombreFichierLu,TYPE_FICHIER typeFichier[],SDL_Surface* ecran,SDL_Surface* iconDossier,SDL_Surface* iconPrecedent,SDL_Surface* iconMusique,SDL_Surface* iconPlay,SDL_Surface** surfaceMusique,SDL_Surface** liste_dossier_ou_fichier)
{
    int i;
    SDL_Rect pos, BlitMusiqueUnderMax;

    if(x>274)
    {
        pos.h = HAUTEUR_FENETRE_LISTE - POSITION_Y_DOSSIER;
        pos.w = LARGEUR_FENETRE_LISTE - POSITION_X_DOSSIER;
        pos.y = POSITION_Y_DOSSIER;
        pos.x = POSITION_X_DOSSIER;
        SDL_FillRect(ecran,&pos,SDL_MapRGB(ecran->format,0,0,0));

        for(i = debut_blit_dir_file; i < (BLIT_DIR_FILE_MAX+debut_blit_dir_file) && i < nombreFichierLu; i++)
        {
            typeFichier[i] == DOSSIER? SDL_BlitSurface(iconDossier,NULL,ecran,&pos) : (typeFichier[i] == DOSSIER_PARENT?SDL_BlitSurface(iconPrecedent,NULL,ecran,&pos) : SDL_BlitSurface(iconMusique,NULL,ecran,&pos));
            pos.x += 18;
            if(typeFichier[i] != DOSSIER_PARENT) pos.y += 3;
            SDL_BlitSurface(liste_dossier_ou_fichier[i],NULL,ecran,&pos);
            pos.y += liste_dossier_ou_fichier[i]->h + 5;
            pos.x -= 18;
        }
    }
    else
    {
        pos.h = HAUTEUR_FENETRE_LISTE - POSITION_Y_DOSSIER;
        pos.w = 258;
        pos.y = POSITION_Y_MUSIQUE;
        pos.x = POSITION_X_MUSIQUE;
        BlitMusiqueUnderMax.x = 0;
        BlitMusiqueUnderMax.y = 0;
        SDL_FillRect(ecran,&pos,SDL_MapRGB(ecran->format,0,0,0));

        for(i = *Pdebut_blit_surfaceMusique; surfaceMusique[i] != NULL && i < PLAYLIST_MAX && i < (BLIT_MUSIQUE_MAX+(*Pdebut_blit_surfaceMusique)); i++)
        {
            i != musiqueJoueeActuelle? SDL_BlitSurface(iconMusique,NULL,ecran,&pos) : SDL_BlitSurface(iconPlay,NULL,ecran,&pos);
            pos.x += 18;
            pos.y += 3;
            if(surfaceMusique[i]->w > WIDTH_MAX_SURFACE_MUSIQUE)
            {
                BlitMusiqueUnderMax.w = WIDTH_MAX_SURFACE_MUSIQUE;
            }

            SDL_BlitSurface(surfaceMusique[i],&BlitMusiqueUnderMax,ecran,&pos);
            pos.y += surfaceMusique[i]->h + 5;
            pos.x -= 18;
        }
    }

    SDL_Flip(ecran);
}

void doubleclik_event(bool *stop,FMOD_SYSTEM *systeme,FMOD_CHANNEL **canal,FMOD_SOUND *musique[PLAYLIST_MAX],FMOD_DSP **dsp,FMOD_DSP_PARAMETER_FFT **spectre,float volume,unsigned int longueurMusique[PLAYLIST_MAX],SDL_Surface* ecran,int* musiqueJoueeActuelle,SDL_Surface** dossierActuel,char format[19][6],char* dossierOuvert,TTF_Font* razorface_regular,TTF_Font* gannister,TTF_Font* arial,SDL_Color couleurVert,SDL_Color couleurBleu,SDL_Color couleurBlanc,SDL_Color couleurNoire,Uint16 x,Uint16 y,TYPE_FICHIER* typeFichier,SDL_Surface** liste_dossier_ou_fichier,SDL_Surface* surfaceMusique[PLAYLIST_MAX],SDL_Surface* iconPrecedent,SDL_Surface* iconDossier,SDL_Surface* iconMusique,SDL_Surface* iconPlay,int* POINTEUR_debut_blit_dir_file,int* POINTEUR_debut_blit_surfaceMusique,int* PnombreFichierLu,int* PnombreMusique,char liste_nom[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX],char liste_path_surfaceMusique[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX],char liste_basename_surfaceMusique[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX])
{
    TYPE_FICHIER type;
    int position, i = 0, j = 0;
    static int positionRecent = -1;
    char *stringTemp = NULL, *TestFormat = NULL;
    DIR *dossier = NULL, *TestDossier = NULL;
    dirent* fichier_ou_dossier = NULL;
    SDL_Rect pos, BlitMusiqueUnderMax;
    FMOD_RESULT resultat;

    if(x>274)
    {
        y -= POSITION_Y_DOSSIER - 3;
        position = y/HAUTEUR_DOSSIER_FICHIER;
        if(typeFichier[(*POINTEUR_debut_blit_dir_file)+position] == DOSSIER && strlen(dossierOuvert)+strlen(liste_nom[(*POINTEUR_debut_blit_dir_file)+position]) < 97 && strcmp(liste_nom[(*POINTEUR_debut_blit_dir_file)+position],"") != 0)
        {
            strcat(dossierOuvert,liste_nom[(*POINTEUR_debut_blit_dir_file)+position]);
            strcat(dossierOuvert,"\\");
            type = DOSSIER;
        }
        else if(typeFichier[(*POINTEUR_debut_blit_dir_file)+position] == DOSSIER_PARENT)
        {
            getNameParentDir(dossierOuvert);
            type = DOSSIER_PARENT;
        }
        else if(typeFichier[(*POINTEUR_debut_blit_dir_file)+position] == MUSIQUE && *PnombreMusique < PLAYLIST_MAX)
        {
            type = MUSIQUE;
        }
        else
            return;

        if(type == DOSSIER_PARENT || type == DOSSIER)
        {
            for(i = 0; i < *PnombreFichierLu; i++)
                strcpy(liste_nom[i],"");

            i = 0;

            dossier = opendir(dossierOuvert);
            while((fichier_ou_dossier = readdir(dossier)) != NULL && i < NOMBRE_DOSSIER_MAX)
            {
                stringTemp = (char*) malloc(sizeof(dossierOuvert)+sizeof(fichier_ou_dossier->d_name));
                sprintf(stringTemp,"%s%s",dossierOuvert,fichier_ou_dossier->d_name);
                if((TestDossier = opendir(stringTemp)) != NULL)
                {
                    closedir(TestDossier);
                    if(fichier_ou_dossier->d_namlen == 1 && strcmp(fichier_ou_dossier->d_name,".") == 0);
                    else if(fichier_ou_dossier->d_namlen == 2 && strcmp(fichier_ou_dossier->d_name,"..") == 0)
                    {
                        typeFichier[i] = DOSSIER_PARENT;
                        strcpy(liste_nom[i],fichier_ou_dossier->d_name);
                        liste_dossier_ou_fichier[i] = TTF_RenderText_Shaded(gannister," DOSSIER_PARENT",couleurBlanc,couleurNoire);
                        i++;
                    }
                    else
                    {
                        typeFichier[i] = DOSSIER;
                        strcpy(liste_nom[i],fichier_ou_dossier->d_name);
                        liste_dossier_ou_fichier[i] = TTF_RenderText_Shaded(arial,liste_nom[i],couleurBlanc,couleurNoire);
                        i++;
                    }
                }
                else if((TestFormat = strrchr(fichier_ou_dossier->d_name,'.')) != NULL)
                {
                    for(j = 0; j < 19 && strcmp(format[j],strtolower(TestFormat)) != 0; j++);
                    if(j==19) continue;
                    typeFichier[i] = MUSIQUE;
                    strcpy(liste_nom[i],fichier_ou_dossier->d_name);
                    liste_dossier_ou_fichier[i] = TTF_RenderText_Shaded(arial,liste_nom[i],couleurBlanc,couleurNoire);
                    i++;
                }
                free(stringTemp);
            }
            closedir(dossier);
            *PnombreFichierLu = i;

            *dossierActuel = TTF_RenderText_Shaded(razorface_regular,dossierOuvert,couleurVert,couleurNoire);

            pos.h = (*dossierActuel)->h;
            pos.w = LARGEUR_FENETRE_LISTE;
            pos.y = POSITION_Y_dossierActuel;
            pos.x = 0;
            SDL_FillRect(ecran,&pos,SDL_MapRGB(ecran->format,0,0,0));

            pos.x = (ecran->w - (*dossierActuel)->w)/2;
            if(pos.x < 0) pos.x = 0;
            SDL_BlitSurface(*dossierActuel,NULL,ecran,&pos);

            pos.h = HAUTEUR_FENETRE_LISTE - POSITION_Y_DOSSIER;
            pos.w = LARGEUR_FENETRE_LISTE - POSITION_X_DOSSIER;
            pos.y = POSITION_Y_DOSSIER;
            pos.x = POSITION_X_DOSSIER;
            *POINTEUR_debut_blit_dir_file = 0;

            SDL_FillRect(ecran,&pos,SDL_MapRGB(ecran->format,0,0,0));

            for(i = *POINTEUR_debut_blit_dir_file; i < (BLIT_DIR_FILE_MAX+(*POINTEUR_debut_blit_dir_file)) && i < *PnombreFichierLu; i++)
            {
                typeFichier[i] == DOSSIER? SDL_BlitSurface(iconDossier,NULL,ecran,&pos) : (typeFichier[i] == DOSSIER_PARENT?SDL_BlitSurface(iconPrecedent,NULL,ecran,&pos) : SDL_BlitSurface(iconMusique,NULL,ecran,&pos));
                pos.x += 18;
                if(typeFichier[i] != DOSSIER_PARENT) pos.y += 3;
                SDL_BlitSurface(liste_dossier_ou_fichier[i],NULL,ecran,&pos);
                pos.y += liste_dossier_ou_fichier[i]->h + 5;
                pos.x -= 18;
            }

            SDL_Flip(ecran);

        }
        else if(type == MUSIQUE)
        {
            strcpy(liste_path_surfaceMusique[*PnombreMusique],dossierOuvert);
            strcpy(liste_basename_surfaceMusique[*PnombreMusique],liste_nom[(*POINTEUR_debut_blit_dir_file)+position]);

            stringTemp = (char *) malloc(sizeof(liste_path_surfaceMusique[*PnombreMusique])+sizeof(liste_basename_surfaceMusique[*PnombreMusique]));
            sprintf(stringTemp,"%s%s",liste_path_surfaceMusique[*PnombreMusique],liste_basename_surfaceMusique[*PnombreMusique]);

            UTF8(stringTemp);

            if((resultat = FMOD_System_CreateStream(systeme,stringTemp,FMOD_ACCURATETIME,NULL,&musique[*PnombreMusique])) != FMOD_OK)
            {
                fprintf(stderr,"Erreur de chargement de la musique %s: %d\n",stringTemp,resultat);
                surfaceMusique[*PnombreMusique] = TTF_RenderText_Shaded(arial,"[*Musique Invalide*]",couleurBlanc,couleurNoire);
            }
            free(stringTemp);

            if((resultat = FMOD_Sound_GetLength(musique[*PnombreMusique],&longueurMusique[*PnombreMusique],FMOD_TIMEUNIT_MS)) != FMOD_OK)
            {
                fprintf(stderr,"Erreur de chargement de la longueur de la musique: %d\n",resultat);
                longueurMusique[*PnombreMusique] = 0;
                if(surfaceMusique[*PnombreMusique] != NULL) surfaceMusique[*PnombreMusique] = TTF_RenderText_Shaded(arial,"[*Musique Invalide*]",couleurBlanc,couleurNoire);
            }
            else
            {
                surfaceMusique[*PnombreMusique] = TTF_RenderText_Shaded(arial,liste_nom[(*POINTEUR_debut_blit_dir_file)+position],couleurBlanc,couleurNoire);
                (*PnombreMusique)++;
            }


            //Reblit

            pos.h = HAUTEUR_FENETRE_LISTE - POSITION_Y_DOSSIER;
            pos.w = 258;
            pos.y = POSITION_Y_MUSIQUE;
            pos.x = POSITION_X_MUSIQUE;
            BlitMusiqueUnderMax.x = 0;
            BlitMusiqueUnderMax.y = 0;
            SDL_FillRect(ecran,&pos,SDL_MapRGB(ecran->format,0,0,0));

            for(i = *POINTEUR_debut_blit_surfaceMusique; surfaceMusique[i] != NULL && i < PLAYLIST_MAX && i < (BLIT_MUSIQUE_MAX+(*POINTEUR_debut_blit_surfaceMusique)); i++)
            {
                i != *musiqueJoueeActuelle? SDL_BlitSurface(iconMusique,NULL,ecran,&pos) : SDL_BlitSurface(iconPlay,NULL,ecran,&pos);
                pos.x += 18;
                pos.y += 3;
                if(surfaceMusique[i]->w > WIDTH_MAX_SURFACE_MUSIQUE)
                {
                    BlitMusiqueUnderMax.w = WIDTH_MAX_SURFACE_MUSIQUE;
                }

                SDL_BlitSurface(surfaceMusique[i],&BlitMusiqueUnderMax,ecran,&pos);
                pos.y += surfaceMusique[i]->h + 5;
                pos.x -= 18;
            }

            SDL_Flip(ecran);

        }
    }
    else// if(y>POSITION_Y_MUSIQUE && y<POSITION_Y_MUSIQUE+(((*POINTEUR_debut_blit_surfaceMusique)-BLIT_MUSIQUE_MAX)*HAUTEUR_DOSSIER_FICHIER))
    {
        y -= POSITION_Y_MUSIQUE;
        position = y/HAUTEUR_DOSSIER_FICHIER;

        if(surfaceMusique[(*POINTEUR_debut_blit_surfaceMusique)+position] == NULL) return;
        *musiqueJoueeActuelle = (*POINTEUR_debut_blit_surfaceMusique)+position;
        surfaceMusique[0] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[0],couleurBlanc,couleurNoire);
        surfaceMusique[*musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[*musiqueJoueeActuelle],couleurBleu,couleurNoire);

        if(*musiqueJoueeActuelle < PLAYLIST_MAX && musique[*musiqueJoueeActuelle] != NULL && longueurMusique[*musiqueJoueeActuelle] > 0)
        {
            *stop = false;
            MusicPlay(systeme,canal,musique,dsp,spectre,*musiqueJoueeActuelle,volume);
        }


        if(positionRecent >= 0 && positionRecent != (*POINTEUR_debut_blit_surfaceMusique)+position) surfaceMusique[positionRecent] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[positionRecent],couleurBlanc,couleurNoire);

        positionRecent = (*POINTEUR_debut_blit_surfaceMusique)+position;

        pos.h = HAUTEUR_FENETRE_LISTE - POSITION_Y_DOSSIER;
        pos.w = 258;
        pos.y = POSITION_Y_MUSIQUE;
        pos.x = POSITION_X_MUSIQUE;
        BlitMusiqueUnderMax.x = 0;
        BlitMusiqueUnderMax.y = 0;
        SDL_FillRect(ecran,&pos,SDL_MapRGB(ecran->format,0,0,0));

        for(i = *POINTEUR_debut_blit_surfaceMusique; surfaceMusique[i] != NULL && i < PLAYLIST_MAX && i < (BLIT_MUSIQUE_MAX+(*POINTEUR_debut_blit_surfaceMusique)); i++)
        {
            i != *musiqueJoueeActuelle? SDL_BlitSurface(iconMusique,NULL,ecran,&pos) : SDL_BlitSurface(iconPlay,NULL,ecran,&pos);
            pos.x += 18;
            pos.y += 3;
            if(surfaceMusique[i]->w > WIDTH_MAX_SURFACE_MUSIQUE)
            {
                BlitMusiqueUnderMax.w = WIDTH_MAX_SURFACE_MUSIQUE;
            }

            SDL_BlitSurface(surfaceMusique[i],&BlitMusiqueUnderMax,ecran,&pos);
            pos.y += surfaceMusique[i]->h + 5;
            pos.x -= 18;
        }

        SDL_Flip(ecran);
    }
}

void tracerParPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void clickGauche(bool* pleinEcran, bool* listeAudio, int nombreMusique, Uint16 x, Uint16 y,int reperage_x, int* pContinuer,FMOD_SYSTEM *systeme,FMOD_CHANNEL **canal,FMOD_SOUND *musique[PLAYLIST_MAX],FMOD_DSP **dsp,FMOD_DSP_PARAMETER_FFT **spectre,int *musiqueJoueeActuelle,unsigned int longueurMusique[PLAYLIST_MAX], bool* play, bool* stop, float* volume,SDL_Surface* surfaceMusique[PLAYLIST_MAX],char liste_basename_surfaceMusique[NOMBRE_DOSSIER_MAX][NOM_DOSSIER_MAX],SDL_Color couleurBleu,SDL_Color couleurNoire,SDL_Color couleurBlanc,TTF_Font *arial)
{
    if(y>0 && y<25)
    {
        if(x<LARGEUR_FENETRE && x>(LARGEUR_FENETRE-40))
        {
            *pContinuer = 0;
        }
        else if(x<(LARGEUR_FENETRE-40) && x>(LARGEUR_FENETRE-80) && !(*stop))
        {
            *pleinEcran = true;
        }
        else if(x<(LARGEUR_FENETRE-80) && x>(LARGEUR_FENETRE-120))
        {
            SDL_WM_IconifyWindow();
        }
    }
    else if(musique[*musiqueJoueeActuelle] != NULL && y>(HAUTEUR_FENETRE-60) && y<(HAUTEUR_FENETRE-10) && x<(LARGEUR_FENETRE-reperage_x-115) && x>(LARGEUR_FENETRE-reperage_x-165))
    {
        if(*play||*stop)
            *play = 0;
        else
            *play = 1;
    }
    else if(y>(HAUTEUR_FENETRE-48) && y<(HAUTEUR_FENETRE-23))
    {
        if(x<(LARGEUR_FENETRE-reperage_x) && x>(LARGEUR_FENETRE-reperage_x-25))
        {
            if(*volume < 1.0)
            {
                *volume += 0.1;
                FMOD_Channel_SetVolume(*canal,*volume);
            }
        }
        else if(x<(LARGEUR_FENETRE-reperage_x-30) && x>(LARGEUR_FENETRE-reperage_x-55))
        {
            if(*volume > 0.1)
            {
                *volume -= 0.1;
                FMOD_Channel_SetVolume(*canal,*volume);
            }
        }
        else if(musique[*musiqueJoueeActuelle] != NULL && x<(LARGEUR_FENETRE-reperage_x-60) && x>(LARGEUR_FENETRE-reperage_x-110))
        {
            surfaceMusique[*musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[*musiqueJoueeActuelle],couleurBlanc,couleurNoire);
            (*musiqueJoueeActuelle)++;
            if(*musiqueJoueeActuelle < nombreMusique && !(longueurMusique[*musiqueJoueeActuelle] > 0))
            {
                (*musiqueJoueeActuelle)++;
            }
            if(*musiqueJoueeActuelle < nombreMusique && musique[*musiqueJoueeActuelle] != NULL)
            {

                MusicPlay(systeme,canal,musique,dsp,spectre,*musiqueJoueeActuelle,*volume);
                surfaceMusique[*musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[*musiqueJoueeActuelle],couleurBleu,couleurNoire);
                *stop = false;
            }
            else
            {
                (*musiqueJoueeActuelle)--;
            }
        }
        else if(musique[*musiqueJoueeActuelle] != NULL && x<(LARGEUR_FENETRE-reperage_x-170) && x>(LARGEUR_FENETRE-reperage_x-220))
        {
            surfaceMusique[*musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[*musiqueJoueeActuelle],couleurBlanc,couleurNoire);
            (*musiqueJoueeActuelle)--;
            if(*musiqueJoueeActuelle >= 0 && !(longueurMusique[*musiqueJoueeActuelle] > 0))
            {
                (*musiqueJoueeActuelle)--;
            }
            if(*musiqueJoueeActuelle < PLAYLIST_MAX && musique[*musiqueJoueeActuelle] != NULL)
            {

                MusicPlay(systeme,canal,musique,dsp,spectre,*musiqueJoueeActuelle,*volume);
                surfaceMusique[*musiqueJoueeActuelle] = TTF_RenderText_Shaded(arial,liste_basename_surfaceMusique[*musiqueJoueeActuelle],couleurBleu,couleurNoire);
                *stop = false;
            }
            else
            {
                (*musiqueJoueeActuelle)++;
            }
        }
        else if(musique[*musiqueJoueeActuelle] != NULL && x<(LARGEUR_FENETRE-reperage_x-225) && x>(LARGEUR_FENETRE-reperage_x-250) && !(*stop))
        {
            FMOD_Channel_Stop(*canal);
            *stop = true;
            *play = false;
        }
        else if(x<(LARGEUR_FENETRE-reperage_x-255) && x>(reperage_x))
        {
            *listeAudio = true;
        }
    }
    else if(musique[*musiqueJoueeActuelle] != NULL && y > POSITION_Y_POSITION - 5 && y < POSITION_Y_POSITION + 5 && x > 5 && x < 517 && !(*stop))
    {
        FMOD_Channel_SetPosition(*canal,(longueurMusique[*musiqueJoueeActuelle]*x/512)-2000,FMOD_TIMEUNIT_MS);
    }
}

FOCUS focus(Uint16 x,Uint16 y,int reperage_x,bool play,bool stop)
{
    FOCUS valeurRetournee;

    if(y>0 && y<25)
    {
        valeurRetournee.position.y = 0;

        if(x<LARGEUR_FENETRE && x>(LARGEUR_FENETRE-40))
        {
            valeurRetournee.position.x = LARGEUR_FENETRE-40;
            valeurRetournee.focusRetour = QUITTER_FOCUS;
        }
        else if(x<(LARGEUR_FENETRE-40) && x>(LARGEUR_FENETRE-80))
        {
            valeurRetournee.position.x = LARGEUR_FENETRE-80;
            valeurRetournee.focusRetour = AGRANDIR_FOCUS;
        }
        else if(x<(LARGEUR_FENETRE-80) && x>(LARGEUR_FENETRE-120))
        {
            valeurRetournee.position.x = LARGEUR_FENETRE-120;
            valeurRetournee.focusRetour = REDUIRE_FOCUS;
        }
        else
        {
            valeurRetournee.position.x = 0;
            valeurRetournee.focusRetour = SANS_FOCUS;
        }
    }
    else if(y>(HAUTEUR_FENETRE-60) && y<(HAUTEUR_FENETRE-10))
    {
        valeurRetournee.position.y = HAUTEUR_FENETRE-60;

        if(y>(HAUTEUR_FENETRE-48) && y<(HAUTEUR_FENETRE-23))
        {
            valeurRetournee.position.y = HAUTEUR_FENETRE-48;

            if(x<(LARGEUR_FENETRE-reperage_x) && x>(LARGEUR_FENETRE-reperage_x-25))
            {
                valeurRetournee.position.x = LARGEUR_FENETRE-reperage_x-25;
                valeurRetournee.focusRetour = AUGMENTEVOLUME_FOCUS;
            }
            else if(x<(LARGEUR_FENETRE-reperage_x-30) && x>(LARGEUR_FENETRE-reperage_x-55))
            {
                valeurRetournee.position.x = LARGEUR_FENETRE-reperage_x-55;
                valeurRetournee.focusRetour = BAISSEVOLUME_FOCUS;
            }
            else if(x<(LARGEUR_FENETRE-reperage_x-60) && x>(LARGEUR_FENETRE-reperage_x-110))
            {
                valeurRetournee.position.x = LARGEUR_FENETRE-reperage_x-110;
                valeurRetournee.focusRetour = SUIVANT_FOCUS;
            }
            else if(x<(LARGEUR_FENETRE-reperage_x-115) && x>(LARGEUR_FENETRE-reperage_x-165))
            {
                valeurRetournee.position.y = HAUTEUR_FENETRE-60;
                valeurRetournee.position.x = LARGEUR_FENETRE-reperage_x-165;
                if(play||stop)
                    valeurRetournee.focusRetour = PLAY_FOCUS;
                else
                    valeurRetournee.focusRetour = PAUSE_FOCUS;
            }
            else if(x<(LARGEUR_FENETRE-reperage_x-170) && x>(LARGEUR_FENETRE-reperage_x-220))
            {
                valeurRetournee.position.x = LARGEUR_FENETRE-reperage_x-220;
                valeurRetournee.focusRetour = PRECEDENT_FOCUS;
            }
            else if(x<(LARGEUR_FENETRE-reperage_x-225) && x>(LARGEUR_FENETRE-reperage_x-250))
            {
                valeurRetournee.position.x = LARGEUR_FENETRE-reperage_x-250;
                valeurRetournee.focusRetour = STOP_FOCUS;
            }
            else if(x<(LARGEUR_FENETRE-reperage_x-255) && x>(reperage_x))
            {
                valeurRetournee.position.x = reperage_x;
                valeurRetournee.focusRetour = LISTE_FOCUS;
            }
            else
            {
                valeurRetournee.position.x = 0;
                valeurRetournee.focusRetour = SANS_FOCUS;
            }
        }
        else if(x<(LARGEUR_FENETRE-reperage_x-115) && x>(LARGEUR_FENETRE-reperage_x-165))
        {
            valeurRetournee.position.x = LARGEUR_FENETRE-reperage_x-165;
            if(play||stop)
                    valeurRetournee.focusRetour = PLAY_FOCUS;
                else
                    valeurRetournee.focusRetour = PAUSE_FOCUS;
        }
        else
        {
            valeurRetournee.position.x = 0;
            valeurRetournee.focusRetour = SANS_FOCUS;
        }
    }
    else
    {
        valeurRetournee.position.y = 0;
        valeurRetournee.position.x = 0;
        valeurRetournee.focusRetour = SANS_FOCUS;
    }
    return valeurRetournee;
}
