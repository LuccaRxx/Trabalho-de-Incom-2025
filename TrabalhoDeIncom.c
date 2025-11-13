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


// Agenda global - matriz de dias x horas
struct Compromisso agenda[DIAS][HORAS];

// Nomes dos dias pra mostrar bonitinho
char *nomes_dias[] = {"Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado", "Domingo"};

// Protótipos das funções
void inicializar_agenda();
void mostrar_menu();
void adicionar_compromisso();
void remover_compromisso();
void ver_agenda();
void editar_compromisso();
void mudar_prioridade();
int verificar_disponibilidade(int dia, int hora, int duracao);
void limpar_buffer();


int main() {
    int opcao;
    
    printf("\n");
    printf("══════════════════════════════\n");
    printf("        AGENDA SEMANAL         \n");
    printf("══════════════════════════════\n");
    
    // Inicializa tudo zerado
    inicializar_agenda();
    
    // Loop principal
    do {
        mostrar_menu();
        printf("\nEscolha uma opção: ");
        
        if(scanf("%d", &opcao) != 1) {
            printf("Opção inválida! Digite um número.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();
        
        // Switch pra ficar mais organizado
        switch(opcao) {
            case 1:
                adicionar_compromisso();
                break;
            case 2:
                remover_compromisso();
                break;
            case 3:
                ver_agenda();
                break;
            case 4:
                editar_compromisso();
                break;
            case 5:
                mudar_prioridade();
                break;
            case 0:
                printf("\nAté mais! Saindo da agenda...\n");
                break;
            default:
                printf("Opção não existe! Tente de novo.\n");
        }
        
    } while(opcao != 0);
    
    return 0;
}
