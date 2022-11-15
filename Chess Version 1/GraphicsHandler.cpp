//
//  Board.cpp
//  Chess
//
//  Created by Mack Ragland on 10/20/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include "GraphicsHandler.hpp"

using namespace Graphics;

GraphicsHandler::GraphicsHandler(): darkSquares(NULL),lightSquares(NULL),squares(NULL), m_renderer(NULL) {}
GraphicsHandler::GraphicsHandler(SDL_Renderer *renderer): darkSquares(NULL),lightSquares(NULL),squares(NULL) {
    m_renderer = renderer;
}

    void GraphicsHandler::boardGraphicsInit(){
        
        if (m_renderer == NULL) {
            std::cout<< "Function init was given a NULL input";
            return;
        }
        
        squares = new Rectbuild[BOARD_SQUARES];
        lightSquares = new Rectbuild[BOARD_SQUARES/2];
        darkSquares = new Rectbuild[BOARD_SQUARES/2];

        for (int i = 0; i< BOARD_SQUARES; i++) {
            getCoordinate(i);
            squares[i] = Rectbuild((m_xBoardCoordinate*SQUARE_WIDTH)+BOARD_MARGIN/2, m_yBoardCoordinate*SQUARE_HEIGHT+BOARD_MARGIN/2, SQUARE_WIDTH, SQUARE_HEIGHT, 0, 0, 0, 0, m_renderer);
        }
        
        for (int i = 0; i < BOARD_SQUARES/2; i++) {
            if (i/4%2 == 0) {
                lightSquares[i] = squares[(i*2)];
                lightSquares[i].setColor(222,184,135,255);
                darkSquares[i] = squares[i*2+1];
                darkSquares[i].setColor(139,69,19, 255);
            }
            else if (i/4%2 == 1){
                lightSquares[i] = squares[(i*2+1)];
                lightSquares[i].setColor(222,184,135,255);
                darkSquares[i] = squares[i*2];
                darkSquares[i].setColor(139,69,19, 255);
            }

        }


    }
    
    void GraphicsHandler::getCoordinate(int index){
        int& x = m_xBoardCoordinate;
        int& y = m_yBoardCoordinate;
        
        x = index % 8;
        y = index / 8;
        
    }
    
    void GraphicsHandler::drawBoard(Piece* m_pBoardState){
        SDL_RenderClear(m_renderer);
        for (int i = 0; i<32; i++) {
            lightSquares[i].draw();
            darkSquares[i].draw();
            
        }
        for (int i = 0; i < 64; i++) {
            int file = m_pBoardState[i].getFile();
            int rank = m_pBoardState[i].getRank();
            string color = m_pBoardState[i].getColor();
            char type = m_pBoardState[i].getType();
            string fileName, fileNameColor, fileNameType;
            
            if (color == "White") {
                fileNameColor = "White";
            }
            else if (color == "Black"){
                fileNameColor = "Black";
            }
            switch (type) {
                case 'P':
                    fileNameType = "Pawn.png";
                    break;
                case 'R':
                    fileNameType = "Rook.png";
                    break;
                case 'N':
                    fileNameType = "Knight.png";
                    break;
                case 'B':
                    fileNameType = "Bishop.png";
                    break;
                case 'Q':
                    fileNameType = "Queen.png";
                    break;
                case 'K':
                    fileNameType = "King.png";
                    break;
                    
                default:
                    break;
            }
            fileName = fileNameColor + fileNameType;
            if (fileNameColor == "Black" || fileNameColor == "White") {
            auto placeHolderPiece = Rectbuild(calcPieceXPosition(file+1),calcPieceYPosition(rank+1),PIECE_SIDELENGTH,PIECE_SIDELENGTH,fileName, m_renderer);
                placeHolderPiece.draw();
            }
        }
        
    }

int GraphicsHandler::calcPieceXPosition(int m_file){
    int m_x = GraphicsHandler::BOARD_MARGIN+GraphicsHandler::SQUARE_WIDTH*(m_file-1)-GraphicsHandler::SQUARE_WIDTH/2 - 5;
    return m_x;
}
int GraphicsHandler::calcPieceYPosition(int m_rank){
    int m_y = Graphics::GraphicsHandler::BOARD_MARGIN+Graphics::GraphicsHandler::SQUARE_HEIGHT*(8-m_rank) - Graphics::GraphicsHandler::SQUARE_HEIGHT/2;
    return m_y;
}

    
    
    void GraphicsHandler::destroyBoard(){
        delete[] squares;
        delete[] lightSquares;
        delete[] darkSquares;
    }

    GraphicsHandler::~GraphicsHandler(){
        delete[] squares;
        delete[] lightSquares;
        delete[] darkSquares;

}
    
    
    
    

