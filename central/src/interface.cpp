#include "interface.hpp"

FILE *file;

int clienteSocket, servidorSocket;

bool programa_pode_continuar = true;

float temperatura = 32.1;
float umidade = 73.1;

mutex mtx_interface;

unsigned int clienteLength;
struct sockaddr_in clienteAddr;
int socketCliente;

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

int iniciar_conexao_servidor() {
    if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        return 1;
    }

    struct sockaddr_in servidorAddr;

    memset(&servidorAddr, 0, sizeof(servidorAddr));
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(10028);

    if(bind(servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0) {
        return 2;
    }

    if(listen(servidorSocket, 10) < 0) {
        return 3;
    }

    if((socketCliente = accept(servidorSocket, (struct sockaddr *) &clienteAddr, &clienteLength)) < 0) {
        return 4;
    }

    return 0;
}

int iniciar_conexao_cliente() {
    if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        return 1;
    }

    struct sockaddr_in servidorAddr;
    memset(&servidorAddr, 0, sizeof(servidorAddr));
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr("192.168.0.52");
	servidorAddr.sin_port = htons(10128);

    if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0) {
        return 2;
    }

    return 0;
}

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
    mvwprintw(menu, 16, 7, "%s%s", valores[6] ? "Desligar " : "Ligar ", opcoes[7]);
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

void pegar_opcao(WINDOW *escolhas) {
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

        clienteLength = sizeof(clienteAddr);

        send(clienteSocket, &opcao, sizeof(opcao), 0);

        //atualizar_csv(opcao, ligar);
    } while (programa_pode_continuar);

    fclose(file);
    close(socketCliente);
    close(servidorSocket);
}

void atualizar_valores() {
    int bytes_recebidos;
    float temperatura_umidade_servidor[2];
    int valores_servidor[15];

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
        for(int i = 0; i < 15; ++i) {
            if(valores_servidor[i] == 0 or valores_servidor[i] == 1) {
                valores[i] = valores_servidor[i];
            }
        }
    }
}

void thread_atualizacao(WINDOW *menu, WINDOW *info) {
    while(programa_pode_continuar) {
        atualizar_valores();
        atualizar_menu(menu);
        atualizar_info(info);
    }

    close(clienteSocket);
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
            ligou == 1 ? "Ligar " : ligou == 0 ? "Desligar " : "",
            opcoes[opcao]
        );
    }
}
