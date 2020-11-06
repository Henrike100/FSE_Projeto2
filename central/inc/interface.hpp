#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <mutex>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "constantes.hpp"

#define PORT 8080

using namespace std;

int iniciar_conexao_servidor();
int iniciar_conexao_cliente();
void abrir_csv();
void atualizar_menu(WINDOW *menu);
void iniciar_menu(WINDOW *menu);
void atualizar_info(WINDOW *info);
void iniciar_info(WINDOW *info);
float pegar_temperatura(WINDOW *escolhas);
void pegar_opcao(WINDOW *menu);
void atualizar_valores();
void thread_atualizacao(WINDOW *menu, WINDOW *info);
void atualizar_csv(const int opcao, const int ligou);

#endif // INTERFACE_HPP
