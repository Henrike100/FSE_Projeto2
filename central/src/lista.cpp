#include "lista.hpp"

int toggle_dispositivo(int *dispositivo) {
    *dispositivo = 1 - *dispositivo;
    return *dispositivo;
}

void ligar(int *dispositivo) {
    *dispositivo = 1;
}

void desligar(int *dispositivo) {
    *dispositivo = 0;
}
