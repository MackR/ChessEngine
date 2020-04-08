# ChessEngine
A chess game built using OpenGL with CPU Player

Screen.cpp - The class that handles the window the program pops up in
GraphicsHandler.cpp - class for grabbing piece images, moving pieces around the board, and creating the board
Rect.cpp - Easy class interface that GraphicsHandler uses for handling the individual chess piece graphics as they move around the screen
Piece.cpp - Handles all the logic for an individual piece properties and what types of moves each piece is allowed to make
Board.cpp - The chess board class that handles what moves are actually valid (castling, checks, en passant...)
Player.cpp - Class to interface with player, declare checkmate, swap turns, modify the board, contains CPU gameplay calculator (maximizer and minimizer could be combined into single function)
