#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Definições e Constantes ---
#define QTD_TERRITORIOS 5

// --- Structs (Nível Novato) ---
typedef struct {
    char nome[50];
    char corExercito[20];
    int numTropas;
} Territorio;

typedef struct {
    char descricao[100];
    char alvoCor[20];   // Para missão de destruir cor
    int alvoQtd;        // Para missão de conquistar X territórios
    int tipo;           // 1 = Conquistar territórios, 2 = Destruir cor
} Missao;

// --- Protótipos das Funções (Nível Mestre - Modularização) ---
void inicializarMapa(Territorio* mapa); // Automático no Mestre
void exibirMapa(const Territorio* mapa); // const correctness
void atacar(Territorio* mapa); // Lógica Aventureiro
void verificarMissao(const Territorio* mapa, Missao missao);
void limparBuffer();

int main() {
    // Configura a semente para números aleatórios (Nível Aventureiro)
    srand(time(NULL));

    // 1. Alocação Dinâmica com calloc (Nível Aventureiro)
    // calloc inicializa tudo com 0, diferente do malloc
    Territorio* mapa = (Territorio*) calloc(QTD_TERRITORIOS, sizeof(Territorio));

    if (mapa == NULL) {
        printf("Erro de alocação de memória!\n");
        return 1;
    }

    // 2. Inicialização e Definição da Missão
    inicializarMapa(mapa);
    
    // Exemplo de missão fixa: Conquistar 3 territórios (Simulando que o jogador é o Azul)
    Missao missaoAtual = {"Conquistar 3 Territorios com o exercito Azul", "Azul", 3, 1};

    int opcao;
    do {
        printf("\n=== WAR: MENU ESTRATEGICO ===\n");
        printf("1. Atacar\n");
        printf("2. Verificar Missao\n");
        printf("3. Ver Mapa\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
            case 1:
                atacar(mapa);
                break;
            case 2:
                verificarMissao(mapa, missaoAtual);
                break;
            case 3:
                exibirMapa(mapa);
                break;
            case 0:
                printf("Encerrando a guerra...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);

    // 3. Liberação de Memória (Essencial em C)
    free(mapa);

    return 0;
}

// --- Implementação das Funções ---

void inicializarMapa(Territorio* mapa) {
    // Hardcoded para o Nível Mestre (auto-inicia)
    // Se fosse Novato, usaríamos scanf/fgets aqui dentro
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].corExercito, "Azul"); // Jogador
    mapa[0].numTropas = 5;

    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].corExercito, "Vermelho");
    mapa[1].numTropas = 3;

    strcpy(mapa[2].nome, "Argelia");
    strcpy(mapa[2].corExercito, "Verde");
    mapa[2].numTropas = 2;

    strcpy(mapa[3].nome, "Egito");
    strcpy(mapa[3].corExercito, "Amarelo");
    mapa[3].numTropas = 4;

    strcpy(mapa[4].nome, "Franca");
    strcpy(mapa[4].corExercito, "Preto");
    mapa[4].numTropas = 6;
}

void exibirMapa(const Territorio* mapa) {
    printf("\n--- ESTADO ATUAL DO MUNDO ---\n");
    for(int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n", 
            i+1, mapa[i].nome, mapa[i].corExercito, mapa[i].numTropas);
    }
}

void atacar(Territorio* mapa) {
    int idxAtk, idxDef;
    
    exibirMapa(mapa);
    printf("\nEscolha o indice do territorio ATACANTE (1-5): ");
    scanf("%d", &idxAtk);
    printf("Escolha o indice do territorio DEFENSOR (1-5): ");
    scanf("%d", &idxDef);
    
    // Ajuste de índice (usuário digita 1-5, vetor é 0-4)
    idxAtk--; 
    idxDef--;

    // Validações simples
    if (idxAtk < 0 || idxAtk >= QTD_TERRITORIOS || idxDef < 0 || idxDef >= QTD_TERRITORIOS) {
        printf("Territorios invalidos!\n");
        return;
    }
    if (mapa[idxAtk].numTropas <= 1) {
        printf("O atacante nao tem tropas suficientes para atacar (minimo 2)!\n");
        return;
    }

    printf("\n--- BATALHA INICIADA! ---\n");
    printf("%s vs %s\n", mapa[idxAtk].nome, mapa[idxDef].nome);

    // Simulação de Dados (1 a 6)
    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("Dados: Atacante [%d] x Defensor [%d]\n", dadoAtk, dadoDef);

    // Regra do Desafio: Empates favorecem o ATACANTE
    if (dadoAtk >= dadoDef) {
        printf("Vitoria do ATACANTE!\n");
        mapa[idxDef].numTropas--;
        
        // Lógica de Conquista
        if (mapa[idxDef].numTropas == 0) {
            printf("TERRITORIO CONQUISTADO! %s agora pertence ao exercito %s.\n", 
                mapa[idxDef].nome, mapa[idxAtk].corExercito);
            // Muda a cor e move 1 tropa
            strcpy(mapa[idxDef].corExercito, mapa[idxAtk].corExercito);
            mapa[idxDef].numTropas = 1;
            mapa[idxAtk].numTropas--;
        }
    } else {
        printf("Vitoria da DEFESA! Atacante perde 1 tropa.\n");
        mapa[idxAtk].numTropas--;
    }
}

void verificarMissao(const Territorio* mapa, Missao missao) {
    printf("\n--- MISSAO: %s ---\n", missao.descricao);
    
    if (missao.tipo == 1) { // Conquistar X territorios com a cor alvo
        int contagem = 0;
        for (int i = 0; i < QTD_TERRITORIOS; i++) {
            if (strcmp(mapa[i].corExercito, missao.alvoCor) == 0) {
                contagem++;
            }
        }
        printf("Progresso: %d/%d territorios controlados por %s.\n", contagem, missao.alvoQtd, missao.alvoCor);
        
        if (contagem >= missao.alvoQtd) {
            printf("🏆 PARABENS! MISSAO CUMPRIDA! 🏆\n");
        } else {
            printf("Ainda falta conquistar mais territorios.\n");
        }
    }
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}