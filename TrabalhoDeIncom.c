#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Defines pra facilitar a vida
#define DIAS 7
#define HORAS 24

// Estrutura do compromisso
struct Compromisso {
    char descricao[100];
    char local[50];
    int ocupado;        // 0 = livre, 1 = ocupado
    int duracao;        // em horas
    int prioridade;     // 0 = normal, 1 = alta
};
