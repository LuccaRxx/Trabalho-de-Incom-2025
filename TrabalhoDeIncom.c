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
// Adiciona um compromisso novo
void adicionar_compromisso() {
    int dia, hora, duracao;
    char descricao[100];
    char local[50];

    printf("\n═════ NOVO COMPROMISSO ═════\n");

    printf("Dia (1-7): ");
    if(scanf("%d", &dia) != 1) { printf("Entrada inválida.\n"); limpar_buffer(); return; }
    limpar_buffer();
    dia--;
    if(dia < 0 || dia >= DIAS) { printf("Dia fora do intervalo.\n"); return; }

    printf("Hora inicial (0-23): ");
    if(scanf("%d", &hora) != 1) { printf("Entrada inválida.\n"); limpar_buffer(); return; }
    limpar_buffer();
    if(hora < 0 || hora >= HORAS) { printf("Hora fora do intervalo.\n"); return; }

    printf("Duração em horas (1-24): ");
    if(scanf("%d", &duracao) != 1) { printf("Entrada inválida.\n"); limpar_buffer(); return; }
    limpar_buffer();
    if(duracao < 1 || duracao > HORAS || hora + duracao > HORAS) { printf("Duração inválida.\n"); return; }

    if(verificar_disponibilidade(dia, hora, duracao)) {
        printf("Conflito de horário. Escolha outro.\n");
        return;
    }

    printf("Descrição: ");
    if(fgets(descricao, sizeof(descricao), stdin) == NULL) return;
    descricao[strcspn(descricao, "\n")] = '\0';

    printf("Local: ");
    if(fgets(local, sizeof(local), stdin) == NULL) return;
    local[strcspn(local, "\n")] = '\0';

    // Preenche primeira hora
    strncpy(agenda[dia][hora].descricao, descricao, sizeof(agenda[dia][hora].descricao)-1);
    agenda[dia][hora].descricao[sizeof(agenda[dia][hora].descricao)-1] = '\0';
    strncpy(agenda[dia][hora].local, local, sizeof(agenda[dia][hora].local)-1);
    agenda[dia][hora].local[sizeof(agenda[dia][hora].local)-1] = '\0';
    agenda[dia][hora].ocupado = 1;
    agenda[dia][hora].duracao = duracao;
    agenda[dia][hora].prioridade = 0;

    // Horas seguintes marcadas como continuação
    for(int i = 1; i < duracao; i++) {
        agenda[dia][hora + i].ocupado = 1;
        agenda[dia][hora + i].duracao = 0; // indica continuação
        agenda[dia][hora + i].prioridade = 0;
        agenda[dia][hora + i].descricao[0] = '\0';
        agenda[dia][hora + i].local[0] = '\0';
    }

    printf("✅ Compromisso adicionado!\n");
}

// Remove compromisso (apenas a partir da hora inicial)
void remover_compromisso() {
    int dia, hora;

    printf("\n═════ REMOVER COMPROMISSO ═════\n");

    printf("Dia (1-7): ");
    if(scanf("%d", &dia) != 1) { printf("Entrada inválida.\n"); limpar_buffer(); return; }
    limpar_buffer();
    dia--;
    if(dia < 0 || dia >= DIAS) { printf("Dia fora do intervalo.\n"); return; }

    printf("Hora inicial (0-23): ");
    if(scanf("%d", &hora) != 1) { printf("Entrada inválida.\n"); limpar_buffer(); return; }
    limpar_buffer();
    if(hora < 0 || hora >= HORAS) { printf("Hora fora do intervalo.\n"); return; }

    if(!agenda[dia][hora].ocupado) { printf("Não há compromisso aqui.\n"); return; }
    if(agenda[dia][hora].duracao == 0) { printf("Esta é continuação. Remova pelo início.\n"); return; }

    int dur = agenda[dia][hora].duracao;
    for(int i = 0; i < dur && hora + i < HORAS; i++) {
        agenda[dia][hora + i].descricao[0] = '\0';
        agenda[dia][hora + i].local[0] = '\0';
        agenda[dia][hora + i].ocupado = 0;
        agenda[dia][hora + i].duracao = 1;
        agenda[dia][hora + i].prioridade = 0;
    }
    printf("✅ Compromisso removido!\n");
}

// Edita um compromisso existente
void editar_compromisso() {
    int dia, hora;
    printf("\n═════ EDITAR COMPROMISSO ═════\n");

    printf("Dia (1-7): ");
    if(scanf("%d", &dia) != 1) { printf("Entrada inválida.\n"); limpar_buffer(); return; }
    limpar_buffer();
    dia--; if(dia < 0 || dia >= DIAS) { printf("Dia inválido.\n"); return; }

    printf("Hora inicial (0-23): ");
    if(scanf("%d", &hora) != 1) { printf("Entrada inválida.\n"); limpar_buffer(); return; }
    limpar_buffer();
    if(hora < 0 || hora >= HORAS) { printf("Hora inválida.\n"); return; }

    if(!agenda[dia][hora].ocupado || agenda[dia][hora].duracao == 0) { printf("Não encontrado (ou é continuação).\n"); return; }

    int dur_atual = agenda[dia][hora].duracao;
    printf("Atual: %s | Local: %s | Duração: %d h\n", agenda[dia][hora].descricao, agenda[dia][hora].local, dur_atual);

    char buffer[120];
    printf("Nova descrição (ENTER mantém): ");
    if(fgets(buffer, sizeof(buffer), stdin)) {
        if(buffer[0] != '\n') {
            buffer[strcspn(buffer, "\n")] = '\0';
            strncpy(agenda[dia][hora].descricao, buffer, sizeof(agenda[dia][hora].descricao)-1);
            agenda[dia][hora].descricao[sizeof(agenda[dia][hora].descricao)-1] = '\0';
        }
    }

    printf("Novo local (ENTER mantém): ");
    if(fgets(buffer, sizeof(buffer), stdin)) {
        if(buffer[0] != '\n') {
            buffer[strcspn(buffer, "\n")] = '\0';
            strncpy(agenda[dia][hora].local, buffer, sizeof(agenda[dia][hora].local)-1);
            agenda[dia][hora].local[sizeof(agenda[dia][hora].local)-1] = '\0';
        }
    }

    printf("Nova duração (0 mantém): ");
    int nova_dur;
    if(scanf("%d", &nova_dur) != 1) { printf("Entrada inválida.\n"); limpar_buffer(); return; }
    limpar_buffer();

    if(nova_dur > 0 && nova_dur != dur_atual) {
        if(nova_dur < dur_atual) {
            // Encurtar: liberar horas excedentes
            for(int i = nova_dur; i < dur_atual && hora + i < HORAS; i++) {
                agenda[dia][hora + i].descricao[0] = '\0';
                agenda[dia][hora + i].local[0] = '\0';
                agenda[dia][hora + i].ocupado = 0;
                agenda[dia][hora + i].duracao = 1;
                agenda[dia][hora + i].prioridade = 0;
            }
            agenda[dia][hora].duracao = nova_dur;
        } else {
            // Aumentar: verificar disponibilidade adicional
            int extra = nova_dur - dur_atual;
            if(verificar_disponibilidade(dia, hora + dur_atual, extra)) {
                printf("Não há espaço para aumentar a duração.\n");
            } else {
                for(int i = dur_atual; i < nova_dur && hora + i < HORAS; i++) {
                    agenda[dia][hora + i].ocupado = 1;
                    agenda[dia][hora + i].duracao = 0;
                    agenda[dia][hora + i].prioridade = agenda[dia][hora].prioridade;
                    agenda[dia][hora + i].descricao[0] = '\0';
                    agenda[dia][hora + i].local[0] = '\0';
                }
                agenda[dia][hora].duracao = nova_dur;
            }
        }
    }

    printf("✅ Compromisso editado!\n");
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
