#include <stdio.h>
#include "interface.hpp"
#include <thread>

int main(int argc, const char *argv[]) {
    int size_x, size_y;
    initscr();

    getmaxyx(stdscr, size_y, size_x);
    const int min_colunas = 140, min_linhas = 39;

    if(size_y < min_linhas or size_x < min_colunas) {
        endwin();
        printf("Para uma melhor experiência, ajuste o tamanho do terminal para, no mínimo:\n");
        printf("%d colunas e %d linhas (%dx%d)\n", min_colunas, min_linhas, min_colunas, min_linhas);
        printf("Atual: %dx%d\n", size_x, size_y);
        return 0;
    }

    int status_conexao = iniciar_conexao();

    if(status_conexao == 1) {
        endwin();
        printf("Erro no socket()\n");
        return 0;
    }
    else if(status_conexao == 2) {
        endwin();
        printf("Erro no connect()\n");
        return 0;
    }

    WINDOW *menu = newwin(size_y, 2*size_x/3, 0, 0), *info = newwin(size_y, size_x/3, 0, 2*size_x/3);

    box(menu, 0, 0);
    box(info, 0, 0);
    refresh();
    wrefresh(menu);
    wrefresh(info);

    abrir_csv();
    iniciar_menu(menu);
    iniciar_info(info);

    thread thread_input(pegar_opcao, menu);
    thread thread_info(thread_atualizacao, menu, info);

    thread_input.join();
    thread_info.join();

    delwin(menu);
    delwin(info);

    endwin();

    return 0;
}
