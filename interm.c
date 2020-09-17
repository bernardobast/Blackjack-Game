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
#define MAX_CARTAS 11         // teoricamente, usando 6 decks, há 24 Áses no baralho, logo a maximo de cartas que um jogador pode pedir sem fazer bust é 11
#define MAX_DECK 312         //maximo de cartas possiveis no baralho

void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int , int , const char* , TTF_Font *, SDL_Color *, SDL_Renderer * );
int RenderLogo(int , int , SDL_Surface *, SDL_Renderer * );
void RenderTable(int _money[], TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, int nplayer, int *player_status, int* player_totals, int inicio, int*players_results, int fim_da_ronda, int house_total,int carta_virada,int house_money, int new_game, int aposta, int round);
void RenderCard(int , int , int , SDL_Surface **, SDL_Renderer * );
void RenderHouseCards(int [], int , SDL_Surface **, SDL_Renderer *, int inicio, int playing, int carta_virada);
void RenderPlayerCards(int [][MAX_CARD_HAND], int [], SDL_Surface **, SDL_Renderer *,int playing );
void LoadCards(SDL_Surface **);
void UnLoadCards(SDL_Surface **);
int pede_decks(void);
void pede_dinheiro (int*);
int pede_aposta (int*);
int valor_minimo (int*);
void Inic_deck(int*, int);
void shufle (int*,int);
void troca (int*,int,int);
void dar_cartas_iniciais (int[][MAX_CARTAS], int*, int*);
void print_hands (int[][MAX_CARTAS], int*);
void print_deck (int*);
int card_value (int);
int did_he_bust (int[][MAX_PLAYERS]);
void print_totals (int*);
void update_stats (int player_values[MAX_PLAYERS],int player_status[MAX_PLAYERS],int hands[][MAX_CARTAS],int house[MAX_CARTAS], int pos_house, int* pos_players);
int hand_value2 (int*player_values,int player_status[MAX_PLAYERS],int hands[][MAX_CARTAS], int *house);
int da_uma_carta (int(*vector)[MAX_CARTAS],int *casa ,int*pos, int *pos_house, int* deck, int player);
int stil_playing (int*);
int ases (int player_cards[MAX_PLAYERS][MAX_CARTAS], int house_cards[MAX_CARTAS], int player);
void calc_money (int * player_totals, int *pos_players, int pos_house, int house_total, int*money, int *house_money, int aposta, int*players_results, int[][3], int*);
void quit_game ( FILE *fp, int resultados[][3], int house_cash, int *money);


const char myName[] = "Rodrigo Simas";
const char myNumber[] = "IST 81536";
const char * playerNames[] = {"1) Bertinho", "2) Carlos  ", "3) Ascenso ", "4) Tanmay  "};

int main( int argc, char* args[] )
{
    //-------------PARA A PARTE GRÁFICA------------
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int delay=300;
    //-------------PARA A PARTE GRÁFICA------------
    int ndecks; //numero de dedcks utilizado para jogar
    int money[MAX_PLAYERS]; // vector com os dinheiros de cada jogador
    FILE *fp; //variavel do tipo ficheiro onnde serao guardados os resultados do jogo
    int player_cards[MAX_PLAYERS][MAX_CARTAS]; // vector com as maos de cada jagador
    int deck[MAX_DECK]; // vector com todas as carats do deck elemnto 0 corresponde á carta de cima, se um valor do vector for -1, a carta nao existe
    int house[MAX_CARTAS]; // cartas da casa
    int pos_house_hand=0; // quantas cartas mostar para a casa
    int pos_player_hand[MAX_PLAYERS]; // quantas cartas mostar para cada jogador
    int aposta; // valor fixo da aposta
    int players_totals[MAX_PLAYERS]; // soma dos valores das cartas de cada jogador
    int players_status[MAX_PLAYERS]; //vestor que guarda o estado de cada jogador: 0->bust 1->playing 2->blackjack 3->stand 4->out of cash
    int playing_player=1; //variavel para determinar a fase da ronda
    int quit=0; //variavel para determinar a fase da ronda
    char input[10]; // string que será lida do terminal para o comando
    char comando; // comando
    int aux,aux2; // variaveis de ciclo
    int playing=1;//variavel para determinar a fase da ronda
    int playing_round=1; //variavel para determinar a fase da ronda
    int inicio_jogo=1;//variavel para determinar a fase da ronda
    int house_cash=0; // dinheiro da casa
    int house_total; // total da casa
    int new_game=0; //variavel para determinar a fase da ronda
    int players_results[MAX_PLAYERS];
    int fim_da_ronda=0;
    int carta_virada=0;
    int resultados[MAX_PLAYERS][3]= {{0}}; // coluna 0=n de derrotas coluna coluna1=n de empates coluna3=n de vitorias
    int numero_carta;
    int round=1;

    fp=fopen("stats.txt", "w"); //ABRE FICHEIRO EM MODO ESCRITA
    ndecks=pede_decks(); //PEDE O NUMERO DE DECKS QUE SERAO UTILIZADOS NO JOGO
    pede_dinheiro(money); // PEDE A CADA JOGADOR COM QUANTO DINHEIRO ELES QUEREM ENTRAR NO JOGO
    aposta=pede_aposta (money); // PEDE O VALOR DA APOSTA

    InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer); //INICIALIZA A JANELA GRAFICA E CARREGA AS CARTAS
    LoadCards(cards); //descarrega as imagens das cartas
    Inic_deck(deck,ndecks); //INICIALIZA O DECK E BARALHA
    shufle(deck,ndecks);
    do
    {
        if(inicio_jogo==1) // SE ESTIVERMOS NO INICIO DA RONDA VAMOS EXECUTAR AS FUNÇOES DESTE IF:
        {
            printf ("Distribuindo jogo...\n");

            for (aux=0; aux<MAX_PLAYERS; aux++) //INICIALIZA O VECTOR QUE GUARDA O RESULTADO DOS JOGADORES A '-1' (VALOR FIGURATIVO)
            {
                players_results[aux]=-1; // SIGNIFICADO DOS VALORES PARA O VECTOR PLAYERS_RESULTS: 0-PERDEU 1-EMPATOU 2-GANHOU
            }

            for (aux=0; aux<MAX_PLAYERS; aux++) //inicializa o numero de cartas a mostrar (0) NO INICIO DA RONDA
            {
                pos_player_hand[aux]=0; // para todos os jogador
            }
            pos_house_hand=0; // para a casa

            for (aux=0; aux<MAX_PLAYERS; aux++) //limpa as maos dos jogadores e da casa
            {
                for(aux2=0; aux2<MAX_CARTAS; aux2++)
                {
                    player_cards[aux][aux2]=-1;
                    house[aux2]=-1;
                }
            }

            for (aux=0; aux<MAX_PLAYERS; aux++) //verifica se cada jogador pode participar na ronda ou nao em função do seu saldo
            {
                if (money[aux]>=aposta)
                {
                    //significado dos valores do vector players_status:
                    //0=jogador fez bust
                    players_status[aux]=1; //1=pode jogar e participa na ronda

                }                          //2=fez blackjack
                else                       //3=fez stand
                    players_status[aux]=4; //4=nao pode jogar por falta de dinheiro
            }
            // Dá alternadamente o jogo (uma carta a cada pessoa de cada vez
            for (aux=1; aux<=2; aux++) // faz o ciclo 2 vezes
            {
                for (playing_player=1; playing_player<=5; playing_player++) // player entre 1 e 4 dá ao jogador, player=5 dá a casa
                {
                    if (players_status[playing_player-1]==1 || playing_player==5) // se o jogador poder jogar: dar carta, a casa pode sempre jogar
                    {
                        da_uma_carta (player_cards, house, pos_player_hand, &pos_house_hand, deck, playing_player-1); //dá a carta
                        RenderTable(money, serif, imgs, renderer, playing_player, players_status, players_totals,1, players_results, //desenha
                                    fim_da_ronda,house_total, carta_virada,house_cash,new_game,aposta,round);                        //desenha
                        RenderHouseCards(house, pos_house_hand, cards, renderer, inicio_jogo,playing,carta_virada);                  //desenha
                        RenderPlayerCards(player_cards, pos_player_hand, cards, renderer, playing);                                  //desenha
                        SDL_RenderPresent(renderer);
                        SDL_Delay( 1 );

                    }
                }
                carta_virada=1; // variavel para determinar se se apresenta a carta da casa voltada para baixo ou nao
            }                   // passa a 1 no fim do 1º ciclo de modo que na segunda vez que se da a carta a casa ela vai estar voltada para baixo
            inicio_jogo=0;      // depois de inicializada a ronda esta variavel passa a 0 para quando o ciclo correr novamente nao voltar a inicializar o jogo
            playing=1;
            playing_round=1;    // variavel que determina se ainda há jogadas por realizar
            playing_player=1;   // o primeiro jogador a jogar é o 1

        }

        printf ("RONDA %d\n",round);
        while (playing_round==1) //enquanto há jogadas por realizar
        {
            hand_value2 (players_totals,players_status,player_cards,house); //calcula os totais das maos
            update_stats (players_totals,players_status,player_cards,house,pos_house_hand,pos_player_hand); // actualiza o vector players_status em funçao dos totais das maos e do tatol de cartas de cada jogador

            if (players_status[playing_player-1]!=1) // no caso do jogador seleccionado nao poder jogar
            {

                do  //skips player até encontrar um que possa jogar
                {
                    playing_player=playing_player+1;
                }
                while (players_status[playing_player-1]!=1);

            }

            RenderTable(money, serif, imgs, renderer, playing_player, players_status, players_totals,inicio_jogo, players_results, //desenha
                        fim_da_ronda,house_total,carta_virada,house_cash,new_game,aposta,round);                                   //desenha
            RenderHouseCards(house, pos_house_hand, cards, renderer,inicio_jogo,playing, carta_virada);                            //desenha
            RenderPlayerCards(player_cards, pos_player_hand, cards, renderer,playing);                                             //desenha
            SDL_RenderPresent(renderer);                                                                                           //desenha
            SDL_Delay( 1 );

            do  //pede ao utilizador um comando, é resistente a inputs errados
            {
                scanf("%s", input);
                sscanf(input, "%c", &comando); // uso da funçao sscanf para evitar erros relacionados com dados incorrectos no buffer
            }
            while(comando!='h' && comando!='s' && comando!='n' && comando!= 'q');

            if (comando=='h') // caso HIT
            {
                //da uma carta e faz update dos stats do playing player
                da_uma_carta (player_cards, house, pos_player_hand, &pos_house_hand, deck, playing_player-1);
                hand_value2 (players_totals,players_status,player_cards, house);
                update_stats(players_totals,players_status,player_cards,house,pos_house_hand,pos_player_hand);


            }
            if (comando=='s') // caso STAND
            {
                players_status[playing_player-1]=3;// muda o estado do playing_player para modo stand
                playing_player++; // passa ao player seguinte

            }
            if (comando=='q') // QUIT THE PROGRAM
            {
                quit_game (fp, resultados, house_cash,money);
                quit=1;
                break;
            }

            playing_round=stil_playing (players_status); //avalia se ainda há jogadas por realizar

        }
        if (quit==1)
            break;

        carta_virada=0; //para mostar a 2a carta da casa
        RenderTable(money, serif, imgs, renderer, playing_player, players_status, players_totals,inicio_jogo, players_results,
                    fim_da_ronda,house_total,carta_virada,house_cash,new_game,aposta, round);
        RenderHouseCards(house, pos_house_hand, cards, renderer,inicio_jogo,playing, carta_virada);
        RenderPlayerCards(player_cards, pos_player_hand, cards, renderer,playing);
        SDL_RenderPresent(renderer);
        SDL_Delay( 1000 );



        do // enquanto o total da casa for 16 ou menos dá carta á casa e desenha fazendo sempre uma actualização do total da casa e do status dela no inicio do ciclo
        {

            house_total=hand_value2 (players_totals,players_status,player_cards, house);

            if (house_total<17)
            {
                numero_carta=da_uma_carta(player_cards,house,pos_player_hand,&pos_house_hand,deck, 4); //4 para a carta ser dada á casa
                house_total=hand_value2 (players_totals,players_status,player_cards, house);
            }

            RenderTable(money, serif, imgs, renderer, playing_player, players_status, players_totals,inicio_jogo, players_results, fim_da_ronda,house_total,
                        carta_virada,house_cash,new_game,aposta, round);
            RenderHouseCards(house, pos_house_hand, cards, renderer,inicio_jogo,playing, carta_virada);
            RenderPlayerCards(player_cards, pos_player_hand, cards, renderer,playing);
            SDL_RenderPresent(renderer);
            SDL_Delay( 500 );

        }
        while (house_total<17);

        if (new_game==1)
        {
            quit=1;
        }

        if (numero_carta>ndecks*52/2) // tal como no casino, se atingirmos o meio do baralho, volta-se a baralhar
        {
            printf ("MEIO DO BARALHO ATINGIDO\n");
            Inic_deck(deck,ndecks);
            shufle (deck,ndecks);
        }

        fim_da_ronda=1;
        calc_money(players_totals, pos_player_hand,pos_house_hand,house_total,money,&house_cash,aposta,players_results, resultados, players_status); // calcula quem ganhou perdeu ou empatou e distribui o dinheiro

        RenderTable(money, serif, imgs, renderer, -1, players_status, players_totals,inicio_jogo,players_results,
                    fim_da_ronda, house_total,carta_virada,house_cash,new_game,aposta, round);
        RenderHouseCards(house, pos_house_hand, cards, renderer,inicio_jogo,playing,carta_virada);
        RenderPlayerCards(player_cards, pos_player_hand, cards, renderer,playing);
        SDL_RenderPresent(renderer);
        SDL_Delay (delay);

        do  //pede ao utilizador um comando, é resistente a inputs errados
        {
            scanf("%s",input);
            sscanf(input, "%c", &comando); // uso da funçao sscanf para evitar erros relacionados com dados incorrectos no buffer
        }
        while(comando!='n' && comando!='q'); // enquanto o comando nao for n ou q
        if (comando=='q') // no caso de q termina o programa e executa a funçao quit que escreve o ficheiro
        {
            quit_game (fp, resultados, house_cash,money); //escreve o ficheiro dos resultados
            quit=1; //flag que permite sair
        }

        inicio_jogo=1;
        fim_da_ronda=0;
        round++;
    }
    while (quit==0);

    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();



    return 0;
}
/**
*recebe uma veriavel do tipo FILE onde vao ser escritos os resultados, este ficheiro ja foi aberto em mode w na funçao main
*recebe tambem um vector com os resultados do jogo (n de derrotas vitorias e empates de cada jogador) e o vector money
*/
void quit_game ( FILE *fp, int resultados [][3], int house_balance, int*money )
{
    int aux;
    fprintf (fp,"   Player   * WINS * LOSSES * TIES * FINAL BALANCE\n");
    for (aux=0; aux<MAX_PLAYERS; aux++) // para cada jogador
    {
        fprintf (fp,"%s *  %i   *   %i    *   %i  *     %i \n", playerNames[aux], resultados[aux][2],resultados[aux][0],resultados[aux][1],money[aux]);
    }

    fprintf (fp,"\n\n               * House balance=%d *\n",house_balance);
    fclose(fp);
}

/**
*determina o resultado da ronda para cada jogador
*distribui o dionheiro
*/
void calc_money (int * player_totals, int *pos_players, int pos_house, int house_total, int*money, int *house_money, int aposta, int*players_results, int resultados[][3], int *player_status)
{
    int aux=0;
    int valor_blackjack;
    int blackjack=0;

    valor_blackjack=aposta/2;

    for (aux=0; aux<MAX_PLAYERS; aux++) // for every player
    {
        if (player_status[aux]!=4) // se o jogador participou na ronda
        {
            blackjack=0; // flag que está a 1 no caso de o jogador ter blackjack

            if (player_totals[aux]==21 && pos_players[aux]==2 && house_total<21) // o jogador fez blackjack e casa nao
            {

                money[aux]=money[aux] + valor_blackjack;
                *house_money=*house_money - valor_blackjack;
                printf ("PLAYER %d BLACKJACKED and house didnt\n",aux+1);
                players_results[aux]=2;
                resultados[aux][2]++;
                blackjack=1;

            }
            if ( (player_totals[aux]>house_total && player_totals[aux]<=21 && blackjack==0) || (house_total>21 && player_totals[aux]<=21 && blackjack==0) )//o jogador ganha
            {
                money[aux]=money[aux] + aposta;
                *house_money=*house_money-aposta;
                players_results[aux]=2;
                resultados[aux][2]++;
            }
            if (player_totals[aux]==house_total)// TIE
            {
                if (player_totals[aux]==21 || house_total==21)
                {
                    if (pos_players[aux]==2 && pos_house!=2)//PLAYER BLACKJACKS HOUSE DOESNT
                    {
                        money[aux]+=valor_blackjack;
                        *house_money=*house_money-valor_blackjack;
                        players_results[aux]=2;
                        resultados[aux][2]++;
                    }
                    if (pos_players[aux]!=2 && pos_house==2) //HOUSE BLACKJACKS PLAYER DOESNT
                    {
                        house_money+=aposta;
                        money[aux]-=aposta;
                        players_results[aux]=0;
                        resultados[aux][0]++;
                    }
                    if ( (pos_players[aux]==2 && pos_house==2) || (pos_players[aux]>2 && pos_house>2))// both blackjack or both have 21 but noone has blackjack
                    {
                        players_results[aux]=1;
                        resultados[aux][1]++;
                    }

                }
                else //outros casos de empate sem ser a 21 pontos
                {
                    players_results[aux]=1;
                    resultados[aux][1]++;
                }

            }
            if (player_totals[aux]>21)//bust
            {
                *house_money=*house_money+aposta;
                money[aux]-=aposta;
                players_results[aux]=0;
                resultados[aux][0]++;
            }
            if (house_total>player_totals[aux] && house_total<=21)//casa ter mais que o jogador sem fazer bust
            {
                *house_money=*house_money+aposta;
                money[aux]-=aposta;
                players_results[aux]=0;
                resultados[aux][0]++;

            }
        }

    }
}
/**
funão que recebe os vectores das mãos da casa e dos jogadores, e que em funçao do player especificado retorna o numero de ases que a mão do jogador em casa tem
*/
int ases (int player_cards[MAX_PLAYERS][MAX_CARTAS], int house_cards[MAX_CARTAS], int player)
{
    int aux;
    int carta;
    int nases=0;

    if (player<MAX_PLAYERS) // para os jogadores
    {
        for (aux=0; player_cards[player][aux]>-1; aux++)
        {
            carta=player_cards[player][aux];
            if ((carta+1)%13==0) //caso seja um ás incrementa a variavel nases que foi inicializada a 0
                nases++;
        }
    }
    if (player==MAX_PLAYERS) //para a casa
    {
        for (aux=0; house_cards[aux]>-1; aux++)
        {
            if (card_value(house_cards[aux])==11)
                nases++;
        }

    }
    return nases;
}
/**
avalia se ainda há jogadas por realizar na ronda, recebe o vector dos estados dos jogadores
0->bust
1->playing
2->blackjack
3->stand
4->out of money
retorna 1 se ainda se joga, ou 0 se ja nao ha jogadas
*/

int stil_playing (int*vector_status)
{
    int aux;
    int playing=0;//assumimos que há 0 jogadores que possam jogar

    for (aux=0; aux<MAX_PLAYERS; aux++) //determina se ainda há jogadores que podem jogar
    {
        if (vector_status[aux]==1) //no caso de encontrar pelo menos um que possa jogar
        {
            playing=1;
            break;
        }
    }
    return playing;

}
/**
da uma carta ao jogador especificado no parametro player, se o player for 0-3 dá ao jogador, se player=4 dá á casa
*/
int da_uma_carta (int(*vector)[MAX_CARTAS],int *casa ,int*pos, int* pos_house, int* deck, int player)
{
    int aux=0;
    int aux2=0;
    int aux3=0;

    for (aux2=0; aux2<MAX_DECK; aux2++) //determina a  1a posiçao do vector deck que tem uma carta que nao tenha o valor -1
    {
        if (deck[aux2]!=-1)
            break;
    }

    if (player<MAX_PLAYERS) // encontra a posição da mão do jogador que vai receber a carta
    {
        for (aux=0; aux<MAX_CARD_HAND; aux++)
        {
            if(vector[player][aux]==-1)
                break;
        }
        vector[player][aux]=deck[aux2]; //dá a carta
        pos[player]++; // mostrar mais uma carta
    }

    if (player==MAX_PLAYERS) // se a carta a dar for para a casa
    {
        for (aux3=0; aux3<MAX_CARD_HAND; aux3++)
        {
            if(casa[aux3]==-1)
                break;
        }

        casa[aux3]=deck[aux2];

        *pos_house=*pos_house+1;
    }
    deck[aux2]=-1; // tira a carta do deck

    return aux2;

}
/**
calcula os valores dos totais das maos dos jogadores e da casa
faz uso da função nases para o caso em que se ultrapassa os 21 pontos mas a mao tem ases a valerem 11
retorna o total da casa para usar no main
*/
int hand_value2 (int*player_values,int player_status[MAX_PLAYERS],int hands[][MAX_CARTAS], int *house)
{
    int aux=0, aux2=0;
    int valor_aux, house_aux;
    int value=0;
    int value_house=0;
    int nases=0;

    for (aux=0; aux<MAX_PLAYERS; aux++) //calcula os valores das maos de cada jogador
    {
        if (player_status[aux]!=4)
        {
            for (aux2=0; hands[aux][aux2]>-1; aux2++)
            {
                valor_aux=card_value(hands[aux][aux2]);
                value=value+valor_aux;
            }
            player_values[aux]=value;
        }
        value=0;
    }
    for (aux=0; aux<=MAX_PLAYERS; aux++) //checks if any hand total is over 21, if so search for ACES
    {
        nases=ases(hands,house,aux);
        if (player_values[aux]>21)
        {
            if (nases>0)
            {
                for (; player_values[aux]>21 && nases>0; nases--)
                    player_values[aux]=player_values[aux]-10;
            }
        }

    }


    for (aux=0; house[aux]!=-1; aux++) //para a casa
    {
        house_aux=card_value(house[aux]);
        value_house=value_house+house_aux;
    }

    nases=0;
    nases=ases(hands,house, 4);

    if (value_house>21 && nases>0)
    {
        for (; nases>0; nases--)
        {
            value_house-=10;
            if (value_house<=21)
            {
                break;
            }
        }
    }


    return value_house;
}
/**funçao auxiliar apenas para verificar os valores do vector ao longo do decorrer do programa*/
void print_status (int*player_status)
{
    int aux;
    for (aux=0; aux<MAX_PLAYERS; aux++)
    {
        printf ("[%d]",player_status[aux]);
    }

    printf("\n");
}
/**funçao auxiliar apenas para verificar os valores do vector ao longo do decorrer do programa*/
void print_totals (int*valores)
{
    int aux;

    for (aux=0; aux<=MAX_PLAYERS; aux++)
    {
        printf ("[%d] ", valores[aux] );

    }
    printf ("\n");


}
/**
actualisa o vector player_status em funçao do total das maos e do numero de cartas de cada jogador
esta funçao e sempre executada no main em sequencia com a funçao que calcula os totais
*/
void update_stats (int players_totals[MAX_PLAYERS],int player_status[MAX_PLAYERS],int hands[][MAX_CARTAS],int house[MAX_CARTAS], int pos_house, int* pos_players)
{
    int aux=0;
    int value=0;

    for (aux=0; aux<MAX_PLAYERS; aux++) //para todos os jogadores...
    {
        value=players_totals[aux];
        if (player_status[aux]!=4&& player_status[aux]!=3)
        {
            if (value==21 && pos_players[aux]==2) //blackjack
            {
                player_status[aux]=2;
            }
            if (value==21 && pos_players[aux]!=2) //auto-stand
            {
                player_status[aux]=3;
            }
            if (value>21) // bust!
            {
                player_status[aux]=0;
            }
            if (value<21 ) //playing
            {
                player_status[aux]=1;
            }
        }


    }
    return;
}
/**
recebe um numero entre 0 e 51 e retorna um inteiro com a valor da carta no jogo blackjack
*/
int card_value (int carta)
{
    int value=-1;

    for (carta=carta; carta>12; carta=carta-13)
    {}

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
/**funçao auxiliar apenas para verificar os valores do vector ao longo do decorrer do programa*/
void print_deck (int* deck)
{
    int i;

    for (i=0; i<MAX_DECK; i++)
    {
        printf ("[%d]", deck[i]);
        if (i==51||i==103||i==155||i==207||i==259||i==311)
        {
            printf("\n\n");
        }
    }

}
/**funçao auxiliar apenas para verificar os valores do vector ao longo do decorrer do programa*/
void print_hands (int player_cards[][MAX_CARTAS], int*house)
{
    int ii,jj,i;


    for (ii=0; ii<MAX_PLAYERS; ii++)
    {
        printf ("Mão do jogador %d:\n", ii+1);
        for(jj=0; jj<MAX_CARTAS; jj++)
        {
            printf ("%d;",player_cards[ii][jj]);
        }
        printf ("\n");
    }
    printf("Mão da casa:\n");
    for (i=0; i<=MAX_CARTAS; i++)
    {
        printf ("%d;",house[i]);
    }
    printf("\n\n");

}
/**função que baralha o deck*/
void shufle (int*deck,int ndecks)
{
    int i;
    int ncartas;
    int randomnumber;
    int auxvalue;

    ncartas=52*ndecks;
    printf ("SHUFLING DECKS...\n");
    //srand(time(NULL));
    srand (456);// semente de randomize pedida pelo professor

    for (i=0; i<ncartas; i++)
    {
        randomnumber=rand()%(ncartas-1); // gera um numero aleatorio entre 0 e (por exemplo 103) no caso de 2 baralhos
        auxvalue=deck[randomnumber]; //auxiliar que guarda o valor do deck nessa posição aleatoria

        deck[randomnumber]=deck[i]; // troca
        deck[i]=auxvalue;

    }

}
/**inicialisa o deck
mete 52 cartas de forma ordenada n vezes sendo que n=numero de decks
os restantes valores inicializa a -1 dado que são cartas que nao serao utilizadas
*/
void Inic_deck(int *vector, int ndecks)
{
    int auxi=0;
    int aux=0;
    int ncartas;
    ncartas=ndecks*52;

    for (auxi=0,aux=0; auxi<ncartas; auxi++) //cartas validas
    {

        vector[auxi]=aux;
        aux++;
        if (aux==52)
        {
            aux=0;
        }

    }

    for (auxi=auxi; auxi<=MAX_DECK; auxi++) //cartas nao utilizadas
    {
        vector[auxi]=-1;
    }

}
/**
retorna o valor da menor dos saldos dos varios jogadores para posteriormente calcular o valor da aposta maxima
*/
int valor_minimo (int*vector)
{
    int aux;
    int menor=vector[0]; //assumo que a menor aposta é a primeira

    for (aux=1; aux<MAX_PLAYERS; aux++)
    {
        if (vector[aux]<menor) // ao percorrer o vector das apostas, no caso de encontrar uma que seja mais baixa actualizo o valor da aposta mais baixa
        {
            menor=vector[aux];
        }

    }
    return menor;
}
/**
pede o valor para a aposta
*/
int pede_aposta (int*apostas)
{
    int menor_aposta, aposta, erro=1;
    char string[10];

    menor_aposta=valor_minimo(apostas);

    printf ("O valor das apostas sera fixo durante todo o jogo...\n Introduza o valor da aposta:\n");
    do
    {
        scanf ("%s",string);
        sscanf(string,"%d",&aposta);
        if (aposta<1 || aposta>(menor_aposta/5))
        {
            printf ("Aposta invalida!\nA aposta nao pode ser maior do que %d.\n",menor_aposta/5);
        }
        else
        {
            erro=0;
        }

    }
    while (erro==1);


    return aposta;
}
/**
pede o dinheiro inicial para cada jogador
*/

void pede_dinheiro (int*vector)
{
    char string[10];
    int money, aux=0;

    printf ("introduza as apostas dos varios jogadores...\n");
    do
    {
        printf ("Aposta do jogador %d:\n", aux+1);
        money=0;
        scanf ("%s",string);
        sscanf(string,"%d",&money);
        if (money>=10)
        {
            vector[aux]=money;
            aux++;
        }
        else
        {
            printf("A aposta tem que ser maior do que 10!\n");
        }

    }
    while (aux<MAX_PLAYERS);

    return;
}

/**
*pede_decks: pede ao utilizador para introduzir o numero de decks com que quer jogar
*retorna o pŕoprio valor inteiro correspondente ao numero de decks
*resistente a inputs errados como numeros maiores que 6; menores que 1; caracteres
*/
int pede_decks (void)
{
    int ndecks=0, erro=1;
    char string[10];

    printf ("_-_-_-_-BEM VINDO AO BLACKJACK!-_-_-_-_\nComo jogar:\n Use o teclado para escrever no terminal o comando:\n'h'->hit\n's'->stand!\nNo final da ronda prima 'n' para jogar novamente\n");
    printf ("Se desejar sair prima 'q' a qualquer altura\nBOM JOGO!\n");
    printf ("introduza o numero de decks com que quer jogar (1-6).\n ");
    do
    {
        scanf ("%s",string); //PEDE UMA STRING
        sscanf(string,"%d",&ndecks); //ANALISA ESSA MESMA STRING
        if (ndecks>=1 && ndecks<=6)
        {
            erro=0;
        }
        else
        {
            printf("Numero de decks invalido!\n");
        }

    }
    while (erro==1);

    return ndecks;
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
 * em funçao da fase do jogo em que nos encontramos (determinada pelas variaveis inicio, fim da ronda )e tambem o vector player status esolhe que tring imprimir e em que cor desenhar cada jogador
 */
void RenderTable(int _money[], TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer, int nplayer, int *player_status,
                 int* player_totals, int inicio, int*players_results, int fim_da_ronda, int house_total,int carta_virada,int house_money, int new_game, int aposta, int round)
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0 };
    SDL_Color red = { 255, 0, 0, 0};
    SDL_Color green = { 0,  255, 0};
    SDL_Color pink = {255 ,20 ,147,0};
    SDL_Color purple = {204 ,0 ,204,0};

    SDL_Color color;
    char name_money_str[STRING_SIZE];
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // seperates the left from the right part of the window
    int height;
    int total;
    char string_estado[30];
    char string_estado2[30];
    char string_round[30];

    nplayer--;

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
    height += RenderText(separatorPos+3*MARGIN, height, myName, _font, &black, _renderer);
    // this renders the student number
    RenderText(separatorPos+3*MARGIN, height, myNumber, _font, &black, _renderer);

    for ( int i = 0; i < MAX_PLAYERS; i++) //for every player
    {

        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 ); //by default cor=branca e a string de estado do jogador é string vazia
        color=white;
        strcpy(string_estado," ");

        if (fim_da_ronda==1); // se estivermos no final da ronda (depois dod dealer virar as cartas
        {
            total=player_totals[i]; // para apresentar o tatal da mão do jogador depois do nome

            if (players_results[i]==0) //se perdeu
            {
                SDL_SetRenderDrawColor(_renderer, 255 , 0 , 0 ,0 );
                color=red;
                strcpy (string_estado, "PERDEU");

            }
            if (players_results[i]==1)//se empatou
            {
                SDL_SetRenderDrawColor(_renderer, 255 , 255 , 0 ,0 );
                color=yellow;
                strcpy (string_estado, "EMPATOU");

            }
            if (players_results[i]==2) // se ganhou
            {
                SDL_SetRenderDrawColor(_renderer, 0,  255, 0 ,0 );
                color=green;
                strcpy (string_estado, "GANHOU");

            }
            if (player_status[i]==2 && players_results[i]==2) //se fez blackjack
            {
                SDL_SetRenderDrawColor(_renderer, 204 , 0 , 204 ,0 );
                color=purple;
                strcpy (string_estado, "BLACKJACK");

            }
        }
        if (fim_da_ronda==0) // se estivermos no meio da ronda (parte em que se faz hit ou stand)
        {
            if (nplayer==i && player_status[i]==1) // se o jogador puder jogar (status==1) e for a sua vez escreve playing por cima dele e fica a cor-de-rosa
            {
                SDL_SetRenderDrawColor(_renderer,255 ,20 ,147,0);

                if (inicio!=1)
                {
                    color=pink;
                    strcpy (string_estado, "PLAYING...");
                }

            }
            if (player_status[i]==2)
            {
                SDL_SetRenderDrawColor(_renderer, 0 , 0 , 0 ,0 );
                color=black;
                strcpy (string_estado, "BLACKJACK");

            }
            if (player_status[i]==3)
            {
                SDL_SetRenderDrawColor(_renderer, 0 , 0 , 0 ,0 );
                color=black;
                strcpy (string_estado, "STAND");
            }
            if (player_status[i]==0)
            {
                SDL_SetRenderDrawColor(_renderer, 0 , 0 , 0 ,0 );
                color=black;
                strcpy (string_estado, "BUST!");

            }
            if (inicio==1) // para quando o jogo for distribuido nao aparerecerem os totais ou os estados dos jogadores
            {
                total=0;
                SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255 );
                color=white;
                strcpy(string_estado," ");
            }
            else
            {
                total=player_totals[i];
            }

        }
        if (player_status[i]==4) // no caso de ja nao poder jogar
        {
            SDL_SetRenderDrawColor(_renderer, 0 , 0 , 0 ,0 );
            color=black;
            strcpy (string_estado, "OUT OF CASH!");
        }


        playerRect.x = i*(separatorPos/4-5)+10;
        playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
        playerRect.w = separatorPos/4-5;
        playerRect.h = (int) (0.42f*HEIGHT_WINDOW);
        sprintf(name_money_str,"%s -- %d$ (%d)", playerNames[i], _money[i], total);
        SDL_RenderDrawRect(_renderer, &playerRect);
        RenderText(playerRect.x+20, playerRect.y-50, string_estado, _font, &color, _renderer);

        if (player_status[i]!=4) // escreve a string de estado do jogador que é selecionada nos ifs
        {
            RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &color, _renderer);
        }

    }
//algumas strings informativas que aparecem no ecrã
    color=white;
    sprintf (string_estado, "House balance: %d$",house_money);
    RenderText(700, playerRect.y-150, string_estado, _font, &color, _renderer);
    sprintf (string_estado, "Valor da aposta: %d$",aposta);
    RenderText(700, 160, string_estado, _font, &color, _renderer);
    if (new_game==1)
    {
        sprintf (string_estado, "O jogo ira terminar no final da ronda!");
        RenderText(20, 160, string_estado, _font, &color, _renderer);
    }

    if (fim_da_ronda==1)
        sprintf (string_estado, "House total:%d",house_total);
    else
        sprintf (string_estado, "House total:?");

    if (fim_da_ronda==1)
        sprintf (string_estado2, "Press N for new round");
    else
        sprintf (string_estado2, " ");

    RenderText(20, 160, string_estado2, _font, &color, _renderer);

    RenderText(20, playerRect.y-150, string_estado, _font, &color, _renderer);

    sprintf (string_round, "ROUND %d", round);
    RenderText(400, 114, string_round, _font, &color, _renderer);
    // destroy everything
    SDL_DestroyTexture(table_texture);

}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderHouseCards(int _house[], int _pos_house_hand, SDL_Surface **_cards, SDL_Renderer* _renderer, int inicio, int playing, int carta_virada)
{
    int card, x, y;
    int div = WIDTH_WINDOW/CARD_WIDTH;

    // drawing all house cards
    for ( card = 0; card < _pos_house_hand; card++)
    {
        // calculate its position
        x = (div/2-_pos_house_hand/2+card)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        // render it !
        if (playing!=0)
            RenderCard(x, y, _house[card], _cards, _renderer);
    }
    // just one card ?: draw a card face down
    if (_pos_house_hand == 2 && carta_virada==1)
    {
        x = (div/2-_pos_house_hand/2+1)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        if (playing!=0)
            RenderCard(x, y, MAX_DECK_SIZE, _cards, _renderer);
    }
}

/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards(int _player_cards[][MAX_CARD_HAND], int _pos_player_hand[], SDL_Surface **_cards, SDL_Renderer* _renderer, int playing)
{
    int pos, x, y, num_player, card;

    // for every card of every player
    for ( num_player = 0; num_player < MAX_PLAYERS; num_player++)
    {
        for ( card = 0; card < _pos_player_hand[num_player]; card++)
        {
            // draw all cards of the player: calculate its position: only 4 positions are available !
            pos = card % 4;
            x = (int) num_player*((0.95f*WIDTH_WINDOW)/4-5)+(card/4)*12+15;
            y = (int) (0.55f*HEIGHT_WINDOW)+10;
            if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
            if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
            // render it !
            if( _player_cards[num_player][card]!=-1 && playing!=0)
                RenderCard(x, y, _player_cards[num_player][card], _cards, _renderer);
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
