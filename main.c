#include "board.h"   // Fichier utilisé pour le jeu
#include <ctype.h>   // Librairie utilisée pour la fonction isdigit()
#include <stdbool.h> // Librairie utilisée afin d'utiliser les booléens
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Librairie utilisée afin d'utiliser des fonctions pour les chaînes de caractères
#include <unistd.h> // Librairie utilisée pour la fonction sleep()

#define TAILLE_STRING_MAX 20
#define TAILLE_MAX_PLATEAU 3

// Définition des couleurs utilisés dans le printf
#define CBLUE "\x1B[34m"
#define CNORMAL "\x1B[0m"
#define CRED "\x1B[31m"

#define NBPTS_WIN 10
#define NBPTS_LOSE -8
// Fonctions dites utilitaires qu'on appelle dans plusieurs autres fonctions
/**
 * @brief Renvoie la longueur d'une chaine de caractères
 * @param string La chaîne de caractère a analyser
 */
int longueur_string(char string[]) {
  int count = 0;
  while (string[count] != '\0' && count < TAILLE_STRING_MAX - 1) {
    count++;
  }
  return count + 1;
}

/**
 * @brief Permet d'entrer une chaîne de caractères sans fautes de frappes
 * @param string La chaîne de caractère a analyser
 */
void entrer_string(char string[]) {
  bool accents = false;
  int i;

  while (scanf("%s", string) != 1 || accents == true) {
    printf("Problème d'enregistrement de l'entrée, veuillez réessayer.\n");

    accents = false;
    for (i = 0; i < longueur_string(string); i++) {
      if (isdigit(string[i] == 1)) {
        accents = true;
      }
    }
  }
}

/**
 * @brief Permet d'entrer un entier sans faire de fautes de frappes
 * @param min La borne minimum
 * @param max La borne maximum
 */
int entrer_entier(int min, int max) {
  int nb;
  while (scanf("%d", &nb) == 0 || nb < min || nb > max) {
    getchar();
  }
  return nb;
}

/**
 * @brief Permet d'afficher un nombre nb de retour à la ligne
 * @param nb Le nombre de retour a effectuer
 */
void printf_n(int nb) {
  int i;
  for (i = 0; i < nb; i++) {
    printf("\n");
  }
}

/**
 * @brief Permet d'afficher nb fois une chaîne de caractères
 * @param c La chaîne de caractère a afficher
 * @param nb Le nombre de fois
 */
void printf_char(char c[], int nb) {
  int i;
  for (i = 0; i < nb; i++) {
    printf("%s", c);
  }
}

// Choix du nom de l'utilisateur

/**
 * @brief Selection du nom du joueur
 * @param nom_joueur le nom du joueur
 * @param joueur_nb Le numéro initial du joueur
 */
void entrer_nom(char nom_joueur[], int joueur_nb) {
  int satif = 0;
  bool booleen = false;

  while (booleen == false) {
    printf("\nVeuillez entrer le nom du joueur n°%d : ", joueur_nb);

    entrer_string(nom_joueur);

    printf("Donc nous avons ↦ Joueur n°%d : %s\n", joueur_nb, nom_joueur);
    printf("Êtes-vous satisfait (0/1) : ");

    satif = entrer_entier(0, 1);
    if (satif == 1) {
      booleen = true;
    }
  }

  printf("Le nom a bien été enregistré.\n\n");
}

/**
 * @brief Affichage des pièces dans le tableau
 * @param game le jeu à verifier
 * @param char_f chaine de caractère finale
 * @param i la ligne
 * @param j la colonne
 */
char *piece(board game, char char_f[], int i, int j) {
  int k = 0, count;

  if (get_place_holder(game, i, j) == 1) {
    for (k = 0; k < longueur_string(CBLUE); k++) {
      char_f[k] = CBLUE[k];
    }
  } else {
    for (k = 0; k < longueur_string(CRED); k++) {
      char_f[k] = CRED[k];
    }
  }

  count = longueur_string(CRED) - 1;

  if (get_piece_size(game, i, j) == 1) {
    char_f[count] = 'S';
  } else if (get_piece_size(game, i, j) == 2) {
    char_f[count] = 'M';
  } else if (get_piece_size(game, i, j) == 3) {
    char_f[count] = 'L';
  } else if (get_piece_size(game, i, j) == 0) {
    char_f[count] = ' ';
  }
  count++;
  for (k = count; k < count + longueur_string(CNORMAL); k++) {
    char_f[k] = CNORMAL[k - count];
  }

  return char_f;
}

// Affichage du plateau

/**
 * @brief Affiche le plateau
 * @param game le jeu à verifier
 * @param nb_tour le numéro du tour
 */
void printf_plateau(board game, int nb_tour) {
  int i;
  char c[40];
  int temp;
  temp = system("clear");
  temp++;

  printf("═══════ Tour n°%2d ═══════\n\n", nb_tour);
  printf("    0       1       2\n\n");
  printf("╔═══════╦═══════╦═══════╗");
  
  for (i = 0; i < TAILLE_MAX_PLATEAU; i++) {
    printf("\n");

    printf_char("║       ", 3);
    printf("║\n");

    printf("║   ");
    printf("%s", piece(game, c, i, 0));
    printf("   ║   %s", piece(game, c, i, 1));
    printf("   ║   %s", piece(game, c, i, 2));
    printf("   ║    %d\n", i);

    printf_char("║       ", 3);
    printf("║\n");
    if (i < TAILLE_MAX_PLATEAU-1) {
      printf("╠═══════╬═══════╬═══════╣");
    }
  }
  printf("╚═══════╩═══════╩═══════╝");

  printf_n(2);
}

// Fonction interne du jeu

/**
 * @brief Demande le choix de jeu au joueur
 * @param game le jeu à verifier
 * @param player_nb le numéro du joueur
 * @param nom_joueur le nom du joueur
 */
int choix_joueur(board game, int player_nb, char nom_joueur[]) {
  int small, medium, large, choice;

  printf("\nIl te reste : \n");

  small = get_nb_piece_in_house(game, player_nb, SMALL);
  medium = get_nb_piece_in_house(game, player_nb, MEDIUM);
  large = get_nb_piece_in_house(game, player_nb, LARGE);

  printf("      1. SMALL ║ 2. MEDIUM ║ 3. LARGE \n");
  printf("     ══════════╬═══════════╬══════════\n");
  printf("          %d    ║     %d     ║    %d     \n", small, medium, large);
  printf_n(1);
  printf("Saisis la taille de la pièce que tu veut placer : ");
  choice = entrer_entier(1, 3);
  while (small == 0 && choice == 1) {
    printf(
        "Erreur, vous n'avez plus de pièce de type SMALL, veuillez réessayer.");
    choice = entrer_entier(1, 3);
  }
  while (medium == 0 && choice == 2) {
    printf("Erreur, vous n'avez plus de pièce de type MEDIUM, veuillez "
           "réessayer.");
    choice = entrer_entier(1, 3);
  }
  while (large == 0 && choice == 3) {
    printf(
        "Erreur, vous n'avez plus de pièce de type LARGE, veuillez réessayer.");
    choice = entrer_entier(1, 3);
  }

  switch (choice) {
  case 1:
    printf("Votre choix de pièce est : SMALL\n");
    small--;
    break;
  case 2:
    printf("Votre choix de pièce est : MEDIUM\n");
    medium--;
    break;
  case 3:
    printf("Votre choix de pièce est : LARGE\n");
    large--;
    break;
  default:
    break;
  }
  return choice;
}

/**
 * @brief Choix de la ligne
 * @return Numéro choisi
 */
int choice_line() {
  int line;
  line = entrer_entier(0, 2);
  return line;
}

/**
 * @brief Choix de la colonne
 * @return Numéro choisi
 */
int choice_column() {
  int colomn;
  colomn = entrer_entier(0, 2);
  return colomn;
}

/**
 * @brief Ajoute une piece sur le plateau
 * @param game le jeu à verifier
 * @param player_name le nom du joueur
 */
void placer_piece(board game, char *player_name, int tour) {
  int choice, line, column, piece;
  choice = choix_joueur(game, tour, player_name);

  printf("Veuillez saisir la ligne que vous souhaiter (0, 2) : ");
  line = choice_line();
  printf("Veuillez saisir la colonne que vous souhaiter (0, 2) : ");
  column = choice_column();

  piece = get_piece_size(game, line, column);

  while (piece >= choice) {
    printf("Erreur, votre pièce ne peut pas manger la pièce déjà présente !\n");
    choice = choix_joueur(game, tour, player_name);
    printf("Veuillez saisir la ligne que vous souhaiter (0, 2) : ");
    line = choice_line();
    printf("Veuillez saisir la colonne que vous souhaiter (0, 2) : ");
    column = choice_column();
    piece = get_piece_size(game, line, column);
  }

  place_piece(game, choice, line, column);
}

/**
 * @brief Permet de déplacer des pièces
 * @param game Le plateau de jeu
 * @param nb_player Le numéro du joueur
 */
void deplacer_piece(board game, int * nb_player) {
  int line_b, col_b, col_f, line_f, piece_pres, piece_size_f, piece_size_i;
  bool boolean = false;

  printf("Veuillez saisir la ligne de départ : ");
  line_b = choice_line();
  printf("Veuillez saisir la colonne de départ : ");
  col_b = choice_column();
  printf("Veuillez saisir la ligne d'arrivée : ");
  line_f = choice_line();
  printf("Veuillez saisir la colonne d'arrivée : ");
  col_f = choice_column();
  while (boolean == false) {
    piece_pres = get_place_holder(game, line_b, col_b);
    piece_size_f = get_piece_size(game, line_f, col_f);
    piece_size_i = get_piece_size(game, line_b, col_b);
    if (piece_pres != *nb_player) {
      printf("Erreur, ce n'est pas votre pièce !\n");
      boolean = true;
      *nb_player = next_player(game);
      sleep(3);
    } else {
      if (piece_size_i > piece_size_f) {
        move_piece(game, line_b, col_b, line_f, col_f);
        boolean = true;
      } else {
        printf(
            "Vous ne pouvez pas déplacer votre piece, veuillez réessayer.\n");
        printf("Veuillez saisir la ligne de départ : ");
        line_b = choice_line();
        printf("Veuillez saisir la colonne de départ : ");
        col_b = choice_column();
        printf("Veuillez saisir la ligne d'arrivée : ");
        line_f = choice_line();
        printf("Veuillez saisir la colonne d'arrivée : ");
        col_f = choice_column();
      }
    }
  }
}

/**
 * @brief Fonction affichée afin d'afficher le titre "SAE 1.01" affiché au démarrage
 */
void disp_title() {
  printf(" .d8888b.        d8888 8888888888       d888        .d8888b.   d888   \n");
  printf("d88P  Y88b      d88888 888             d8888       d88P  Y88b d8888   \n");
  printf("Y88b.          d88P888 888               888       888    888   888   \n");
  printf(" \"Y888b.      d88P 888 8888888           888       888    888   888   \n");
  printf("    \"Y88b.   d88P  888 888               888       888    888   888   \n");
  printf("      \"888  d88P   888 888               888       888    888   888   \n");
  printf("Y88b  d88P d8888888888 888               888   d8b Y88b  d88P   888   \n");
  printf(" \"Y8888P\" d88P     888 8888888888      8888888 Y8P  \"Y8888P\"  8888888\n\n");
}

/**
 * @brief Permet de récupérer les données du fichier classement.txt
 * @param rank_name Le tableau qui contiendra les noms des joueurs du leaderboard
 * @param rank_scores Le tableau contenant les scores de ces joueurs
 * @return nb Le nombre de joueurs présents dans le fichier classement.txt
 */
int retrieve_score(char rank_names[100][20], int rank_scores[100][2]){
  FILE *f = fopen("classement.txt", "r");
  int i, nb, temp; // La variable temp est utilisée afin d'éviter des Warning liés au fscanf
  temp = fscanf(f,"%d\n",&nb);
  temp++; 
  for (i = 0 ; i < nb ; i++) {
    temp = fscanf(f, "%s ", rank_names[i]);
    temp = fscanf(f,"%d\n", &rank_scores[i][0]);
    rank_scores[i][1] = i;
  }
  
  fclose(f);
  return nb;
}

/**
 * @brief Fonction principale utilisée dans le jeu, elle coordinne l'appel aux
 * autres fonctions
 * @param game Le jeu utilisé
 */
void jeu(board game) {
  bool gagnant = false, boolean_bis = false, choice;
  char nom_joueur1[TAILLE_STRING_MAX], nom_joueur2[TAILLE_STRING_MAX];
  int count = 0, nb_player = 0, nb_tours = 0, i;

  // Menu de choix pour le chargement d'une sauvegarde
  FILE * f = fopen("save.txt","r");
  if (f != NULL) {
    printf("Nous avons détecté une sauvegarde non-terminée, voulez-vous la recharger (0/1) : ");
    choice = entrer_entier(0,1);

    if (choice == 1) {
      game = load_game("save.txt");
      printf("La partie a été chargée !\n");
    } else {
      printf("C'est noté, la partie est réinitialisée.\n");
    }
  }

  // Saisie des pseudonymes
  do {
    entrer_nom(nom_joueur1, PLAYER_1);
    entrer_nom(nom_joueur2, PLAYER_2);
    if (strcmp(nom_joueur1, nom_joueur2) == 0) {
      printf("Les pseudonymes sont identiques, veuillez recommencer les "
             "entrées :\n");
      sleep(1);
    } else {
      boolean_bis = true;
    }
  } while (boolean_bis == false);

  // Boucle principale du jeu fonctionnant jusqu'à obtention d'un gagnant
  while (gagnant == false) {
    nb_player = next_player(game);
    nb_tours = nb_tours + 1;
    printf_plateau(game, nb_tours);
    save_game(game, "save.txt");
    if (nb_player == PLAYER_1) {
      printf("%s%s%s :\n", CBLUE, nom_joueur1, CNORMAL);
      printf("Voulez vous placer une pièce ou en déplacer une (1, 2) : ");
      count = entrer_entier(1, 2);
      if (count == 1) {
        placer_piece(game, nom_joueur1, nb_player);
      } else {
        deplacer_piece(game, &nb_player);
      }
    } else {
      printf("%s%s%s :\n", CRED, nom_joueur2, CNORMAL);
      printf("Voulez vous placer une pièce ou en déplacer une (1, 2) : ");
      count = entrer_entier(1, 2);
      if (count == 1) {
        placer_piece(game, nom_joueur2, nb_player);
      } else {
        deplacer_piece(game, &nb_player);
      }
    }

    if (get_winner(game) != 0) {
      gagnant = true;
      printf_plateau(game, nb_tours);
      // Affichage des messages lors de la victoire d'un joueur
      if (get_winner(game) == 1) {
        printf("Bravo %s%s%s ! Vous avez gagné le jeu :)\n\n\n", CBLUE, nom_joueur1, CNORMAL);
      } else if (get_winner(game) == 2) {
        printf("Bravo %s%s%s ! Vous avez gagné le jeu :)\n\n\n", CRED, nom_joueur2, CNORMAL);
      }

      // Mise à jour des scores des joueurs
      char rank_names[100][20];
      int rank_scores[100][2], nb_p, nb_alr = 0;
      nb_p = retrieve_score(rank_names,rank_scores) + 2;
      int bool_p1 = 0, bool_p2 = 0;
      FILE *f = fopen("classement.txt", "w+");
      
      for (i = 0; i<nb_p - nb_alr; i++) {
        if (strcmp(rank_names[i], nom_joueur1) == 0) {
          if (get_winner(game) == 1) {
            rank_scores[i][0] = rank_scores[i][0] + NBPTS_WIN;
          } else {
            rank_scores[i][0] = rank_scores[i][0] + NBPTS_LOSE;
          }
          bool_p1 = 1;
          nb_alr++;
        } else if (strcmp(rank_names[i], nom_joueur2) == 0) {
          if (get_winner(game) == 2) {
            rank_scores[i][0] = rank_scores[i][0] + NBPTS_WIN;
          } else {
            rank_scores[i][0] = rank_scores[i][0] + NBPTS_LOSE;
          }
          bool_p2 = 1;
          nb_alr++;
        }
      }
      if (nb_alr == 2 || nb_alr == 0) {
        nb_p = nb_p - 2;
        if (nb_alr == 2) {
          fprintf(f,"%d\n",nb_p);
        } else {
          fprintf(f,"%d\n",nb_p+2);
        }
        for (i=0;i<nb_p;i++) {
          fprintf(f,"%s %d\n",rank_names[i],rank_scores[i][0]);
        }
        if(strcmp(rank_names[nb_p], nom_joueur1) != 0 && strcmp(rank_names[nb_p], nom_joueur2) != 0 && nb_alr == 0){
          if (get_winner(game) == 1) {
            fprintf(f, "%s %d\n", nom_joueur1, 0+NBPTS_WIN);
            fprintf(f, "%s %d\n", nom_joueur2, 0+NBPTS_LOSE);
          } else {
            fprintf(f, "%s %d\n", nom_joueur1, 0+NBPTS_LOSE);
            fprintf(f, "%s %d\n", nom_joueur2, 0+NBPTS_WIN);
          }
        }
      } else if (nb_alr == 1) {
        nb_p = nb_p - 1;
        printf("ICI\n");
        if (bool_p2 == 1) {
          strcpy(rank_names[nb_p - nb_alr],nom_joueur1);
          rank_scores[nb_p - nb_alr][0] = NBPTS_WIN;
        } else if (bool_p1 == 1) {
          strcpy(rank_names[nb_p - nb_alr],nom_joueur2);
          rank_scores[nb_p - nb_alr][0] = NBPTS_LOSE;
        }
        fprintf(f,"%d\n",nb_p);
        for (i=0;i<nb_p;i++) {
          fprintf(f,"%s %d\n",rank_names[i],rank_scores[i][0]);
        }
      }

      fclose(f);
      // Suppression du fichier de sauvegarde après la victoire d'un joueur
      remove("save.txt");
    }
  }
}

/**
 * @brief Fonction d'affichage du classement
 */
void display_classement(){
  int temp, i, nb;
  bool change;
  char c;
  temp = system("clear");
  temp++;

  FILE *f = fopen("classement.txt", "r");
  if(f == NULL){
    printf("Une erreur est survenue, veuillez réessayer.");
    return;
  } 
  
  char rank_names[100][20];
  int rank_scores[100][2];
  nb = retrieve_score(rank_names,rank_scores);

  change = true;
  while (change == true) {
    change = false;
    for (i=0;i<nb-1;i++) {
      if (rank_scores[i][0] < rank_scores[i+1][0]) {
        temp = rank_scores[i][0];
        rank_scores[i][0] = rank_scores[i+1][0];
        rank_scores[i+1][0] = temp;

        temp = rank_scores[i][1];
        rank_scores[i][1] = rank_scores[i+1][1];
        rank_scores[i+1][1] = temp;
        change = true;
      }
    }
  }
  
  // Affiche du titre "Ranking"
  printf(" 888888ba                    dP       oo                   \n");
  printf(" 88    `8b                   88                            \n");
  printf("a88aaaa8P' .d8888b. 88d888b. 88  .dP  dP 88d888b. .d8888b. \n");
  printf(" 88   `8b. 88'  `88 88'  `88 88888\"   88 88'  `88 88'  `88 \n");
  printf(" 88     88 88.  .88 88    88 88  `8b. 88 88    88 88.  .88 \n");
  printf(" dP     dP `88888P8 dP    dP dP   `YP dP dP    dP `8888P88 \n");
  printf("                                                       .88 \n");
  printf("                                                   d8888P\n");

  printf("╔════════════════╦════════════════╗\n");
  printf("║ Players' names ║     Scores     ║\n");
  printf("╠════════════════╬════════════════╣\n");
  for (i = 0; i<nb; i++) {
    printf("║%11s     ║   %6d       ║\n",rank_names[rank_scores[i][1]],rank_scores[i][0]);
  }
  printf("╚════════════════╩════════════════╝\n");
  
  fclose(f);

  printf("Entrez un caractère pour continuer...\n");
  entrer_string(&c);
  
}

/**
 * @brief Fonction de choix entre le jeu et l'affichage du classement
 * @param game Le jeu utilisé
 */
void menu(board game){
  int choice;
  printf("Bienvenue sur le jeu gobblet-gobblers !\n");
  printf_n(2);
  disp_title();
  printf_n(2);

  printf("Veuillez faire un choix: \n\n");
  printf("Jouer une partie (1) / Voir le classement (2)\n");
  
  choice = entrer_entier(1, 2);
  if(choice == 2){
    display_classement();
    int temp;
    temp = system("clear");
    temp++;
    menu(game);
  }
  else jeu(game);
}

/**
 * @brief Fonction main()
 */
int main() {

  int temp;
  temp = system("clear");
  temp++; // Pour éviter un warning venant de la fonction system()
  FILE * f = fopen("classement.txt", "a");
  if (f == NULL) {
    fclose(f);
    FILE * f1 = fopen("classement.txt","w+");
    fclose(f1);
  }
  
  board game = new_game();
  menu(game);
  destroy_game(game);
  return 0;
}