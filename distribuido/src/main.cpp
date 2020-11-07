#include "constantes.hpp"
#include "configuracoes.hpp"

using namespace std;

bool programa_pode_continuar = true;

float temperatura = 0.0;
float umidade = 0.0;
float temperatura_usuario = -1.0;

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

int clienteSocket;
int servidorSocket;
int socketCliente;

struct bme280_dev dev;
struct identifier id;

void enviar() {
    float temperatura_umidade[2];
    temperatura_umidade[0] = temperatura;
    temperatura_umidade[1] = umidade;

    int bytes_enviados;

    bytes_enviados = send(socketCliente, temperatura_umidade, sizeof(temperatura_umidade), 0);
    if(bytes_enviados != sizeof(temperatura_umidade)) {
        printf("Erro ao enviar temperatura e umidade\n");
    }

    bytes_enviados = send(socketCliente, valores, sizeof(valores), 0);
    if(bytes_enviados != sizeof(valores)) {
        printf("Erro ao enviar valores\n");
    }
}

void receber() {
    int comando;
    int bytesRecebidos = recv(clienteSocket, &comando, sizeof(comando), 0);
    if(bytesRecebidos < 0) {
        printf("Erro ao receber comando\n");
    }
    else if(bytesRecebidos == 0) {
        printf("Não há dado disponível para comando\n");
    }
    else if(comando != 7) {
        printf("Comando recebido: (%d) %s\n", comando, opcoes[comando]);
        float temp_recebida;
        switch (comando) {
        case 1:
            valores[0] = 1 - valores[0];
            bcm2835_gpio_write(GPIO_LAMPADA_COZINHA, 1-valores[0]);
            break;
        case 2:
            valores[1] = 1 - valores[1];
            bcm2835_gpio_write(GPIO_LAMPADA_SALA, 1-valores[1]);
            break;
        case 3:
            valores[2] = 1 - valores[2];
            bcm2835_gpio_write(GPIO_LAMPADA_QUARTO_01, 1-valores[2]);
            break;
        case 4:
            valores[3] = 1 - valores[3];
            bcm2835_gpio_write(GPIO_LAMPADA_QUARTO_02, 1-valores[3]);
            break;
        case 5:
            valores[0] = valores[1] = valores[2] = valores[3] = 1;
            bcm2835_gpio_write(GPIO_LAMPADA_COZINHA, 0);
            bcm2835_gpio_write(GPIO_LAMPADA_SALA, 0);
            bcm2835_gpio_write(GPIO_LAMPADA_QUARTO_01, 0);
            bcm2835_gpio_write(GPIO_LAMPADA_QUARTO_02, 0);
            break;
        case 6:
            valores[0] = valores[1] = valores[2] = valores[3] = 0;
            bcm2835_gpio_write(GPIO_LAMPADA_COZINHA, 1);
            bcm2835_gpio_write(GPIO_LAMPADA_SALA, 1);
            bcm2835_gpio_write(GPIO_LAMPADA_QUARTO_01, 1);
            bcm2835_gpio_write(GPIO_LAMPADA_QUARTO_02, 1);
            break;
        case 8:
            bytesRecebidos = recv(clienteSocket, &temp_recebida, sizeof(temp_recebida), 0);
            if(bytesRecebidos < 0) {
                printf("Erro ao receber temperatura\n");
            }
            else if(bytesRecebidos == 0) {
                printf("Não há dado disponível para temperatura\n");
            }
            else {
                // conseguiu receber temperatura
                // verifica se é um valor aceitável para o contexto atual
                // 0 <= temp <= 50
                if(0 <= temp_recebida && temp_recebida <= 50) {
                    printf("Temperatura Solicitada: %.1f\n", temp_recebida);
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
    while(programa_pode_continuar) {
        receber();
    }
}

void leitura_sensores_bme280() {
    struct bme280_data comp_data;

    if (bme280_set_sensor_mode(BME280_FORCED_MODE, &dev) == BME280_OK) {
        if (bme280_get_sensor_data(BME280_ALL, &comp_data, &dev) == BME280_OK) {
            float temp_lida = comp_data.temperature;
            float umid_lida = comp_data.humidity;
            if(0 <= temp_lida && temp_lida <= 50) {
                printf("Temperatura lida: %.1f\n", temp_lida);
                temperatura = temp_lida;
            }
            if(0 <= umid_lida && umid_lida <= 100) {
                printf("Umidade lida: %.1f\n", umid_lida);
                umidade = umid_lida;
            }
        }
    }
}

void leitura_sensores_gpio() {
    valores[6] = (bcm2835_gpio_lev(GPIO_SENSOR_SALA) ? 0 : 1);
    valores[7] = (bcm2835_gpio_lev(GPIO_SENSOR_COZINHA) ? 0 : 1);
    valores[8] = (bcm2835_gpio_lev(GPIO_SENSOR_PORTA_COZINHA) ? 0 : 1);
    valores[9] = (bcm2835_gpio_lev(GPIO_SENSOR_JANELA_COZINHA) ? 0 : 1);
    valores[10] = (bcm2835_gpio_lev(GPIO_SENSOR_PORTA_SALA) ? 0 : 1);
    valores[11] = (bcm2835_gpio_lev(GPIO_SENSOR_JANELA_SALA) ? 0 : 1);
    valores[12] = (bcm2835_gpio_lev(GPIO_SENSOR_JANELA_QUARTO_01) ? 0 : 1);
    valores[13] = (bcm2835_gpio_lev(GPIO_SENSOR_JANELA_QUARTO_02) ? 0 : 1);
}

void controlar_ar_condicionado() {
    // se o usuario ainda não definiu uma temperatura
    if(temperatura_usuario < 0)
        return;
    
    // se a temperatura do ambiente estiver maior que a solicitada pelo usuário
    if(temperatura > temperatura_usuario) {
        // e se o ar-condicionado estiver desligado
        if(valores[4] == 0) {
            // liga os dois
            valores[4] = valores[5] = 1;
            bcm2835_gpio_write(GPIO_AR_CONDICIONADO_01, 0);
            bcm2835_gpio_write(GPIO_AR_CONDICIONADO_02, 0);
        }
    }
    else { // caso a temperatura ambiente esteja igual ou menor que a solicitada
        // e se o ar-condicionado estiver ligado
        if(valores[4] == 1) {
            // desliga os dois
            valores[4] = valores[5] = 0;
            bcm2835_gpio_write(GPIO_AR_CONDICIONADO_01, 1);
            bcm2835_gpio_write(GPIO_AR_CONDICIONADO_02, 1);
        }
    }
}

void enviar_valores() {
    while(programa_pode_continuar) {
        leitura_sensores_bme280();
        leitura_sensores_gpio();
        controlar_ar_condicionado();
        enviar();
        sleep(1);
    }
}

int main(int argc, const char *argv[]) {
    int erro = configurar_gpio();
    if(erro) {
        printf("Não foi possível configurar a GPIO\n");
        return 0;
    }

    erro = configurar_sensores(&dev, &id);
    if(erro) {
        printf("Não foi possível configurar os sensores\n");
        return 0;
    }

    erro = iniciar_servidor(&servidorSocket, &socketCliente);
    if(erro) {
        printf("Não foi possível iniciar o servidor\n");
        return 0;
    }
    
    sleep(1);

    erro = iniciar_client(&clienteSocket);
    if(erro) {
        printf("Não foi possível se conectar ao Servidor Central\n");
        return 0;
    }

    thread thread_send(enviar_valores);
    thread thread_recv(receber_comandos);

    thread_send.join();
    thread_recv.join();

    close(clienteSocket);
    close(id.fd);
    close(socketCliente);
    close(servidorSocket);

    return 0;
}
