//
//  GraphicsHandler.hpp
//  Chess
//
//  Created by Mack Ragland on 10/20/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#ifndef GraphicsHandler_hpp
#define GraphicsHandler_hpp

#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "Screen.hpp"
#include "Piece.hpp"
#include "Rect.hpp"


namespace Graphics {
    class GraphicsHandler{
        
    private:
        Piece* m_pBoardstate;
        
    public:
        static const int BOARD_SQUARES = 64;
        static const int BOARD_MARGIN = 0.15*((SDLScreen::Screen::SCREEN_WIDTH+SDLScreen::Screen::SCREEN_HEIGHT)/2);
        static const int SQUARE_WIDTH = (SDLScreen::Screen::SCREEN_WIDTH-BOARD_MARGIN)/8;
        static const int SQUARE_HEIGHT = (SDLScreen::Screen::SCREEN_HEIGHT-BOARD_MARGIN)/8;
        static const int PIECE_SIDELENGTH = SQUARE_WIDTH*0.7;
        
        int m_xBoardCoordinate = 0;
        int m_yBoardCoordinate = 0;
        Rectbuild *squares;
        Rectbuild *lightSquares;
        Rectbuild *darkSquares;
        SDL_Renderer *m_renderer;
        
        
    public:
        GraphicsHandler();
        GraphicsHandler(SDL_Renderer *renderer);
        ~GraphicsHandler();
        void boardGraphicsInit();
        void getCoordinate(int index);
        void drawBoard(Piece* m_pBoardState);
        void destroyBoard();
        int  calcPieceYPosition(int m_rank);
        int  calcPieceXPosition(int m_file);
        
        
    };
}

#endif /* GraphicsHandler_hpp */
