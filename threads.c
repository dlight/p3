# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>

// 0   1   2
//   l0   l1
//   3 l2 4

extern unsigned int slides[5];
extern unsigned int pos[5];

pthread_mutex_t lock[3];

int holding_lock[3] = { -1, -1, -1 };

typedef enum {
	state_0, state_1, state_2, state_3
} state_t;

void tick(unsigned tid)
{
	if (pos[tid] >= 399)
		pos[tid] = 0;
	else
		pos[tid]++;

	usleep(33000 - 300*slides[tid]);
}

void lock_mutex(unsigned i, unsigned p)
{
	if (pthread_mutex_lock(&lock[i])) {
		perror("pthread_mutex_lock");
		exit(EXIT_FAILURE);
	}
	holding_lock[i] = p;
}
	
void unlock_mutex(unsigned i, unsigned p)
{
	holding_lock[i] = -1;

	if (pthread_mutex_unlock(&lock[i])) {
		perror("pthread_mutex_unlock");
		exit(EXIT_FAILURE);
	}
}


void* thread_0(void* n)
{
	/* state_0 (initial)
                       \      try to hold lock1
	release /\     \/
	 lock1   \----- state_1
	*/

	unsigned tid = atoi((char*) n);
	state_t s = state_0;
	unsigned bp[2] = {80, 270};

	while (1) {
		if (s == state_0 && pos[tid] > bp[0] && pos[tid] < bp[1]) {
			//printf("0 locking: %d\n", pos[tid]);
			lock_mutex(0, tid);
			s = state_1;
		}
		else if (s == state_1 && (pos[tid] <= bp[0] || pos[tid] >= bp[1])) {
			//printf("0 unlocking: %d\n", pos[tid]);
			unlock_mutex(0, tid);
			s = state_0;
		}

		tick(tid);
	}
}

void* thread_2(void* n)
{
	// similar to 0

	unsigned tid = atoi((char*) n);
	state_t s = state_0;
	unsigned bp[2] = {230, 20};

	while (1) {
		if (s == state_0 && (pos[tid] > bp[0] || pos[tid] < bp[1])) {
			//printf("2 locking: %d\n", pos[tid]);
			lock_mutex(1, tid);
			s = state_1;
		}
		else if (s == state_1 && pos[tid] <= bp[0] && pos[tid] >= bp[1]) {
			//printf("2 locking: %d\n", pos[tid]);
			unlock_mutex(1, tid);
			s = state_0;
		}

		tick(tid);
	}
}

void* thread_1(void* n)
{
	//        state_0 -> get

	unsigned tid = atoi((char*) n);
	state_t s = state_0;
	unsigned bp[3] = {80, 280, 20};

	while (1) {
		if (s == state_0 && pos[tid] > bp[0] && pos[tid] < bp[1]) {
			lock_mutex(1, tid);
			lock_mutex(0, tid);
			s = state_1;
		}
		else if (s == state_1 && (pos[tid] >= bp[1] || pos[tid] < bp[2])) {
			unlock_mutex(1, tid);
			s = state_2;
		}
		else if (s == state_2 && pos[tid] >= bp[2] && pos[tid] <= bp[0]) {
			unlock_mutex(0, tid);
			s = state_0;
		}

		tick(tid);
	}
}

void* thread_3(void* n)
{
	//        state_0 -> get

	unsigned tid = atoi((char*) n);
	state_t s = state_0;
	unsigned bp[4] = {380, 80, 110, 220};

	while (1) {
		if (s == state_0 && (pos[tid] > bp[0] || pos[tid] < bp[1])) {
			lock_mutex(0, tid);
			s = state_1;
		}
		else if (s == state_1 && (pos[tid] >= bp[1] && pos[tid] < bp[2])) {
			//printf("aa\n");
			lock_mutex(2, tid);
			//printf("bb\n");
			s = state_2;
		}
		else if (s == state_2 && pos[tid] >= bp[2] && pos[tid] < bp[3]) {
			unlock_mutex(0, tid);
			s = state_3;
		}
		else if (s == state_3 && pos[tid] >= bp[3] && pos[tid] <= bp[0]) {
			unlock_mutex(2, tid);
			s = state_0;
		}

		tick(tid);
	}
}

void* thread_4(void* n)
{
	//        state_0 -> get

	unsigned tid = atoi((char*) n);
	state_t s = state_0;
	unsigned bp[4] = {280, 380, 10, 120};

	while (1) {
		if (s == state_0 && pos[tid] > bp[0] && pos[tid] < bp[1]) {
			lock_mutex(1, tid);
			lock_mutex(2, tid);
			s = state_1;
		}
		/*else if (s == state_1 && (pos[tid] >= bp[1] || pos[tid] < bp[2])) {
			printf("aa\n");
			lock_mutex(1, tid);
			printf("bb\n");
			s = state_2;
		}
		else if (s == state_2 && pos[tid] >= bp[2] && pos[tid] < bp[3]) {

			s = state_3;
		}*/
		else if (s == state_1 && pos[tid] >= bp[3] && pos[tid] <= bp[0]) {
			unlock_mutex(2, tid);
			unlock_mutex(1, tid);
			s = state_0;
		}

		tick(tid);
	}
}


void* thread_nosync(void* n)
{
	unsigned tid = atoi((char*) n);

	while (1) {
		tick(tid);
	}
}

void create_thread(void* (*start_routine) (void *), char* id)
{
	pthread_t thread;
	if (pthread_create(&thread, NULL, start_routine, id)) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
}


void begin_threads ()
{
	create_thread(thread_0, "0");
	create_thread(thread_1, "1");
	create_thread(thread_2, "2");
	create_thread(thread_3, "3");
	create_thread(thread_4, "4");
}

void begin_threads_nosync ()
{
	create_thread(thread_nosync, "0");
	create_thread(thread_nosync, "1");
	create_thread(thread_nosync, "2");
	create_thread(thread_nosync, "3");
	create_thread(thread_nosync, "4");
}
