#ifndef BJ_C_INCLUDED
#define BJ_C_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "bj.h"

no_carta * inic_deck (no_carta *head)
{
    head = (no_carta*)malloc(sizeof(no_carta));
    if (head==NULL)
    {
        printf("erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    else
    {
        return head;
    }
}

/*void carta_value (int Baralhos, no_carta *head)
{
    int Numero_cartas=0, i, a=0;
    carta carta_p;

    Numero_cartas = 52 * Baralhos;
    for(i=0; i<Numero_cartas; i++)
    {
        carta_p.codigo = i % 13;
        carta_p.naipe = a;
        if(carta_p.codigo==12)
        {
            a++;
        }
        if(carta_p.naipe==4)
        {
            a=0;
        }
        insert_list(head, carta_p);
    }

}*/

/*void insert_list(no_carta *head, carta x)
{
    no_carta *aux =NULL;
    no_carta *aux2;
    aux= (no_carta*)malloc(sizeof(no_carta));

    if (aux==NULL)
    {
        printf("erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    if (head == NULL)
    {
        head = aux;
        head->next = NULL;
        head->card = x;
        aux2 = head;
    }
    else
    {
        aux2->next=aux;
        aux->next=NULL;
        aux->card=x;
        aux2 = aux;
    }
    printf("%d %d", aux2.card->codigo, aux2.card->naipe);
}*/

/**Ler parametros
* lê e regista o numero de Baralhos, lê e regista o numero de Jogares
* le o ficheiro da EA
*/

void Ler_Parametros (int *Baralhos, int *jogadores, char *argvector[])
{
    FILE *f_EA;
    FILE *f_param;
    char str[STRING_SIZE];
    char tipo[10];
    int first=1; //flag para diferenciar leitura da primeira linha das restantes


    f_param = fopen(argvector[1], "r");
    if(f_param==NULL)
    {
        printf("ERROR OPENING FILE \n");
        exit(EXIT_FAILURE);
    }

    f_EA = fopen(argvector[2], "r");
    if(f_EA==NULL)
    {
        printf("ERROR OPENING FILE \n");
        exit(EXIT_FAILURE);
    }

    do
    {
        fgets(str, STRING_SIZE, f_param);

        if (first==1)
        {
            sscanf (str, "%d-%d", Baralhos, jogadores);
            first=0;
        }
        else
        {
            //sscanf (str, "%s-%s-%d-%d",);

        }
    }while (feof(f_param)==0);
}

lista_jogadores* inic_lista_jogadores (lista_jogadores *base)
{
    base = (lista_jogadores*)malloc(sizeof(lista_jogadores));
    if (base==NULL)
    {
        printf("erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    else
    {
        base->next_player=NULL;
        return base;
    }

}

void push_lista_jogadores (lista_jogadores *no, jogador x)
{
    lista_jogadores *aux;

    aux= (lista_jogadores*)malloc(sizeof(lista_jogadores));
    if (aux==NULL)
    {
        printf("erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    //aux->next_player=n


}



#endif // BJ_C_INCLUDED
