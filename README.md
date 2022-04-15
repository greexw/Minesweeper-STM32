# Minesweeper-STM32
Implementation of Minesweeper game for STM32 microcontroller

## General Information
* The board size (square) is defined in 'MINEFIELD_SIZE', size of each field in 'SQUARE_SIZE'.
* The maximum size of the board depends on the number of pixels on the microcontroller screen.
* Mines are drawn using srand library. Seed is getting as a current ADC1 voltage status.
