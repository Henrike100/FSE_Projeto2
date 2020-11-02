#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

#define PORT 8080

bool programa_pode_continuar = true;

float temperatura = 0.0;
float umidade = 0.0;

int valores[] = {
    0, // Lampada Cozinha
    0, // Lampada Sala
    0, // Lampada Quarto 1
    0, // Lampada Quarto 2
    0, // Ar Condicionado 1
    0, // Ar Condicionado 2
    0, // Alarme
    0, // Sala
    0, // Cozinha
    0, // Porta Cozinha
    0, // Janela Cozinha
    0, // Porta Sala
    0, // Janela Sala
    0, // Janela Quarto 1
    0, // Janela Quarto 2
};

int contador = 0;

void receber_comandos(int socketCliente) {

}

void enviar_valores(int socketCliente) {
    float temperatura_umidade[2];
    temperatura_umidade[0] = temperatura;
    temperatura_umidade[1] = umidade;

    send(socketCliente, temperatura_umidade, sizeof(temperatura_umidade), 0);
    send(socketCliente, valores, sizeof(valores), 0);

    temperatura += 0.1;
    umidade += 0.2;

    valores[contador] = 1 - valores[contador];
    contador = (contador+1)%15;
}

int main(int argc, const char *argv[]) {
    int servidorSocket;
    if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printf("falha no socket do Servidor\n");
        return 0;
    }

    struct sockaddr_in servidorAddr;

    memset(&servidorAddr, 0, sizeof(servidorAddr));
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(PORT);

    if(bind(servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0) {
        printf("Falha no Bind\n");
        return 0;
    }

    if(listen(servidorSocket, 10) < 0) {
        printf("Falha no Listen\n");
        return 0;
    }

    unsigned int clienteLength;
    struct sockaddr_in clienteAddr;
    int socketCliente;

    while(programa_pode_continuar) {
        clienteLength = sizeof(clienteAddr);
        if((socketCliente = accept(servidorSocket, (struct sockaddr *) &clienteAddr, &clienteLength)) < 0) {
            printf("Falha no Accept\n");
        }

        receber_comandos(socketCliente);
        enviar_valores(socketCliente);

        close(socketCliente);
        sleep(1);
    }

    close(servidorSocket);
    return 0;
}
