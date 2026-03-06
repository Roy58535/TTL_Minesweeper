#include <iostream>
#include "minesweeper.h"
#include "display.h"
#include "common.h"
#include <SPI.h>
#include <vector>

#define GRID_HEIGHT 8
#define GRID_WIDTH 8

int mineGrid[GRID_HEIGHT][GRID_WIDTH] = { 0 };
int flagGrid[GRID_HEIGHT][GRID_WIDTH] = { 0 };
int numGrid[GRID_HEIGHT][GRID_WIDTH] = { 0 };
int openedGrid[GRID_HEIGHT][GRID_WIDTH] = { 0 };
bool setupDone = false;
bool firstOpened = false;
bool isGameOver = false;
int mineCount = 0;
int cursorPos[2] = { 0, 0 };


void gameSetup() {
  if (!setupDone) {
	cursorPos[0] = 0;
	cursorPos[1] = 0;
	firstOpened = false;
	isGameOver = false;
	mineCount = 0;
	for (int i = 0; i < GRID_HEIGHT; i++) {
	  for (int j = 0; j < GRID_WIDTH; j++) {
		openedGrid[i][j] = 0;
		flagGrid[i][j] = 0;
		
		if (mineGrid[i][j] == 0) {
		  numGrid[i][j] = 0;
		  for (int k = -1; k <= 1; k++) {
			for (int l = -1; l <= 1; l++) {
			  if (i + k >= 0 && i + k < GRID_HEIGHT && j + l >= 0 && j + l < GRID_WIDTH) {
				numGrid[i][j] += mineGrid[i + k][j + l];
			  }
			}
		  }
		}
		else {
		  numGrid[i][j] = -1; // -1 indicates a mine
		  mineCount++;
		}
	  }
	}
	setupDone = true;
  }
}

void gameLoop(int upButton, int downButton, int leftButton, int rightButton, int flagButton, int openButton) {
  if (upButton == 1 && cursorPos[0] > 0) {
	moveCursor(cursorPos[0], cursorPos[1], cursorPos[0] - 1, cursorPos[1]);
	cursorPos[0]--;
  }
  else if (downButton == 1 && cursorPos[0] < GRID_HEIGHT - 1) {
	moveCursor(cursorPos[0], cursorPos[1], cursorPos[0] + 1, cursorPos[1]);
	cursorPos[0]++;
  }
  else if (leftButton == 1 && cursorPos[1] > 0) {
	moveCursor(cursorPos[0], cursorPos[1], cursorPos[0], cursorPos[1] - 1);
	cursorPos[1]--;
  }
  else if (rightButton == 1 && cursorPos[1] < GRID_WIDTH - 1) {
	moveCursor(cursorPos[0], cursorPos[1], cursorPos[0], cursorPos[1] + 1);
	cursorPos[1]++;
  }
  else if (flagButton == 1) {
	flagCell(cursorPos[0], cursorPos[1]);
  }
  else if (openButton == 1) {
	
	if (!firstOpened) {
	  std::vector<std::pair<int, int>> safeCells;
	  for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 0; j < GRID_WIDTH; j++) {
		  if (numGrid[i][j] == 0) {
			safeCells.push_back({i, j});
		  }
		}
	  }
	  int randIdx = random(safeCells.size());
	  moveCursor(cursorPos[0], cursorPos[1], safeCells[randIdx].first, safeCells[randIdx].second);
	  cursorPos[0] = safeCells[randIdx].first;
	  cursorPos[1] = safeCells[randIdx].second;
	  openCell(safeCells[randIdx].first, safeCells[randIdx].second);
	  firstOpened = true;
	}
	else {
	  openCell(cursorPos[0], cursorPos[1]);
	}
  }
}

void flagCell(int row, int col) {
  if (openedGrid[row][col] == 0) {
	if (flagGrid[row][col] == 0) {
	  displayFlag(row, col);
	  mineCount--;
	  flagGrid[row][col] = 1;
	}
	else {
	  displayNotOpened(row, col);
	  mineCount++;
	  flagGrid[row][col] = 0;
	}
  }
}

void readMineGrid(int col, int row, int val) {
  mineGrid[row][col] = val;
}

void openCell(int row, int col) {
  if (openedGrid[row][col] == 1) return;
  openedGrid[row][col] = 1;
  bool gameWon = true;
  for (int i = 0; i < GRID_HEIGHT; i++) {
	for (int j = 0; j < GRID_WIDTH; j++) {
	  if (mineGrid[i][j] == 0 && openedGrid[i][j] == 0) {
		gameWon = false;
	  }
	}
  }
  if (gameWon) displayWin();

  if (mineGrid[row][col] == 1) {
	displayMine(row, col);
	openAllMines();
	displayLoss();
	isGameOver = true;
  }
  else {
	displayNumber(row, col, numGrid[row][col]);
	if (numGrid[row][col] == 0) {
	  for (int k = -1; k <= 1; k++) {
		for (int l = -1; l <= 1; l++) {
		  if (k == 0 && l == 0) {
			continue; // Skip the current cell
		  }
		  if (row + k >= 0 && row + k < GRID_HEIGHT && col + l >= 0 && col + l < GRID_WIDTH) {
			if (flagGrid[row + k][col + l] == 0) {
			  openCell(row + k, col + l);
			}
		  }
		}
	  }
	}
  }
}

void openAllMines() {
  for (int i = 0; i < GRID_HEIGHT; i++) {
	for (int j = 0; j < GRID_WIDTH; j++) {
	  if (mineGrid[i][j] == 1 && flagGrid[i][j] == 0) {
		displayMine(i, j);
	  }
	}
  }
}