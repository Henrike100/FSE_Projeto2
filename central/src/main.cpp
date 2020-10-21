#include <stdio.h>
#include <ncurses.h>
#include <string>

using namespace std;

#define NAO_E_LIGA_DESLIGA -1

int lampada_cozinha = 0;
int lampada_sala = 0;
int lampada_quarto_1 = 0;
int lampada_quarto_2 = 0;
int ar_condicionado_1 = 0;
int ar_condicionado_2 = 0;
int alarme = 0;
float temperatura = 32.1;
float umidade = 73.1;

const char *const opcoes[] = {
    "Lampada da Cozinha",
    "Lampada da Sala",
    "Lampada do Quarto 01",
    "Lampada do Quarto 02",
    "Ligar Todas as Lampadas",
    "Desligar Todas as Lampadas",
    "Ar-Condicionado do Quarto 01",
    "Ar-Condicionado do Quarto 02",
    "Ligar Todos os Ares-Condicionados",
    "Desligar Todos os Ares-Condicionados",
    "Alarme",
    "Definir Temperatura"
};

int valores[] = {
    0,
    1,
    0,
    1,
    0,
    1,
    0,
    1,
    0,
    1,
    0,
    1,
    0,
    1,
    0,
};

void atualizar_menu(WINDOW *menu) {
    const int line_size = getmaxx(menu);

    mvwprintw(menu, 4, 2, "01");
    mvwprintw(menu, 4, 7, "%s%s", valores[0] ? "Desligar " : "Ligar ", opcoes[0]);
    mvwprintw(menu, 4, 2+line_size/2, "02");
    mvwprintw(menu, 4, 7+line_size/2, "%s%s", valores[1] ? "Desligar " : "Ligar ", opcoes[1]);
    mvwhline(menu, 6, 0, 0, line_size);

    mvwprintw(menu, 8, 2, "03");
    mvwprintw(menu, 8, 7, "%s%s", valores[2] ? "Desligar " : "Ligar ", opcoes[2]);
    mvwprintw(menu, 8, 2+line_size/2, "04");
    mvwprintw(menu, 8, 7+line_size/2, "%s%s", valores[3] ? "Desligar " : "Ligar ", opcoes[3]);
    mvwhline(menu, 10, 0, 0, line_size);

    mvwprintw(menu, 12, 2, "05");
    mvwprintw(menu, 12, 7, "Ligar Todas as Lampadas");
    mvwprintw(menu, 12, 2+line_size/2, "06");
    mvwprintw(menu, 12, 7+line_size/2, "Desligar Todas as Lampadas");
    mvwhline(menu, 14, 0, 0, line_size);

    mvwprintw(menu, 16, 2, "07");
    mvwprintw(menu, 16, 7, "%s%s", valores[6] ? "Desligar " : "Ligar ", opcoes[6]);
    mvwprintw(menu, 16, 2+line_size/2, "08");
    mvwprintw(menu, 16, 7+line_size/2, "%s%s", valores[7] ? "Desligar " : "Ligar ", opcoes[7]);
    mvwhline(menu, 18, 0, 0, line_size);

    mvwprintw(menu, 20, 2, "09");
    mvwprintw(menu, 20, 7, "Ligar Todos os Ares-Condicionados");
    mvwprintw(menu, 20, 2+line_size/2, "10");
    mvwprintw(menu, 20, 7+line_size/2, "Desligar Todos os Ares-Condicionados");
    mvwhline(menu, 22, 0, 0, line_size);

    mvwprintw(menu, 24, 2, "11");
    mvwprintw(menu, 24, 7, "%s%s", valores[10] ? "Desligar " : "Ligar ", opcoes[10]);
    mvwprintw(menu, 24, 2+line_size/2, "12");
    mvwprintw(menu, 24, 7+line_size/2, "Definir Temperatura");
    mvwhline(menu, 26, 0, 0, line_size);

    box(menu, 0, 0);
    wrefresh(menu);
}

void iniciar_menu(WINDOW *menu) {
    const int line_size = getmaxx(menu);
    const int num_lines = getmaxy(menu);
    const string spaces((line_size-4)/2, ' ');

    mvwprintw(menu, 1, 1, "%sMENU%s", spaces.c_str(), spaces.c_str());
    mvwhline(menu, 2, 0, 0, line_size);

    mvwvline(menu, 3, 5, 0, 23);
    mvwvline(menu, 3, line_size/2, 0, 23);
    mvwvline(menu, 3, 5+line_size/2, 0, 23);

    atualizar_menu(menu);

    mvwprintw(menu, num_lines-3, 2, "Escolha uma opcao: ");

    box(menu, 0, 0);
    wrefresh(menu);
}

void atualizar_info(WINDOW *info) {
    const int line_size = getmaxx(info);
    const int barra_vertical = 2*line_size/3;

    mvwprintw(info, 3, 2, "Temperatura");
    mvwprintw(info, 3, 2+barra_vertical, "%.1f°C", temperatura);
    mvwhline(info, 4, 0, 0, line_size);

    mvwprintw(info, 5, 2, "Umidade");
    mvwprintw(info, 5, 2+barra_vertical, "%.1f%%", umidade);
    mvwhline(info, 6, 0, 0, line_size);

    mvwprintw(info, 7, 2, "Lampada (Cozinha)");
    mvwprintw(info, 7, 2+barra_vertical, valores[0] ? "Ligada" : "Desligada");
    mvwhline(info, 8, 0, 0, line_size);

    mvwprintw(info, 9, 2, "Lampada (Sala)");
    mvwprintw(info, 9, 2+barra_vertical, valores[1] ? "Ligada" : "Desligada");
    mvwhline(info, 10, 0, 0, line_size);

    mvwprintw(info, 11, 2, "Lampada (Quarto 01)");
    mvwprintw(info, 11, 2+barra_vertical, valores[2] ? "Ligada" : "Desligada");
    mvwhline(info, 12, 0, 0, line_size);

    mvwprintw(info, 13, 2, "Lampada (Quarto 02)");
    mvwprintw(info, 13, 2+barra_vertical, valores[3] ? "Ligada" : "Desligada");
    mvwhline(info, 14, 0, 0, line_size);

    mvwprintw(info, 15, 2, "Ar-Condicionado (Quarto 01)");
    mvwprintw(info, 15, 2+barra_vertical, valores[4] ? "Ligado" : "Desligado");
    mvwhline(info, 16, 0, 0, line_size);

    mvwprintw(info, 17, 2, "Ar-Condicionado (Quarto 02)");
    mvwprintw(info, 17, 2+barra_vertical, valores[5] ? "Ligado" : "Desligado");
    mvwhline(info, 18, 0, 0, line_size);

    mvwprintw(info, 19, 2, "Alarme");
    mvwprintw(info, 19, 2+barra_vertical, valores[6] ? "Ligado" : "Desligado");
    mvwhline(info, 20, 0, 0, line_size);

    mvwprintw(info, 21, 2, "Sala");
    mvwprintw(info, 21, 2+barra_vertical, valores[7] ? "Alguem" : "Vazia");
    mvwhline(info, 22, 0, 0, line_size);

    mvwprintw(info, 23, 2, "Cozinha");
    mvwprintw(info, 23, 2+barra_vertical, valores[8] ? "Alguem" : "Vazia");
    mvwhline(info, 24, 0, 0, line_size);

    mvwprintw(info, 25, 2, "Porta (Cozinha)");
    mvwprintw(info, 25, 2+barra_vertical, valores[9] ? "Aberta" : "Fechada");
    mvwhline(info, 26, 0, 0, line_size);

    mvwprintw(info, 27, 2, "Janela (Cozinha)");
    mvwprintw(info, 27, 2+barra_vertical, valores[10] ? "Aberta" : "Fechada");
    mvwhline(info, 28, 0, 0, line_size);

    mvwprintw(info, 29, 2, "Porta (Sala)");
    mvwprintw(info, 29, 2+barra_vertical, valores[11] ? "Aberta" : "Fechada");
    mvwhline(info, 30, 0, 0, line_size);

    mvwprintw(info, 31, 2, "Janela (Sala)");
    mvwprintw(info, 31, 2+barra_vertical, valores[12] ? "Aberta" : "Fechada");
    mvwhline(info, 32, 0, 0, line_size);

    mvwprintw(info, 33, 2, "Janela (Quarto 01)");
    mvwprintw(info, 33, 2+barra_vertical, valores[13] ? "Aberta" : "Fechada");
    mvwhline(info, 34, 0, 0, line_size);

    mvwprintw(info, 35, 2, "Janela (Quarto 02)");
    mvwprintw(info, 35, 2+barra_vertical, valores[14] ? "Aberta" : "Fechada");
    mvwhline(info, 36, 0, 0, line_size);
    
    mvwprintw(info, 37, 2, "CSV"); mvwprintw(info, 37, 2+barra_vertical, "Iniciando");

    box(info, 0, 0);
    wrefresh(info);
}

void iniciar_info(WINDOW *info) {
    const int line_size = getmaxx(info);
    const string spaces((line_size-12)/2, ' ');
    const int barra_vertical = 2*line_size/3;

    mvwprintw(info, 1, 1, "%sINFORMACOES%s", spaces.c_str(), spaces.c_str());
    mvwhline(info, 2, 0, 0, line_size);
    mvwvline(info, 3, barra_vertical, 0, 35);

    atualizar_info(info);
}

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

    WINDOW *menu = newwin(size_y, 2*size_x/3, 0, 0), *info = newwin(size_y, size_x/3, 0, 2*size_x/3);

    box(menu, 0, 0);
    box(info, 0, 0);
    refresh();
    wrefresh(menu);
    wrefresh(info);

    iniciar_menu(menu);
    iniciar_info(info);

    getchar();

    delwin(menu);
    delwin(info);

    endwin();

    return 0;
}
