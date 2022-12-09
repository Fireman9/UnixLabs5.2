#include "threading.h"
#include <unistd.h>
#include <stdlib.h>

void *threadfunc(void *thread_param) {
	struct thread_data *thread_func_arguments = (struct thread_data *) thread_param;
	usleep(thread_func_arguments->obtain_ms);
	int lock = pthread_mutex_lock(thread_func_arguments->mutex);
	if (lock != 0) thread_func_arguments->thread_complete_success = false;
	else {
		usleep(thread_func_arguments->release_ms);
		int unlock = pthread_mutex_unlock(thread_func_arguments->mutex);
		if (unlock != 0) thread_func_arguments->thread_complete_success = false;
	}
	thread_func_arguments->thread_complete_success = true;
	return thread_param;
}

bool start_thread_obtaining_mutex(pthread_t *thread,
								  pthread_mutex_t *mutex,
								  int wait_to_obtain_ms,
								  int wait_to_release_ms) {
	struct thread_data *threads = malloc(sizeof(struct thread_data));
	memset(threads, 0, sizeof(struct thread_data));
	threads->mutex = mutex;
	threads->obtain_ms = wait_to_obtain_ms;
	threads->release_ms = wait_to_release_ms;
	int rc = pthread_create(thread, NULL, threadfunc, (void *) threads);
	if (rc != 0) threads->thread_complete_success = false;
	else threads->thread_complete_success = true;
	return threads->thread_complete_success;
}

