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

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//structure-liste qui va contenir les events
typedef struct linked_list
{
  struct itimerval timer; //temporisateur
  void *param;  // sauvegarde de param
  unsigned long time_signal;   // de 0 à 4294967295, correspond au temps où le signal SIGALRM est généré
  struct linked_list *next; // un pointeur vers le prochain event
} linked_list;

// insère l'élèment event dans la liste chaînée ll en tenant compte de l'ordre chronologique de délivrance du signal SIGALRM (time_signal)
void insert(linked_list **ll, linked_list **event)
{
  linked_list *tmp = NULL;
  linked_list *cll = *ll;
  while(cll && cll->time_signal < (*event)->time_signal)
  {
    tmp = cll;
    cll = cll->next;
  }
  (*event)->next = cll;
  if(tmp)
    tmp->next = (*event);
  else
    *ll = (*event);
}

// supprime la tête de la liste chaînée
void pop(linked_list **ll)
{
  linked_list *tmp = (*ll)->next;
  free(*ll);
  *ll = tmp;
}

linked_list *first_event = NULL; // création de la liste chaînée

void handler(int sig)
{
  //printf("sdl_push_event(%p) appelée au temps %ld\n", first_event->param, get_time ());
  unsigned long current;
  unsigned long after;
  unsigned long diff;
  if(first_event->next != NULL){
    current = first_event->time_signal;
    after = first_event->next->time_signal;
    diff = after - current;
  }
  sdl_push_event (first_event->param);
  pop(&first_event);
  if(first_event == NULL)
    return;
  if(first_event != NULL){
    first_event->timer.it_value.tv_sec = diff/1000000;       // secondes
    first_event->timer.it_value.tv_usec = (diff%1000000);    // microsecondes
    first_event->timer.it_interval.tv_sec = 0;
    first_event->timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &(first_event->timer), NULL);
  }

}

// daemon va attendre les signaux SIGALRM (signaux envoyés à un processus lorsqu'une limite de temps s'est écoulée) et gérer les évènements
void *daemon(void *arg)
{
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

  return 1; // Implementation not ready
}

void timer_set (Uint32 delay, void *param)
{
  linked_list *event = malloc(sizeof(struct linked_list));  // création de la structure de l'event

  // configure le timer pour expirer après delay msec...
  event->timer.it_value.tv_sec = delay/1000;       // secondes
  event->timer.it_value.tv_usec = (delay%1000)*1000;    // microsecondes
  // ... et seulement 1 fois
  event->timer.it_interval.tv_sec = 0;
  event->timer.it_interval.tv_usec = 0;

  event->param = param;

  event->time_signal = get_time() + delay*1000;
  event->next = NULL;
  // on ajoute event tout en le triant dans la liste
  insert(&first_event, &event);


  if(event == first_event)
  {
    setitimer(ITIMER_REAL, &(first_event->timer), NULL);
  }
}

#endif

// Création de la structure-liste : ok !
// méthodes d'accès, d'insertion, de suppression à implémenter : ok
// timer_set à modifier en fonction de la structure --> ne pas oublier l'allocation de la structure et de l'ajouter à la liste chaînée : ok !
// création de la liste chainée de base : ok !
// problème de delay à résoudre !!! : ok !

      /*
      struct itimerval timer;
      unsigned int delay = first_event->next->time_signal - first_event->time_signal; 
      timer.it_value.tv_sec = delay/1000;       // secondes
      timer.it_value.tv_usec = (delay%1000)*1000;    // microsecondes
      timer.it_interval.tv_sec = 0;
      timer.it_interval.tv_usec = 0;
      setitimer(ITIMER_REAL, &timer, NULL);*/