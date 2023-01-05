#include "board.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * \file board.c
 *
 * \brief Source code associated with \ref board.h
 *
 * \author Alenso LOPES && Julien SAILLY
 */

/**
 * @brief The board of the game. sizes
 */
struct board_s {
  enum sizes_e cases_t[DIMENSIONS][DIMENSIONS][NB_SIZE + 1];
  enum players_e cases_p[DIMENSIONS][DIMENSIONS][NB_SIZE + 1];
  enum players_e turn;
  int houses[NB_PLAYERS][NB_SIZE + 1];
};

/**
 * @brief Defines a new empty board for starting a game of gobblet-gobblers.
 *
 * The first player is always PLAYER_1
 * @return the newly created board
 */
board new_game() {
  board new_board = malloc(sizeof(struct board_s));

  int i, j, k;
  for (i = 0; i < DIMENSIONS; i++) {
    for (j = 0; j < DIMENSIONS; j++) {
      for (k = 0; k < NB_SIZE; k++) {
        new_board->cases_t[i][j][k] = NONE;
        new_board->cases_p[i][j][k] = NO_PLAYER;
      }
    }
  }

  new_board->turn = PLAYER_2;

  for (i = 0; i < NB_PLAYERS; i++) {
    for (j = 0; j < NB_SIZE; j++) {
      new_board->houses[i][j] = NB_INITIAL_PIECES;
    }
  }

  return new_board;
}

/**
 * @brief Makes a deep copy of the game.
 * @param original_game the game to copy.
 * @return a new copy fully independent of the original game.
 */
board copy_game(board original_game) {
  board new_board = malloc(sizeof(struct board_s));
  int i, j;

  for (i = 0; i < DIMENSIONS; i++) {
    for (j = 0; j < DIMENSIONS; j++) {
      for (int k = 0; k < NB_SIZE; k++) {
        new_board->cases_t[i][j][k] = original_game->cases_t[i][j][k];
        new_board->cases_p[i][j][k] = original_game->cases_p[i][j][k];
      }
    }
  }

  for (i = 0; i < NB_PLAYERS; i++) {
    for (j = 0; j < NB_SIZE; j++) {
      new_board->houses[i][j] = original_game->houses[i][j];
    }
  }

  return new_board;
}

/**
 * @brief Delete the game and frees all associated memory.
 * @param game the game to destroy.
 */
void destroy_game(board game) { free(game); }

/**@{
 * \name Accessing game data functionalities.
 */

/**
 * @brief return the player whose turn it is to play.
 *
 * @param game the game to consider
 * @return the next player in the game
 */
player next_player(board game) {
  if (game->turn == PLAYER_1)
    game->turn = PLAYER_2;
  else
    game->turn = PLAYER_1;

  return game->turn;
}

/**
 * @brief returns the player holding the indicated place.
 * @param game the game from which to collect information
 * @param line the line number
 * @param column the columne number
 * @return the player holding the biggest piece of that square, 
 * NO_PLAYER if no player is holding the place 
 * or if the request is misplaced.
 */
player get_place_holder(board game, int line, int column) {
  int i;

  for (i = NB_PLAYERS; i >= 0; i--) {
    if (game->cases_p[line][column][i] != NO_PLAYER)
      return game->cases_p[line][column][i];
  }

  return NO_PLAYER;
}

/**
 * @brief returns the visible piece size.
 * @param game the game from which to collect information.
 * @param line the line number of the square.
 * @param column the column number of the square.
 * @return the size of the holding piece, 
 * or NONE if there is no piece on this square of the board 
 * or is the request is misplaced.
 */
size get_piece_size(board game, int line, int column) {
  int i;

  for (i = NB_SIZE - 1; i >= 0; i--) {
    if (game->cases_t[line][column][i] != NONE)
      return game->cases_t[line][column][i];
  }

  return NONE;
}

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
player get_winner(board game) {
  int i, player_w[2] = {0, 0};

  for (i = 0; i < DIMENSIONS; i++) {
    if (get_place_holder(game, i, 0) == get_place_holder(game, i, 1) &&
        get_place_holder(game, i, 1) == get_place_holder(game, i, 2) &&
        get_place_holder(game, i, 2) != NO_PLAYER) {
      player_w[get_place_holder(game, i, 0) - 1] = 1;
    }
    if (get_place_holder(game, 0, i) == get_place_holder(game, 1, i) &&
        get_place_holder(game, 1, i) == get_place_holder(game, 2, i) &&
        get_place_holder(game, 2, i) != NO_PLAYER) {
      player_w[get_place_holder(game, 0, i) - 1] = 1;
    }
  }

  if (get_place_holder(game, 0, 0) == get_place_holder(game, 1, 1) &&
      get_place_holder(game, 1, 1) == get_place_holder(game, 2, 2) &&
      get_place_holder(game, 2, 2) != NO_PLAYER) {
    player_w[get_place_holder(game, 0, 0) - 1] = 1;
  }

  if (get_place_holder(game, 0, 2) == get_place_holder(game, 1, 1) &&
      get_place_holder(game, 1, 1) == get_place_holder(game, 2, 0) &&
      get_place_holder(game, 2, 0) != NO_PLAYER) {
    player_w[get_place_holder(game, 2, 0) - 1] = 1;
  }

  if (player_w[0] == 1 && player_w[1] == 0)
    return PLAYER_1;
  else if (player_w[0] == 0 && player_w[1] == 1)
    return PLAYER_2;
  return NO_PLAYER;
}

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
int get_nb_piece_in_house(board game, player checked_player, size piece_size) {
  return game->houses[checked_player - 1][piece_size - 1];
}

/**@}*/

/**@{
 * \name Playing functionalities
 */

/**
 * @brief Remove a pice in a player's house according to the piece size
 * and the player currently playing.
 * 
 * @param game The game used
 * @param piece_size The piece to remove
 */
void rm_piece(board game, size piece_size) {
  game->houses[game->turn - 1][piece_size - 1] =
      game->houses[game->turn - 1][piece_size - 1] - 1;
}

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
enum return_code place_piece(board game, size piece_size, int line,
                             int column) {

  if (get_nb_piece_in_house(game, game->turn, piece_size) <= 0) {
    return SOURCE;
  } else if ((line < 0 || line >= DIMENSIONS) ||
             (column < 0 || column >= DIMENSIONS)) {
    return POSITION;
  } else if (piece_size <= get_piece_size(game, line, column)) {
    return TARGET;
  } else {
    game->cases_p[line][column][piece_size - 1] = game->turn;
    game->cases_t[line][column][piece_size - 1] = piece_size;
    rm_piece(game, get_piece_size(game, line, column));
    return OK;
  }
}

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
enum return_code move_piece(board game, int source_line, int source_column,
                            int target_line, int target_column) {
  int piece_size;

  if ((source_line < 0 || source_line >= DIMENSIONS) ||
      (source_column < 0 || source_column >= DIMENSIONS) ||
      (target_line < 0 || target_line >= DIMENSIONS) ||
      (target_column < 0 || target_column >= DIMENSIONS)) {
    return TARGET;
  } else if (get_place_holder(game, source_line, source_column) != game->turn) {
    return SOURCE;
  } else if (get_piece_size(game, source_line, source_column) <=
             get_piece_size(game, target_line, target_column)) {
    return TARGET;
  } else {
    piece_size = get_piece_size(game, source_line, source_column);

    game->cases_p[target_line][target_column][piece_size - 1] = game->turn;

    game->cases_t[target_line][target_column][piece_size - 1] = piece_size;

    game->cases_p[source_line][source_column][piece_size - 1] = NO_PLAYER;

    game->cases_t[source_line][source_column][piece_size - 1] = NONE;

    return OK;
  }
}

/**@}*/

/**
 * saves the game in the corresponding filename.
 * @param filename the filename to save the game to, extension .sav is suggested.
 **/
void save_game(board game, char *filename) {
  FILE *f = fopen(filename, "w+");
  int i, j, k;

  fprintf(f, "sizes: ");
  for (i = 0; i < DIMENSIONS; i++) {
    for (j = 0; j < DIMENSIONS; j++) {
      for (k = 0; k < NB_SIZE + 1; k++) {
        fprintf(f, "%d,", game->cases_t[i][j][k]);
      }
      fprintf(f, ";");
    }
    fprintf(f, "|");
  }
  fprintf(f, "\n");

  fprintf(f, "players: ");
  for (i = 0; i < DIMENSIONS; i++) {
    for (j = 0; j < DIMENSIONS; j++) {
      for (k = 0; k < NB_SIZE + 1; k++) {
        fprintf(f, "%d,", game->cases_p[i][j][k]);
      }
      fprintf(f, ";");
    }
    fprintf(f, "|");
  }
  fprintf(f, "\n");

  fprintf(f, "turn_player: %d,\n", game->turn);

  fprintf(f, "houses: ");
  for (i = 0; i < NB_PLAYERS; i++) {
    for (j = 0; j < NB_SIZE; j++) {
      fprintf(f, "%d,", game->houses[i][j]);
    }
    fprintf(f, "|");
  }

  fprintf(f, "%c", EOF);

  fclose(f);
}

/**
 * loads the game saved in filename and returns the loaded game.
 * Returns NULL if the load fails.
 * @param filename the filename to load the game from.
 * @return a new game containing the loaded game.
 **/
board load_game(char *filename) {
  board game = new_game();
  FILE *f = fopen("save.txt", "r");
  char c;
  int i, j, k, val, nb;

  val = fscanf(f, "sizes: ");
  for (i = 0; i < DIMENSIONS; i++) {
    for (j = 0; j < DIMENSIONS; j++) {
      for (k = 0; k < NB_SIZE + 1; k++) {
        val = fscanf(f, "%d,", &nb);
        switch (nb) {
        case 0:
          game->cases_t[i][j][k] = NONE;
          break;
        case 1:
          game->cases_t[i][j][k] = SMALL;
          break;
        case 2:
          game->cases_t[i][j][k] = MEDIUM;
          break;
        case 3:
          game->cases_t[i][j][k] = LARGE;
          break;
        default:
          game->cases_t[i][j][k] = NONE;
          break;
        }
      }
      val = fscanf(f, ";");
    }
    val = fscanf(f, "|");
  }
  val++;
  val = fscanf(f, "\n");

  val = fscanf(f, "players: ");
  for (i = 0; i < DIMENSIONS; i++) {
    for (j = 0; j < DIMENSIONS; j++) {
      for (k = 0; k < NB_SIZE + 1; k++) {
        val = fscanf(f, "%d,", &nb);
        switch (nb) {
        case 0:
          game->cases_p[i][j][k] = NO_PLAYER;
          break;
        case 1:
          game->cases_p[i][j][k] = PLAYER_1;
          break;
        case 2:
          game->cases_p[i][j][k] = PLAYER_2;
          break;
        default:
          game->cases_t[i][j][k] = NONE;
          break;
        }
      }
      val = fscanf(f, ";");
    }
    val = fscanf(f, "|");
  }
  val = fscanf(f, "\n");

  val = fscanf(f, "turn_player: %d,\n", &nb);
  switch (nb) {
  case 1:
    game->turn = PLAYER_2;
    break;
  case 2:
    game->turn = PLAYER_1;
    break;
  default:
    game->turn = PLAYER_1;
    break;
  }

  val = fscanf(f, "houses: ");
  for (i = 0; i < NB_PLAYERS; i++) {
    for (j = 0; j < NB_SIZE; j++) {
      val = fscanf(f, "%d,", &game->houses[i][j]);
    }
    val = fscanf(f, "|");
  }

  val = fscanf(f, "%c", &c);

  fclose(f);
  return game;
}