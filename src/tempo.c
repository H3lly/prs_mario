#define _XOPEN_SOURCE 700 // à rajouter sinon error: 'SIG_BLOCK' undeclared...
#include <SDL.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#include "timer.h"

// Return number of elapsed µsec since... a long time ago
static unsigned long get_time (void)
{
  struct timeval tv;

  gettimeofday (&tv ,NULL);

  // Only count seconds since beginning of 2016 (not jan 1st, 1970)
  tv.tv_sec -= 3600UL * 24 * 365 * 46;
  
  return tv.tv_sec * 1000000UL + tv.tv_usec;
}

#ifdef PADAWAN

void *param_event;

void handler(int sig){
  printf("L'identité du thread courant est : %d\n", pthread_self());
  //printf("sdl_push_event(%p) appelée au temps %ld\n", param_event, get_time ());
}

// daemon va attendre les signaux SIGALRM (signaux envoyés à un processus lorsqu'une limite de temps s'est écoulée) et gérer les évènements
void *daemon(void *arg){
  struct itimerval timer;
  // configure le timer pour expirer après 250msec...
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 250000;
  // ... et toutes les 250 msec
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 250000;
  // enclenche le timer
  setitimer(ITIMER_REAL, &timer, NULL);

  sigset_t mask;  // masque de blocage de signaux 
  sigfillset(&mask);  // ajoute tous les signaux possibles au masque
  sigdelset(&mask, SIGALRM); // retire le signal SIGALRM du masque

  struct sigaction sa; // déclaration d'une structure pour la mise en place du gestionnaire
  sa.sa_handler = handler; // adresse de la fonction gestionnaire
  sa.sa_flags = 0; // mise à 0 du champ sa_flags théoriquement ignoré
  sigemptyset(&sa.sa_mask); // on vide le masque, on ne bloque pas de signaux spécifiques
  
  sigaction(SIGALRM, &sa, NULL); // mise en place du gestionnaire pour le signal SIGALRM

  while(1){
    sigsuspend(&mask); // remplace temporairement le masque de signaux du processus appelant avec le masque fourni et suspend le processus jusqu'à livraison d'un signal SIGALRM
  }
  
}

// timer_init returns 1 if timers are fully implemented, 0 otherwise
int timer_init (void)
{

  sigset_t mask;  // masque de blocage de signaux 
  sigemptyset(&mask); // création d'un masque vide
  sigaddset(&mask, SIGALRM);  //on ajoute le signal SIGALRM au masque
  pthread_sigmask(SIG_BLOCK, &mask, NULL);  // les autres threads crées par timer_init hériteront d'une copie du masque de blocage de signaux

  pthread_t thread; // thread

  if(pthread_create(&thread, NULL, daemon, NULL) == -1){ //création du thread qui exécute la fonction daemon
    perror("pthread_create");
    return EXIT_FAILURE;
  }
  //pthread_join(thread, NULL); // attend la fin du thread

  return 0; // Implementation not ready
}

void timer_set (Uint32 delay, void *param)
{
  /*
  // sauvegarde de param
  param_event = param;

  struct itimerval timer;
  // configure le timer pour expirer après delay msec...
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = delay;
  // ... et seulement 1 fois
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 0;
  // enclenche le timer
  setitimer(ITIMER_REAL, &timer, NULL);
  */
}

#endif