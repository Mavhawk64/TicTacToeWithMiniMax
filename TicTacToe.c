#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
bool checkFull(int grid[3][3]);
bool validLoc(int grid[3][3], int x[2]);
int *computerRandomGuess(int grid[3][3]);
int *computerMiniMaxGuess(int grid[3][3]);
void showBoard(int grid[3][3]);
int gameOver(int grid[3][3]);
int checkCols(int grid[3][3]);
int checkRows(int grid[3][3]);
int checkDiag(int grid[3][3]);
int findBestGuess(int grid[3][3], int, bool);
int max(int,int);
int min(int,int);
int main(int argc, char *argv[]) {
  printf(
      "Welcome to the Tic Tac Toe Game!\nWould you like to play easy (random) "
      "or hard (minimax) mode?\n");
  int alpha;
  printf("Choose:\n1. Easy (Random)\n2. Hard (MiniMax)\nYour Choice: ");
  scanf(" %d", &alpha);
  bool minimax = alpha == 2;
  int grid[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
  while (gameOver(grid) == 0 && !checkFull(grid)) {
    showBoard(grid);
    printf(
        "Using coordinates, type in your move! (Of form: 0 2 (This is the top "
        "right corner))\n");
    int x[] = {0, 0};
    for (int i = 0; i < 2; i++) {
      scanf("%d", &x[i]);
    }
    if (validLoc(grid, x)) {
      grid[x[0]][x[1]] = 1;
    } else {
      printf("Invalid, computer goes for free!!!\n");
    }
    if (gameOver(grid) == 0 && !checkFull(grid)) {
      int *o = minimax ? computerMiniMaxGuess(grid) : computerRandomGuess(grid);
      if (o[0] != -1) {
        grid[o[0]][o[1]] = -1;
      }
    }
  }

  showBoard(grid);
  printf("Game Over! %s Would you like to play another!?!\ny/n: ",
         gameOver(grid) == 0
             ? "It's a tie!"
             : gameOver(grid) > 0 ? "You Win!" : "Computer Wins!");
  char r = 'n';
  scanf(" %c", &r);
  if (r == 'y') {
    main(argc, argv);
  }
  return 0;
}

bool checkFull(int grid[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (grid[i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}

bool validLoc(int grid[3][3], int x[2]) { return grid[x[0]][x[1]] == 0; }

//----------------RANDOM----------------//
int *computerRandomGuess(int grid[3][3]) {
  int x, y;
  srand((unsigned int)time(NULL));
  x = rand() % 3;
  y = rand() % 3;
  int *guess = malloc(sizeof(*guess) * 2);
  guess[0] = x;
  guess[1] = y;

  int count = 0;
  while (!validLoc(grid, guess)) {
    x = rand() % 3;
    y = rand() % 3;
    guess[0] = x;
    guess[1] = y;
    if (count > 100) {
      printf("COMPUTER COULD NOT FIND VALID SOLUTION\n");
      break;
    }
    count++;
  }
  if (count >= 100) {
    guess[0] = -1;
    guess[1] = -1;
  }
  return guess;
}

//----------------MiniMax----------------//
int *computerMiniMaxGuess(int grid[3][3]) {
  int bestScore = -9999;
  int * move = malloc(sizeof(*move) * 2);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (grid[i][j] == 0) {
        grid[i][j] = -1;
        int score = findBestGuess(grid, 0, false);
        //printf("Score: %d\n", score);
        grid[i][j] = 0;
        if(score > bestScore) {
          bestScore = score;
          move[0] = i;
          move[1] = j; 
        }
      }
    }
  }
  //printf("Computer's Move: %d %d\n", move[0], move[1]);
  return move;
}

int findBestGuess(int grid[3][3], int depth, bool maximize) {
  int result = gameOver(grid);
  if(checkFull(grid)) {
    return -result;
  }
  int player = maximize ? 1 : -1;
  int bestScore = -player * 9999;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      if(grid[i][j] == 0) {
        grid[i][j] = -player;
        int score = findBestGuess(grid, depth + 1, !maximize);
        grid[i][j] = 0;
        if(maximize) {
          bestScore = max(score, bestScore);
        } else {
          bestScore = min(score, bestScore);
        }
      }
    }
  }
  return bestScore;
}

int max(int num1, int num2) {
  return num1 > num2 ? num1 : num2;
}
int min(int num1, int num2) {
  return num1 < num2 ? num1 : num2;
}





void showBoard(int grid[3][3]) {
  int rows = 3;
  int cols = 3;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (grid[i][j] == 1) {
        printf(" x ");
      } else if (grid[i][j] == -1) {
        printf(" o ");
      } else {
        printf(" _ ");
      }
      if (j < cols - 1) {
        printf("|");
      }
    }
    if (i < rows - 1) printf("\n-----------");
    printf("\n");
  }
}

//--------------------CHECK ENDING--------------------//
int gameOver(int grid[3][3]) {
  int ret = 0;
  int d = checkDiag(grid);
  int r = checkRows(grid);
  int c = checkCols(grid);
  if (d == 1 || d == -1) {
    ret = d;
  } else if (r == 1 || r == -1) {
    ret = r;
  } else if (c == 1 || c == -1) {
    ret = c;
  }
  return ret;
}

int checkCols(int grid[3][3]) {
  int ret = 0;
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) {
      if (grid[i][j] == 1) {
        if (ret == -1) {
          ret = 0;
          break;
        }
        ret = 1;
      } else if (grid[i][j] == -1) {
        if (ret == 1) {
          ret = 0;
          break;
        }
        ret = -1;
      } else {
        ret = 0;
        break;
      }
    }
    if (ret == 1 || ret == -1) {
      break;
    }
  }
  return ret;
}

int checkDiag(int grid[3][3]) {
  int ret = 1;
  // Checks if User won.
  for (int i = 0; i < 3; i++) {
    if (grid[i][i] == 1) {
      ret = 1;
    } else {
      ret = 0;
      break;
    }
  }
  if (ret == 0) {
    for (int i = 0; i < 3; i++) {
      if (grid[2 - i][i] == 1) {
        ret = 1;
      } else {
        ret = 0;
        break;
      }
    }
  }

  // Checks if Comp won.
  if (ret == 0) {
    for (int i = 0; i < 3; i++) {
      if (grid[i][i] == -1) {
        ret = -1;
      } else {
        ret = 0;
        break;
      }
    }

    if (ret == 0) {
      for (int i = 0; i < 3; i++) {
        if (grid[2 - i][i] == -1) {
          ret = -1;
        } else {
          ret = 0;
          break;
        }
      }
    }
  }
  return ret;
}

int checkRows(int grid[3][3]) {
  int ret = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (grid[i][j] == 1) {
        if (ret == -1) {
          ret = 0;
          break;
        }
        ret = 1;
      } else if (grid[i][j] == -1) {
        if (ret == 1) {
          ret = 0;
          break;
        }
        ret = -1;
      } else {
        ret = 0;
        break;
      }
    }
    if (ret == 1 || ret == -1) {
      break;
    }
  }
  return ret;
}
