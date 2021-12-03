/*
_____________________________________________________________________________________________

                                    joueurMusique.c
_____________________________________________________________________________________________

****************        ******************************************************        *******

                ******* Par Hidekela Fanaperantsoa, facebook.com/404.dont.find *******

****************        ******************************************************        *******




---------------------------------------------------------------------------------------------

            Rôle: Fonction qui joue la musique entre les deux modes video controleur

                                    et liste de musique

---------------------------------------------------------------------------------------------

                        Date de création: 25/04/2020 à 21:49
                        Date de dernière modification: 25/04/2020 à 21:49

                                ~~~~~~~~~~~~~~~~~~~~~

*/
#include <stdio.h>
#include <stdlib.h>
#include <FMOD/fmod.h>
#include "joueurMusique.h"

void MusicPlay(FMOD_SYSTEM *systeme,FMOD_CHANNEL **canal,FMOD_SOUND *musique[PLAYLIST_MAX],FMOD_DSP **dsp,FMOD_DSP_PARAMETER_FFT **spectre,int musiqueJoueeActuelle,float volume)
{
    FMOD_RESULT resultat;
    if((resultat = FMOD_System_PlaySound(systeme,musique[musiqueJoueeActuelle],NULL,0,canal)) != FMOD_OK)
    {
        fprintf(stderr,"Erreur de lecture de la musique: %d\n",resultat);
        exit(EXIT_FAILURE);
    }
    if((resultat = FMOD_Channel_SetVolume(*canal,volume)) != FMOD_OK)
    {
        fprintf(stderr,"Erreur de volume de la musique: %d\n",resultat);
        exit(EXIT_FAILURE);
    }
    FMOD_Channel_GetDSP(*canal,0,dsp);
    FMOD_System_CreateDSPByType(systeme,FMOD_DSP_TYPE_FFT,dsp);
    FMOD_Channel_AddDSP(*canal,1,*dsp);
    if((resultat = FMOD_DSP_GetParameterData(*dsp,FMOD_DSP_FFT_SPECTRUMDATA,(void **)spectre,0,NULL,0)) != FMOD_OK)
    {
        fprintf(stderr,"Erreur de dsp %d\n",resultat);
        exit(EXIT_FAILURE);
    }
    FMOD_DSP_SetParameterInt(*dsp,FMOD_DSP_FFT_WINDOWSIZE,1024);
    return;
}
