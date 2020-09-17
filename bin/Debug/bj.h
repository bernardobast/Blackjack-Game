#ifndef BJ_H_INCLUDED
#define BJ_H_INCLUDED

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
#define MAX 8


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
        char nome[MAX];
        char tipo[MAX];
        int dinheiro;
        int aposta;
        int status;
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
void RenderTable(int [], TTF_Font *, SDL_Surface **, SDL_Renderer * );
void RenderCard(int , int , int , SDL_Surface **, SDL_Renderer * );
void RenderHouseCards(int [], int , SDL_Surface **, SDL_Renderer * );
void RenderPlayerCards(int [][MAX_CARD_HAND], int [], SDL_Surface **, SDL_Renderer * );
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);
no_carta *inic_deck (no_carta *head);
void insert_list(no_carta *head, carta x);
void Ler_Parametros (int *Baralhos, int *jogadores, char*argvector[]);
void carta_value (int Baralhos, no_carta *head);
lista_jogadores* inic_lista_jogadores (lista_jogadores *base);
void push_lista_jogadores (lista_jogadores *no, jogador x);


#endif // BJ_H_INCLUDED
