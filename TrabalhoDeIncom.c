// Trabalho de Incom - Agenda Semanal
// Grupo: Eduardo, Lucca, Marcos e Isabela

#include <stdio.h>
#include <string.h>

#define DIAS 7
#define HORAS 24

// estrutura  dos compromisso
struct Compromisso {
    char descricao[100];
    char local[50];
    int ocupado;    
    int duracao;   
    int prioridade; 
};

struct Compromisso agenda[DIAS][HORAS];

char *nomes_dias[] = {"Segunda","Terca","Quarta","Quinta","Sexta","Sabado","Domingo"};

void inicializar_agenda();
void mostrar_menu();
void adicionar_compromisso();
void remover_compromisso();
void ver_agenda();
void editar_compromisso();
void mudar_prioridade();
int verificar_disponibilidade(int dia, int hora, int duracao);

// eduardo -> função principal
int main() {

    int opcao;

    printf("=== AGENDA SEMANAL ===\n");

    inicializar_agenda();

    do{
        mostrar_menu();
        printf("\nOpcao: ");
        scanf("%d",&opcao);

        switch(opcao){
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
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }

    } while(opcao != 0);

    return 0;
}

//  Marcos -> funcao que inicializa agenda vazia
void inicializar_agenda(){
    for(int i=0;i<DIAS;i++){
        for(int j=0;j<HORAS;j++){
            agenda[i][j].ocupado = 0;
            agenda[i][j].descricao[0] = '\0';
            agenda[i][j].local[0] = '\0';
            agenda[i][j].duracao = 1;
            agenda[i][j].prioridade = 0;
        }
    }
}

// Marcos -> funcao que mostra o menu principal
void mostrar_menu(){
    printf("\n--- MENU ---\n");
    printf("1 - Adicionar compromisso\n");
    printf("2 - Remover compromisso\n");
    printf("3 - Ver agenda\n");
    printf("4 - Editar compromisso\n");
    printf("5 - Mudar prioridade\n");
    printf("0 - Sair\n");
}

// Isabela -> função que adiciona compromisso a agenda
void adicionar_compromisso() {

    int dia, hora, duracao;
    char temp;

    printf("\nAdicionar compromisso\n");

    printf("Dia (1-7): ");
    scanf("%d",&dia);
    dia--;

    if(dia < 0 || dia >= DIAS){
        printf("Dia invalido.\n");
        return;
    }

    printf("Hora (0-23): ");
    scanf("%d",&hora);

    if(hora < 0 || hora >= HORAS){
        printf("Hora invalida.\n");
        return;
    }

    printf("Duracao em horas: ");
    scanf("%d",&duracao);

    if(hora + duracao > HORAS){
        printf("Passou do horario do dia.\n");
        return;
    }

    if(verificar_disponibilidade(dia, hora, duracao)){
        printf("Ja tem compromisso nesse horario.\n");
        return;
    }

    // limpar buffer
    while((temp = getchar()) != '\n' && temp != EOF);

    printf("Descricao: ");
    fgets(agenda[dia][hora].descricao,100,stdin);
    agenda[dia][hora].descricao[strlen(agenda[dia][hora].descricao)-1]='\0';

    printf("Local: ");
    fgets(agenda[dia][hora].local,50,stdin);
    agenda[dia][hora].local[strlen(agenda[dia][hora].local)-1]='\0';

    agenda[dia][hora].ocupado = 1;
    agenda[dia][hora].duracao = duracao;

    for(int i=1; i<duracao; i++){
        agenda[dia][hora+i].ocupado = 1;
        agenda[dia][hora+i].duracao = 0; 
    }

    printf("Compromisso adicionado.\n");
}

// lucca -> função que remove compromisso da agenda
void remover_compromisso(){
    int dia, hora;

    printf("\nRemover compromisso\n");

    printf("Dia (1-7): ");
    scanf("%d",&dia);
    dia--;

    printf("Hora (0-23): ");
    scanf("%d",&hora);

    if(!agenda[dia][hora].ocupado){
        printf("Nao tem nada aqui.\n");
        return;
    }

    if(agenda[dia][hora].duracao == 0){
        printf("Esse horario é parte de outro compromisso. Tire pelo horario inicial.\n");
        return;
    }

    int d = agenda[dia][hora].duracao;

    for(int i=0; i<d; i++){
        agenda[dia][hora+i].ocupado = 0;
        agenda[dia][hora+i].descricao[0] = '\0';
        agenda[dia][hora+i].local[0] = '\0';
        agenda[dia][hora+i].duracao = 1;
        agenda[dia][hora+i].prioridade = 0;
    }

    printf("Removido.\n");
}

// Eduardo -> funcao que visualizaa a agenda como um todo
void ver_agenda(){

    printf("\n--- Agenda ---\n");

    int vazio = 1;

    for(int d=0; d<DIAS; d++){
        int mostrou = 0;

        for(int h=0; h<HORAS; h++){

            if(agenda[d][h].ocupado && agenda[d][h].duracao > 0){

                vazio = 0;

                if(!mostrou){
                    printf("\n%s:\n", nomes_dias[d]);
                    mostrou = 1;
                }

                printf("Horario %dh - %dh\n", h, h + agenda[d][h].duracao);
                printf("Descricao: %s\n", agenda[d][h].descricao);
                printf("Local: %s\n", agenda[d][h].local);

                if(agenda[d][h].prioridade == 1){
                    printf("Prioridade alta\n");
                }
            }
        }
    }

    if(vazio){
        printf("Nenhum compromisso ainda.\n");
    }
}

// Isabela -> funcao que edita os compromissos da agenda
void editar_compromisso(){

    int dia, hora;
    char temp;

    printf("\nEditar compromisso\n");

    printf("Dia (1-7): ");
    scanf("%d",&dia);
    dia--;

    printf("Hora: ");
    scanf("%d",&hora);

    if(!agenda[dia][hora].ocupado || agenda[dia][hora].duracao == 0){
        printf("Nao tem compromisso inicial aqui.\n");
        return;
    }

    while((temp = getchar()) != '\n' && temp != EOF);

    printf("Nova descricao: ");
    fgets(agenda[dia][hora].descricao,100,stdin);
    agenda[dia][hora].descricao[strlen(agenda[dia][hora].descricao)-1]='\0';

    printf("Novo local: ");
    fgets(agenda[dia][hora].local,50,stdin);
    agenda[dia][hora].local[strlen(agenda[dia][hora].local)-1]='\0';

    printf("Atualizado.\n");
}

// Marcos -> funcao que muda a prioridade dos compromissos da agenda
void mudar_prioridade(){
    int dia, hora;

    printf("\nAlterar prioridade\n");

    printf("Dia (1-7): ");
    scanf("%d",&dia);
    dia--;

    printf("Hora (0-23): ");
    scanf("%d",&hora);

    if(!agenda[dia][hora].ocupado){
        printf("Sem compromisso.\n");
        return;
    }

    printf("1 - Alta\n2 - Normal\nNova prioridade: ");

    int op;
    scanf("%d",&op);

    int p = (op == 1);

    int dur = agenda[dia][hora].duracao;

    for(int i=0; i<dur; i++){
        agenda[dia][hora+i].prioridade = p;
    }

    printf("Alterado.\n");
}

// Lucca -> funcao que verifica se o horario ta disponivel
int verificar_disponibilidade(int dia, int hora, int duracao){
    for(int i=0; i<duracao; i++){
        if(agenda[dia][hora+i].ocupado){
            return 1;
        }
    }
    return 0;
}
