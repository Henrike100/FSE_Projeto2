#include "interface.hpp"

bool programa_pode_continuar = true;
mutex mtx_interface;
mutex mtx_csv;

FILE *file;

int alarme = 0;
float temperatura = 0;
float umidade = 0;
int valores[] = {
    0, // Lampada Cozinha
    0, // Lampada Sala
    0, // Lampada Quarto 1
    0, // Lampada Quarto 2
    0, // Ar Condicionado 1
    0, // Ar Condicionado 2
    0, // Sala
    0, // Cozinha
    0, // Porta Cozinha
    0, // Janela Cozinha
    0, // Porta Sala
    0, // Janela Sala
    0, // Janela Quarto 1
    0, // Janela Quarto 2
};

void signal_handler(int signum) {
    programa_pode_continuar = false;
}

int abrir_csv() {
    file = fopen("arquivo.csv", "w+");
    if(file == NULL) {
        return 1;
    }

    if(fprintf(file, "Data/Hora, Fonte, Ocorrido\n") <= 0)
        return 2;

    return 0;
}

void fechar_csv() {
    fclose(file);
}

void atualizar_menu(WINDOW *menu) {
    mtx_interface.lock();
    const int line_size = getmaxx(menu);

    for(int i = 4; i <= 28; i += 4) {
        wmove(menu, i, 1);
        wclrtoeol(menu);
    }

    mvwprintw(menu, 4, 2, "01");
    mvwprintw(menu, 4, 7, "%s%s", valores[0] ? "Desligar " : "Ligar ", opcoes[1]);
    mvwprintw(menu, 4, 2+line_size/2, "02");
    mvwprintw(menu, 4, 7+line_size/2, "%s%s", valores[1] ? "Desligar " : "Ligar ", opcoes[2]);

    mvwprintw(menu, 8, 2, "03");
    mvwprintw(menu, 8, 7, "%s%s", valores[2] ? "Desligar " : "Ligar ", opcoes[3]);
    mvwprintw(menu, 8, 2+line_size/2, "04");
    mvwprintw(menu, 8, 7+line_size/2, "%s%s", valores[3] ? "Desligar " : "Ligar ", opcoes[4]);

    mvwprintw(menu, 12, 2, "05");
    mvwprintw(menu, 12, 7, "%s", opcoes[5]);
    mvwprintw(menu, 12, 2+line_size/2, "06");
    mvwprintw(menu, 12, 7+line_size/2, "%s", opcoes[6]);

    mvwprintw(menu, 16, 2, "07");
    mvwprintw(menu, 16, 7, "%s%s", alarme ? "Desligar " : "Ligar ", opcoes[7]);
    mvwprintw(menu, 16, 2+line_size/2, "08");
    mvwprintw(menu, 16, 7+line_size/2, "%s", opcoes[8]);

    mvwprintw(menu, 20, 2, "00");
    mvwprintw(menu, 20, 7, "%s", opcoes[0]);

    mvwvline(menu, 3, 5, 0, 20);
    mvwvline(menu, 3, line_size/2, 0, 16);
    mvwvline(menu, 3, 5+line_size/2, 0, 16);

    for(int i = 6; i <= 20; i += 4) {
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
    mvwprintw(info, 19, 2+barra_vertical, alarme ? "Ligado" : "Desligado");

    mvwprintw(info, 21, 2, "Sala");
    mvwprintw(info, 21, 2+barra_vertical, valores[6] ? "Alguem" : "Vazia");

    mvwprintw(info, 23, 2, "Cozinha");
    mvwprintw(info, 23, 2+barra_vertical, valores[7] ? "Alguem" : "Vazia");

    mvwprintw(info, 25, 2, "Porta (Cozinha)");
    mvwprintw(info, 25, 2+barra_vertical, valores[8] ? "Aberta" : "Fechada");

    mvwprintw(info, 27, 2, "Janela (Cozinha)");
    mvwprintw(info, 27, 2+barra_vertical, valores[9] ? "Aberta" : "Fechada");

    mvwprintw(info, 29, 2, "Porta (Sala)");
    mvwprintw(info, 29, 2+barra_vertical, valores[10] ? "Aberta" : "Fechada");

    mvwprintw(info, 31, 2, "Janela (Sala)");
    mvwprintw(info, 31, 2+barra_vertical, valores[11] ? "Aberta" : "Fechada");

    mvwprintw(info, 33, 2, "Janela (Quarto 01)");
    mvwprintw(info, 33, 2+barra_vertical, valores[12] ? "Aberta" : "Fechada");

    mvwprintw(info, 35, 2, "Janela (Quarto 02)");
    mvwprintw(info, 35, 2+barra_vertical, valores[13] ? "Aberta" : "Fechada");

    mvwprintw(info, 37, 2, "CSV"); mvwprintw(info, 37, 2+barra_vertical, "Funcionando");

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

float pegar_temperatura(WINDOW *escolhas) {
    float temp;
    bool invalid = false;

    mtx_interface.lock();
    const int num_lines = getmaxy(escolhas);
    mtx_interface.unlock();

    do {
        mtx_interface.lock();
        wmove(escolhas, num_lines-2, 1);
        wclrtoeol(escolhas);
        box(escolhas, 0, 0);
        if(invalid) {
            mvwprintw(escolhas, num_lines-2, 2, "Temperatura deve estar entre 0 e 50");
        }

        wrefresh(escolhas);
        mvwprintw(escolhas, num_lines-3, 2, "Digite a temperatura: ");
        mtx_interface.unlock();
        mvwscanw(escolhas, num_lines-3, 25, " %f", &temp);
        invalid = temp < 0 || temp > 50;
    } while (invalid);

    return temp;
}

void pegar_opcao(WINDOW *escolhas, int socketCliente, int servidorSocket) {
    mtx_interface.lock();
    const int num_lines = getmaxy(escolhas);
    mtx_interface.unlock();

    int opcao;
    bool invalid = false;

    do {
        do {
            mtx_interface.lock();
            wmove(escolhas, num_lines-3, 1);
            wclrtoeol(escolhas);
            wmove(escolhas, num_lines-2, 1);
            wclrtoeol(escolhas);
            box(escolhas, 0, 0);

            if(invalid) {
                mvwprintw(escolhas, num_lines-2, 2, "Escolha deve estar entre 0 e 8");
            }

            wrefresh(escolhas);
            mvwprintw(escolhas, num_lines-3, 2, "Escolha uma opcao: ");
            mtx_interface.unlock();
            mvwscanw(escolhas, num_lines-3, 21, " %d", &opcao);
            invalid = opcao < 0 || opcao > 8;
        } while (invalid);

        if(opcao == 0)
            programa_pode_continuar = false;
        
        if(opcao == 7)
            alarme = 1-alarme;
        
        float temp;

        if(opcao != 7 && opcao != 0)
            send(socketCliente, &opcao, sizeof(opcao), 0);
        
        if(opcao == 8) {
            temp = pegar_temperatura(escolhas);
            send(socketCliente, &temp, sizeof(temp), 0);
        }

        int ligar;

        if(opcao == 5 or opcao == 6 or opcao == 8 or opcao == 0)
            ligar = -1;
        else
            ligar = valores[opcao];

        atualizar_csv(opcao, ligar);
    } while (programa_pode_continuar);

    opcao = 0;
    send(socketCliente, &opcao, sizeof(opcao), 0);
}

void atualizar_valores(int clienteSocket) {
    int bytes_recebidos;
    float temperatura_umidade_servidor[2];
    int valores_servidor[14];

    bytes_recebidos = recv(clienteSocket, temperatura_umidade_servidor, sizeof(temperatura_umidade_servidor), 0);
    if(bytes_recebidos == sizeof(temperatura_umidade_servidor)) {
        float temp = temperatura_umidade_servidor[0], umid = temperatura_umidade_servidor[1];
        if(temp > 0 && temp < 50) {
            temperatura = temp;
        }

        if(umid >= 0 && umid <= 100) {
            umidade = umid;
        }
    }

    bytes_recebidos = recv(clienteSocket, valores_servidor, sizeof(valores_servidor), 0);
    if(bytes_recebidos == sizeof(valores_servidor)) {
        for(int i = 0; i < 14; ++i) {
            if(valores_servidor[i] == 0 or valores_servidor[i] == 1) {
                valores[i] = valores_servidor[i];
            }
        }
    }
}

void thread_atualizacao(WINDOW *menu, WINDOW *info, int clienteSocket) {
    while(programa_pode_continuar) {
        atualizar_valores(clienteSocket);
        atualizar_menu(menu);
        atualizar_info(info);
    }
}

void thread_alarme() {
    while(programa_pode_continuar) {
        if(alarme) {
            bool tocar = (
                valores[6] or
                valores[7] or
                valores[8] or
                valores[9] or
                valores[10] or
                valores[11] or
                valores[12] or
                valores[13]
            );
    
            if(tocar) {
                system("omxplayer alarme.mp3 > /dev/null");
                time_t now = time(0);
                tm *ltm = localtime(&now);
    
                for(int i = 6; i < 14; ++i) {
                    if(valores[i]) {
                        mtx_csv.lock();
                        fprintf(file, "%02d/%02d/%d %02d:%02d:%02d, Alarme, %s\n",
                            ltm->tm_mday,
                            ltm->tm_mon+1,
                            ltm->tm_year+1900,
                            ltm->tm_hour,
                            ltm->tm_min,
                            ltm->tm_sec,
                            sensores[i-6]
                        );
                        mtx_csv.unlock();
                    }
                }
            }
        }
        sleep(2);
    }
}

void atualizar_csv(const int opcao, const int ligou) {
    if(opcao) {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        mtx_csv.lock();
        fprintf(file, "%02d/%02d/%d %02d:%02d:%02d, Usuário, %s%s\n",
            ltm->tm_mday,
            ltm->tm_mon+1,
            ltm->tm_year+1900,
            ltm->tm_hour,
            ltm->tm_min,
            ltm->tm_sec,
            ligou == 1 ? "Ligar " : ligou == 0 ? "Desligar " : "",
            opcoes[opcao]
        );
        mtx_csv.unlock();
    }
}
