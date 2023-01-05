#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>

/**
 * \file board.h
 *
 * \brief Gobblet game engine functions.
 *
 * In this file, all the functions for having a game of gobblets or of gobblet-gobblers 
 * run are defined.
 *  
 * \author Dorbec
 *
 */

/**
 * \mainpage SAE 1.1 and 1.2 for IUT Grand Ouest Normandie, computer science department
 * \section descritpion Description of the project
 * This project is aimed to implement a two player game, 
 * close to the Tic-Tac-Toe game. 
 * The rules of the games are described below.
 * 
 * The project is divided into various files. Mostly, the engine of the game,
 * implementing the detailed rules, is provided in the files board.c and board.h
 *
 * \section rules Detailed rules of the game.
 * 
 * The game is played by two players on a three by three squares board, 
 * though variants may be played on larger boards 
 * (in particular on a four by four board for gobblets).
 * Each player has control of a set of six pieces whose owner is easily 
 * identifiable, for example by the shape or the color of the piece.
 * The pieces are of three different
 * sizes (big, medium, small), each player having two of each size. 
 * 
 * During the course of the game, players alternately make a move that consists 
 * in either:
 * - adding a piece from their collection on the board.
 * - moving one of his visible pieces already on the board to another square.
 * 
 * A piece can be placed either on an empty square, 
 * or on a square containing only smaller pieces, 
 * independently of who owns the piece.
 * In that latter case, the piece covers and hides the smaller pieces.
 * When a piece placed over other pieces is moved from a square, 
 * the largest piece covered is revealed and can be played in a later turn.
 * 
 * The game is won by the first player who manages to make a visible line of 
 * his pieces across the board (on a line, a column, 
 * or a diagonal, like for Tic-Tac-Toe.)
 *
 */

/**
 * @brief Game dimension.
 *
 * For being general in terms of dimensions,
 * they are defined in a general parameter.
 * In the following, all indices are given from 0 to DIMENSIONS - 1.
 */
#define DIMENSIONS 3

/**
 * @brief Pointer to the structure that holds the game. 
 *
 * Details of the content are not necessary for its use, so the structure is not 
 * included here.
 */
typedef struct board_s * board;

/**
 * @brief the different sizes of pieces.
 *
 * This set includes references to an 'empty' size, for representing no piece at all. 
 * They are ordered by increasing size, so that they can be compared with < or >.
 */
enum sizes_e {NONE, SMALL, MEDIUM, LARGE};

/**
 * @brief the different sizes of pieces.
 *
 * This also includes a empty size for having a same type when there is no piece 
 * in a place.
 */
typedef enum sizes_e size;

/**
 */
enum return_code {
    OK,
    POSITION,
    SOURCE,
    TARGET
};


/**
 * @brief number of sizes for robustness if the enumeration is modified.
 */
#define NB_SIZE 3

/**
 * @brief number of pieces of each size per player at the beginning.
 */
#define NB_INITIAL_PIECES 2

/**
 * @brief the different players for further reference. 
 * NO_PLAYER is used when informing that a square is empty.
 */
enum players_e {
	NO_PLAYER, 
	PLAYER_1, 
	PLAYER_2
	};

typedef enum players_e player;

/**
 * @brief number of players in the game.
 */
#define NB_PLAYERS 2

/**@{
 * \name Game creation/deletion functionalities.
 */

/**
 * @brief Defines a new empty board for starting a game of gobblet-gobblers.
 *
 * The first player is always PLAYER_1
 * @return the newly created board
 */
board new_game();

/**
 * @brief Makes a deep copy of the game.
 * @param original_game the game to copy.
 * @return a new copy fully independent of the original game.
 */
board copy_game(board original_game);

/**
 * @brief Delete the game and frees all associated memory.
 * @param game the game to destroy.
 */
void destroy_game(board game);

/**@}*/

/**@{
 * \name Accessing game data functionalities.
 */

/**
 * @brief return the player whose turn it is to play.
 *
 * @param game the game to consider
 * @return the next player in the game
 */
player next_player(board game);

/**
 * @brief returns the player holding the indicated place.
 * @param game the game from which to collect information
 * @param line the line number
 * @param column the columne number
 * @return the player holding the biggest piece of that square, 
 * NO_PLAYER if no player is holding the place 
 * or if the request is misplaced.
 */
player get_place_holder(board game, int line, int column);

/**
 * @brief returns the visible piece size.
 * @param game the game from which to collect information.
 * @param line the line number of the square.
 * @param column the column number of the square.
 * @return the size of the holding piece, 
 * or NONE if there is no piece on this square of the board 
 * or is the request is misplaced.
 */
size get_piece_size(board game, int line, int column);

/**
 * @brief Checks whether the game has a winner already
 *
 * Recall that winning condition is to have a full row,
 * column or diagonal covered with visible pieces.
 * If both players fulfill the winning condition, this function returns NO_PLAYER.
 * One possible interpretation is that the game should continue until only 
 * one player is winning.
 * @param game the game to test.
 * @return the winning player or NO_PLAYER if no player is winning.
 */
player get_winner(board game);

/**
 * @brief Collect data about the pieces in a player's house.
 * 
 * Returns the number of pieces of the given size there are left
 * in the given player's house.
 * 
 * @param game the game to check
 * @param checked_player whose player house is to check
 * @param piece_size which piece size is to check
 * @return the number of pieces in the house, 0 if anything is wrong.
 */

int get_nb_piece_in_house(board game, player checked_player, size piece_size);


/**@}*/

/**@{
 * \name Playing functionalities
 */

/**
 * @brief Adds a piece on the board if possible.
 *
 * The piece is taken from the current player's house. 
 * Whether the piece is available is tested. 
 * The piece is then placed on the board, 
 * covering any existing smaller piece,
 * provided all the pieces on that place are smaller than the current piece.
 * Then it changes the players turn.
 * 
 * If placing the piece is not possible, this returns a positive return_code
 * encoding the problem identified: 
 *   0. OK: if everything went smoothly.
 *   1. POSITION: line or column do not correspond to a valid position.
 *   2. SOURCE: the chosen piece is not available in the player's house.
 *   3. TARGET: the chosen piece is available but too small to be put at the target place.
 *
 * @param game the game where to add a piece.
 * @param piece_size the size of the piece to add.
 * @param line the line number of where to place the piece.
 * @param column the column number of where to place the piece.
 * @return a return_code, positive if the operation fails.
 */
enum return_code place_piece(board game, size piece_size, int line, int column);

/**
 * @brief Moves a piece from a place to another, if possible.
 *
 * If it is a legal move for the current player, moves the piece, and end the player's turn.
 *
 * If placing the piece is not possible, this returns a positive return_code
 * encoding the problem identified: 
 *   0. SUCCESS: the move is successful
 *   1. POSITION: line or column do not correspond to a valid position.
 *   2. SOURCE: there is no visible piece belonging to the player on the corresponding place
 *   3. TARGET: a piece was available, but too small to be put at the target place.
 *
 * @param game the game where to move a piece.
 * @param source_line the line number of where to move the piece from.
 * @param source_column the column number of where to move the piece from.
 * @param target_line the line number of where to move the piece to.
 * @param target_column the column number of where to move the piece to.
 * @return an integer encoding the result of the placement, positive if the move failed.
 */
enum return_code move_piece(board game, int source_line, int source_column, int target_line, int target_column);

/**@}*/

/**
 * loads the game saved in filename and returns the loaded game.
 * Returns NULL if the load fails.
 * @param filename the filename to load the game from.
 * @return a new game containing the loaded game.
 **/
board load_game(char * filename);

/**
 * saves the game in the corresponding filename.
 * @param filename the filename to save the game to, extension .sav is suggested.
 **/
void save_game(board game, char * filename);



#endif /*_BOARD_H_*/





