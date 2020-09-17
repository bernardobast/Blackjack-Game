#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "bj.h"

#define STRING_SIZE 100       // max size for some strings
#define WIDTH_WINDOW 900      // window width
#define HEIGHT_WINDOW 525     // window height
#define MAX_DECK_SIZE 52      // number of max cards in the deck
#define MAX_CARD_HAND 11      // 11 cards max. that each player can hold
#define CARD_WIDTH 67         // card width
#define CARD_HEIGHT 97        // card height
#define WINDOW_POSX 500       // initial position of the window: x
#define WINDOW_POSY 250       // initial position of the window: y
#define EXTRASPACE 150
#define MARGIN 5
#define MAX_PLAYERS 4         // number of maximum players


int main( int argc, char* args[] )
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int n_decks;
    int n_players=0;
    int fim_ronda=0;
    int mudancas=1;
    int seleccao_jogadores=0;
    int decisao=0;
    int **mat_EA=NULL;
    int time_delay=300;
    int quit_game=0;
    int cartas_no_deck;
    int high_low_value=0;
    int posicao=0;
    int mouse_position_x=0, mouse_position_y=0;
    lista_jogadores *end_casa=NULL, *topo_lista_jogadores=NULL, *aux=NULL, *jogador_actual=NULL;
    no_carta *head;
    FILE *stats;

    stats=fopen("STATS.txt", "w");
    topo_lista_jogadores = inic_lista_jogadores(topo_lista_jogadores); //inicializa a lista e retorna um ponteiro para a base da lista
    end_casa = topo_lista_jogadores; // o primeiro elemento da lista dos jogadores é a casa
    mat_EA=aloca_matriz(mat_EA);
    Ler_Parametros (&n_decks, &n_players, args, end_casa, mat_EA);//Baralhos, Numero Jogadores. Jogador: Tipo, Nome, Dinheiro, Aposta //Cria a lista de jogadores
    print_matriz(mat_EA);
    //print_jogadores (topo_lista_jogadores);
    head=inic_deck(head);
    faz_deck(n_decks,&head);
    shuffle (head,n_decks);
    //print_deck(head);

    two_cards_each (end_casa, &head);

    //printf ("%d\n",player_aux->player.lista_hand->card.codigo);
    //print_deck(head);


    // initialize graphics
    InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);

    jogador_actual=end_casa->next_player; // para começar no jogador 1

    while( quit == 0 )
    {
        ///se o jogador actual for artificial
        if (end_casa->next_player!=NULL)
        {
            if (strcmp("EA",jogador_actual->player.tipo)==0 && fim_ronda==0)
            {
                do
                {
                    decisao=decisao_EA (mat_EA,jogador_actual,end_casa);
                    accao_EA (decisao,jogador_actual,&head);

                    RenderTable(end_casa, serif, imgs, renderer, time_delay);
                    RenderHouseCards(end_casa->player, cards, renderer, fim_ronda, serif);
                    RenderPlayerCards(end_casa->next_player, cards, renderer, jogador_actual, serif, fim_ronda);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(time_delay);
                }
                while (jogador_actual->player.status==0);
            }
        }
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT || quit_game==1)
            {
                exit(0);
            }
            else
            {
                {
                    switch(event.type)
                    {
                    case SDL_KEYDOWN:
                        switch ( event.key.keysym.sym )
                        {
                        case SDLK_q:
                            if(fim_ronda==1)
                            {
                                free_deck(head);
                                free(mat_EA);
                                for (aux=end_casa;aux->next_player!=NULL;aux=aux->next_player);
                                for(; aux!=NULL; aux=aux->prev_player)
                                {
                                    pop_jogador(aux, stats);
                                }
                                quit_game=1;
                            }
                            break;

                        case SDLK_s:
                            if (fim_ronda==0 && strcmp(jogador_actual->player.tipo, "HU")==0)
                                jogador_actual->player.status=2;
                            break;
                        // todo
                        case SDLK_h:
                            if (fim_ronda==0 && strcmp(jogador_actual->player.tipo, "HU")==0)
                            {
                                print_jogadores(end_casa);
                                da_uma_carta (&head,jogador_actual);
                            }
                            break;

                        case SDLK_d:
                            if (fim_ronda==0 && jogador_actual->player.numero_de_cartas==2 && strcmp(jogador_actual->player.tipo, "HU")==0 && jogador_actual->player.dinheiro>=2*jogador_actual->player.aposta)
                            {
                                da_uma_carta (&head,jogador_actual);
                                jogador_actual->player.flag_double=1;
                                if(jogador_actual->player.status==0)
                                {
                                    jogador_actual->player.status=2;
                                }
                            }
                            break;

                        case SDLK_r:
                            if (fim_ronda==0 && strcmp(jogador_actual->player.tipo, "HU")==0)
                            {
                                jogador_actual->player.status=4;
                            }
                            break;

                        case SDLK_b:
                            if (fim_ronda==1)
                            {
                                do
                                {
                                    pede_nome (end_casa->next_player);
                                    mudancas=mais_mudancas_de_aposta(); //retorna 1 se ainda se tem que fazer mais mudanças, 0 se ja nao houver mais mudanças a fazer

                                }
                                while (mudancas);

                            }
                            break;


                        case SDLK_n:
                            if (fim_ronda==1)
                            {
                                avalia_dinheiro (end_casa->next_player, stats);
                                high_low_bet(high_low_value, head, end_casa->next_player);
                                for (aux=end_casa; aux!=NULL; aux=aux->next_player) // for every player
                                {
                                    limpa_player (aux);
                                }
                                fim_ronda=0;
                                jogador_actual=end_casa->next_player; //voltamos ao jogador 1
                                cartas_no_deck=conta_cartas_no_deck(head);
                                if (cartas_no_deck<n_decks*52/2)
                                {
                                    printf ("Metade das cartas usadas\n BARALHANDO...\n");
                                    head=inic_deck(head);
                                    faz_deck(n_decks,&head);
                                    shuffle (head,n_decks);
                                }
                                two_cards_each (end_casa,&head);
                                print_jogadores (topo_lista_jogadores);
                                printf ("acabou\n");
                            }
                            break;

                        case SDLK_a:
                            if (fim_ronda==1)
                            {
                                printf ("Use o rato para escolher o lugar onde se quer sentar.\n");
                                seleccao_jogadores=1;
                            }
                            break;

                        case SDLK_UP:
                            if (fim_ronda==1)
                            {
                                time_delay+=50;
                            }
                            break;

                        case SDLK_DOWN:
                            if (fim_ronda==1 && time_delay>50)
                            {
                                time_delay-=50;
                            }
                            break;
                        // todo
                        default:
                            break;
                        }
                        break;

                    case SDL_MOUSEBUTTONDOWN:
                        if (seleccao_jogadores==1)
                        {
                               if((event.button.x>11&&event.button.x<840)&&(event.button.y>290&&event.button.y<500)) /// Adiciona apenas na área de jogo
                               {
                                   mouse_position_x=event.button.x;
                                   mouse_position_y=event.button.y;
                                   posicao = calcular_posicao(mouse_position_x, mouse_position_y,end_casa->next_player);
                                   printf ("pos:%d\n", posicao);
                                    seleccao_jogadores=0;
                                switch (event.button.button)
                                {
                                case SDL_BUTTON_LEFT:
                                        if (posicao!=99)
                                        {
                                            printf ("chegou\n");
                                            novo_jogador (end_casa, posicao);
                                        }
                                        else
                                        {
                                            printf ("Apenas é possivel adicionar novos jogadores no final de cada ronda e o maximo de jogadores é 4\n");
                                        }
                                    break;

                                break;
                            }
                        }
                    }


                    default:
                        break;
                    }
                }
            }

        }
        if (end_casa->next_player==NULL)   //Se não estiver nenhum jogador a jogar o programa acaba
        {
            printf ("nao ha mais jogadores\n");
            free_deck(head);
            free_matriz(mat_EA);
            pop_jogador(end_casa,stats);
            quit_game=1;
            break;
        }
        else
        {
            if(fim_ronda==0 && jogador_actual->player.status!=0)
            {
                high_low (jogador_actual, &high_low_value);
                printf("OUT:%d\n", high_low_value);
            }

            if(jogador_actual->player.status!=0)
            {
                jogador_actual=jogador_actual->next_player;
            }
            if(jogador_actual==NULL) //Ultimo jogador terminou a jogada
            {
                jogador_actual=end_casa;
                if (fim_ronda==0)
                {
                    fim_ronda=1;
                    end_casa->player.cartas_viradas=2; // para mostrar a carta que estava virada para baixo
                    dealer_show_cards(end_casa,&head);
                    high_low (jogador_actual, &high_low_value);
                    distribui_cash (end_casa);
                }
            }
        RenderTable(end_casa, serif, imgs, renderer, time_delay);
        RenderPlayerCards(end_casa->next_player, cards, renderer, jogador_actual, serif, fim_ronda);
        RenderHouseCards(end_casa->player, cards, renderer, fim_ronda, serif);
        SDL_RenderPresent(renderer);
        SDL_Delay( delay );
        }
    }

    // free memory allocated for images and textures and close everything including fonts
    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
