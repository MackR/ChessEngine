//
//  main.cpp
//  Chess
//
//  Created by Mack Ragland on 10/19/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include <iostream>
//#include <SDL2/SDL.h>
//#include "Screen.hpp"
#include "ChessConstants.h"
#include "TextBoard.hpp"
#include "Player.hpp"
// #include "Game.hpp"
//#include "Rect.hpp"
//#include "GraphicsHandler.hpp"
using namespace std;

void printBoard(TextBoard &board);



int main(int argc, const char * argv[]) {
    
    //SDLScreen::Screen screen;
    //screen.init();
    
    
    //auto boardstate = board.getBoardstate()[17];
    //auto playerPieces = player1.m_pieces;
    //SDL_Delay(500);
    //auto imageSurface = IMG_Load("WhitePawn.png");
    //if (imageSurface == NULL) {
    //    cout << "Could not load imageSurface" << endl;
    //}
    //Rectbuild my_rect = Rectbuild(200,200,77,77,"WhitePawn.png", screen);
    
    /////test space/////

    
    ///////////////
    
    
    //my_rect.draw();
    //Graphics::GraphicsHandler GFX(screen.getRenderer());
    //GFX.boardGraphicsInit();
    //GFX.drawBoard(game.m_board);
    //screen.update();
    //SDL_Delay(500);
    Player white(CONSTANTS::Color::WHITE, false); // false -> computer player
    Player black(CONSTANTS::Color::BLACK, false); // false -> computer player
    TextBoard chessboard;
    
    
    
    while (1) {
        //SDL_RenderClear(screen.getRenderer());
        //board.drawBoard(screen.getRenderer());
        //screen.update();
        //SDL_Delay(500);
        printBoard(chessboard);

        if (white.takeTurn(&chessboard)) {
            printBoard(chessboard);
            cout << "Checkmate!  White wins!" << endl;
            break;
        }
        //GFX.drawBoard(chessboard);
        //screen.update();
        printBoard(chessboard);

        if (black.takeTurn(&chessboard)){
            printBoard(chessboard);
            cout << "Checkmate!  Black wins!" << endl;
            // cout << "Press any entry then enter to quit" << endl;
            // string quit;
            // cin >> quit;
            break;
        }
        //GFX.drawBoard(chessboard);
        //screen.update();
        
        //if(screen.processEvents() == false){
        //    break;
        //}
    }
    

    //screen.close();
    
    
    
    return 0;
}

// Creates a console printout of the chess board - temporary replacement for graphics
void printBoard(TextBoard &board){

std::cout << "  A. B. C .D. E. F. G. H." << std::endl;
std::cout << ".__.__.__.__.__.__.__.__." << std::endl;
for (int row = 7; row >=0; --row){
    for (int col = 0; col < 8; ++col){
        char pieceType = board.getPieceType(col,row);
        if (pieceType == 'E') pieceType = '*';
        CONSTANTS::Color nColor = board.getPieceColor(col,row);
        char cColor;
        switch (nColor){
            case CONSTANTS::Color::EMPTY:
                cColor = '*';
                break;
            case CONSTANTS::Color::BLACK:
                cColor = 'B';
                break;
            case CONSTANTS::Color::WHITE:
                cColor = 'W';
                break;
        }
        
        string name = std::string() + pieceType + cColor;
        std::cout << "|" << name;
    }
    std::cout << "|" <<std::endl;
    if(row != 0){
        std::cout << "|--.--.--.--.--.--.--.--|" << std::endl;
    }
    else{
        std::cout << "|__|__|__|__|__|__|__|__|" << std::endl;
    }
    
}
std::cout << std::endl;

};