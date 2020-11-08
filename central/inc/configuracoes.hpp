#ifndef CONFIGURACOES_HPP
#define CONFIGURACOES_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int iniciar_conexao_servidor(int *servidorSocket, int *socketCliente);
int iniciar_conexao_cliente(int *clienteSocket);

#endif // CONFIGURACOES_HPP