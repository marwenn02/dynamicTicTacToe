#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define boardWidth 5
#define boardHeight 5

typedef enum {
	Empty,
	Cross,
	Circle
} State;

void printState(State *squares) {
	int i;
	for(i = 0; i < boardWidth * boardHeight; i++) {
		switch(*(squares + i)) {
			case Empty:	
				printf(". ");
				break;
			case Cross:
				printf("X ");
				break;
			case Circle:
				printf("O ");
				break;
			default:
				printf("ERROR %d => %d", i, *(squares + i));
				break;
		}
		if((i + 1) % boardWidth == 0)
			puts("");
	}
}

int getMove() {
	char buf[boardWidth * boardHeight];
	int move;
	printf("Move: ");
	fflush(stdout);
	fgets(buf, boardWidth * boardHeight, stdin);
	move = atoi(buf) - 1;
	assert((move > -1) && (move < boardWidth * boardHeight));
	return move;	
}

int isThereWin(State *squares) {
	int i, j, collect;

	//	. . .
	//	X X X
	//	. . . 
	for(i = 0; i < boardHeight * boardWidth; i += boardWidth) {
		if(*(squares + i) == Empty) continue;
		collect = 0;
		for(j = i; j < i + boardWidth; j++) {
			if(*(squares + j) != *(squares + i)) break;
			collect++;
		}
		if(collect == boardWidth) {
			return *(squares + i);
		}
	}
	
	//	. X .
	//	. X .
	//	. X .
	for(i = 0; i < boardWidth; i++) {
		if(*(squares + i) == Empty) continue;
		collect = 0;
		for(j = i; j < (boardHeight * boardWidth) - (boardWidth - (i + 1)); j += boardWidth) {
			if(*(squares + j) != *(squares + i)) break;
			collect++;
		}
		if(collect == boardHeight) {
			return *(squares + i);
		}
	}

	//	X . X
	//	. X .
	//	X . X
	if((*squares == Empty) && (*(squares + boardWidth - 1) == Empty)) return 0;
	
	//checking top left down to bottom right
	collect = 0;
	for(i = 0; i < boardWidth * boardHeight; i += boardWidth + 1) {
		if(*squares != *(squares + i)) break; //this will work really wierd if boardWidth : boardHeight != 1
		collect++;
	}
	if(collect == (int) ((boardWidth + boardHeight) >> 1) && *squares != Empty) return *squares;
	
	//checking top right down to bottom left
	collect = 0;
	for(i = boardWidth - 1; i < (boardWidth * boardHeight) - (boardWidth - 1); i += boardWidth - 1) {
		if(*(squares + boardWidth - 1) != *(squares + i)) break;
		collect++;
	}
	if((collect == (int) ((boardWidth + boardHeight) >> 1)) && (*(squares + boardWidth - 1) != Empty)) return *(squares + boardWidth - 1);
	
	return 0;
}

int main(void) {
	State *squares = malloc(boardWidth * boardHeight * sizeof *squares);
	int i, playerMove, winner, currentPlayer = Cross;
	_Bool gameState = 0;

	for(i = 0; i < boardWidth * boardHeight; i++)
		*(squares + i) = Empty;
	i = 0;
	printState(squares);
	while((gameState == 0) && (i < boardWidth * boardHeight)) {
		playerMove = getMove();
		if(*(squares + playerMove) != Empty) continue;
		*(squares + playerMove) = currentPlayer;
		printState(squares);
		winner = isThereWin(squares);
		if(winner != 0) {
			printf("Winner: %s\n", winner - 1 ? "O" : "X");
			exit(0);
		}
		currentPlayer = currentPlayer - 1 ? Cross : Circle; 
		i++;
	}
	printf("%s\n", "Tie!\n");

}
