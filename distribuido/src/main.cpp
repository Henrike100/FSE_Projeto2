#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>

using namespace std;

bool programa_pode_continuar = true;

float temperatura = 0.0;
float umidade = 0.0;
float temperatura_usuario = 0.0;

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

void enviar(int socket) {
    float temperatura_umidade[2];
    temperatura_umidade[0] = temperatura;
    temperatura_umidade[1] = umidade;

    send(socket, temperatura_umidade, sizeof(temperatura_umidade), 0);
    send(socket, valores, sizeof(valores), 0);

    temperatura += 0.1;
    umidade += 0.2;
}

void receber(int socket) {
    int comando;
    int bytesRecebidos = recv(socket, &comando, sizeof(comando), 0);
    if(bytesRecebidos < 0) {
        printf("Erro ao receber comando\n");
    }
    else if(bytesRecebidos == 0) {
        printf("Não há dado disponível para comando\n");
    }
    else {
        float temp_recebida;
        switch (comando) {
        case 1:
        case 2:
        case 3:
        case 4:
            valores[comando-1] = 1 - valores[comando-1];
            break;
        case 5:
            valores[0] = valores[1] = valores[2] = valores[3] = 1;
            break;
        case 6:
            valores[0] = valores[1] = valores[2] = valores[3] = 0;
            break;
        case 8:
            bytesRecebidos = recv(socket, &temp_recebida, sizeof(temp_recebida), 0);
            if(bytesRecebidos < 0) {
                printf("Erro ao receber temperatura\n");
            }
            else if(bytesRecebidos == 0) {
                printf("Não há dado disponível para comando\n");
            }
            else {
                // conseguiu receber temperatura
                // verifica se é um valor aceitável para o contexto atual
                // 0 <= temp <= 50
                if(0 <= temp_recebida && temp_recebida <= 50) {
                    temperatura_usuario = temp_recebida;
                }
                else {
                    printf("Temperatura estranha recebida: %.1f\n", temp_recebida);
                }
            }
            break;
        case 0:
            programa_pode_continuar = false;
            break;
        default:
            printf("Comando estranho recebido: %d\n", comando);
            break;
        }
    }
}

void receber_comandos() {
    int clienteSocket;

    if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        programa_pode_continuar = false;
        return;
    }

    struct sockaddr_in servidorAddr;
    memset(&servidorAddr, 0, sizeof(servidorAddr));
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr("192.168.0.53");
	servidorAddr.sin_port = htons(10028);

    if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0) {
        programa_pode_continuar = false;
        return;
    }

    while(programa_pode_continuar) {
        receber(clienteSocket);
    }

    close(clienteSocket);
}

void enviar_valores() {
    int servidorSocket;
    if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printf("falha no socket do Servidor\n");
        programa_pode_continuar = false;
        return;
    }

    struct sockaddr_in servidorAddr;

    memset(&servidorAddr, 0, sizeof(servidorAddr));
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(10128);

    if(bind(servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0) {
        printf("Falha no Bind\n");
        programa_pode_continuar = false;
        return;
    }

    if(listen(servidorSocket, 10) < 0) {
        printf("Falha no Listen\n");
        programa_pode_continuar = false;
        return;
    }

    unsigned int clienteLength;
    struct sockaddr_in clienteAddr;
    int socketCliente;

    while(programa_pode_continuar) {
        clienteLength = sizeof(clienteAddr);
        if((socketCliente = accept(servidorSocket, (struct sockaddr *) &clienteAddr, &clienteLength)) < 0) {
            printf("Falha no Accept\n");
            sleep(1);
            continue;
        }

        enviar(socketCliente);

        close(socketCliente);
        sleep(1);
    }

    close(servidorSocket);
}

int main(int argc, const char *argv[]) {
    thread thread_send(enviar_valores);
    thread thread_recv(receber_comandos);

    thread_send.join();
    thread_recv.join();

    return 0;
}
