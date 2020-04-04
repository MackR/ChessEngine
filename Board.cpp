//
//  Board.cpp
//  Chess
//
//  Created by Mack Ragland on 11/5/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include "Board.hpp"

namespace chess {
    
    Board::Board() {
        
    }
    
    void Board::boardInit() {
        
        m_Boardstate[0] = Piece::Piece(0,0,'R', "White");
        m_Boardstate[1] = Piece::Piece(0,1,'N', "White");
        m_Boardstate[2] = Piece::Piece(0,2,'B', "White");
        m_Boardstate[3] = Piece::Piece(0,3,'Q', "White");
        m_Boardstate[4] = Piece::Piece(0,4,'K', "White");
        m_Boardstate[5] = Piece::Piece(0,5,'B', "White");
        m_Boardstate[6] = Piece::Piece(0,6,'N', "White");
        m_Boardstate[7] = Piece::Piece(0,7,'R', "White");
        for (int i = 8; i<16; i++) {
            m_Boardstate[i] = Piece::Piece(1, i-8, 'P', "White");
        }
        for (int i = 16; i<48; i++) {
            m_Boardstate[i] = Piece::Piece(i/8, i%8, 'E', "Empty");
        }
        
        for (int i = 48; i<56; i++) {
            m_Boardstate[i] = Piece::Piece(6, i-48, 'P', "Black");
        }
        m_Boardstate[56] = Piece::Piece(7,0,'R', "Black");
        //m_Boardstate[48] = Piece::Piece(6,0,'P', "White");  // Pawn on last rank for testing purposes
        //m_Boardstate[56] = Piece::Piece(7,0,'E', "Empty");  // 56th spot still needs initialized without a piece
        m_Boardstate[57] = Piece::Piece(7,1,'N', "Black");
        m_Boardstate[58] = Piece::Piece(7,2,'B', "Black");
        m_Boardstate[59] = Piece::Piece(7,3,'Q', "Black");
        m_Boardstate[60] = Piece::Piece(7,4,'K', "Black");
        m_Boardstate[61] = Piece::Piece(7,5,'B', "Black");
        m_Boardstate[62] = Piece::Piece(7,6,'N', "Black");
        m_Boardstate[63] = Piece::Piece(7,7,'R', "Black");

        

    }
    
    Board::~Board(){
        
        //delete[] m_pBoardstate;
    }
    
    Piece* Board::getBoardstate(){
        
        return m_Boardstate;
        
    }
    
     vector<string>* Board::getWhiteMoves(){
        return &m_whiteMoves;
    }
    
     vector<string>* Board::getBlackMoves(){
        return &m_blackMoves;
    }
    
    vector<Piece*>* Board::findPlayerPieces(string playerColor){
        vector<Piece*>* pPlayerPieces;
        if (playerColor == "White") {
            m_whitePieces.clear();
            pPlayerPieces = &m_whitePieces;
        }
        else if (playerColor == "Black"){
            m_blackPieces.clear();
            pPlayerPieces = &m_blackPieces;
        }
        else {
            return NULL;
        }
        for (int i = 0; i < 64; i++) {
            if (m_Boardstate[i].getColor() == playerColor) {
                    pPlayerPieces->push_back(&m_Boardstate[i]);
                
            }
        }
        return pPlayerPieces;
    }
    Piece* Board::findPiece(string color, char type){
        for (int i = 0; i < 64; i++) {
            if (m_Boardstate[i].getColor() == color && m_Boardstate[i].getType() == type) {
                return &m_Boardstate[i];
                // Need to keep coding the find piece function/ check that it works
            }
        }
        cout << "Piece could not be found, returning NULL" << endl;
        return NULL;
    }
    
    
    
    
    
    void Board::calcPlayerMoveset(string playerColor, int currentTurn, bool checkMoveValidity){
        
        //cout << "Calculating moveset for player: " << playerColor << endl;
        
        // Piece kingPiece = *findPiece(playerColor, 'K');  // commented this out because think i dont need it
        
        vector<Piece*>* pPlayerColorPieces = findPlayerPieces(playerColor);
        vector<Piece*> playerColorPieces = *pPlayerColorPieces;
        vector<string>* completePlayerMoveset;
        vector<string> enemyMoveset;
        bool firstTimeAddingCastling = true;
        if (playerColor == "White") {
            m_whiteMoves.clear();
            completePlayerMoveset = &m_whiteMoves;
            enemyMoveset = m_blackMoves;
            
        }
        else if (playerColor == "Black"){
            m_blackMoves.clear();
            completePlayerMoveset = &m_blackMoves;
            enemyMoveset = m_whiteMoves;
        }
        else return;
        
        for(auto it = playerColorPieces.begin(); it != playerColorPieces.end(); it++){
            Piece tempPiece = **it;
            
            
            //Add castling to the move choices if it is viable
            if (tempPiece.getType() == 'K' && firstTimeAddingCastling && tempPiece.getFile() == 4) {
                bool check = false;
                int attackerNum = tempPiece.countNumAttackers(enemyMoveset, tempPiece.getFile(), tempPiece.getRank());
                if (attackerNum > 0) {check = true;}
                
                //All conditions for castling
                
                //Mutual:
                bool notInCheck = !check;
                bool kingHasMoved = tempPiece.getMoved();
                
                // Kingside:
                
                bool rightRookHasMoved = m_Boardstate[tempPiece.convertCoordinateToBoardIndex(tempPiece.getRank(), tempPiece.getFile()+3)].getMoved();
                int pieceTypeOnSameRankPlus1FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()+1, tempPiece.getRank());
                int pieceTypeOnSameRankPlus2FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()+2, tempPiece.getRank());
                int numAttackersOnSameRankPlus1FILE = this->countNumAttackers(enemyMoveset, 5, 0);
                int numAttackersOnSameRankPlus2FILE = this->countNumAttackers(enemyMoveset, 6, 0);
                
                // Queenside:
                bool leftRookHasMoved = m_Boardstate[tempPiece.convertCoordinateToBoardIndex(tempPiece.getRank(), tempPiece.getFile()-4)].getMoved();
                int pieceTypeOnSameRankminus1FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()-1, tempPiece.getRank());
                int pieceTypeOnSameRankminus2FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()-2, tempPiece.getRank());
                int pieceTypeOnSameRankminus3FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()-3, tempPiece.getRank());
                int numAttackersOnSameRankMinus1FILE = this->countNumAttackers(enemyMoveset, 5, 0);
                int numAttackersOnSameRankMinus2FILE = this->countNumAttackers(enemyMoveset, 6, 0);
                
                
                
                if (notInCheck && pieceTypeOnSameRankPlus1FILE == 0 && pieceTypeOnSameRankPlus2FILE == 0 && kingHasMoved == false && rightRookHasMoved == false && numAttackersOnSameRankPlus1FILE == 0 && numAttackersOnSameRankPlus2FILE == 0){
                    
                    completePlayerMoveset->push_back("O-O");
                    //std::cout << "Player may castle king-side" << endl;
                }
                if (notInCheck && pieceTypeOnSameRankminus1FILE == 0 && pieceTypeOnSameRankminus2FILE == 0 && pieceTypeOnSameRankminus3FILE == 0 && tempPiece.getMoved() == false && leftRookHasMoved == false && numAttackersOnSameRankMinus1FILE == 0 && numAttackersOnSameRankMinus2FILE == 0) {
                    completePlayerMoveset->push_back("O-O-O");
                    //std::cout << "Player may castle queen-side" << endl;
                }
                firstTimeAddingCastling = false;
            }
            
            // get the piece moves from the pieces that are on the board that are your color
            if (tempPiece.getColor() == playerColor) {

                auto partialMoveset = tempPiece.getMoves(m_Boardstate, currentTurn);
                completePlayerMoveset->insert(completePlayerMoveset->end(), partialMoveset->begin(),partialMoveset->end());
                if (tempPiece.getType() == 'K' && checkMoveValidity) {
                //std::cout << "Appended moveset for " << tempPiece.getType() << ", containing " << partialMoveset->size() << " possible moves" << endl;
                //std::cout << "Moves are: " ;Q
                }
                if (checkMoveValidity) {
                    for (auto it = partialMoveset->begin(); it!=partialMoveset->end(); it++) {
                        //cout << *it << ", ";
                    }
                }
                
                //cout << endl;
            }
            else {
                cout << "Piece captured" << endl;
            }
            
        }
        
        
        
        
        //Check the moves and remove invalid ones here
        // to know if move is invalid
        if (checkMoveValidity) {
            
            chess::Board tempBoard = *this;
            Piece* tempBoardstate = tempBoard.getBoardstate();
            Piece kingPiece = *findPiece(playerColor, 'K');
            
            for (auto it = completePlayerMoveset->begin(); it != completePlayerMoveset->end();) {
                auto move = *it;
                int prevFile = 0;
                int prevRank = 0;
                int newFile = 0;
                int newRank = 0;
                char pieceType;
                char promotionChoice = 'P';
                bool promotePawn = false;
                int prevIndex = 0;
                int newIndex = 0;
                
        if (!(move == "O-O" || move == "O-O-O")) {
            
            pieceType = move[0];
            prevFile = Piece::cFileToIndex(move[1]);
            prevRank = move[2] - '0'-1;
            newFile = Piece::cFileToIndex(move[3]);
            newRank = move[4] -'0'-1;    // Convert string to int
            if (pieceType == 'P' && ((prevRank == 6 && newRank == 7) || (prevRank == 1 && newRank == 0))) { // check if piece is a pawn that is promoting
                try {
                    promotionChoice = move.at(5);
                    promotePawn = true;
                } catch (...) {
                    promotionChoice = 'P';
                }
            }
            
            
            
            // makes the move the player chose on this hypothetical board
            prevIndex = Piece::convertCoordinateToBoardIndex(prevFile, prevRank);
            newIndex = Piece::convertCoordinateToBoardIndex(newFile, newRank);
            
            // Check for en passant and delete the en passanted piece if it's true
            if (tempBoardstate[newIndex].getColor() == "Empty" && tempBoardstate[prevIndex].getType() == 'P' && abs(newFile-prevFile) == 1) {
                int enPassantedPieceIndex = Piece::convertCoordinateToBoardIndex(newFile, prevRank);
                Piece enPassantedPiece = tempBoardstate[enPassantedPieceIndex];
                tempBoardstate[enPassantedPieceIndex].setColor("Empty");
                tempBoardstate[enPassantedPieceIndex].setType('E');
            }
            
            // move the piece
            tempBoardstate[newIndex].setColor(tempBoardstate[prevIndex].getColor());
            if (promotePawn) {
                tempBoardstate[newIndex].setType(promotionChoice);
                promotePawn = false;
            }
            else {
            tempBoardstate[newIndex].setType(tempBoardstate[prevIndex].getType());
            }
            
            tempBoardstate[prevIndex].setColor("Empty");
            tempBoardstate[prevIndex].setType('E');
            
            
        }
        else if (move == "O-O"){
            
            

            auto prevKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile(), kingPiece.getRank());
            auto newKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()+2, kingPiece.getRank());
            auto prevRookIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()+3, kingPiece.getRank());
            auto newRookIndex = newKingIndex-1;
            
            tempBoardstate[newKingIndex].setColor(tempBoardstate[prevKingIndex].getColor());
            tempBoardstate[newKingIndex].setType(tempBoardstate[prevKingIndex].getType());
            tempBoardstate[prevKingIndex].setColor("Empty");
            tempBoardstate[prevKingIndex].setType('E');
            tempBoardstate[prevKingIndex].Moved();
            
            tempBoardstate[newRookIndex].setColor(tempBoardstate[prevRookIndex].getColor());
            tempBoardstate[newRookIndex].setType(tempBoardstate[prevRookIndex].getType());
            tempBoardstate[prevRookIndex].setColor("Empty");
            tempBoardstate[prevRookIndex].setType('E');
            tempBoardstate[prevRookIndex].Moved();
            
        }
        else if (move == "O-O-O"){
            // Check conditions for the player to castle Queenside
            
            
            // makes the move the player chose on this hypothetical board
            auto prevKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile(), kingPiece.getRank());
            auto newKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()-2, kingPiece.getRank());
            auto prevRookIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()-4, kingPiece.getRank());
            auto newRookIndex = newKingIndex+1;
            
            tempBoardstate[newKingIndex].setColor(tempBoardstate[prevKingIndex].getColor());
            tempBoardstate[newKingIndex].setType(tempBoardstate[prevKingIndex].getType());
            tempBoardstate[prevKingIndex].setColor("Empty");
            tempBoardstate[prevKingIndex].setType('E');
            tempBoardstate[prevKingIndex].Moved();
            
            tempBoardstate[newRookIndex].setColor(tempBoardstate[prevRookIndex].getColor());
            tempBoardstate[newRookIndex].setType(tempBoardstate[prevRookIndex].getType());
            tempBoardstate[prevRookIndex].setColor("Empty");
            tempBoardstate[prevRookIndex].setType('E');
            tempBoardstate[prevRookIndex].Moved();
            
        }
        
        // we now have our hypothetical board move made, but we need to check it's validity
        
        
        
        
        //calculates the players moves after making the move on the hypothetical board
        
        
        if (playerColor == "White") {
            
            // time check of evaluate board function
            auto start = std::chrono::system_clock::now();
            tempBoard.calcPlayerMoveset("Black", currentTurn, false); // generates possible moves for black and doesn't check move if moves put black in check
            auto end = std::chrono::system_clock::now();
            
            std::chrono::duration<double> elapsed_seconds = end-start;
            //std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            
            std::cout << "finished calc moveset false computation.  "
            << "Elapsed time: " << elapsed_seconds.count() << "s\n";
            
            
            
            enemyMoveset = *tempBoard.getBlackMoves();
        }
        else if (playerColor == "Black"){
            tempBoard.calcPlayerMoveset("White", currentTurn+1, false);
            enemyMoveset = *tempBoard.getWhiteMoves();
        }
        
        // find the player's king on this new hypothetical board
        auto tempPiece = *tempBoard.findPiece(playerColor, 'K');
        
        //calculate the number of pieces attacking the king
        int attackerNum = tempPiece.countNumAttackers(enemyMoveset, tempPiece.getFile(), tempPiece.getRank());
        
        // if the number of attackers on king == 0, declare it a verified move and make the real board equal to the hypothetical board
        if (attackerNum == 0) {
            // do nothing, the move is valid
            ++it;
        }
        else {
            it = completePlayerMoveset->erase(it);
        }
                tempBoard = *this; // after making the moves and checking their validity, reset the board back to how it was so we can check next move
        // if the num of attackers is not equal to zero, modify the rejection message to user based on if they were already in check
        
        // if flag check for valid moves = true, check them, if false, don't check them
        
        } // end of for loop for each move
        //std::cout << endl;
        }
    }
    
    int Board::countNumAttackers(vector<string> playerMoveset, int forFile, int forRank){
        int attackerCount = 0;
        for (auto it = playerMoveset.begin(); it != playerMoveset.end(); it++) {
            string simple = it->c_str();
            int file = Piece::cFileToIndex(simple[3]);
            int rank = simple[4] - '0'-1;
            
            if (file == forFile && rank == forRank) {
                attackerCount++;
            }
        }
        return attackerCount;
    }
    
    void Board::getAttackingMoves(vector<string> playerMoves, int forFile, int forRank, vector<string>* pCalcResultVector){
        vector<string> attackingMoves;
        for (auto it = playerMoves.begin(); it != playerMoves.end(); it++) {
            string simple = it->c_str();
            int file = Piece::cFileToIndex(simple[3]);
            int rank = simple[4] - '0'-1;
            
            if (file == forFile && rank == forRank) {
                attackingMoves.push_back(simple);
            }
        }
        *pCalcResultVector = attackingMoves;
    }
    
    void Board::makeColinearSquaresVector(string pieceMove, vector<string>* pColinearSquaresResultVector){
        pColinearSquaresResultVector->clear();
        char pieceType;
        char cPrevFile;
        int prevRank;
        char cNewFile;
        int newRank;
        
        parseMove(pieceMove, &pieceType, &cPrevFile, &prevRank, &cNewFile, &newRank);
        
        if (pieceType == 'N') {
            cout << "Cannot make colinear squares vector from knight moves" << endl;
        }
        
        
        int fileDiff = cNewFile - cPrevFile;
        int rankDiff = newRank - prevRank; // gets difference between squares
        int fileIncrementer = 0;
        int rankIncrementer = 0;
        int loopIterations = 0;
        
        if (fileDiff != 0) {
            fileIncrementer = fileDiff/abs(fileDiff);
            loopIterations = abs(fileDiff);
        }
        if (rankDiff != 0) {
            rankIncrementer = rankDiff/abs(rankDiff); // if rankDiff is negative/positive, it increments negative/positive respectively
            loopIterations = abs(rankDiff);
        }
        
        
        
        for (int i = 0; i < loopIterations; i++) {
            
            char colinearSquareFile = static_cast<char>(cPrevFile + fileIncrementer*i);
            int colinearSquareRank = prevRank + rankIncrementer*i;
            
            string sColinearSquareFile (1,colinearSquareFile);
            string sColinearSquareRank = to_string(colinearSquareRank);
            
            pColinearSquaresResultVector->push_back(sColinearSquareFile+sColinearSquareRank);
            
            
        }
        
        cout << "Colinear squares created" << endl;
        
    }
    
    void Board::parseMove( string move, char* pieceTypeAddress, char* prevFileAddress, int* prevRankAddress, char* newFileAddress, int* newRankAddress){
        *pieceTypeAddress = move[0];
        *prevFileAddress = Piece::cFileToIndex(move[1]);
        *prevRankAddress = move[2] - '0'-1;
        *newFileAddress = Piece::cFileToIndex(move[3]);
        *newRankAddress = move[4] -'0'-1;    // Convert string to int
    }
    
}
