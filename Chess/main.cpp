//
//  main.cpp
//  Chess
//
//  Created by Mack Ragland on 10/19/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include <iostream>
#include <SDL2/SDL.h>
#include "Screen.hpp"
#include "TextBoard.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "Rect.hpp"
#include "GraphicsHandler.hpp"
using namespace std;
using namespace SDLScreen;
using namespace Graphics;



int main(int argc, const char * argv[]) {
    
    Screen screen;
    screen.init();

    ChessGame::Game game;
    game.init();
    
    
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
    GraphicsHandler GFX(screen.getRenderer());
    GFX.boardGraphicsInit();
    GFX.drawBoard(game.m_board.getBoardstate());
    screen.update();
    //SDL_Delay(500);
    
    game.m_white.humanPlayer = true;  // tells game it's a comp player
    game.m_black.humanPlayer = true;  // tells game it's a comp player
    
    
    
    while (1) {
        //SDL_RenderClear(screen.getRenderer());
        //board.drawBoard(screen.getRenderer());
        //screen.update();
        //SDL_Delay(500);
        game.checkmateOnBlack = ;
        
        GFX.drawBoard(game.m_board.getBoardstate());
        screen.update();
        if (game.checkmateOnBlack == true) {
            cout << "Checkmate!  White wins!" << endl;
            break;
        }
        game.checkmateOnWhite = game.m_black.makeMove(&game.m_board);
        GFX.drawBoard(game.m_board.getBoardstate());
        screen.update();
        if (game.checkmateOnWhite == true){
            cout << "Checkmate!  Black wins!" << endl;
            cout << "Press any entry then enter to quit" << endl;
            string quit;
            cin >> quit;
            break;
        }
        
        if(screen.processEvents() == false){
            break;
        }
    }
    
    // still need to add: checkmate checker and setter, en passant
    
    // Pieces set, board set, need to test pieces, need game class to decide who plays next, ask for moves in console
    

    screen.close();
    
    
    
    return 0;
}
