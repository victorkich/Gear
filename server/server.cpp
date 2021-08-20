#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define TOTAL 5
#define PORT 9097

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

struct nodo {
    int newsockfd;
};

struct nodo nodo[5];

void *cliente(void *arg) {
    long cid = (long)arg;
    int i, n;
    char buffer[256];
    while (true) {
        bzero(buffer,sizeof(buffer));
        n = read(nodo[cid].newsockfd,buffer,50);
        printf("Recebeu: %s - %lu\n", buffer,strlen(buffer));
        if (n <= 0) {
            printf("Erro lendo do socket id %lu!\n", cid);
            close(nodo[cid].newsockfd);
            nodo[cid].newsockfd = -1;

            pthread_exit(NULL);
        }
        // MUTEX LOCK - GERAL
        pthread_mutex_lock(&m);
        for (i = 0;i < TOTAL; i++) {
            if ((i != cid) && (nodo[i].newsockfd != -1)) {
                n = write(nodo[i].newsockfd,buffer,50);
                if (n <= 0) {
                    printf("Erro escrevendo no socket id %i!\n", i);
//                    close(nodo[i].newsockfd);
//                    nodo[i].newsockfd = -1;
                }
            }
        }
        pthread_mutex_unlock(&m);
        // MUTEX UNLOCK - GERAL
    }
}

int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    int sockfd, portno;
    pthread_t t;
    long i;

    if (argc < 2) {
        printf("Erro, porta nao definida!\n");
        printf("./SOFTWARE PORTA");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Erro abrindo o socket!\n");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        printf("Erro fazendo bind!\n");
        exit(1);
    }
    listen(sockfd,5);

    for (i = 0; i < TOTAL; i++) {
      nodo[i].newsockfd = -1;
    }
    while (true) {
        for (i = 0; i < TOTAL; i++) {
          if (nodo[i].newsockfd == -1) break;
        }
        nodo[i].newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
        // MUTEX LOCK - GERAL
        pthread_mutex_lock(&m);
        if (nodo[i].newsockfd < 0) {
            printf("Erro no accept!\n");
            exit(1);
        }
        pthread_create(&t, NULL, cliente, (void *)i);
        pthread_mutex_unlock(&m);
        // MUTEX UNLOCK - GERAL
    }
    //    close(sockfd);
    return 0; 
}
