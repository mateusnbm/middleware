//
// nodeadlock.cpp
//

#import <stdio.h>
#import <stdlib.h>
#import <pthread.h>

pthread_cond_t p_cond;
pthread_cond_t c_cond;
pthread_mutex_t mutex;

int job_count = 100;
int job_queue_count = 0;
int job_queue_max_count = 5;

void *producer(void *) {

    for (int i = 0; i < job_count; i++) {

        pthread_mutex_lock(&mutex);

        if (job_queue_count == job_queue_max_count) {

            pthread_cond_wait(&p_cond, &mutex);

        }

        job_queue_count++;

        if (job_queue_count == 1) {

            pthread_cond_signal(&c_cond);

        }

        pthread_mutex_unlock(&mutex);

    }

    return NULL;

}

void *consumer(void *) {

    for (int i = 0; i < job_count; i++) {

        pthread_mutex_lock(&mutex);

        if (job_queue_count == 0) {

            pthread_cond_wait(&c_cond, &mutex);

        }

        job_queue_count--;

        if (job_queue_count == (job_queue_max_count - 1)) {

            pthread_cond_signal(&p_cond);

        }

        pthread_mutex_unlock(&mutex);

    }

    return NULL;

}

int main (int argc, const char * argv[]) {

    printf("Started.\n");

    pthread_t pid, cid;

    pthread_cond_init(&p_cond, NULL);
    pthread_cond_init(&c_cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    printf("Finished.\n");

    return 0;

}
