#include "lista.hpp"

void toggle_dispositivo(int *dispositivo) {
    *dispositivo = 1 - *dispositivo;
}

void ligar(int *dispositivo) {
    *dispositivo = 1;
}

void desligar(int *dispositivo) {
    *dispositivo = 0;
}
