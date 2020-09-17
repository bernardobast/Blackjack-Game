#ifndef BJ_H_INCLUDED
#define BJ_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

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
#define MAX_FILE_NAME 50
#define MAX 20
#define MAX_CARTAS 11
#define MAX_LINE 18
#define MAX_COLLUMN 10

/** Estrutura para cada carta
* organiza as cartas numa estrutura com o codigo (1-13)
* valor da carta
* organiza as cartas numa estrutura com naipe (0-3)
*/
typedef struct
{
    int codigo;
    int value;
    int naipe;
} carta;

/** Estrutura para o baralho de cartas
* organiza as cartas num baralho
*/

typedef struct no_carta
{
    carta card;
    struct no_carta * next;
} no_carta;

/** Jogador
* Nome do jogador
* Tipo
* Aposta
*/

typedef struct {
        int n_player;
        char nome[MAX];
        char tipo[MAX];
        int dinheiro;
        int aposta;
        int aposta_inicial;
        int status;  /** 0 - playing, 1 - BlackJack, 2 - stand  3- bust*/
        no_carta *lista_hand;
        int numero_de_cartas;
        int cartas_viradas;
        int numero_ases;
        int pontos;
        int vitorias;
        int derrotas;
        int empates;
        int flag_double;
        int flag_soft;
} jogador;

typedef struct lista_jogadores {

        jogador player;
        struct lista_jogadores * next_player;
        struct lista_jogadores * prev_player;
}   lista_jogadores;


void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int , int , const char* , TTF_Font *, SDL_Color *, SDL_Renderer * );
int RenderLogo(int , int , SDL_Surface *, SDL_Renderer * );
void RenderTable(lista_jogadores *topo, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, int delay_ea);
void RenderCard(int , int , int , SDL_Surface **, SDL_Renderer * );
void RenderHouseCards(jogador casa, SDL_Surface **_cards, SDL_Renderer* _renderer, int fim_ronda, TTF_Font *_font);
void RenderPlayerCards(lista_jogadores *primeiro_jogador, SDL_Surface **_cards, SDL_Renderer* _renderer, lista_jogadores *current_player, TTF_Font *_font, int fim_ronda);
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);
no_carta *inic_deck (no_carta *head);
void insert_list(no_carta **head, carta x);
void Ler_Parametros (int *Baralhos, int *jogadores, char *argvector[], lista_jogadores *head, int **Matriz);
void faz_deck (int Baralhos, no_carta **head);
lista_jogadores* inic_lista_jogadores (lista_jogadores *base);
void push_lista_jogadores (lista_jogadores *topo, jogador x, int n_player);
void shuffle (no_carta *head, int n_decks);
void trocar (no_carta *head, no_carta *current, int num_rand);
int card_value (int carta);
void calcula_pontos(lista_jogadores *player_atual, carta x);
void print_jogadores (lista_jogadores *head);
void da_uma_carta (no_carta **topo_deck, lista_jogadores *jogador_actual);
lista_jogadores* find_player (lista_jogadores *head_lista, int n_player);
void print_hands (lista_jogadores *topo);
void dealer_show_cards(lista_jogadores * dealer, no_carta **topo_deck);
void update_status (lista_jogadores * base);
void distribui_cash (lista_jogadores *topo_lista);
void limpa_player (lista_jogadores *current);
void two_cards_each (lista_jogadores *end_casa, no_carta **head);
void avalia_dinheiro (lista_jogadores *primeiro_jogador, FILE *fp);
void pop_jogador (lista_jogadores *jogador_a_apagar, FILE *fp);
void pede_nome (lista_jogadores *primeiro_jogador);
int mais_mudancas_de_aposta ();
void accao_EA (int decisao, lista_jogadores *current, no_carta **head_deck);
int decisao_EA (int **mat, lista_jogadores *actual, lista_jogadores *casa);
void free_deck (no_carta *head);
int conta_cartas_no_deck (no_carta *head);
int** aloca_matriz (int **mat_EA);
void print_matriz (int **matriz);
void Preencher_matriz(FILE *fp, int **Matriz);
int numero_de_jogadores (lista_jogadores *primeiro_jogador);
void escreve_ficheiro(FILE *fp, lista_jogadores*aux);
void high_low (lista_jogadores *jogador_Atual, int *high_low_value);
void push_carta (carta x, lista_jogadores *ponteiro);
void high_low_bet(int high_low_value, no_carta *head, lista_jogadores *base);
void free_matriz(int **mat_EA);
void novo_jogador (lista_jogadores *topo, int posicao);
int calcular_posicao(int x, int y, lista_jogadores *primeiro_jogador);
void free_matriz(int **mat_EA);


#endif // BJ_H_INCLUDED
