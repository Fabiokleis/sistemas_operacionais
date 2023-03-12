/* gcc fork/main.c -o fork/main */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {

    int pid; /* identificador de processo */

    pid = fork(); /* replicacao de processo */
    
    if (pid < 0) { /* fork funcionou ? */
        perror("Erro: "); /* nao funcionou, encerra este processo */
        exit(-1);

    } else {
        if (pid > 0) { /* fork funcionou, verifica se é o pai */
            printf("forkou!\n");
            wait(0); /* espera o filho concluir */
        } else {
            usleep(1000000); /* espera 1 segundo */
            execve("/bin/date", argv, envp); /* nao é o pai, é o processo filho, entao carrega outro código binario para executar */
            perror("Erro: "); /* caso execve nao funcione */
        }
    }

    printf("Tchau! \n");
    exit(0); /* encerra este processo */
}
