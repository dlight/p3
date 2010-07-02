# include <pthread.h>
//# include <time.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>
# include <stdlib.h>

extern unsigned int slides[5];
extern unsigned int pos[5];

void *PrintHello(void* threadid)
{
   long tid = (long) threadid;
    struct timespec t;

   while (1) {
        long k = 33000 - 300*slides[tid];

       //printf("qq #%ld! %u, %ld!!\n", tid, slides[tid], k);


    //t.tv_sec = slides[tid] * 10;
    //t.tv_nsec = (slides[tid] % 10) * 10e6;
    

       //nanosleep(&t, NULL); //(slides[tid] * 300);

        if (pos[tid] >= 400)
            pos[tid] = 0;
        else
            pos[tid]++;


        usleep(k);
    }

   //pthread_exit(NULL);
}

void begin_threads ()
{
   pthread_t threads[5];
   int rc;
   long t;
   for(t=0; t< 5; t++){
      //printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      if (rc) {
         perror("pthread_create");
         exit(-1);
      }
   }
   //pthread_exit(NULL);
}
