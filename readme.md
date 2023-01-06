# Gobblets-Gobblers
This project _Gobblets-Gobblers_ was realized by Alenso Lopes and Julien Sailly in the framework of study in BUT Informatique.

Coded thanks to the programming language C, it allows to play this board game in the linux terminal.

This project includes a ranking between the different players, an automatic save option when a game is not finished with the possibility to resume it.

## How to use

  1) Compile the program including the main.c file and the board.c file.
  gcc -Wall main.c board.c -o gobblers`
  2) Run the program.
  `./gobblers`

  ![Compilation gif](img/COMPILE.gif)

  4) Once the program is launched, you can access the game and its ranking.
  5) Once in the game, each player must choose a nickname.
  6) Then, the launching steps are ready. You can now play.
  7) To play, you must decide whether you want to place a piece or move one.
![Piece placement gif](img/PLACE.gif) 
  8) If you want to move a piece, if you don't have any piece, it will not be possible to move one.
![Gif of moving pieces](img/MOVE.gif)
  9) If you want to place a piece, you have to select the piece to place and enter its coordinates (0,2).
  10) This is a turn-based game, so it will be the second player to play.
![Victory Gif](img/WIN.gif)
  11) If you leave the game while it is in progress, and if you restart the program you will be able to choose: to start again a game or to continue the one not finished.
  ![Save Gif](img/save.gif)
  12) Once the game is finished, you can access the ranking with points according to the victories and defeats. (+10/-8).
  ![Ranking Gif](img/RANK.gif)

## Leaderboard data storage system
  1) The first number in the leaderboard.txt file is the number of players in the leaderboard.
  2) The first information in each column is the name of the player
  3) The second information in each column is the number of points of the player.
```
2
Alenso 10
Julien -8
```

## Data storage system

  1) The first information corresponds to the sizes of the pieces on the board. Each line is separated by `|` and each square by `;`.
  2) The second information corresponds to the owners of each piece on the board. The lines are separated by `|` and the squares by `;`.
  3) The third information corresponds to the current player's turn.
  4) The last information corresponds to the number of pieces available in the houses for each player. The players are separated by `|`.

At the beginning of the game we have :
```
sizes: 0,0,0,0,;0,0,0,0,;0,0,0,0,;|0,0,0,0,;0,0,0,0,;0,0,0,0,;|0,0,0,0,;0,0,0,0,;0,0,0,0,;|
players: 0,0,0,0,;0,0,0,0,;0,0,0,0,;|0,0,0,0,;0,0,0,0,;0,0,0,0,;|0,0,0,0,;0,0,0,0,;0,0,0,0,;|
turn_player: 1,
houses: 2,2,2,|2,2,2,|�
```

## Selected extensions

### Backup

The save extension allows you to save a game in a `save.txt` file. This save takes place automatically at the end of each round and records the state of the board, the player to play and the pieces belonging to the players.

### Ranking

The purpose of this extension is to save in a file the names of the different players who have started a game, as well as their respective scores. 

When the game is launched, it is possible to view this ranking sorted in descending order according to the score. 

Finally, the winning player sees his score increase by +10 points and the losing player loses 8 points. 