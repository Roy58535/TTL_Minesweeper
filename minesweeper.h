#pragma once
void gameLoop(int upButton, int downButton, int leftButton, int rightButton, int flagButton, int openButton);
void readMineGrid(int col, int row, int val);
void gameSetup();
void openCell(int i, int j);
void flagCell(int i, int j);
void openAllMines();