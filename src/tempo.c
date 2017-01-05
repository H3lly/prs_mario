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

void *daemon(void *arg){
/*
	while(1){
		sigsuspend();
	}
*/
}



// timer_init returns 1 if timers are fully implemented, 0 otherwise
int timer_init (void)
{

  sigset_t mask;	// masque de blocage de signaux 
  sigemptyset(&mask);	// création d'un masque vide
  sigaddset(&mask, SIGALRM);	//on ajoute le signal SIGALRM au masque
  pthread_sigmask(SIG_BLOCK, &mask, NULL);	// les autres threads crées par timer_init hériteront d'une copie du masque de blocage de signaux

  pthread_t thread; // thread

  if(pthread_create(&thread, NULL, daemon, NULL) == -1){ //création du thread qui exécute la fonction daemon
  	perror("pthread_create");
  	return EXIT_FAILURE;
  }
  pthread_join(thread, NULL); // attend la fin du thread

  return 0; // Implementation not ready
}

void timer_set (Uint32 delay, void *param)
{
  // TODO
}

#endif
