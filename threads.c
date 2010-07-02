# include <pthread.h>
# include <stdio.h>
# include <errno.h>
# include <stdlib.h>

void *PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   pthread_exit(NULL);
}

void begin_threads ()
{
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   for(t=0; t< 5; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      if (rc) {
         perror("pthread_create");
         exit(-1);
      }
   }
   //pthread_exit(NULL);
}
