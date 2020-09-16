# Blackjack Game

Fully functional blackjack game. The ability to add "Artificial Players" was implemented and their playing strategy was programmed in order to reach the maximum profit.

# Running

To compile the code, the following command must be used:

```sh
gcc main.c bj.c -g -I/usr/local/include -Wall -pedantic -std=c99 -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -o blackjack
```

The folder including the used libraries can vary according to the Linux version used. (Use the command "sdl2-config --cflags --libs" to find the right location)

To run the game, the following command must be used:

```sh
./blackjack.c parametros.txt EA.txt
```


# Functioning

The player has 4 options when playing:

* Hit - Press H: The player gets a card.
* Stand - Press S: Pass. The next player plays.
* New -  Press N: The game starts a new round.
* Quit - Press Q: The game ends and writs an output file with the game statistics.
* Doube - Press D: Duplicates the value of the bet.
* Surrender - Press R: The player quits the game and the dealer gets half of the bet.
* Bet - Press B: Select the value of a new bet.
* Add new Player - Press A: At the end of each round the user can ad a new Human or Artificial player. The information about the player must be specified in the command line.

# Input File

The input file has a series of parameters which will be used to specify:

* The number of decks of 52 cards. Can be between 4 and 8 decks.
* The number of players that will participate in the game.
For each player, the file contains information about:

* Specifies if the player is human (HU) or artificial (EA).
* Players name. Maximum size is 8 characters.
* Initial value each player begins the game. Between 10 and 500.
* Initial bet value of each player. 

An example of an input file with the name parametros.txt is provided.

# Output Statistics File

The output file prints statistics about each player, specifying:

* Name of the player.
* The number of games the player has won, tie or lost.
* The money value each player has ended the game.
* The money value each player has won or lost. 

An example of output file is provided.
