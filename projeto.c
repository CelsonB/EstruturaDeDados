#include <stdio.h>
#include <stdlib.h>

// variaveis globais
int tempo = 0;
float lucro = 0, lotesPrejuizo = 0;
int count = 0, lotesFora = 0;

typedef struct Produto
{
    int tipo;
    float custo;
    float preco;
    int tempo;
    struct Produto *prox;
} Produto;

typedef struct Maquina
{
    int tipo;
    int marca;
    float preco;
    int consumo;
    int tempo;
    struct Maquina *prox;
} Maquina;

typedef struct Empacotamento
{
    struct Maquina *maquina;
    int relogio;
    struct Produto *esteiraInicio;
    struct Produto *esteiraFim;
    struct Empacotamento *prox;
} Empacotamento;

// funções
void inserirMaquina(Maquina **maquina, int marca, int tipo, int tempo, int consumo, float preco, Empacotamento **empacotamento);
float calcularCusto(Maquina *maquina);
void inserirEsteira(Maquina **maquinas, int tipo, Empacotamento **empacotamento);
void empacotando(Maquina **maquina, int tipo, Empacotamento **empacotamento);
void empacotar(Maquina **maquinas, Empacotamento **empacotamento);
void gastoLuz(Maquina *maquinas);

void main()
{
    Produto *coxinha = NULL, *peixe = NULL, *almondega = NULL;

    Maquina *maquinas = NULL;
    Empacotamento *empacotamento = NULL;

    int modelo = 1;

    while (modelo != 0)
    {
        printf("1 - FishPak\n"
               "2 - ChickenPak\n"
               "3 - Plastific\n"
               "4 - EnSacAll\n"
               "5 - Universal\n"
               "0 - Calcular\n"
               "Informe as máquinas que deseja utilizar: ");
        scanf("%d", &modelo);

        switch (modelo)
        {
            /* 1- coxinha 2- peixe 3- almondega 4- todos
            Cria as máquinas em uma fila*/
        case 1:
            inserirMaquina(&maquinas, 1, 2, 10, 20, 100000.00, &empacotamento);

            break;
        case 2:
            inserirMaquina(&maquinas, 2, 1, 16, 20, 100000.00, &empacotamento);

            break;
        case 3:
            inserirMaquina(&maquinas, 3, 4, 25, 20, 60000.00, &empacotamento);

            break;
        case 4:
            inserirMaquina(&maquinas, 4, 4, 25, 20, 50000.00, &empacotamento);

            break;
        case 5:
            inserirMaquina(&maquinas, 5, 4, 25, 20, 30000.00, &empacotamento);
        }
    }

    int objeto, lotes;

    float custo = calcularCusto(maquinas);
    printf("Custo total: %.2f\n", custo);

    while (lucro < custo && count < 17520) // Compara se chegou a pagar a maquina ou fez 2 anos de operação
    {

        objeto = rand() % 100; /*Gera um número aleatorio utilizado para calcular a probabilidade de cada tipo de lote*/
        if (objeto > 80)
        {
            inserirEsteira(&maquinas, 3, &empacotamento); /*Insere um lote de almondega*/
            lotes++;
            tempo = tempo + 2;
            /*Os lotes entram de 2 em 2 segundos*/
            empacotar(&maquinas, &empacotamento); /*Empacota os lotes, removendo da esteira*/
        }
        else if (objeto > 50)
        {
            inserirEsteira(&maquinas, 2, &empacotamento); /*Insere um lote de peixe*/
            lotes++;
            tempo = tempo + 2;
            empacotar(&maquinas, &empacotamento);
        }
        if (objeto < 50)
        {
            inserirEsteira(&maquinas, 1, &empacotamento); /*Insere um lote de coxinha*/
            lotes++;
            tempo = tempo + 2;
            empacotar(&maquinas, &empacotamento);
        }
        if (tempo % 3600 == 0)
        {
            count++;            /*Conta o numero de horas*/
            gastoLuz(maquinas); /*Calcula o gasto de luz*/
        }
    }

    printf("Lotes empacotados: %d\n", lotes);
    printf("Lotes jogados fora: %d\n", lotesFora);
    printf("Custo de lotes jogados fora: %.2f\n", lotesPrejuizo);
    printf("Tempo gasto (horas): %d\nLucro total: %.2f\n", count, lucro);
}

void inserirMaquina(Maquina **maquina, int marca, int tipo, int tempo, int consumo, float preco, Empacotamento **empacotamento)
{
    Maquina *aux = NULL;
    aux = (Maquina *)malloc(sizeof(Maquina));

    if (aux == NULL)
        return;

    aux->tipo = tipo; /*Cria a nova máquina com todas as informações sobre o modelo*/
    aux->marca = marca;
    aux->preco = preco;
    aux->consumo = consumo;
    aux->tempo = tempo;
    aux->prox = NULL;

    if ((*maquina) == NULL)
    {
        (*maquina) = aux;
    }
    else
    {
        aux->prox = *maquina;
        (*maquina) = aux;
    }

    // /*Insere o lote na fila de empacotamento*/
    if ((*empacotamento) == NULL)
    {
        (*empacotamento) = (Empacotamento *)malloc(sizeof(Empacotamento));
        (*empacotamento)->maquina = (*maquina);
        (*empacotamento)->relogio = (*empacotamento)->relogio + (*empacotamento)->maquina->tempo;
        (*empacotamento)->prox = NULL;
    }
    else
    {
        Empacotamento *aux = (*empacotamento);
        (*empacotamento) = (*empacotamento)->prox;
        (*empacotamento) = (Empacotamento *)malloc(sizeof(Empacotamento));
        (*empacotamento)->maquina = (*maquina);
        (*empacotamento)->relogio = (*empacotamento)->relogio + (*empacotamento)->maquina->tempo;
        (*empacotamento)->prox = aux;
    }
}

float calcularCusto(Maquina *maquina)
{
    /*Calcula o custo total de todas as máquinas inseridas pelo usuario*/
    float custo = 0;
    while (maquina != NULL)
    {
        custo += maquina->preco;
        maquina = maquina->prox;
    }
    return custo;
}

void inserirEsteira(Maquina **maquinas, int tipo, Empacotamento **empacotamento)
{
    /*Busca uma máquina que produza o tipo de lote inserido*/
    Maquina *aux = (*maquinas);

    while (aux != NULL)
    {
        if (aux->tipo == tipo || aux->tipo == 4)
        {
            empacotando(&aux, tipo, empacotamento);
            return;
        }
        else
        {
            aux = aux->prox;
        }
    }
    lotesFora++;
    if (tipo == 1)
    {
        lotesPrejuizo += 0.50;
    }
    else if (tipo == 2)
    {
        lotesPrejuizo += 1.00;
    }
    else if (tipo == 3)
    {
        lotesPrejuizo += 2.00;
    }
}

void empacotando(Maquina **maquina, int tipo, Empacotamento **empacotamento)
{
    Produto *novo = (Produto *)malloc(sizeof(Produto));
    /*Insere um novo lote na esteira de acordo com o tipo*/
    /* 1- coxinha 2- peixe 3- almondega*/

    if (tipo == 1)
    {
        novo->custo = 0.8;
        novo->preco = 1.45;
        novo->tempo = 50 + tempo;
    }
    if (tipo == 2)
    {
        novo->custo = 0.7;
        novo->preco = 2.00;
        novo->tempo = 20 + tempo;
    }
    if (tipo == 3)
    {
        novo->custo = 0.4;
        novo->preco = 0.8;
        novo->tempo = 90 + tempo;
    }

    if ((*empacotamento)->esteiraInicio == NULL)
    {
        (*empacotamento)->esteiraInicio = novo;
        (*empacotamento)->esteiraFim = novo;
        novo->prox = NULL;
    }
    else
    {
        (*empacotamento)->esteiraFim->prox = novo;
        (*empacotamento)->esteiraFim = novo;
    }
}

void empacotar(Maquina **maquinas, Empacotamento **empacotamento)
{

    /*Empacota o lote na esteira*/
    Produto *aux = (*empacotamento)->esteiraInicio;

    if (aux != NULL)
    {
        if (tempo > aux->tempo) // Compara para ver se o tempo de deterioração do produto foi atingido
        {
            lotesFora++;
            lotesPrejuizo = lotesPrejuizo + aux->custo;
            lucro = lucro - aux->custo;
            (*empacotamento)->esteiraInicio = (*empacotamento)->esteiraInicio->prox;
            free(aux);
        }
        else if (tempo > (*empacotamento)->relogio)
        {
            lucro = lucro + (aux->preco);
            (*empacotamento)->esteiraInicio = (*empacotamento)->esteiraInicio->prox;
            free(aux);
        }
    }
    if ((*empacotamento)->esteiraInicio == (*empacotamento)->esteiraFim)
    {
        (*empacotamento)->esteiraInicio = NULL;
        (*empacotamento)->esteiraFim = NULL;
    }
}

void gastoLuz(Maquina *maquinas)
{
    /*Calcula o gasto de luz a cada hora*/
    while (maquinas != NULL)
    {
        if (maquinas->marca == 1 || maquinas->marca == 2)
        {
            lucro = lucro - 20;
        }
        if (maquinas->marca == 3 || maquinas->marca == 5)
        {
            lucro = lucro - 35;
        }
        if (maquinas->marca == 4)
        {
            lucro = lucro - 40;
        }
        maquinas = maquinas->prox;
    }
}
