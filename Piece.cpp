//
//  Piece.cpp
//  Chess
//
//  Created by Mack Ragland on 10/26/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include "Piece.hpp"

Piece::Piece(){
    m_type = 'E';
    m_color = "Empty";
}

Piece::Piece(int rank, int file, char type, string color){
    
    m_rank = rank;
    m_file = file;
    if (type != 'P' && type != 'R' && type != 'N' && type != 'B' && type != 'Q' && type != 'K') {
       // cout << "WARNING: Passed unapplicable type or 'E', using 'E' type" << endl;
        m_type = 'E';
    }
    else m_type = type;
    
    if (color == "Empty") {
       // cout << "Created blank piece" << endl;
        m_color = "Empty";
    }
    else if (color != "White" && color != "Black") { // can change "Black" and "White" to enums
        // cout << "WARNING: Passed unapplicable color, creating blank piece" << endl;
        m_color = "Empty";
    }
    else m_color = color;
}

int Piece::getRank(){
    return m_rank;
}
void Piece::setRank(int rank){
    m_rank = rank;
}
int Piece::getFile(){
    return m_file;
}
void Piece::setFile(int nFile){
    m_file = nFile;
}
char Piece::getType(){
    return m_type;
}
string Piece::getColor(){
    
    return m_color;
}
bool Piece::getMoved(){
    return m_moved;
}
void Piece::Moved(){
    m_moved = true;
    
}
void Piece::setColor(string color){
    m_color = color;
};
void Piece::setType(char type){
    m_type = type;
};
void Piece::setDoubleJumpTurn(int turn){
    m_doubleJumpTurn = turn;
}
int Piece::getDoubleJumpedTurn(){
    return m_doubleJumpTurn;
}

 int Piece::cFileToIndex(char cFile){
    return cFile-'A';
}
char Piece::indexTo_cFile(int nFile){
    return static_cast<char>(nFile + 'A');
}

int Piece::convertCoordinateToBoardIndex(int nFile, int rank){
    
    if (nFile >= 0 && nFile <= 7 && rank >= 0 && rank <= 7) {
        return rank*8 + nFile;
    }
    //cout << "Warning, illegal rank and file numbers" << endl;
    
    return -1;
    
}

int Piece::containsFriendlyPiece(Piece* boardState, int nFile, int rank){
    if (nFile >= 0 && nFile <= 7 && rank >= 0 && rank <= 7) {
        
        if (boardState[convertCoordinateToBoardIndex(nFile, rank)].getColor() == "Empty"){
          //  cout << "Square " << indexTo_cFile(nFile) << rank+1 << " is empty" << endl;
        return EMPTY;
        }
        else if(boardState[convertCoordinateToBoardIndex(nFile, rank)].getColor() == m_color){
            //  cout << "Friendly piece on square " << indexTo_cFile(nFile) << rank << endl;
            return FRIENDLY;
        }
        else {
          //  cout << "Enemy piece on square " << indexTo_cFile(nFile) << rank << endl;
            
            return ENEMY;
        }
    }
    
    //cout << "Warning, illegal rank and file numbers" << endl;
    
    return INVALID;
}

string Piece::buildPositionString(int boardIndexNew){
    int nFile = boardIndexNew%8;
    int rank = boardIndexNew/8;
    string type (1, m_type);
    string sFilePrev (1,indexTo_cFile(m_file));
    string sFileNew (1,indexTo_cFile(nFile));
    string moveString = type + sFilePrev + to_string(m_rank+1) + sFileNew + to_string(rank+1);
    //cout << "Building position from index: " << moveString << endl;
    return moveString;
}
string Piece::buildPositionString(int nFileNew, int rankNew){
    string type (1, m_type);
    string sFilePrev (1,indexTo_cFile(m_file));
    string sFileNew (1,indexTo_cFile(nFileNew));
    string moveString = type + sFilePrev + to_string(m_rank+1) + sFileNew + to_string(rankNew+1);
    //cout << "Building position from (int, int): " << moveString << endl;
    return moveString;
}
string Piece::buildPositionString(char cFileNew, int rankNew){
    string type (1, m_type);
    string sFilePrev (1,indexTo_cFile(m_file));
    string sFileNew (1,cFileNew);
    string moveString = type + sFilePrev + to_string(m_rank+1) + sFileNew + to_string(rankNew+1);
    //cout << "Building position from (char, int): " << moveString << endl;
    return moveString;
}

vector<string>* Piece::getMoves(Piece* boardState, int currentTurn){
    m_moves.clear();
    vector<string>* moves;
    moves = &m_moves;
    
    switch (m_type) {
        case 'P':return pawnMoves(boardState, currentTurn);
            break;
        case 'R':return rookMoves(boardState);
            break;
        case 'N':return knightMoves(boardState);
            break;
        case 'B':return bishopMoves(boardState);
            break;
        case 'Q':return queenMoves(boardState);
            break;
        case 'K':return kingMoves(boardState);
            break;
        case 'E': return moves;
        default: cout << "Passed bad piece type to function getMoves()";
            return NULL;
            break;
    }
    
    // check the type of piece being calculated for and runs its move calc function
    
}

vector<string>* Piece::pawnMoves(Piece* boardState, int currentTurn){
    static vector<string>* moves;
        moves = &m_moves;
        int rank = m_rank;
        int file = m_file;
        int inFrontPawnIndex = -1;
        int doubleJumpPawnIndex = -1;
        int pawnLeftDiagonalIndex = -1;
        int pawnRightDiagonalIndex = -1;
        int singleJumpSquareStatus = -1;
        int prePromotionRank = -1;
        bool enPassantValidLeft = false;
        bool enPassantValidRight = false;
        Piece leftPiece;
        Piece rightPiece;
    
        //Build the possible moves depending on the location of the pawn
        if (m_color == "White") {
            prePromotionRank = 6;
            if (rank != 7) {
                inFrontPawnIndex = convertCoordinateToBoardIndex(file, rank+1);
            }
            if(file != 0 && (rank != 0 || rank != 7)) {
                pawnLeftDiagonalIndex = convertCoordinateToBoardIndex(file-1, rank+1);
            }
            if(file != 7 && (rank != 0 || rank != 7)){
                pawnRightDiagonalIndex = convertCoordinateToBoardIndex(file+1, rank+1);
            }
            if (rank == 1 ) {
                doubleJumpPawnIndex = convertCoordinateToBoardIndex(file, rank+2);
                singleJumpSquareStatus = containsFriendlyPiece(boardState, file, rank+1);

        }
            if (rank == 4) { // En passant
                if (file != 0) {
                    leftPiece = boardState[convertCoordinateToBoardIndex(file-1, rank)];
                    if (leftPiece.getType() == 'P'  && leftPiece.getColor() == "Black" && leftPiece.getDoubleJumpedTurn() == currentTurn-1){
                        enPassantValidLeft = true;
                    }
                }
                if (file != 7){
                    rightPiece = boardState[convertCoordinateToBoardIndex(file+1, rank)];
                    if (rightPiece.getType() == 'P'  && rightPiece.getColor() == "Black" && rightPiece.getDoubleJumpedTurn() == currentTurn-1){
                        enPassantValidRight = true;
                    }
                }
            }
        }
        else if (m_color == "Black"){
            prePromotionRank = 1;
            if (rank != 0) {
                inFrontPawnIndex = convertCoordinateToBoardIndex(file, rank-1);
            }
            if (file != 0 && (rank != 0 || rank != 7)) {
                pawnLeftDiagonalIndex = convertCoordinateToBoardIndex(file-1, rank-1);
            }
            if(file != 7 && (rank != 0 || rank != 8)){
                pawnRightDiagonalIndex = convertCoordinateToBoardIndex(file+1, rank-1);
            }
            if (rank == 6 ) { // black side double jump
                doubleJumpPawnIndex = convertCoordinateToBoardIndex(file, rank-2);
                singleJumpSquareStatus = containsFriendlyPiece(boardState, file, rank-1);
            }
            if (rank == 3) { // En passant
                if (file != 0) {
                    leftPiece = boardState[convertCoordinateToBoardIndex(file-1, rank)];
                    if (leftPiece.getType() == 'P'  && leftPiece.getColor() == "White" && leftPiece.getDoubleJumpedTurn() == currentTurn){
                        enPassantValidLeft = true;
                    }
                }
                if (file != 7){
                    rightPiece = boardState[convertCoordinateToBoardIndex(file+1, rank)];
                    if (rightPiece.getType() == 'P'  && rightPiece.getColor() == "White" && rightPiece.getDoubleJumpedTurn() == currentTurn){
                        enPassantValidRight = true;
                    }
                }
            }
        }


    
        //Add the possible moves to the index depending if there is no piece, a friendly piece, or enemy piece on the coordinate
    if (inFrontPawnIndex != -1) {
        
        if (boardState[inFrontPawnIndex].getColor() == "Empty") {
            if (rank != prePromotionRank) {
                moves->push_back(buildPositionString(inFrontPawnIndex));
            }
            else { // Pawn promotion by going straight
                moves->push_back(buildPositionString(inFrontPawnIndex)+"R");
                moves->push_back(buildPositionString(inFrontPawnIndex)+"N");
                moves->push_back(buildPositionString(inFrontPawnIndex)+"B");
                moves->push_back(buildPositionString(inFrontPawnIndex)+"Q");
            }
            
        }
        
    }
    if (doubleJumpPawnIndex != -1) {
        if (boardState[doubleJumpPawnIndex].getColor() == "Empty" && singleJumpSquareStatus == 0) {
        moves->push_back(buildPositionString(doubleJumpPawnIndex));
        }
    }
    if (pawnLeftDiagonalIndex != -1) {
        if ((boardState[pawnLeftDiagonalIndex].getColor() != m_color && boardState[pawnLeftDiagonalIndex].getColor() != "Empty") || enPassantValidLeft) {
            if (rank != prePromotionRank) {
                moves->push_back(buildPositionString(pawnLeftDiagonalIndex));
            }
            else { // Pawn promotion left diagonal capture
                moves->push_back(buildPositionString(pawnLeftDiagonalIndex)+"R");
                moves->push_back(buildPositionString(pawnLeftDiagonalIndex)+"N");
                moves->push_back(buildPositionString(pawnLeftDiagonalIndex)+"B");
                moves->push_back(buildPositionString(pawnLeftDiagonalIndex)+"Q");
            }
            
        }
    }
    if (pawnRightDiagonalIndex != -1) {
        if ((boardState[pawnRightDiagonalIndex].getColor() != m_color && boardState[pawnRightDiagonalIndex].getColor() != "Empty") || enPassantValidRight) {
            if (rank != prePromotionRank) {
                moves->push_back(buildPositionString(pawnRightDiagonalIndex));
            }
            else { // Pawn promotion right diagonal capture
                moves->push_back(buildPositionString(pawnRightDiagonalIndex)+"R");
                moves->push_back(buildPositionString(pawnRightDiagonalIndex)+"N");
                moves->push_back(buildPositionString(pawnRightDiagonalIndex)+"B");
                moves->push_back(buildPositionString(pawnRightDiagonalIndex)+"Q");
            }
        }
    }
     // filling with KA2B3,   NEED TO change what the index is filled with, should be a KA2B3 format, not just one position
    
    
    // return one space infront of pawn, two if on its first move
    
    return moves;
    
}
vector<string>* Piece::rookMoves(Piece* boardState){
    static vector<string>* moves;
    
    moves = &m_moves;
    
    int rank = m_rank;
    int file = m_file;
    
    bool stopFlag[4] = {false, false, false, false};
    
    for ( int i = 1; i < 8; i++) {

    
    if (stopFlag[0] == false) {
        
        int squareStatus = containsFriendlyPiece(boardState, file +i, rank);
        if (squareStatus == INVALID) {
            stopFlag[0] = true;
        }
        else if (squareStatus == ENEMY){
            moves -> push_back(buildPositionString(file +i, rank));
            stopFlag[0] = true;
        }
        else if (squareStatus == EMPTY){
            moves -> push_back(buildPositionString(file +i, rank));
        }
        else if (squareStatus == FRIENDLY){
            stopFlag[0] = true;
        }
    }
    if (stopFlag[1] == false) {
        
        int squareStatus = containsFriendlyPiece(boardState, file -i, rank);
        if (squareStatus == INVALID) {
            stopFlag[1] = true;
        }
        else if (squareStatus == ENEMY){
            moves -> push_back(buildPositionString(file -i, rank));
            stopFlag[1] = true;
        }
        else if (squareStatus == EMPTY){
            moves -> push_back(buildPositionString(file -i, rank));
        }
        else if (squareStatus == FRIENDLY){
            stopFlag[1] = true;
        }
    }
    if (stopFlag[2] == false) {
        
        int squareStatus = containsFriendlyPiece(boardState, file, rank + i);
        if (squareStatus == INVALID) {
            stopFlag[2] = true;
        }
        else if (squareStatus == ENEMY){
            moves -> push_back(buildPositionString(file, rank +i));
            stopFlag[2] = true;
        }
        else if (squareStatus == EMPTY){
            moves -> push_back(buildPositionString(file, rank +i));
        }
        else if (squareStatus == FRIENDLY){
            stopFlag[2] = true;
        }
    }
    if (stopFlag[3] == false) {
        
        int squareStatus = containsFriendlyPiece(boardState, file, rank-i);
        if (squareStatus == INVALID) {
            stopFlag[3] = true;
        }
        else if (squareStatus == ENEMY){
            moves -> push_back(buildPositionString(file, rank-i));
            stopFlag[3] = true;
        }
        else if (squareStatus == EMPTY){
            moves -> push_back(buildPositionString(file, rank-i));
        }
        else if (squareStatus == FRIENDLY){
            stopFlag[3] = true;
        }
    }
        
    }
    
    return moves;
}
vector<string>* Piece::knightMoves(Piece* boardState){
    static vector<string>* moves;
    moves = &m_moves;
    
    int rank = m_rank;
    int file = m_file;
    int squareStatus = containsFriendlyPiece(boardState, file+2, rank+1);
    
    
    if (squareStatus != 1 && squareStatus != -2) {
        moves->push_back(buildPositionString(file+2, rank+1));
    }
    squareStatus = containsFriendlyPiece(boardState, file-2, rank+1);

    if (squareStatus != 1 && squareStatus != -2) {
        moves->push_back(buildPositionString(file-2, rank+1));
    }
    squareStatus = containsFriendlyPiece(boardState, file+2, rank-1);

    if (squareStatus != 1 && squareStatus != -2) {
        moves->push_back(buildPositionString(file+2, rank-1));
    }
    squareStatus = containsFriendlyPiece(boardState, file-2, rank-1);

    if (squareStatus != 1 && squareStatus != -2) {
        moves->push_back(buildPositionString(file-2, rank-1));
    }
    squareStatus = containsFriendlyPiece(boardState, file+1, rank+2);

    if (squareStatus != 1 && squareStatus != -2) {
        moves->push_back(buildPositionString(file+1, rank+2));
    }
    squareStatus = containsFriendlyPiece(boardState, file-1, rank+2);

    if (squareStatus != 1 && squareStatus != -2) {
        moves->push_back(buildPositionString(file-1, rank+2));
    }
    squareStatus = containsFriendlyPiece(boardState, file+1, rank-2);

    if (squareStatus != 1 && squareStatus != -2) {
        moves->push_back(buildPositionString(file+1, rank-2));
    }
    squareStatus = containsFriendlyPiece(boardState, file-1, rank-2);

    if (squareStatus != 1 && squareStatus != -2) {
        moves->push_back(buildPositionString(file-1, rank-2));
    }
    
    
    
    // moves to 8 spots except off board
    
    return moves;
}
vector<string>* Piece::bishopMoves(Piece* boardState){
    static vector<string>* moves;
    moves = &m_moves;
    
    int rank = m_rank;
    int file = m_file;

    bool stopFlag[4] = {false, false, false, false};
    
    for ( int i = 1; i < 8; i++) {
        if (stopFlag[0] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file + i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[0] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file +i, rank +i));
                stopFlag[0] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file +i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
        }
        if (stopFlag[1] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file + i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[1] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file +i, rank -i));
                stopFlag[1] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file +i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[1] = true;
            }
        }
        if (stopFlag[2] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[2] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file -i, rank+i));
                stopFlag[2] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file -i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
        }
        if (stopFlag[3] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[3] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file -i, rank -i));
                stopFlag[3] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file -i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
        }
    }
    
    return moves;
}
vector<string>* Piece::queenMoves(Piece* boardState){
    static vector<string>* moves;
    moves = &m_moves;
    
    int rank = m_rank;
    int file = m_file;
    
    bool stopFlag[8] = {false, false, false, false, false, false, false, false};
    
    for ( int i = 1; i < 8; i++) {
        if (stopFlag[0] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file + i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[0] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file +i, rank +i));
                stopFlag[0] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file +i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
        }
        if (stopFlag[1] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file + i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[1] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file +i, rank -i));
                stopFlag[1] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file +i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[1] = true;
            }
        }
        if (stopFlag[2] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[2] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file -i, rank +i));
                stopFlag[2] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file -i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
        }
        if (stopFlag[3] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[3] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file -i, rank -i));
                stopFlag[3] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file -i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
        }
        if (stopFlag[4] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file + i, rank);
            if (squareStatus == INVALID) {
                stopFlag[4] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file +i, rank));
                stopFlag[4] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[4] = true;
            }
        }
        if (stopFlag[5] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank);
            if (squareStatus == INVALID) {
                stopFlag[5] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file -i, rank));
                stopFlag[5] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file -i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[5] = true;
            }
        }
        if (stopFlag[6] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[6] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file, rank +i));
                stopFlag[6] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[6] = true;
            }
        }
        if (stopFlag[7] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[7] = true;
            }
            else if (squareStatus == ENEMY){
                moves -> push_back(buildPositionString(file, rank -i));
                stopFlag[7] = true;
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[7] = true;
            }
        }
    }
    
    return moves;
}
vector<string>* Piece::kingMoves(Piece* boardState){
    static vector<string>* moves;
    moves = &m_moves;
    
    int rank = m_rank;
    int file = m_file;

    // The numbers next to each file denotes the desired position to check / move to, relative to the pieces location.
    
    if (containsFriendlyPiece(boardState, file+1, rank+1) != 1 && containsFriendlyPiece(boardState, file+1, rank+1) != -2) {
        moves->push_back(buildPositionString(file+1, rank+1));
    }
    if (containsFriendlyPiece(boardState, file-1, rank+1) != 1 && containsFriendlyPiece(boardState, file-1, rank+1) != -2) {
        moves->push_back(buildPositionString(file-1, rank+1));
    }
    if (containsFriendlyPiece(boardState, file+1, rank-1) != 1 && containsFriendlyPiece(boardState, file+1, rank-1) != -2) {
        moves->push_back(buildPositionString(file+1, rank-1));
    }
    if (containsFriendlyPiece(boardState, file-1, rank-1) != 1 && containsFriendlyPiece(boardState, file-1, rank-1) != -2) {
        moves->push_back(buildPositionString(file-1, rank-1));
    }
    if (containsFriendlyPiece(boardState, file, rank+1) != 1 && containsFriendlyPiece(boardState, file, rank+1) != -2) {
        moves->push_back(buildPositionString(file, rank+1));
    }
    if (containsFriendlyPiece(boardState, file, rank-1) != 1 && containsFriendlyPiece(boardState, file, rank-1) != -2) {
        moves->push_back(buildPositionString(file, rank-1));
    }
    if (containsFriendlyPiece(boardState, file+1, rank) != 1 && containsFriendlyPiece(boardState, file+1, rank) != -2) {
        moves->push_back(buildPositionString(file+1, rank));
    }
    if (containsFriendlyPiece(boardState, file-1, rank) != 1 && containsFriendlyPiece(boardState, file-1, rank) != -2) {
        moves->push_back(buildPositionString(file-1, rank));
    }
    return moves;
}
    // function for checking how many moves in a moveset attack a certain file and rank on the board
int Piece::countNumAttackers(vector<string> playerMoveset, int forFile, int forRank){
    int attackerCount = 0;
    for (auto it = playerMoveset.begin(); it != playerMoveset.end(); it++) {
        string simple = it->c_str();
        int file = cFileToIndex(simple[3]);
        int rank = simple[4] - '0'-1;
        
        if (file == forFile && rank == forRank) {
            attackerCount++;
        }
    }
    return attackerCount;
}

void Piece::enPassantInitiated(){
    if (m_type == 'P') {
        m_type = 'E';
        m_color = "Empty";
        m_moves.clear();
        m_doubleJumpTurn = -1;
    }
    else cout << "Failed to en passant piece type " << m_type << endl;

}

// Screen attackers functions

vector<string>* Piece::getScreenMoves(Piece* boardState){
    m_moves.clear();
    vector<string>* moves;
    moves = &m_moves;
    
    switch (m_type) {
        case 'P':return moves;
            break;
        case 'R':return rookScreenMoves(boardState);
            break;
        case 'N':return moves;
            break;
        case 'B':return bishopScreenMoves(boardState);
            break;
        case 'Q':return queenScreenMoves(boardState);
            break;
        case 'K':return moves;
            break;
        case 'E': return moves;
        default: cout << "Passed bad piece type to function getMoves()";
            return moves;
            break;
    }
    
    // check the type of piece being calculated for and runs its move calc function
    
}

vector<string>* Piece::rookScreenMoves(Piece* boardState){
    static vector<string>* moves;
    
    moves = &m_moves;
    
    int rank = m_rank;
    int file = m_file;
    
    bool stopFlag[4] = {false, false, false, false};
    int screenNum[4] = {0,0,0,0};
    
    for ( int i = 1; i < 8; i++) {
        string screenedPiece;
        
        if (stopFlag[0] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file +i, rank);
            if (squareStatus == INVALID) {
                stopFlag[0] = true;
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[0];
                if (screenNum[0] == 2 && boardState[convertCoordinateToBoardIndex(file +i, rank)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file +i, rank));
                    stopFlag[0] = true;
                }
               // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
                if (screenNum[0] > 1) {
                    stopFlag[0] = true;
                }
                
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
        }
        if (stopFlag[1] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank);
            if (squareStatus == INVALID) {
                stopFlag[1] = true;
            }
            else if (squareStatus == ENEMY){
                
                ++screenNum[1];
                if (screenNum[1] == 2 && boardState[convertCoordinateToBoardIndex(file -i, rank)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file -i, rank));
                    stopFlag[1] = true;
                }
                if (screenNum[1] > 1) {
                    stopFlag[1] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file -i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[1] = true;
            }
        }
        if (stopFlag[2] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[2] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[2];
                
                if (screenNum[2] == 2 && boardState[convertCoordinateToBoardIndex(file, rank+i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file, rank+i));
                    stopFlag[2] = true;
                }
                if (screenNum[2] > 1) {
                    stopFlag[2] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
        }
        if (stopFlag[3] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file, rank-i);
            if (squareStatus == INVALID) {
                stopFlag[3] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[3];
                
                if (screenNum[3] == 2 && boardState[convertCoordinateToBoardIndex(file, rank-i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file, rank-i));
                    stopFlag[3] = true;
                }
                if (screenNum[3] > 1) {
                    stopFlag[3] = true;
                }
            }
            else if (squareStatus == EMPTY){
                moves -> push_back(buildPositionString(file, rank-i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
        }
        
    }
    
    return moves;
}
vector<string>* Piece::bishopScreenMoves(Piece* boardState){
    static vector<string>* moves;
    moves = &m_moves;
    
    int rank = m_rank;
    int file = m_file;
    
    bool stopFlag[4] = {false, false, false, false};
    int screenNum[4] = {0,0,0,0};
    
    for ( int i = 1; i < 8; i++) {
        if (stopFlag[0] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file + i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[0] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[0];
                
                if (screenNum[0] == 2 && boardState[convertCoordinateToBoardIndex(file+i, rank+i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file+i, rank+i));
                    stopFlag[0] = true;
                }
                if (screenNum[0] > 1) {
                    stopFlag[0] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file +i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
        }
        if (stopFlag[1] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file + i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[1] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[1];
                
                if (screenNum[1] == 2 && boardState[convertCoordinateToBoardIndex(file+i, rank-i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file+i, rank-i));
                    stopFlag[1] = true;
                }
                if (screenNum[1] > 1) {
                    stopFlag[1] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file +i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[1] = true;
            }
        }
        if (stopFlag[2] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[2] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[2];
                
                if (screenNum[2] == 2 && boardState[convertCoordinateToBoardIndex(file-i, rank+i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file-i, rank+i));
                    stopFlag[2] = true;
                }
                if (screenNum[2] > 1) {
                    stopFlag[2] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file -i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
        }
        if (stopFlag[3] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[3] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[3];
                
                if (screenNum[3] == 2 && boardState[convertCoordinateToBoardIndex(file-i, rank-i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file-i, rank-i));
                    stopFlag[3] = true;
                }
                if (screenNum[3] > 1) {
                    stopFlag[3] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file -i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
        }
    }
    
    return moves;
}
vector<string>* Piece::queenScreenMoves(Piece* boardState){
    static vector<string>* moves;
    moves = &m_moves;
    
    int rank = m_rank;
    int file = m_file;
    
    bool stopFlag[8] = {false, false, false, false, false, false, false, false};
    int screenNum[8] = {0,0,0,0,0,0,0,0};
    
    for ( int i = 1; i < 8; i++) {
        if (stopFlag[0] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file + i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[0] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[0];
                
                if (screenNum[0] == 2 && boardState[convertCoordinateToBoardIndex(file+i, rank+i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file+i, rank+i));
                    stopFlag[0] = true;
                }
                if (screenNum[0] > 1) {
                    stopFlag[0] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file +i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[0] = true;
            }
        }
        if (stopFlag[1] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file + i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[1] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[1];
                
                if (screenNum[1] == 2 && boardState[convertCoordinateToBoardIndex(file+i, rank-i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file+i, rank-i));
                    stopFlag[1] = true;
                }
                if (screenNum[1] > 1) {
                    stopFlag[1] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file +i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[1] = true;
            }
        }
        if (stopFlag[2] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[2] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[2];
                
                if (screenNum[2] == 2 && boardState[convertCoordinateToBoardIndex(file-i, rank+i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file-i, rank+i));
                    stopFlag[2] = true;
                }
                if (screenNum[2] > 1) {
                    stopFlag[2] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file -i, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[2] = true;
            }
        }
        if (stopFlag[3] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[3] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[3];
                
                if (screenNum[3] == 2 && boardState[convertCoordinateToBoardIndex(file-i, rank-i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file-i, rank-i));
                    stopFlag[3] = true;
                }
                if (screenNum[3] > 1) {
                    stopFlag[3] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file -i, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[3] = true;
            }
        }
        if (stopFlag[4] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file + i, rank);
            if (squareStatus == INVALID) {
                stopFlag[4] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[4];
                
                if (screenNum[4] == 2 && boardState[convertCoordinateToBoardIndex(file+i, rank)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file+i, rank));
                    stopFlag[4] = true;
                }
                if (screenNum[4] > 1) {
                    stopFlag[4] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file +i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[4] = true;
            }
        }
        if (stopFlag[5] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file -i, rank);
            if (squareStatus == INVALID) {
                stopFlag[5] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[5];
                
                if (screenNum[5] == 2 && boardState[convertCoordinateToBoardIndex(file-i, rank)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file-i, rank));
                    stopFlag[5] = true;
                }
                if (screenNum[5] > 1) {
                    stopFlag[5] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file -i, rank));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[5] = true;
            }
        }
        if (stopFlag[6] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file, rank + i);
            if (squareStatus == INVALID) {
                stopFlag[6] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[6];
                
                if (screenNum[6] == 2 && boardState[convertCoordinateToBoardIndex(file, rank+i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file, rank+i));
                    stopFlag[6] = true;
                }
                if (screenNum[6] > 1) {
                    stopFlag[6] = true;
                }
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file, rank +i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[6] = true;
            }
        }
        if (stopFlag[7] == false) {
            
            int squareStatus = containsFriendlyPiece(boardState, file, rank -i);
            if (squareStatus == INVALID) {
                stopFlag[7] = true;
            }
            else if (squareStatus == ENEMY){
                ++screenNum[7];
                
                if (screenNum[7] == 2 && boardState[convertCoordinateToBoardIndex(file, rank-i)].getType() == 'K') {
                    moves -> push_back(buildPositionString(file, rank-i));
                    stopFlag[7] = true;
                }
                
                if (screenNum[7] > 1) {
                    stopFlag[7] = true;
                }
                
            }
            else if (squareStatus == EMPTY){
                //moves -> push_back(buildPositionString(file, rank -i));
            }
            else if (squareStatus == FRIENDLY){
                stopFlag[7] = true;
            }
        }
    }
    
    return moves;
}

