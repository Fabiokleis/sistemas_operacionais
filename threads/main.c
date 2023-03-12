/* gcc -lpthread thread/main.c -o thread/main */
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 6

/* uma maneira de guardar as informaçoes de thread
 *
 * cada variavel do struct pode ser alterado como quiser
 * apenas o pthread_t define a thread
 *
*/
struct thread_info {
    pthread_t thread_id;
    int thread_num;
    int rand_num; 
};

// cada thread ira executar esta funcao
static void *print_hello(void *tinfo_arg) {
    struct thread_info *tinfo = tinfo_arg;
    printf("%ld: Hello World!\n", (long) tinfo->thread_num);

    sleep(2); // para mostrar que nem sempre são sequenciais
    printf("%ld: Bye bye World %d!\n", (long) tinfo->thread_num, tinfo->rand_num);

    char *res = malloc(16 * sizeof(char));
    snprintf(res, 16, "random number %d", tinfo->rand_num * 10);
    return res;
}

int main(int argc, char **argv) {
    long status;
    void *res;

    // vetor de threads
    struct thread_info *tinfo = malloc(NUM_THREADS * sizeof(*tinfo));

    if (tinfo == NULL) {
        perror("malloc");
        exit(-1);
    }

    srand(time(NULL));
    for (long i = 0; i < NUM_THREADS; i++) {
        printf("Creating thread %ld\n", i);
        tinfo[i].thread_num = i;
        tinfo[i].rand_num = rand() % 10 + 1;

        // todas as funcoes do pthread retornam um status
        status = pthread_create(&tinfo[i].thread_id, NULL, print_hello, &tinfo[i]); // chamada para criar uma thread

        // status diferente de 0 são erros
        if (status) {
            perror("pthread_create");
            exit(-1);
        }
    }

    for (long i = 0; i < NUM_THREADS; i++) {
        status = pthread_join(tinfo[i].thread_id, &res); // join para cada thread do vetor tinfo
        if (status) {
            perror("pthread_join");
            exit(-1);
        }

        // mostrando o valor retornado da funcao que rodo na thread
        printf("Join with thread %d; returned value was %s\n",
                tinfo[i].thread_num, (char *) res);
        free(res); // desaloca cada string alocada na thread
    }

    free(tinfo);
    pthread_exit(NULL);
}
