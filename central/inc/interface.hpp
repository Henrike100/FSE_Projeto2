#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <mutex>

#include "constantes.hpp"
#include "lista.hpp"

using namespace std;

void atualizar_menu(WINDOW *menu);
void iniciar_menu(WINDOW *menu);
void atualizar_info(WINDOW *info);
void iniciar_info(WINDOW *info);
void pegar_opcao(WINDOW *menu);
void thread_atualizacao(WINDOW *info);

#endif // INTERFACE_HPP
