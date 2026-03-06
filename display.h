#pragma once
#include <cstdint>
#include <SPI.h>

void setupDisplay();
void displayFlagButton();
void displayOpenButton();
void displayNumber(int row, int col, int num);
void displayNotOpened(int row, int col);
void displayMine(int row, int col);
void displayFlag(int row, int col);
void displayCursor(int row, int col);
void deleteCursor(int row, int col);
void moveCursor(int prevRow, int prevCol, int newRow, int newCol);
void resetCell(int row, int col);
void displayTimer(int32_t time);
void displayWin();
void displayLoss();
void displayTitle(String title, uint16_t color);