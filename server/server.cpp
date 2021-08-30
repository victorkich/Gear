#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <iostream>
#include <cstring>
#include <vector>

#define TOTAL 5
#define PORT 9097

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

struct nodo {
    int newsockfd;
};

struct nodo nodo[5];

std::vector<double> data_processing(int id, int operation, double v1, double v2){
    std::vector<double> result;
    switch (id) {
        case 1:
            if (operation) {
                v1 += 1;
                v2 += 1;
            } else {
                v1 += -1;
                v2 += -1;
            }
            break;
        case 2:
            if (operation) {
                v1 += 1;
                v2 += 1;
            } else {
                v1 += -1;
                v2 += -1;
            }
            break;
        case 3:
            if (operation) {
                v1 += 1;
                v2 += 1;
            } else {
                v1 += -1;
                v2 += -1;
            }
            break;
        case 4:
            if (operation) {
                v1 += 0.001;
                v2 += -0.001;
            } else {
                v1 += -0.001;
                v2 += 0.001;
            }
            break;
        case 5:
            if (operation) {
                v1 += 1;
                v2 += 1;
            } else {
                v1 += -1;
                v2 += -1;
            }
            break;
    }
    result.push_back(v1);
    result.push_back(v2);
    return result;
}


void *cliente(void *arg) {
    long cid = (long)arg;
    int i, n;
    char buffer[256];
    while (true) {
        bzero(buffer,sizeof(buffer));
        n = read(nodo[cid].newsockfd,buffer,50);
        if (n <= 0) {
            std::cout << "Erro lendo do socket id: " << cid << "\n";
            close(nodo[cid].newsockfd);
            nodo[cid].newsockfd = -1;

            pthread_exit(NULL);
        }

        std::cout << "Recebeu: " << buffer << " - " << strlen(buffer) << "\n";
        char* chars_array = strtok(buffer, "#");
        std::vector<char*> str;
        while(chars_array) {
            str.push_back(chars_array);
            chars_array = strtok(NULL, "#");
        }

        int id, operation;
        double v1, v2;
        sscanf(str[0], "%d", &id);
        sscanf(str[1], "%d", &operation);
        sscanf(str[2], "%lf", &v1);
        sscanf(str[3], "%lf", &v2);
        std::cout << "ID: " << id << " Operation: " << operation << " Value 1: " << v1 << " Value 2: " << v2 << "\n";

        std::vector<double> data;
        data = data_processing(id, operation, v1, v2);
        sprintf(buffer, "%d#%lf#%lf", id, data[0], data[1]);

        // MUTEX LOCK - GERAL
        pthread_mutex_lock(&m);
        for (i = 0;i < TOTAL; i++) {
            if ((i == cid) && (nodo[i].newsockfd != -1)) {
                n = write(nodo[i].newsockfd, buffer, 50);
                std::cout << "Enviando para o cliente: " << buffer << "\n";
                if (n <= 0) {
                    std::cout << "Erro escrevendo no socket id: " << i << "\n";
//                  close(nodo[i].newsockfd);
//                  nodo[i].newsockfd = -1;
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
        std::cout << "Erro, porta nao definida!\n";
        std::cout << "./SOFTWARE PORTA";
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cout << "Erro abrindo o socket!\n";
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        std::cout << "Erro fazendo bind!\n";
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
            std::cout << "Erro no accept!\n";
            exit(1);
        }
        pthread_create(&t, NULL, cliente, (void *)i);
        pthread_mutex_unlock(&m);
        // MUTEX UNLOCK - GERAL
    }
    //    close(sockfd);
    return 0; 
}
