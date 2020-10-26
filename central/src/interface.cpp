#include "interface.hpp"

FILE *file;

bool programa_pode_continuar = true;

float temperatura = 32.1;
float umidade = 73.1;

mutex mtx_interface;

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

void abrir_csv() {
    file = fopen("arquivo.csv", "w+");
    if(file == NULL) {
        programa_pode_continuar = false;
        return;
    }

    fprintf(file, "Data/Hora, Fonte, Ocorrido\n");
}

void atualizar_menu(WINDOW *menu) {
    mtx_interface.lock();
    const int line_size = getmaxx(menu);

    for(int i = 4; i <= 28; i += 4) {
        wmove(menu, i, 1);
        wclrtoeol(menu);
    }

    mvwprintw(menu, 4, 2, "01");
    mvwprintw(menu, 4, 7, "%s%s", valores[0] ? "Desligar " : "Ligar ", opcoes[0]);
    mvwprintw(menu, 4, 2+line_size/2, "02");
    mvwprintw(menu, 4, 7+line_size/2, "%s%s", valores[1] ? "Desligar " : "Ligar ", opcoes[1]);

    mvwprintw(menu, 8, 2, "03");
    mvwprintw(menu, 8, 7, "%s%s", valores[2] ? "Desligar " : "Ligar ", opcoes[2]);
    mvwprintw(menu, 8, 2+line_size/2, "04");
    mvwprintw(menu, 8, 7+line_size/2, "%s%s", valores[3] ? "Desligar " : "Ligar ", opcoes[3]);

    mvwprintw(menu, 12, 2, "05");
    mvwprintw(menu, 12, 7, "Ligar Todas as Lampadas");
    mvwprintw(menu, 12, 2+line_size/2, "06");
    mvwprintw(menu, 12, 7+line_size/2, "Desligar Todas as Lampadas");

    mvwprintw(menu, 16, 2, "07");
    mvwprintw(menu, 16, 7, "%s%s", valores[4] ? "Desligar " : "Ligar ", opcoes[6]);
    mvwprintw(menu, 16, 2+line_size/2, "08");
    mvwprintw(menu, 16, 7+line_size/2, "%s%s", valores[5] ? "Desligar " : "Ligar ", opcoes[7]);

    mvwprintw(menu, 20, 2, "09");
    mvwprintw(menu, 20, 7, "Ligar Todos os Ares-Condicionados");
    mvwprintw(menu, 20, 2+line_size/2, "10");
    mvwprintw(menu, 20, 7+line_size/2, "Desligar Todos os Ares-Condicionados");

    mvwprintw(menu, 24, 2, "11");
    mvwprintw(menu, 24, 7, "%s%s", valores[6] ? "Desligar " : "Ligar ", opcoes[10]);
    mvwprintw(menu, 24, 2+line_size/2, "12");
    mvwprintw(menu, 24, 7+line_size/2, "Definir Temperatura");

    mvwprintw(menu, 28, 2, "00");
    mvwprintw(menu, 28, 7, "Encerrar Programa");

    mvwvline(menu, 3, 5, 0, 27);
    mvwvline(menu, 3, line_size/2, 0, 23);
    mvwvline(menu, 3, 5+line_size/2, 0, 23);

    for(int i = 6; i <= 30; i += 4) {
        mvwhline(menu, i, 0, 0, line_size);
    }

    box(menu, 0, 0);
    wrefresh(menu);
    mtx_interface.unlock();
}

void iniciar_menu(WINDOW *menu) {
    mtx_interface.lock();
    const int line_size = getmaxx(menu);
    const string spaces((line_size-4)/2, ' ');

    mvwprintw(menu, 1, 1, "%sMENU%s", spaces.c_str(), spaces.c_str());
    mvwhline(menu, 2, 0, 0, line_size);

    mtx_interface.unlock();

    atualizar_menu(menu);
}

void atualizar_info(WINDOW *info) {
    mtx_interface.lock();
    const int line_size = getmaxx(info);
    const int barra_vertical = 2*line_size/3;

    for(int i = 3; i <= 37; i += 2) {
        wmove(info, i, 1);
        wclrtoeol(info);
    }

    mvwprintw(info, 3, 2, "Temperatura");
    mvwprintw(info, 3, 2+barra_vertical, "%.1f°C", temperatura);

    mvwprintw(info, 5, 2, "Umidade");
    mvwprintw(info, 5, 2+barra_vertical, "%.1f%%", umidade);

    mvwprintw(info, 7, 2, "Lampada (Cozinha)");
    mvwprintw(info, 7, 2+barra_vertical, valores[0] ? "Ligada" : "Desligada");

    mvwprintw(info, 9, 2, "Lampada (Sala)");
    mvwprintw(info, 9, 2+barra_vertical, valores[1] ? "Ligada" : "Desligada");

    mvwprintw(info, 11, 2, "Lampada (Quarto 01)");
    mvwprintw(info, 11, 2+barra_vertical, valores[2] ? "Ligada" : "Desligada");

    mvwprintw(info, 13, 2, "Lampada (Quarto 02)");
    mvwprintw(info, 13, 2+barra_vertical, valores[3] ? "Ligada" : "Desligada");

    mvwprintw(info, 15, 2, "Ar-Condicionado (Quarto 01)");
    mvwprintw(info, 15, 2+barra_vertical, valores[4] ? "Ligado" : "Desligado");

    mvwprintw(info, 17, 2, "Ar-Condicionado (Quarto 02)");
    mvwprintw(info, 17, 2+barra_vertical, valores[5] ? "Ligado" : "Desligado");

    mvwprintw(info, 19, 2, "Alarme");
    mvwprintw(info, 19, 2+barra_vertical, valores[6] ? "Ligado" : "Desligado");

    mvwprintw(info, 21, 2, "Sala");
    mvwprintw(info, 21, 2+barra_vertical, valores[7] ? "Alguem" : "Vazia");

    mvwprintw(info, 23, 2, "Cozinha");
    mvwprintw(info, 23, 2+barra_vertical, valores[8] ? "Alguem" : "Vazia");

    mvwprintw(info, 25, 2, "Porta (Cozinha)");
    mvwprintw(info, 25, 2+barra_vertical, valores[9] ? "Aberta" : "Fechada");

    mvwprintw(info, 27, 2, "Janela (Cozinha)");
    mvwprintw(info, 27, 2+barra_vertical, valores[10] ? "Aberta" : "Fechada");

    mvwprintw(info, 29, 2, "Porta (Sala)");
    mvwprintw(info, 29, 2+barra_vertical, valores[11] ? "Aberta" : "Fechada");

    mvwprintw(info, 31, 2, "Janela (Sala)");
    mvwprintw(info, 31, 2+barra_vertical, valores[12] ? "Aberta" : "Fechada");

    mvwprintw(info, 33, 2, "Janela (Quarto 01)");
    mvwprintw(info, 33, 2+barra_vertical, valores[13] ? "Aberta" : "Fechada");

    mvwprintw(info, 35, 2, "Janela (Quarto 02)");
    mvwprintw(info, 35, 2+barra_vertical, valores[14] ? "Aberta" : "Fechada");

    mvwprintw(info, 37, 2, "CSV"); mvwprintw(info, 37, 2+barra_vertical, "Iniciando");

    mvwvline(info, 3, barra_vertical, 0, 35);
    for(int i = 4; i <= 36; i += 2)
        mvwhline(info, i, 0, 0, line_size);

    box(info, 0, 0);
    wrefresh(info);
    mtx_interface.unlock();
}

void iniciar_info(WINDOW *info) {
    mtx_interface.lock();
    const int line_size = getmaxx(info);
    const string spaces((line_size-12)/2, ' ');
    const int barra_vertical = 2*line_size/3;

    mvwprintw(info, 1, 1, "%sINFORMACOES%s", spaces.c_str(), spaces.c_str());
    mvwhline(info, 2, 0, 0, line_size);
    mvwvline(info, 3, barra_vertical, 0, 35);
    mtx_interface.unlock();

    atualizar_info(info);
}

void pegar_opcao(WINDOW *menu) {
    mtx_interface.lock();
    const int num_lines = getmaxy(menu);
    mtx_interface.unlock();

    int opcao;
    bool invalid = false;

    do {
        do {
            mtx_interface.lock();
            wmove(menu, num_lines-3, 1);
            wclrtoeol(menu);
            wmove(menu, num_lines-2, 1);
            wclrtoeol(menu);
            box(menu, 0, 0);

            if(invalid) {
                mvwprintw(menu, num_lines-2, 2, "Escolha deve estar entre 0 e 12");
            }

            wrefresh(menu);
            mvwprintw(menu, num_lines-3, 2, "Escolha uma opcao: ");
            mtx_interface.unlock();
            mvwscanw(menu, num_lines-3, 21, " %d", &opcao);
            invalid = opcao < 0 || opcao > 12;
        } while (invalid);

        int ligou;

        switch (opcao) {
        case 1:
        case 2:
        case 3:
        case 4:
            ligou = toggle_dispositivo(&valores[opcao-1]);
            break;
        case 5:
            ligar(&valores[0]);
            ligar(&valores[1]);
            ligar(&valores[2]);
            ligar(&valores[3]);
            ligou = -1;
            break;
        case 6:
            desligar(&valores[0]);
            desligar(&valores[1]);
            desligar(&valores[2]);
            desligar(&valores[3]);
            ligou = -1;
            break;
        case 7:
        case 8:
            ligou = toggle_dispositivo(&valores[opcao-3]);
            break;
        case 9:
            ligar(&valores[4]);
            ligar(&valores[5]);
            ligou = -1;
            break;
        case 10:
            desligar(&valores[4]);
            desligar(&valores[5]);
            ligou = -1;
            break;
        case 11:
            ligou = toggle_dispositivo(&valores[6]);
            break;
        case 12:

            break;
        case 0:
            programa_pode_continuar = false;
            break;
        default:
            break;
        }

        atualizar_menu(menu);
        // enviar opcao com tcp
        atualizar_csv(opcao, ligou);
    } while (programa_pode_continuar);
}

void thread_atualizacao(WINDOW *info) {
    while(programa_pode_continuar) {
        atualizar_info(info);
        sleep(1);
    }
}

void atualizar_csv(const int opcao, const int ligou) {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    if(opcao) {
        fprintf(file, "%02d/%02d/%d %02d:%02d:%02d, Usuário, %s%s\n",
            ltm->tm_mday,
            ltm->tm_mon+1,
            ltm->tm_year+1900,
            ltm->tm_hour,
            ltm->tm_min,
            ltm->tm_sec,
            ligou == 1 ? "Ligou " : ligou == 0 ? "Desligou " : "",
            opcoes[opcao-1]
        );
    }
}