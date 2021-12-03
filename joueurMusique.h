/*
_____________________________________________________________________________________________

                                    joueurMusique.h
_____________________________________________________________________________________________

****************        ******************************************************        *******

                ******* Par Hidekela Fanaperantsoa, facebook.com/404.dont.find *******

****************        ******************************************************        *******




---------------------------------------------------------------------------------------------

            R�le: Prototype de la fonction qui joue la musique entre les deux modes

                            video controleur et liste de musique

---------------------------------------------------------------------------------------------

                        Date de cr�ation: 25/04/2020 � 21:49
                        Date de derni�re modification: 25/04/2020 � 21:49

                                ~~~~~~~~~~~~~~~~~~~~~

*/
#ifndef JOUEURMUSIQUE_H_INCLUDED
#define JOUEURMUSIQUE_H_INCLUDED

#define PLAYLIST_MAX 100
typedef enum {false,true} bool;

void MusicPlay(FMOD_SYSTEM *systeme,FMOD_CHANNEL **canal,FMOD_SOUND *musique[PLAYLIST_MAX],FMOD_DSP **dsp,FMOD_DSP_PARAMETER_FFT **spectre,int musiqueJoueeActuelle,float volume);

#endif // JOUEURMUSIQUE_H_INCLUDED
