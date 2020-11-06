#ifndef CONFIGURACOES_HPP
#define CONFIGURACOES_HPP

#include "gpio.hpp"
#include "utilidades.hpp"

int configurar_sensores(struct bme280_dev *dev, struct identifier *id);
int configurar_gpio();
int iniciar_servidor(int *servidorSocket, int *socketCliente);
int iniciar_client(int *clienteSocket);

#endif // CONFIGURACOES_HPP
