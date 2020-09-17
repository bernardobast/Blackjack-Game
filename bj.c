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

const char myName[] = "Prof. Joao Ascenso";
const char myNumber[] = "IST11111";
const char * playerNames[] = {"Player 1", "Player 2", "Player 3", "Player 4"};


void free_matriz(int **mat_EA)
{
    int i=0;
    for(i=0;mat_EA[i]!=NULL; i++)
    {
        free(mat_EA[i]);
    }
    free(mat_EA);
}


int calcular_posicao(int x, int y, lista_jogadores *primeiro_jogador)
{
    int position=99;

    if((x<210 && x>11)&&(y>290 && y <500) && find_player(primeiro_jogador,0)==NULL)
    {
        position=0;
    }
    if((x<425 && x>220)&&(y>290 && y <500)&& find_player(primeiro_jogador,1)==NULL)
    {
        position=1;
    }
    if((x<630 && x>430)&&(y>290 && y <500)&& find_player(primeiro_jogador,2)==NULL)
    {
        position=2;
    }
    if((x<840 && x>635)&&(y>290 && y <500)&& find_player(primeiro_jogador,4)==NULL)
    {
        position=3;
    }
    return position;
}
/**high_low_bet: Recebe o valor obtido através da contagem de cartas e de acordo com este redefine a aposta dos jogadores EA
*\param high_low_value valor obtido atracés da contagem de cartas
*\param head cabeça do deck
*/

void high_low_bet(int high_low_value, no_carta *head, lista_jogadores *base)
{
    lista_jogadores *aux;
    aux=base;
    int numero_cartas=0, numero_baralhos=0;
    int bet_decision=0;
    numero_cartas=conta_cartas_no_deck(head);

    numero_baralhos=numero_cartas/52; //calcula o numero de baralhos

    printf("Numero Baralhos=%d\n", numero_baralhos);

    bet_decision=high_low_value/numero_baralhos; //define o novo valor da aposta de acordo com a estrategia high_low

    for(;aux!=NULL; aux=aux->next_player)
    {
        if((bet_decision>0)&&(strcmp(aux->player.tipo, "EA")==0))
        {
           aux->player.aposta=aux->player.aposta_inicial*2*bet_decision;
        }
    }
}
/** high_low: Lê a mão de cada jogador e conta cartas
*\param jogador_Atual Estrutura do jogador que esta atualmente a jogar
*\param high_low_value Valor obtido atraves da contagem de cartas
*/

void high_low (lista_jogadores *jogador_Atual, int *high_low_value)
{
    no_carta * hand;
    hand=jogador_Atual->player.lista_hand;
    for(;hand!=NULL; hand=hand->next)
    {
        if(hand->card.value==10 || hand->card.value==11)
        {
            (*high_low_value)-=1;
        }
        if(hand->card.value==2||hand->card.value==3||hand->card.value==4||hand->card.value==5||hand->card.value==6)
        {
            (*high_low_value)+=1;
        }
    }
    printf("HIGH_LOW=%d\n", (*high_low_value));
}

/**aloca_matriz: Reserva a memória necessária para a matriz e retorna um apontador para a mesma
*\param mat_EA: Matriz com a estratégia das entidades artificias
*/
int** aloca_matriz (int **mat_EA)
{
    int aux;
    mat_EA=(int**)malloc(MAX_LINE*sizeof(int*));
    for (aux=0;aux<MAX_LINE;aux++)
    {
        mat_EA[aux]=(int*)malloc(MAX_COLLUMN*sizeof(int));
    }

    return mat_EA;
}

/**conta_cartas_no_deck: Percorre o deck e conta o numero de cartas retornando esse valor
*\param head: topo do deck
*/

int conta_cartas_no_deck (no_carta *head)
{
    no_carta *aux;
    int n_cartas=0;
    for (aux=head;aux!=NULL;aux=aux->next)
    {
        n_cartas++; //regista o numero de cartas
    }
    return n_cartas;
}

/** free_deck: Percorre a lista e liberta a memória reservada para o deck
*\param head: topo do deck
*/

void free_deck (no_carta *head)
{
    no_carta *aux, *aux2;

    for (aux=head;aux!=NULL;aux=aux2)
    {
        aux2=aux->next;
        free(aux);
    }
}
/** print_matriz: Recebe e imprime todas as linhas da matriz dos EA
*\param matriz apontador para a matriz dos EA
*/

void print_matriz (int **matriz)
{
    int i, j;

    for (j=0;j<MAX_LINE;j++)
    {
        for (i=0;i<MAX_COLLUMN;i++)
        {
            printf ("[%d]",matriz[j][i]);
        }
        printf("\n");
    }
}
/**
*Recebe um inteiro retirado da matriz da entidade artificial
*este inteiro representa a decisão que a entidade vai realizar
*em função dos dados do jagador toma a decisão
**/
void accao_EA (int decisao, lista_jogadores *current, no_carta **head_deck)
{
    if (current->player.status==0) ///jogador actual ainda está a jogar
    {
        if (decisao==0) //HIT
        {
            da_uma_carta (head_deck, current);
        }
        if (decisao==1) //Double / Hit
        {
            if (current->player.numero_de_cartas==2 && current->player.dinheiro>=2*current->player.aposta) ///pode fazer double
            {
                da_uma_carta (head_deck,current);
                current->player.flag_double=1;
                current->player.status=2;
            }
            else //nao pode fazer double
            {
                da_uma_carta (head_deck,current);
            }
        }
        if (decisao==2) //STAND
        {
            current->player.status=2;
        }
        if (decisao==3) //surrender/stand
        {
            if (current->player.numero_de_cartas==2)///pode fazer surrender
            {
                current->player.status=4;
            }
            if (current->player.numero_de_cartas>2)///nao pode fazer surrender
            {
                current->player.status=2;
            }
        }
        if (decisao==4)//DOUBLE/STAND
        {
            if (current->player.numero_de_cartas==2 && current->player.dinheiro>=2*current->player.aposta) ///pode fazer double
            {
                da_uma_carta (head_deck,current);
                current->player.flag_double=1;
                current->player.status=2;
            }
            else
            {
                current->player.status=2;
            }
        }
    }
    update_status (current);


}
/**decisao_EA
*\param matriz, apontador para a matriz dos EA
*\param actual, apontador para o jogador que esta a jogar
*\param casa, apontador para a casa
*/
int decisao_EA (int **mat, lista_jogadores *actual, lista_jogadores *casa)
{
    int linha=0, coluna=0, soft=0;
    carta carta_n1_casa;

    carta_n1_casa=casa->player.lista_hand->card;

    coluna=carta_n1_casa.value-2;
    printf ("pontos=%d\n",actual->player.pontos);

    //Se o jogador tiver um Às a estragia a adotar é Soft
    if (actual->player.flag_soft==1)
    {
        soft=1;             //flag que activa quando a estrategia a adotar é soft
        linha=linha+10;
    }

    //Se o jogador não tiver ases a estrategia é hard
    if (soft==0)
    {
        linha=actual->player.pontos-8;
        if (linha<0)
        {
            linha=0;
        }
        if (linha>9)
        {
            linha=9;
        }
    }

    //Se o jogador tive às a estrategia a adoptar é soft
    if (soft==1)
    {
        linha=linha+actual->player.pontos-12;
        if (linha>17)
        {
            linha=17;
        }
        if (linha<10)
        {
            linha=10;
        }
    }
    ///printf ("linha=%d coluna=%d\n", linha, coluna);
    return mat[linha][coluna];

}

/** Preencher_matriz: Recebe um ficheiro e preenche a matriz de acordo com o ficheiro (0- Hit, 1- Double/hit, 2- Stand, 3-Stand, 4- Double/stand)
* \param fp recebe o ficheiro EA.txt aberto na função main
*\param matriz recebe a matriz a ser preenchida
*/

void Preencher_matriz(FILE *fp, int **Matriz)
{
    char str[STRING_SIZE];
    int i=0;
    for(i=0;i<MAX_LINE;i++)
    {
        fgets(str, STRING_SIZE, fp);
        sscanf(str,"%d %d %d %d %d %d %d %d %d %d", &Matriz[i][0], &Matriz[i][1], &Matriz[i][2], &Matriz[i][3], &Matriz[i][4], &Matriz[i][5], &Matriz[i][6], &Matriz[i][7], &Matriz[i][8], &Matriz[i][9]);
    }
}

/** numero_de_jogadores: Percorre a lista de jogadores e conta o numero de jogadores que se encontram a jogar
*\param primeiro_jogador, recebe o primeiro jogador da lista
*/

int numero_de_jogadores (lista_jogadores *primeiro_jogador)
{
    int n_jogadores=1;
    lista_jogadores *aux;
    for (aux=primeiro_jogador;aux->next_player!=NULL;aux=aux->next_player)
    {
        n_jogadores++;
    }
    return n_jogadores;
}

/** novo_jogador: Adiciona um novo jogador e pergunta os seus dados
*\param topo, base da lista de jogadores
*/

void novo_jogador (lista_jogadores *topo, int posicao)
{
    jogador jog_aux;
    char input[STRING_SIZE];
    int valid_input=0;
    int aux_int=0, retorno=0;

    printf ("Tipo do jogador:\n");
    do
    {
        fgets (input, STRING_SIZE, stdin);
        input[strlen(input)-1]='\0';
        if (strcmp(input,"EA")==0 || strcmp(input,"HU")==0)
        {
            valid_input=1;
            strcpy (jog_aux.tipo,input);
        }
    }while (valid_input==0); //Se o tipo não for "HU" ou "EA" continua a perguntar

    printf ("Nome do jogador:\n");
    fgets (input, STRING_SIZE, stdin);  //Insere o nome do jogador no terminal
    input[strlen(input)-1]='\0';
    strcpy(jog_aux.nome, input);

    printf ("Dinheiro:\n");
    valid_input=0;
    do
    {
        fgets (input, STRING_SIZE, stdin);
        retorno=sscanf (input, "%d", &aux_int);
        if (retorno==1 && aux_int>=100 && aux_int<=500)
        {
            valid_input=1;
        }
    }while (valid_input==0); //se o dinheiro não estiver entre 100 e 500, continua a perguntar o valor do dinheiro
    jog_aux.dinheiro=aux_int;

    printf ("Aposta:\n");
    valid_input=0;
    do
    {
        fgets (input, STRING_SIZE, stdin);
        retorno=sscanf (input, "%d", &aux_int);
        if (retorno==1 && aux_int>=2 && aux_int<=(jog_aux.dinheiro)*0.25)
        {
            valid_input=1;
        }
    }while (valid_input==0); //se o valor da aposta não estiver entre 2 e 25% do dinheiro do jogador, continua perguntar o valor da aposta
    jog_aux.aposta=aux_int;
    jog_aux.aposta_inicial=aux_int;

    jog_aux.lista_hand=(no_carta*)malloc(sizeof(no_carta)); //Aloca a memoria necessária para a mão do novo jogador
    if (jog_aux.lista_hand==NULL)
    {
        printf ("erro ao alocar memoria\n");
        exit (EXIT_FAILURE);
    }
    //inicializa os restantes valores da estrutura a 0
    jog_aux.lista_hand->next=NULL;          //jogador ainda não tem cartas logo o next é null
    jog_aux.lista_hand->card.codigo=99;     //flag para indicar que é a cabeça da lista de cartas
    jog_aux.cartas_viradas=0;
    jog_aux.numero_de_cartas=0;
    jog_aux.pontos=0;
    jog_aux.numero_ases=0;
    jog_aux.status=0;
    jog_aux.vitorias=0;
    jog_aux.derrotas=0;
    jog_aux.empates=0;
    jog_aux.flag_double=0;
    printf ("vai fazer push\n");
    push_lista_jogadores (topo, jog_aux, posicao);   //função que coloca o novo jogador no topo da lista
    printf ("fez push\n");

}
/** mais_mudancas_de_aposta Pergunta ao utilizador se este quer fazer mais mudanças nas apostas dos jogadores*/

int mais_mudancas_de_aposta ()
{
    char input[STRING_SIZE]="y";
    int valid_input=0;
    int retorno=1;

    do
    {
        printf ("Mais algum jogador quer mudar a sua aposta (y/n)\n");
        fgets (input, STRING_SIZE, stdin);
        input[strlen(input)-1]='\0';
        if (strcmp(input, "y")==0)
        {
            retorno=1;          //o utilizador quer mudar a aposta de mais jogadores
            valid_input=1;
        }
        if (strcmp(input, "n")==0)
        {
            retorno=0;          //o utilizador já não que fazer mais mudanças
            valid_input=1;
        }

    }while (valid_input==0);

    return retorno;
}

/** pede_nome Pergunta o nome do jogador que se pretente mudar o valor da aposta e muda a mesma caso esta seja válida
*\param primeiro_jogador base da lista de jogadores
*/

void pede_nome (lista_jogadores *primeiro_jogador)
{
    int valid_input=0, nova_aposta=0;
    char input[STRING_SIZE];
    lista_jogadores *aux;

    valid_input=0;
    do
    {
        printf ("Intruduza o nome do jogador que pretende mudar o valor da aposta\n");
        fgets (input, STRING_SIZE, stdin);
        input[strlen(input)-1]='\0';
        //verica se o nome introduzido é válido, ou seja, se é igual ao nome de algum jogador
        for (aux=primeiro_jogador;aux!=NULL;aux=aux->next_player)
        {
            if (strcmp(input,aux->player.nome)==0) // comapara a string inserida com o nome da cada jogador
            {
                valid_input=1; //se for igual, ativa a flag e sai do loop
                break;
            }
        }
    }while (valid_input==0);

    valid_input=0;
    do
    {
        printf ("Introduza o valor da nova aposta\n");
        fgets (input, STRING_SIZE, stdin);
        sscanf (input,"%d", &nova_aposta);
        if (nova_aposta <= (aux->player.dinheiro)*0.25 && nova_aposta>=2)
        {
            valid_input=1;
        }
    }while (valid_input==0); //pergunta o valor da aposta ate esta ser valida

    aux->player.aposta=nova_aposta; //atibui esse novo valor à aposta de cada jogador
}

/** avalia_dinheiro Percorre a lista de jogadores e avalia se estes têm dinheiro para jogar, caso contraio, elimina-os
*\param primeiro_jogador, aponta para o primeiro jogador da lista de jogadores
*\param fp, ficheiro stats a ser escrito no final de cada jogo
*/

void avalia_dinheiro (lista_jogadores *primeiro_jogador, FILE *fp)
{
    lista_jogadores *aux=NULL;
    for (aux=primeiro_jogador;aux!=NULL;aux=aux->next_player)
    {
        if (aux->player.dinheiro < aux->player.aposta) //avalia se o dinheiro do jogador é inferior à aposta
        pop_jogador(aux, fp);       //se for expulsa o mesmo do jogo
    }
}
/** escreve_ficheiro Escreve o ficheiro das estatisticas
*\param fp, ficheiro STATS.txt com as estatisticas dos jogadores
*\param aux, jogador a ser escrito no ficheiro
*/

void escreve_ficheiro(FILE *fp, lista_jogadores*aux)
{
    fprintf(fp, "Nome:%s Vitorias:%d Derrotas:%d Empates:%d Dinheiro:%d\n", aux->player.nome, aux->player.vitorias, aux->player.derrotas, aux->player.empates, aux->player.dinheiro);
}

/** pop_jogador Remove um jogador da lista de jogadores
*\param jogador_a_apagar jogadore que se pretende apagar
*\param fp, ficheiro STATS.txt com as estatisticas dos jogadores
*/
void pop_jogador (lista_jogadores *jogador_a_apagar, FILE *fp)
{
    lista_jogadores *aux_anterior, *aux_proximo, *aux;
    escreve_ficheiro(fp, jogador_a_apagar); //antes de remover o jogador, regista-o no ficheiro
    aux_anterior=jogador_a_apagar->prev_player; //auxiliar que aponta para o jogador anterior ao que se pretende remover
    aux_proximo=jogador_a_apagar->next_player; //auxiliar que aponta para o jogador seguinte ao que se pretende remover
    if (aux_anterior!=NULL)
    {
        aux_anterior->next_player=aux_proximo;
    }
    if (aux_proximo!=NULL)
    {
        aux_proximo->prev_player=aux_anterior;
    }
    for (aux=jogador_a_apagar;aux->next_player!=NULL;aux=aux->next_player)
    {}///aux fica a apontar para o ultimo jogador da lista
    aux->next_player=NULL;
    free(jogador_a_apagar); //liberta a memoria guardada para a estrutura do jogador


}

/** two_cards_each Dá duas cartas a cada jogador no inicio do jogo
*\param end_casa endereço da casa (jogador 0)
*\param head, topo do deck
*/

void two_cards_each (lista_jogadores *end_casa, no_carta **head)
{
    int j=1;
    lista_jogadores *jogador_actual;

    for (j=1; j<=2; j++) // duas vezes
    {
        for (jogador_actual=end_casa; jogador_actual!=NULL; jogador_actual=jogador_actual->next_player) //precorre a lista de jogadores
        {
            da_uma_carta(head,jogador_actual);
        }
    }
}
/** distribui_cash Atualiza o valor do dinheiro, vitorias, empates, derrotas de cada jogador
*
*/

void distribui_cash (lista_jogadores *topo_lista)
{
    lista_jogadores *casa, *current;
    casa=topo_lista;
    int ganhou=0,perdeu=0,empatou=0, money=0, ganhou_bj=0, surrender=0;
    for (current=casa->next_player;current!=NULL;current=current->next_player) //for every player
    {
        ganhou=0;
        empatou=0;
        perdeu=0;
        ganhou_bj=0;
        surrender=0;
        money=current->player.aposta;

        if (casa->player.status==1 && current->player.status!=1)
        {
            perdeu=1;
        }
        if(current->player.status==1) //Jogador tem Blackjack
        {
            if(casa->player.status!=1) //a casa não tem BJ
            {
                ganhou_bj=1;
            }
            if(casa->player.status==1) //a casa tem BJ
            {
                empatou=1;
            }
        }
        // BOTH STAND
        if (current->player.status==2 && casa->player.status==2)
        {
            if(current->player.pontos > casa->player.pontos)
            {
                ganhou=1;
            }

            if(current->player.pontos < casa->player.pontos)
            {
                perdeu=1;
            }

            if(current->player.pontos == casa->player.pontos)
            {
                empatou=1;
            }
        }

        if(casa->player.status==3 && current->player.status==2)
        {
            ganhou=1;
        }

        if(current->player.status==3) //BUST
        {
            perdeu=1;
        }

        if (current->player.status==4)//SURRENDER
        {
            surrender=1;
        }

        //De acordo com as flags são feitas as alterações

        if (ganhou_bj==1)
        {
            money/=2;
            current->player.dinheiro+=money;
            topo_lista->player.dinheiro-=money;
            printf ("%s ganhou %d$ (BJ)\n", current->player.nome, money);
        }

        if (surrender==1)
        {
            money/=2;
            current->player.dinheiro-=money;
            topo_lista->player.dinheiro+=money;
            printf ("%s fez surrender\n", current->player.nome);
        }

        if (current->player.flag_double==1)
        {
            money=money*2; //duplica o dinheiro do jogador
        }

        if (ganhou==1)
        {
            current->player.dinheiro+=money;
            current->player.vitorias+=1;
            topo_lista->player.dinheiro-=money;
            printf ("%s ganhou %d$\n", current->player.nome, money);
        }

        if (perdeu==1)
        {
            current->player.dinheiro-=money;
            current->player.derrotas+=1;
            topo_lista->player.dinheiro+=money;
            printf ("%s perdeu %d$\n", current->player.nome, money);
        }
        if (empatou==1)
        {
            printf ("%s empatou\n", current->player.nome);
            current->player.empates+=1;
        }
    }
}
/** Limpa_player Remove um jogador da lista de jogadores
*\param current jogador que se pretende eliminar da lista
*/
void limpa_player (lista_jogadores *current)
{
    no_carta *aux, *aux2;

    current->player.status=0;
    current->player.numero_de_cartas=0;
    current->player.cartas_viradas=1;
    current->player.numero_ases=0;
    current->player.pontos=0;
    current->player.flag_double=0;

    current->player.lista_hand->card.codigo=99;
    current->player.lista_hand->next=NULL;
    aux=current->player.lista_hand->next;
    for (;aux!=NULL;)
    {
        aux2=aux;
        aux=aux->next;
        free(aux2);
    }
    free (aux);


}
/** update_status Atualiza o estado de cada jogador
*\param current jogador que se pretende eliminar da lista
*/

void update_status (lista_jogadores * current)
{
    lista_jogadores * aux;
    aux=current;

        if(aux->player.pontos==21 && aux->player.numero_de_cartas==2)
        {
            aux->player.status=1; /**Blackjack*/
        }
        if (aux->player.pontos==21 && aux->player.numero_de_cartas!=2)
        {
            aux->player.status=2; /**auto-stand*/
        }
        if(aux->player.pontos>21) /**Bust*/
        {
            aux->player.status=3;
        }

}
/** dealer_show_cards Dá as cartas ao dealer ate que a sua pontuação seja superior a 17 pontos
*\param dealer, endereço da casa
*\param topo_devk topo do deck
*/

void dealer_show_cards(lista_jogadores * dealer, no_carta **topo_deck)
{
    while(dealer->player.pontos<17)
    {
        da_uma_carta(topo_deck,dealer);
    }
    if (dealer->player.pontos<=21) //stand se a pontuação for inferior ou igual a 21 pontos
    {
        dealer->player.status=2;
    }

}

/**Calcula_pontos Recebe a estrutura de uma carta e o jogador atual e conta os pontos do jogador de acordo com o valor da carta
*\param player_atual, jogador que se encontra a jogar
*\param x estrutura da carta dada ao jogador
*/

void calcula_pontos(lista_jogadores *player_atual, carta x)
{
    int aux;

    aux=player_atual->player.pontos += x.value;

    //Se tiver um ás e a soma dos pontos for superior a 21, o Ás passa a valer 1;
    if(aux>21&&(player_atual->player.numero_ases)!=0)
    {
        aux -= 10;
        player_atual->player.numero_ases--; //decrementa o numero de ases
        player_atual->player.pontos=aux;
    }
}

/** print_hands Imprime a mão de cada jogador
*\param topo, base das listas de jogadores
*/
void print_hands (lista_jogadores *topo)
{
    no_carta *aux;
    for (; topo!=NULL; topo=topo->next_player)
    {
        printf ("%s:",topo->player.nome);

        for (aux=topo->player.lista_hand; aux!=NULL; aux=aux->next) //percorre a lista
        {
            printf ("[%d]",aux->card.codigo);
        }
        printf("\n");
    }

}
/**
*insere a carta recebido como parametro na lista de cartas do jogador
*calcula o total de pontos do jogador
*posteriormente acualiza o seu estado ( se fez bust, se fez stand, se faz blackjack...)
**/
void push_carta (carta x, lista_jogadores *ponteiro)
{
    no_carta *aux, *aux2;
    no_carta *stack_pointer;

    stack_pointer=ponteiro->player.lista_hand;

    //Verifica se a carta tirada é um Às regista na estrutura
    if(x.value==11)
    {
        ponteiro->player.numero_ases += 1;
        ponteiro->player.flag_soft=1;
    }

    if (stack_pointer->card.codigo==99) //pilha vazia
    {
        stack_pointer->card=x;
        ponteiro->player.numero_de_cartas+=1;
        calcula_pontos(ponteiro,x);
    }
    else
    {
        aux=(no_carta*)malloc(sizeof(no_carta)); //cria novo bloco
        if (aux==NULL)
        {
            printf ("erro ao alocar memoria\n");
            exit (EXIT_FAILURE);
        }

        for (aux2=stack_pointer; aux2->next!=NULL; aux2=aux2->next)
        {}

        aux->card=x;
        aux2->next=aux;
        aux->next=NULL;
        ponteiro->player.numero_de_cartas+=1;
        calcula_pontos(ponteiro,x);
    }

}

/** find__player Encontra um jogador de acordo com o numero do jogador (não utilizada)
*\param head_lista base da lista de jogadores
*\param num_player numero do jogador que se pretende encontrar
*/

lista_jogadores* find_player (lista_jogadores *head_lista, int num_player)
{
    lista_jogadores *aux;
    for (aux=head_lista;aux!=NULL; aux=aux->next_player)
    {
        if (aux->player.n_player==num_player)
        {
            return aux;
        }
    }
    return NULL;
}
/**
*Recebe o apontador para o stack que representa o baralho e o end do jogador a quem vamos dar a carta
*copia a informação da carta no topo do baralho para uma auxiliar
*faz pop do elemento superior do baralho
*faz push da carta na lista de cartas do jogador especificado
**/
void da_uma_carta (no_carta **topo_deck, lista_jogadores *jogador_actual)
{
    no_carta *ponteiro;
    carta carta_aux;

    ponteiro=(*topo_deck); //copia o nodo apontado pelo stack pointer
    carta_aux=ponteiro->card;
    free (*topo_deck); // liberta o nodo que foi poped
    (*topo_deck)=ponteiro->next; //actualiza stack pointer

    push_carta(carta_aux,jogador_actual);
    update_status (jogador_actual);

}

/** shuffle Gera uma posição aleatoria do baralho
*\param head, topo do deck
*\param n_decks, numero de decks em jogo
*/

void shuffle (no_carta *head, int n_decks)
{
    int rand_num=1, i=1;
    no_carta *aux;
    srand(time(NULL));
    for (i=1, aux=head ; i<=52*n_decks ; i++,aux=aux->next)
    {
        rand_num=rand()%(n_decks*52-1);  //gera um numero que esteja dentro do numero possivel de cartas
        trocar (head,aux,rand_num);     //função que troca as cartas
    }
}

/** trocar: Encontra uma posição aleatória no baralho e troca-a
*\param head, topo do deck
*\param current, posição que se pretende trocar
*\param num_rand valor aleatório gerado pela função shuffle
*/

void trocar (no_carta *head, no_carta *current, int num_rand)
{
    int i=1;
    no_carta *auxrand;
    carta carta_aux;

    for (i=1, auxrand=head; i<=num_rand; i++)
    {
        if (i!=num_rand)//ainda nao chegou ao nodo a trocar
        {
            auxrand=auxrand->next;
        }
        else //encontrou a posição para fazer a troca
        {
            carta_aux=current->card;
            current->card=auxrand->card;
            auxrand->card=carta_aux;
        }
    }

}

/** print_jogadores Percorre a lista de jogadores e imprime as suas informações (tipo, nome, dinheiro, aposta)
*\param head, base da lista de jogadores
*/

void print_jogadores (lista_jogadores *head)
{
    lista_jogadores *aux=NULL;

    for (aux=head;aux!=NULL;aux=aux->next_player)
    {
        printf ("-%d TIPO:%s NOME:%s DINHEIRO:%d APOSTA:%d-\n", aux->player.n_player, aux->player.tipo, aux->player.nome, aux->player.dinheiro, aux->player.aposta);
    }
}
/** print_deck Percorre o Deck e imprime o valor do codigo de cada carta
*\param topo, topo do deck
*/
void print_deck (no_carta *topo)
{
    no_carta *aux;
    for (aux=topo; aux!=NULL; aux=aux->next)
    {
        printf ("[%d]",aux->card.codigo);
    }
    printf ("\n");
}

/** inic_deck Inicializa a lista das cartas, ou seja, inicializa o deck
*\param head, topo do deck
*/

no_carta * inic_deck (no_carta *head)
{
    head = (no_carta*)malloc(sizeof(no_carta)); //cria o topo da lista
    head->next=NULL;
    head->card.naipe=99; //"flag" que indica que a cabeça ainda não foi preenchida
    if (head==NULL)
    {
        printf("erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    else
    {
        return head; //retorna a cabeça da lista
    }
}

/** faz_deck Preenche a estrutura da carta com naipe, codigo e valor
*\param Baralhos, numero de baralhos
*\param head, topo do deck
*/

void faz_deck (int Baralhos, no_carta **head)
{
    int Numero_cartas=0, i=0, a=0;
    carta carta_p;

    Numero_cartas = 52 * Baralhos;
    for(i=0; i<Numero_cartas; i++)
    {
        if(a==4)    //naipe entre 0 e 4
        {
            a=0;
        }
        carta_p.codigo = i % 13;
        carta_p.naipe = a;
        carta_p.value=card_value(carta_p.codigo);
        insert_list(head, carta_p);
        if(carta_p.codigo==12)
        {
            a++;
        }
    }

}

/** insert_list Insere a estrutura da carta no deck, organizando os baralhos numa lista
*\param head, topo lista de cartas
*\param x, estrutura do tipo carta que vai ser inserida na lista
*/
void insert_list(no_carta **head, carta x)
{
    no_carta *aux =NULL;

    if ((*head)->card.naipe==99) // se a cabeça estiver vazia, preenche a mesma
    {
        (*head)->card=x;
    }
    else
    {
        aux= (no_carta*)malloc(sizeof(no_carta));
        if (aux==NULL)
        {
            printf("erro ao alocar memória");
            exit(EXIT_FAILURE);
        }

        aux->next=*head;
        aux->card=x;
        *head=aux;
    }
}

/**Ler parametros
* lê e regista o numero de Baralhos, lê e regista o numero de Jogares
* abre o ficheiro da EA
*/

void Ler_Parametros (int *Baralhos, int *jogadores, char *argvector[], lista_jogadores *head, int **Matriz)
{
    //FILE *f_EA;
    FILE *f_param;
    FILE *f_EA;
    char str[STRING_SIZE];
    char *c="-";
    char *sub_string;
    int jogador_aux=0;
    jogador player;

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
    ///Lê o ficheiro da EA e preenche uma matriz que será utilizada para decidir qual a acção da EA
    Preencher_matriz(f_EA, Matriz);

    fgets(str, STRING_SIZE, f_param);
    sscanf (str, "%d-%d", Baralhos, jogadores);
    printf("Baralhos: %d\n", (*Baralhos));
    printf("jogadores:%d\n", (*jogadores));


    for(jogador_aux=1; jogador_aux<=(*jogadores); jogador_aux++)
    {
        fgets(str, STRING_SIZE, f_param);
        //printf("[%d]\n", jogador_aux);
        sub_string = strtok(str, c);
        sscanf(sub_string, "%s", player.tipo);
        printf("%s\n", player.tipo);

        sub_string = strtok(NULL, c);
        sscanf(sub_string, "%s", player.nome);
        //printf("%s\n", player.nome);

        sub_string = strtok(NULL, c);
        sscanf(sub_string, "%d", &(player.dinheiro));
        //printf("%d\n", player.dinheiro);

        sub_string = strtok(NULL, c);
        sscanf(sub_string, "%d", &(player.aposta));
        //printf("%d\n", player.aposta);
        player.aposta_inicial=player.aposta;

        if(player.dinheiro<10||player.dinheiro>500)
        {
            printf("O jogador %d: Tem uma quantia de dinheiro não válida!\n", player.n_player);
            exit(EXIT_FAILURE);
        }

        if (player.aposta>0.25*player.dinheiro || player.aposta<2)
        {
            printf("O jogador %d: Tem uma aposta inválida!\n", player.n_player);
            exit(EXIT_FAILURE);
        }

        sub_string = strtok(NULL, c);

        player.n_player=jogador_aux;
        player.lista_hand=(no_carta*)malloc(sizeof(no_carta));
        if (player.lista_hand==NULL)
        {
            printf ("erro ao alocar memoria\n");
            exit (EXIT_FAILURE);
        }
        player.lista_hand->next=NULL;
        player.lista_hand->card.codigo=99;
        player.cartas_viradas=0;
        player.numero_de_cartas=0;
        player.pontos=0;
        player.numero_ases=0;
        player.status=0;
        player.vitorias=0;
        player.derrotas=0;
        player.empates=0;
        player.flag_double=0;
        player.flag_soft=0;
        push_lista_jogadores (head, player,jogador_aux-1);

    }
}
/**
*Inicia a lista dos jogadores sendo que o primeiro elemnto da lista é a casa
*\param base, base da lista de jogadores
**/
lista_jogadores* inic_lista_jogadores (lista_jogadores *base)
{
    char casa[STRING_SIZE]="CASA";

    base = (lista_jogadores*)malloc(sizeof(lista_jogadores));
    if (base==NULL)
    {
        printf("erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    else
    {
        base->next_player=NULL;
        base->prev_player=NULL;
        base->player.n_player=99;
        base->player.dinheiro=0;
        strcpy (base->player.nome, casa);
        base->player.lista_hand=(no_carta*)malloc(sizeof(no_carta));
        if (base->player.lista_hand==NULL)
        {
            printf("erro ao alocar memória");
            exit(EXIT_FAILURE);
        }
        base->player.lista_hand->next=NULL;
        base->player.lista_hand->card.codigo=99; ///código da 1a carta=99 representa que a mão está vazia
        base->player.cartas_viradas=1;
        base->player.numero_de_cartas=0;
        return base;
    }

}
/**
*Recebe a cabeça da lista dos jogadores
*introduz o nojo jogador que é passado como parametro da função no final da lista
*\param topo, base da lista de jogadores
*\param x, estrutura do tipo jogador que se pretende inserir na lista
**/

void push_lista_jogadores (lista_jogadores *topo, jogador x, int n_player)
{
    lista_jogadores *aux=NULL;
    lista_jogadores *aux2=NULL;
    ///aloca novo bloco
    aux= (lista_jogadores*)malloc(sizeof(lista_jogadores));
    if (aux==NULL)
    {
        printf("erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    ///procura qua é o ultimo elemento da lista (aquele que aponta para NULL)
    for (aux2=topo; aux2->next_player!=NULL; aux2=aux2->next_player)
    {}
    printf("222\n");
    if(aux2->player.n_player==99||aux2->player.n_player<n_player)
    {
        aux2->next_player=aux;///o que era anteriormente o ultimo passa a apontar para a que vamos introduzir (que será o ultimo)
        aux->prev_player=aux2;///o anterior ao que introduzimos é o antigo ultimo
        aux->next_player=NULL;///o novo elemento aponta para NULL pois é o ultimo da lista
        x.n_player=n_player;
        aux->player=x;///x contem toda a informação relativa ao jogador a adicionar
    }
    else
    {    printf("222\n");

        aux2=find_player(topo, (n_player-1));
        if(aux2==NULL)
        {
            aux2=topo;
        }
        printf("N_PLAYER=%d\n", aux2->player.n_player);
        aux->next_player=aux2->next_player;///o novo elemento aponta para NULL pois é o ultimo da lista
        aux2->next_player=aux;///o que era anteriormente o ultimo passa a apontar para a que vamos introduzir (que será o ultimo)
        aux->prev_player=aux2;///o anterior ao que introduzimos é o antigo ultimo
        x.n_player=n_player;
        aux->player=x;///x contem toda a informação relativa ao jogador a adicionar
    }
}
/**
*recebe o codigo da carta a analisar
*retorna um inteiro que corresponte ao valor no jogo blackjack á carta
*\param estrutura do tipo carta que se pertende calcular o valor
**/
int card_value (int carta)
{
    int value=-1;

    if (carta==12) //AS
        value=11;

    if (carta==11 || carta==10 || carta==9 || carta==8) //REI DAMA VALETE OU 10
        value=10;

    if (carta==7) //CARTA 9
        value=9;

    if (carta==6) //CARTA 8
        value=8;

    if (carta==5) //CARTA 7
        value=7;

    if (carta==4) //CARTA 6
        value=6;

    if (carta==3) //CARTA 5
        value=5;

    if (carta==2) //CARTA 4
        value=4;

    if (carta==1) //CARTA 3
        value=3;

    if (carta==0) //CARTA 2
        value=2;

    return value;
}

/**
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  squares to define the playing positions of each player
 * -  names and the available money for each player
 * \param _money amount of money of each player
 * \param _img surfaces where the table background and IST logo were loaded
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderTable(lista_jogadores *topo, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, int delay_ea)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 }; // white
    char name_money_str[STRING_SIZE];
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
    int height, i=0;
    char string_estado[STRING_SIZE]= " ";
    SDL_Color red = {255,0,0};
    char jogador_fora[STRING_SIZE]="LUGAR VAZIO";
    char click[STRING_SIZE]="CLIQUE PARA JOGAR";
    char player_string[STRING_SIZE];
    char delay_string[STRING_SIZE];
    char balance[STRING_SIZE];
    lista_jogadores *aux=NULL;
    int flag_existe_jogador=0;
    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = HEIGHT_WINDOW;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(separatorPos+3*MARGIN, height, "Rodrigo Simas  81536", _font, &black, _renderer);
    height += RenderText(separatorPos+3*MARGIN, height, "Bernardo Bastos  84012", _font, &black, _renderer);

    sprintf(balance,"House Balance: %d$", topo->player.dinheiro);
    height += RenderText(separatorPos+3*MARGIN, height, balance, _font, &black, _renderer);
    ///Delay
    sprintf(delay_string, "DELAY_EA: %d", delay_ea);
    height+=40;
    height += RenderText(separatorPos+3*MARGIN, height,delay_string, _font, &black, _renderer);


    // renders the squares + name for each player
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );

    // renders the areas for each player: names and money too !
    if (topo->next_player!=NULL)
    {
        aux=topo->next_player;
        for (i=0;i<4;i++)
        {
            flag_existe_jogador=0;
            if (aux!=NULL)
            {
                if (aux->player.n_player==i)
                {
                    flag_existe_jogador=1;
                    playerRect.x = i*(separatorPos/4-5)+10;
                    playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
                    playerRect.w = separatorPos/4-5;
                    playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
                    sprintf(name_money_str,"%s - %d euros - %d pts", aux->player.nome, aux->player.aposta, aux->player.pontos);
                    sprintf(player_string, "%s(%s): %d$", aux->player.nome, aux->player.tipo, aux->player.dinheiro);
                    height += RenderText(separatorPos+3*MARGIN, height, player_string, _font, &black, _renderer);
                    RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
                    SDL_RenderDrawRect(_renderer, &playerRect);


                        switch(aux->player.status)
                        {
                            case 1:
                                sprintf(string_estado, "BLACKJACK");
                                break;

                            case 2:
                                sprintf(string_estado, "STAND");
                                break;

                            case 3:
                                sprintf(string_estado, "BUST");
                                break;

                            case 4:
                                sprintf(string_estado, "SURRENDER");
                                break;

                            default:
                                sprintf(string_estado, " ");
                                break;
                        }
                         RenderText(playerRect.x+50, playerRect.y-50, string_estado, _font, &red, _renderer);



                    aux=aux->next_player;
                }
            }
            if (flag_existe_jogador==0)
            {
                playerRect.x = i*(separatorPos/4-5)+10;
                playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
                playerRect.w = separatorPos/4-5;
                playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
                SDL_RenderDrawRect(_renderer, &playerRect);
                RenderText(playerRect.x+45, playerRect.y+80, jogador_fora, _font, &white, _renderer);
                RenderText(playerRect.x+25, playerRect.y+100, click, _font, &white, _renderer);
            }
        }
    // destroy everything
    SDL_DestroyTexture(table_texture);
}
}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderHouseCards(jogador casa, SDL_Surface **_cards, SDL_Renderer* _renderer, int fim_ronda, TTF_Font *_font)
{
    int card, x, y,c;
    int div = WIDTH_WINDOW/CARD_WIDTH;
    no_carta *aux;
    char string_pontos[STRING_SIZE];
    SDL_Color white = { 255, 255, 255 };
    // drawing all house cards
    sprintf (string_pontos, "House:%d",casa.pontos);
    for (aux=casa.lista_hand, card=0; aux!=NULL; aux=aux->next, card++)
    {
        // calculate its position
        x = (div/2-casa.numero_de_cartas/2+card)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        // render it !
        c=aux->card.codigo+aux->card.naipe*13;
        RenderCard(x, y, c, _cards, _renderer);
    }
    // just one card ?: draw a card face down
    if (casa.cartas_viradas == 1)
    {
        x = (div/2-casa.numero_de_cartas/2+1)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        RenderCard(x, y, MAX_DECK_SIZE, _cards, _renderer);
    }
    if (fim_ronda==1)
    {
        RenderText(x+70, y, string_pontos, _font, &white, _renderer);
    }
}

/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards(lista_jogadores *primeiro_jogador, SDL_Surface **_cards, SDL_Renderer* _renderer, lista_jogadores *current_player, TTF_Font *_font, int fim_ronda)
{
    int pos, x, y, card,c, n_jogador=0;
    lista_jogadores *aux;
    char DOUBLE[STRING_SIZE]="DOUBLE!";
    no_carta *aux_card;
    SDL_Rect playerRect;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW);
    SDL_Color white = { 38, 201, 233 };

    // for every card of every player
    if (primeiro_jogador!=NULL)
    {
        for (aux=primeiro_jogador; aux!=NULL; aux=aux->next_player,n_jogador++)
        {

            for (aux_card=aux->player.lista_hand, card=0 ; aux_card!=NULL; aux_card=aux_card->next, card++)
            {
                if (aux_card->card.codigo==99) //jogador acabado de introduzir
                {
                    break;
                }
                c=aux_card->card.codigo + aux_card->card.naipe*13;
                // draw all cards of the player: calculate its position: only 4 positions are available !
                pos = card % 4;
                x = (int) (aux->player.n_player)*((0.95f*WIDTH_WINDOW)/4-5)+(card/4)*12+15; // n_player -1 porque a casa é o zero
                y = (int) (0.55f*HEIGHT_WINDOW)+10;
                if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
                if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
                // render it !
                RenderCard(x, y, c, _cards, _renderer);
            }
            if (aux->player.flag_double==1)
            {
                RenderText(x + CARD_WIDTH + 30, y+ CARD_HEIGHT/3, DOUBLE, _font, &white, _renderer);
            }

            if(aux==current_player && fim_ronda==0)
            {
                SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
                playerRect.x = (aux->player.n_player)*(separatorPos/4-5)+10;
                playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
                playerRect.w = separatorPos/4-5;
                playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
                SDL_RenderDrawRect(_renderer, &playerRect);
            }
        }
    }
}

/**
 * RenderCard: Draws one card at a certain position of the window, based on the card code
 * \param _x X coordinate of the card position in the window
 * \param _y Y coordinate of the card position in the window
 * \param _num_card card code that identifies each card
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    SDL_Texture *card_text;
    SDL_Rect boardPos;

    // area that will be occupied by each card
    boardPos.x = _x;
    boardPos.y = _y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;

    // render it !
    card_text = SDL_CreateTextureFromSurface(_renderer, _cards[_num_card]);
    SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);

    // destroy everything
    SDL_DestroyTexture(card_text);
}

/**
 * LoadCards: Loads all images of the cards
 * \param _cards vector with all loaded card images
 */
void LoadCards(SDL_Surface **_cards)
{
    int i;
    char filename[STRING_SIZE];

    // loads all cards to an array
    for (i = 0 ; i < MAX_DECK_SIZE; i++ )
    {
        // create the filename !
        sprintf(filename, ".//cartas//carta_%02d.png", i+1);
        // loads the image !
        _cards[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_cards[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    // loads the card back
    _cards[i] = IMG_Load(".//cartas//carta_back.jpg");
    if (_cards[i] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * UnLoadCards: unloads all card images of the memory
 * \param _cards vector with all loaded card images
 */
void UnLoadCards(SDL_Surface **_array_of_cards)
{
    // unload all cards of the memory: +1 for the card back
    for (int i = 0 ; i < MAX_DECK_SIZE + 1; i++ )
    {
        SDL_FreeSurface(_array_of_cards[i]);
    }
}

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "BlackJack", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

    return renderer;
}
#endif // BJ_C_INCLUDED
