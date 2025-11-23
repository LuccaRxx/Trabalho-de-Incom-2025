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

void inicializar_agenda();
    
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

// Zera toda a agenda
void inicializar_agenda() {
    int i, j;
    for(i = 0; i < DIAS; i++) {
        for(j = 0; j < HORAS; j++) {
            agenda[i][j].descricao[0] = '\0';
            agenda[i][j].local[0] = '\0';
            agenda[i][j].ocupado = 0;
            agenda[i][j].duracao = 1;
            agenda[i][j].prioridade = 0;
        }
    }
}

// Menu bonitinho
void mostrar_menu() {
    printf("\n          MENU              ");
    printf("\n 1 - ➕ Novo compromisso    ");
    printf("\n 2 - ❌ Remover             ");
    printf("\n 3 - 👀 Ver agenda          ");
    printf("\n 4 - ✏️  Editar             ");
    printf("\n 5 - ⭐ Prioridade          ");
    printf("\n 0 - 🚪 Sair                ");
}

// Adiciona novo compromisso
void adicionar_compromisso() {
    int dia, hora, duracao;
    char descricao[100], local[50];
    
    printf("\n═════ NOVO COMPROMISSO ═════\n");
    
    // Mostra os dias disponíveis
    printf("\nDias da semana:\n");
    for(int i = 0; i < DIAS; i++) {
        printf("%d - %s\n", i+1, nomes_dias[i]);
    }
    
    // Pega o dia
    printf("\nEscolha o dia (1-7): ");
    scanf("%d", &dia);
    limpar_buffer();
    
    if(dia < 1 || dia > 7) {
        printf("Dia inválido!\n");
        return;
    }
    dia--; // Ajusta pro índice do array
    
    // Pega a hora
    printf("Hora de início (0-23): ");
    scanf("%d", &hora);
    limpar_buffer();
    
    if(hora < 0 || hora > 23) {
        printf("Hora inválida!\n");
        return;
    }
    
    // Pega a duração
    printf("Duração (em horas): ");
    scanf("%d", &duracao);
    limpar_buffer();
    
    if(duracao < 1 || duracao > 24) {
        printf("Duração inválida! (1-24 horas)\n");
        return;
    }
    
    // Verifica se tá livre
    if(verificar_disponibilidade(dia, hora, duracao)) {
        printf("\n❌ Conflito de horário! Já tem compromisso nesse período.\n");
        return;
    }
    
    // Pega os detalhes
    printf("Descrição: ");
    fgets(descricao, sizeof(descricao), stdin);
    descricao[strcspn(descricao, "\n")] = '\0';
    
    printf("Local: ");
    fgets(local, sizeof(local), stdin);
    local[strcspn(local, "\n")] = '\0';
    
    // Salva na agenda
    strcpy(agenda[dia][hora].descricao, descricao);
    strcpy(agenda[dia][hora].local, local);
    agenda[dia][hora].ocupado = 1;
    agenda[dia][hora].duracao = duracao;
    agenda[dia][hora].prioridade = 0; // Começa como normal
    
    // Marca as horas seguintes como continuação
    for(int i = 1; i < duracao; i++) {
        if(hora + i < HORAS) {
            agenda[dia][hora + i].ocupado = 1;
            strcpy(agenda[dia][hora + i].descricao, "(continuação)");
            strcpy(agenda[dia][hora + i].local, local);
            agenda[dia][hora + i].duracao = 0; // 0 = é continuação
            agenda[dia][hora + i].prioridade = agenda[dia][hora].prioridade;
        }
    }
    
    printf("\n✅ Compromisso adicionado com sucesso!\n");
}


void removerCompromisso() {
    int dia, hora, i, dur;
    
    printf("\n--- Remover Compromisso ---\n");
    
    for(i = 0; i < 7; i++) {
        printf("%d - %s\n", i+1, dias[i]);
    }
    
    printf("Dia (1-7): ");
    scanf("%d", &dia);
    getchar();
    dia = dia - 1;
    
    printf("Hora (0-23): ");
    scanf("%d", &hora);
    getchar();
    
    if(agenda[dia][hora].ocupado == 0) {
        printf("\nNao tem compromisso nesse horario!\n");
        return;
    }
    
    if(agenda[dia][hora].duracao == 0) {
        printf("\nEsse e uma continuacao. Remova pelo inicio!\n");
        return;
    }
    
    dur = agenda[dia][hora].duracao;
    
    // remove tudo
    for(i = 0; i < dur; i++) {
        if(hora + i < 24) {
            agenda[dia][hora + i].descricao[0] = '\0';
            agenda[dia][hora + i].local[0] = '\0';
            agenda[dia][hora + i].ocupado = 0;
            agenda[dia][hora + i].duracao = 1;
        }
    }
    
    printf("\nCompromisso removido!\n");
}

void ver_agenda() {
    int tem_compromisso = 0;
    
    printf("\n═════ AGENDA COMPLETA ═════\n");
    
    for(int dia = 0; dia < DIAS; dia++) {
        int dia_tem_compromisso = 0;
        
        for(int hora = 0; hora < HORAS; hora++) {
            if(agenda[dia][hora].ocupado && agenda[dia][hora].duracao > 0) {
                if(!dia_tem_compromisso) {
                    printf("\n📅 **%s**\n", nomes_dias[dia]);
                    printf("────────────────────────────\n");
                    dia_tem_compromisso = 1;
                    tem_compromisso = 1;
                }
                
                printf("🕐 %02d:00-%02d:00", hora, hora + agenda[dia][hora].duracao);
                
                if(agenda[dia][hora].prioridade) {
                    printf(" ⭐ PRIORIDADE");
                }
                
                printf("\n📝 %s\n", agenda[dia][hora].descricao);
                printf("📍 %s\n\n", agenda[dia][hora].local);
            }
        }
    }
    
    if(!tem_compromisso) {
        printf("\nNada marcado ainda... Que tal adicionar um compromisso?\n");
    }
}

// Edita compromisso existente
void editar_compromisso() {
    int dia, hora;
    char nova_desc[100], novo_local[50];
    
    printf("\n═════ EDITAR COMPROMISSO ═════\n");
    
    printf("\nDia (1-7): ");
    scanf("%d", &dia);
    limpar_buffer();
    dia--;
    
    printf("Hora (0-23): ");
    scanf("%d", &hora);
    limpar_buffer();
    
    if(!agenda[dia][hora].ocupado || agenda[dia][hora].duracao == 0) {
        printf("Compromisso não encontrado!\n");
        return;
    }
    
    printf("\nEditando: %s\n", agenda[dia][hora].descricao);
    printf("Local atual: %s\n\n", agenda[dia][hora].local);
    
    // Nova descrição
    printf("Nova descrição (enter pra pular): ");
    fgets(nova_desc, sizeof(nova_desc), stdin);
    nova_desc[strcspn(nova_desc, "\n")] = '\0';
    
    // Novo local
    printf("Novo local (enter pra pular): ");
    fgets(novo_local, sizeof(novo_local), stdin);
    novo_local[strcspn(novo_local, "\n")] = '\0';
    
    // Atualiza se não estiver vazio
    if(strlen(nova_desc) > 0) {
        strcpy(agenda[dia][hora].descricao, nova_desc);
    }
    
    if(strlen(novo_local) > 0) {
        strcpy(agenda[dia][hora].local, novo_local);
        
        // Atualiza local nas continuações também
        for(int i = 1; i < agenda[dia][hora].duracao; i++) {
            if(hora + i < HORAS) {
                strcpy(agenda[dia][hora + i].local, novo_local);
            }
        }
    }
    
    printf("✅ Compromisso atualizado!\n");
}

// Muda prioridade
void mudar_prioridade() {
    int dia, hora, opcao;
    
    printf("\n═════ MUDAR PRIORIDADE ═════\n");
    
    printf("\nDia (1-7): ");
    scanf("%d", &dia);
    limpar_buffer();
    dia--;
    
    printf("Hora (0-23): ");
    scanf("%d", &hora);
    limpar_buffer();
    
    if(!agenda[dia][hora].ocupado || agenda[dia][hora].duracao == 0) {
        printf("Compromisso não encontrado!\n");
        return;
    }
    
    printf("\nCompromisso: %s\n", agenda[dia][hora].descricao);
    printf("Prioridade atual: %s\n", 
        agenda[dia][hora].prioridade ? "⭐ ALTA" : "Normal");
    
    printf("\n1 - Marcar como ⭐ PRIORIDADE\n");
    printf("2 - Voltar para Normal\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    limpar_buffer();
    
    int nova_prioridade = (opcao == 1) ? 1 : 0;
    agenda[dia][hora].prioridade = nova_prioridade;
    
    // Aplica pra todas as horas do compromisso
    for(int i = 0; i < agenda[dia][hora].duracao; i++) {
        if(hora + i < HORAS) {
            agenda[dia][hora + i].prioridade = nova_prioridade;
        }
    }
    
    printf("✅ Prioridade atualizada!\n");
}

// Verifica se o horário tá livre
int verificar_disponibilidade(int dia, int hora, int duracao) {
    for(int i = 0; i < duracao; i++) {
        if(hora + i >= HORAS) {
            return 1; // Inválido - passa do dia
        }
        if(agenda[dia][hora + i].ocupado) {
            return 1; // Ocupado
        }
    }
    return 0; // Tudo livre
}

void limpar_buffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}
