#include "configuracoes.hpp"

int iniciar_conexao_servidor(int *servidorSocket, int *socketCliente) {
    if((*servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        return 1;
    }

    struct sockaddr_in servidorAddr;

    memset(&servidorAddr, 0, sizeof(servidorAddr));
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(10028);

    if(bind(*servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0) {
        return 2;
    }

    if(listen(*servidorSocket, 10) < 0) {
        return 3;
    }

    unsigned int clienteLength;
    struct sockaddr_in clienteAddr;

    if((*socketCliente = accept(*servidorSocket, (struct sockaddr *) &clienteAddr, &clienteLength)) < 0) {
        return 4;
    }

    return 0;
}

int iniciar_conexao_cliente(int *clienteSocket) {
    if((*clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        return 1;
    }

    struct sockaddr_in servidorAddr;
    memset(&servidorAddr, 0, sizeof(servidorAddr));
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr("192.168.0.52");
	servidorAddr.sin_port = htons(10128);

    if(connect(*clienteSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0) {
        return 2;
    }

    return 0;
}

int abrir_csv(FILE *file) {
    file = fopen("arquivo.csv", "w+");
    if(file == NULL) {
        return 1;
    }

    if(fprintf(file, "Data/Hora, Fonte, Ocorrido\n") <= 0)
        return 2;

    return 0;
}