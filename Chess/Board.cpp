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
        
        m_Boardstate[0] = Piece::Piece(0,0,'R', WHITE);
        m_Boardstate[1] = Piece::Piece(0,1,'N', WHITE);
        m_Boardstate[2] = Piece::Piece(0,2,'B', WHITE);
        m_Boardstate[3] = Piece::Piece(0,3,'Q', WHITE);
        m_Boardstate[4] = Piece::Piece(0,4,'K', WHITE);
        m_Boardstate[5] = Piece::Piece(0,5,'B', WHITE);
        m_Boardstate[6] = Piece::Piece(0,6,'N', WHITE);
        m_Boardstate[7] = Piece::Piece(0,7,'R', WHITE);
        for (int i = 8; i<16; i++) {
            m_Boardstate[i] = Piece::Piece(1, i-8, 'P', WHITE);
        }
        for (int i = 16; i<48; i++) {
            m_Boardstate[i] = Piece::Piece(i/8, i%8, 'E', "Empty");
        }
        
        for (int i = 48; i<56; i++) {
            m_Boardstate[i] = Piece::Piece(6, i-48, 'P', BLACK);
        }
        m_Boardstate[56] = Piece::Piece(7,0,'R', BLACK);
        //m_Boardstate[48] = Piece::Piece(6,0,'P', WHITE);  // Pawn on last rank for testing purposes
        //m_Boardstate[56] = Piece::Piece(7,0,'E', "Empty");  // 56th spot still needs initialized without a piece
        m_Boardstate[57] = Piece::Piece(7,1,'N', BLACK);
        m_Boardstate[58] = Piece::Piece(7,2,'B', BLACK);
        m_Boardstate[59] = Piece::Piece(7,3,'Q', BLACK);
        m_Boardstate[60] = Piece::Piece(7,4,'K', BLACK);
        m_Boardstate[61] = Piece::Piece(7,5,'B', BLACK);
        m_Boardstate[62] = Piece::Piece(7,6,'N', BLACK);
        m_Boardstate[63] = Piece::Piece(7,7,'R', BLACK);

            for (int i = 0; i< 16; i++) {
                m_whitePieces.push_back(&m_Boardstate[i]);
            }
        
            for (int i = 48; i< 64; i++) {
                m_blackPieces.push_back(&m_Boardstate[i]);
            }

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
    vector<Piece*>* Board::getPieces(string color){
        if (color == WHITE) {
            return &m_whitePieces;
        }
        else if (color == BLACK){
            return &m_blackPieces;
        }
        else{
            cout << "Invalid player color input to function Board::getPieces(string color)" << endl;
            return NULL;
        }
    }
    
    void Board::findPlayerPieces(){
        m_whitePieces.clear();
        m_blackPieces.clear();
        
        for (int i = 0; i < 64; i++) {
            string color = m_Boardstate[i].getColor();
            if (color == WHITE) {
                    m_whitePieces.push_back(&m_Boardstate[i]);
            }
            else if(color == BLACK){
                m_blackPieces.push_back(&m_Boardstate[i]);
            }
        }
        return;
    }
    Piece* Board::findKing(string color){ // this function is only needed for kings
        vector<Piece*>* pieceList;
        if (color == WHITE) {
            pieceList = getPieces(WHITE);
        }
        else if(color == BLACK){
            pieceList = getPieces(BLACK);
        }
        else {
            cout << "King not found, returning NULL" << endl;
            return NULL;
        }
        
        for (auto it = pieceList->begin(); it != pieceList->end(); it++) {
            auto tempPiece = **it;
            if (tempPiece.getType() == 'K'){
                return *it;
            }
        }
        
        cout << "King captured, returning NULL" << endl;
        return NULL;
        
    }
    
    
    
    
    
    void Board::calcValidatedPlayerMoveset(string playerColor, int currentTurn, bool checkMoveValidity){
        
        //cout << "Calculating moveset for player: " << playerColor << endl;
        
        // Piece kingPiece = *findPiece(playerColor, 'K');  // commented this out because think i dont need it
        
        vector<Piece*>* pPlayerColorPieces = getPieces(playerColor);
        vector<Piece*> playerColorPieces = *pPlayerColorPieces;
        vector<string>* completePlayerMoveset;
        vector<string>* enemyMoveset;
        bool firstTimeAddingCastling = true;
        if (playerColor == WHITE) {
            m_whiteMoves.clear();
            completePlayerMoveset = &m_whiteMoves;
            enemyMoveset = &m_blackMoves;
            
        }
        else if (playerColor == BLACK){
            m_blackMoves.clear();
            completePlayerMoveset = &m_blackMoves;
            enemyMoveset = &m_whiteMoves;
        }
        else return;
        
        for(auto it = playerColorPieces.begin(); it != playerColorPieces.end(); it++){
            Piece tempPiece = **it;
            
            
            //Add castling to the move choices if it is viable
            if (tempPiece.getType() == 'K' && firstTimeAddingCastling && tempPiece.getFile() == 4) {
                bool check = false;
                int attackerNum = tempPiece.countNumAttackers(*enemyMoveset, tempPiece.getFile(), tempPiece.getRank());
                if (attackerNum > 0) {check = true;}
                
                //All conditions for castling
                
                //Mutual:
                bool notInCheck = !check;
                bool kingHasMoved = tempPiece.getMoved();
                
                // Kingside:
                
                bool rightRookHasMoved = m_Boardstate[tempPiece.convertCoordinateToBoardIndex(tempPiece.getRank(), tempPiece.getFile()+3)].getMoved();
                int pieceTypeOnSameRankPlus1FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()+1, tempPiece.getRank());
                int pieceTypeOnSameRankPlus2FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()+2, tempPiece.getRank());
                int numAttackersOnSameRankPlus1FILE = this->countNumAttackers(*enemyMoveset, 5, 0);
                int numAttackersOnSameRankPlus2FILE = this->countNumAttackers(*enemyMoveset, 6, 0);
                
                // Queenside:
                bool leftRookHasMoved = m_Boardstate[tempPiece.convertCoordinateToBoardIndex(tempPiece.getRank(), tempPiece.getFile()-4)].getMoved();
                int pieceTypeOnSameRankminus1FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()-1, tempPiece.getRank());
                int pieceTypeOnSameRankminus2FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()-2, tempPiece.getRank());
                int pieceTypeOnSameRankminus3FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()-3, tempPiece.getRank());
                int numAttackersOnSameRankMinus1FILE = this->countNumAttackers(*enemyMoveset, 5, 0);
                int numAttackersOnSameRankMinus2FILE = this->countNumAttackers(*enemyMoveset, 6, 0);
                
                
                
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
            Piece kingPiece = *findKing(playerColor);
            
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
        
        
        if (playerColor == WHITE) {
            
            tempBoard.calcValidatedPlayerMoveset(BLACK, currentTurn, false); // generates possible moves for black and doesn't check move if moves put black in check
            
            enemyMoveset = tempBoard.getBlackMoves(); // this is redundant but I'll leave for clarity purpose
        }
        else if (playerColor == BLACK){
            tempBoard.calcValidatedPlayerMoveset(WHITE, currentTurn+1, false);
            enemyMoveset = tempBoard.getWhiteMoves(); // this is redundant but I'll leave for clarity purpose
        }
        
        // find the player's king on this new hypothetical board
        auto tempPiece = *tempBoard.findKing(playerColor);
        
        //calculate the number of pieces attacking the king
        int attackerNum = tempPiece.countNumAttackers(*enemyMoveset, tempPiece.getFile(), tempPiece.getRank());
        
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
    } // Super slow move calculaion N^2 where N = move num
    
    void Board::calcScreenMovesets(){
        
        //cout << "Calculating moveset for player: " << playerColor << endl;
        
        // Piece kingPiece = *findPiece(playerColor, 'K');  // commented this out because think i dont need it
        
        m_whiteScreenMoves.clear();
        m_blackScreenMoves.clear();
        
        for(auto it = m_whitePieces.begin(); it != m_whitePieces.end(); it++){ // go through all the white pieces
            Piece tempPiece = **it;
            
            // get the moves for white pieces on the board that are "screen" attacks  (pinning attacks)
                auto partialScreenMoveset = tempPiece.getScreenMoves(m_Boardstate); // the -1 parameter isn't needed but function prototype demands it
                m_whiteScreenMoves.insert(m_whiteScreenMoves.end(), partialScreenMoveset->begin(),partialScreenMoveset->end()); // add them all up
        }
        for(auto it = m_blackPieces.begin(); it != m_blackPieces.end(); it++){ // do the same for black
            Piece tempPiece = **it;
            
            // get the piece moves from the pieces that are on the board that are your color
            auto partialScreenMoveset = tempPiece.getScreenMoves(m_Boardstate); // the -1 parameter isn't needed but function prototype demands it
            m_blackScreenMoves.insert(m_blackScreenMoves.end(), partialScreenMoveset->begin(),partialScreenMoveset->end());
        }
        
            }
    
    void Board::calcPlayerMovesetV2(string playerColor, int currentTurn, bool validateMoveset){
        
        //cout << "Calculating moveset for player: " << playerColor << endl;
        
        
        vector<Piece*>* pPlayerColorPieces = getPieces(playerColor);
        vector<Piece*> playerColorPieces = *pPlayerColorPieces;
        vector<string>* completePlayerMoveset;
        vector<string>* enemyMoveset;
        vector<string>* enemyScreenMoveset;
        string enemyColor;
        bool addCastling = false;
        
        if (playerColor == WHITE) {
            m_whiteMoves.clear();
            completePlayerMoveset = &m_whiteMoves;
            enemyMoveset = &m_blackMoves;
            enemyScreenMoveset = &m_blackScreenMoves;
            enemyColor = BLACK;
        }
        else if (playerColor == BLACK){
            m_blackMoves.clear();
            completePlayerMoveset = &m_blackMoves;
            enemyMoveset = &m_whiteMoves;
            enemyScreenMoveset = &m_whiteScreenMoves;
            enemyScreenMoveset->clear();
            enemyColor = WHITE;
        }
        else return;
        
        
        calcScreenMovesets(); // calculate all the screening attacks (attacks that hit the king through another piece)
        
        // find king, calculate checks on king, calculate screen attackers on king
        
        Piece* kingPiece = findKing(playerColor);  // might have issue with this if comp is simulating taking enemy king
        
        if (kingPiece == nullptr) {
            SDL_Delay(1);
            return;
        }
        
        vector<string> attackingKingMoves;
        vector<string> apparentScreenAttackingKingMoves;
        vector<string> screenAttackingKingMoves;
        vector<string> kingUnfilteredMoveset;
        vector<string> kingFilteredMoveset;
        vector<string> castlingMoveset;
        int checksOnKing = 0;
        
        if (!validateMoveset) { // if we don't need to apply normal move rules with regards to the king then just get all the unabridged moves
            for (auto it = pPlayerColorPieces->begin(); it != pPlayerColorPieces->end(); it++) {
                Piece tempPiece = **it;
                if (tempPiece.getType() != 'K') {
                    auto partialMoveset = tempPiece.getMoves(m_Boardstate, currentTurn);
                    completePlayerMoveset->insert(completePlayerMoveset->end(), partialMoveset->begin(),partialMoveset->end());
                }
            }
            return;
        }
        
        calcPlayerMovesetV2(enemyColor, currentTurn, false); // calculate all the possible places the opponents pieces can move at the moment
        
        getAttackingMoves(*enemyMoveset, kingPiece->getFile(), kingPiece->getRank(), &attackingKingMoves); // get all the moves that attack the king
        getAttackingMoves(*enemyScreenMoveset, kingPiece->getFile(), kingPiece->getRank(), &screenAttackingKingMoves); // get all the screen moves on the king
        
        checksOnKing = attackingKingMoves.size();  // find the number of attacks on king
        
        enemyScreenMoveset->clear();
        
        if (checksOnKing == 2){ // If there are two pieces attacking the king, the king must move to survive -  shortest: linear time
            // only get king moves and don't do moveset creation below
            
            kingUnfilteredMoveset = *kingPiece->getMoves(getBoardstate(), currentTurn);
            for (auto it = kingUnfilteredMoveset.begin(); it != kingUnfilteredMoveset.end();) {
                string move = *it;
                
                char pieceType, prevFile, newFile;
                int prevRank, newRank;
                
                parseMove(move, &pieceType, &prevFile, &prevRank, &newFile, &newRank);
                
                if (countNumAttackers(*enemyMoveset, static_cast<int>(newFile-'A'), newRank) != 0) {// CAN IMPROVE SPEED BY GETTING THE MOVES AND DOING COMPARISON
                    kingUnfilteredMoveset.erase(it);
                }
                else {
                    ++it;
                }
                
            }
            kingFilteredMoveset = kingUnfilteredMoveset; // create an updated variable
            completePlayerMoveset->insert(completePlayerMoveset->end(), kingFilteredMoveset.begin(),kingFilteredMoveset.end());
            
            return; // Return because the only viable moves are ones where the king moves
            
        } // if there are two attacks on the king, the king must move
        for(auto it = playerColorPieces.begin(); it != playerColorPieces.end(); it++){
            Piece tempPiece = **it;
            
            
            //Add castling to the move choices if it is viable
            if (tempPiece.getType() == 'K' && tempPiece.getFile() == 4 && tempPiece.getMoved() == false) { // if the king hasn't moved
                bool check = false;
                castlingMoveset.clear();
                int attackerNum = tempPiece.countNumAttackers(*enemyMoveset, tempPiece.getFile(), tempPiece.getRank());
                if (attackerNum > 0) {check = true;}
                
                //All conditions for castling
                
                //Mutual:
                bool notInCheck = !check;
                bool kingHasMoved = tempPiece.getMoved();
                
                // Kingside:
                
                bool rightRookHasMoved = m_Boardstate[tempPiece.convertCoordinateToBoardIndex(tempPiece.getRank(), tempPiece.getFile()+3)].getMoved();
                int pieceTypeOnSameRankPlus1FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()+1, tempPiece.getRank());
                int pieceTypeOnSameRankPlus2FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()+2, tempPiece.getRank());
                int numAttackersOnSameRankPlus1FILE = this->countNumAttackers(*enemyMoveset, 5, 0);
                int numAttackersOnSameRankPlus2FILE = this->countNumAttackers(*enemyMoveset, 6, 0);
                
                // Queenside:
                bool leftRookHasMoved = m_Boardstate[tempPiece.convertCoordinateToBoardIndex(tempPiece.getRank(), tempPiece.getFile()-4)].getMoved();
                int pieceTypeOnSameRankminus1FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()-1, tempPiece.getRank());
                int pieceTypeOnSameRankminus2FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()-2, tempPiece.getRank());
                int pieceTypeOnSameRankminus3FILE = tempPiece.containsFriendlyPiece(m_Boardstate, tempPiece.getFile()-3, tempPiece.getRank());
                int numAttackersOnSameRankMinus1FILE = this->countNumAttackers(*enemyMoveset, 5, 0);
                int numAttackersOnSameRankMinus2FILE = this->countNumAttackers(*enemyMoveset, 6, 0);
                
                
                
                if (notInCheck && pieceTypeOnSameRankPlus1FILE == EMPTY && pieceTypeOnSameRankPlus2FILE == EMPTY && kingHasMoved == false && rightRookHasMoved == false && numAttackersOnSameRankPlus1FILE == 0 && numAttackersOnSameRankPlus2FILE == 0){
                    
                    castlingMoveset.push_back("O-O");
                    addCastling = true;
                    //std::cout << "Player may castle king-side" << endl;
                }
                if (notInCheck && pieceTypeOnSameRankminus1FILE == EMPTY && pieceTypeOnSameRankminus2FILE == EMPTY && pieceTypeOnSameRankminus3FILE == EMPTY && tempPiece.getMoved() == false && leftRookHasMoved == false && numAttackersOnSameRankMinus1FILE == 0 && numAttackersOnSameRankMinus2FILE == 0) {
                    castlingMoveset.push_back("O-O-O");
                    addCastling = true;
                    //std::cout << "Player may castle queen-side" << endl;
                }
            } // add castling if viable
            if (tempPiece.getType() != 'K'){ // if a piece is not the king
                if (!screenAttackingKingMoves.empty()) { // if there is still an unremoved pinning move
                    auto partialMoveset = tempPiece.getMoves(m_Boardstate, currentTurn);

                    for (auto it = screenAttackingKingMoves.begin(); it != screenAttackingKingMoves.end();) { // for each of the pinning moves
                        string move = *it;
                        
                        if (squareIsBetweenSquares(move, tempPiece.getFile(), tempPiece.getRank())) { // if the piece is the one being pinned (in front of king)
                            for (auto it1 = partialMoveset->begin(); it1 != partialMoveset->end();) { // for each of the moves in the single piece's moveset
                                auto possibleMove = *it1;
                                int testFile = possibleMove[3]-'A';
                                int testRank = possibleMove[4]-'1';
                                if (!squareIsBetweenSquares(move, testFile, testRank)) { // if the move is not between the king and the attacker
                                    partialMoveset->erase(it1); // erase the possible move from the vector as invalid
                                }
                                else {
                                    ++it1;
                                }
                            }
                            screenAttackingKingMoves.erase(it); // erase the pinning move from the pinning move moveset because we've found the piece being pinned and have modified its moveset to account for being pinned
                        }
                        else {
                            ++it;
                        }
                    }
                    
                    completePlayerMoveset->insert(completePlayerMoveset->end(), partialMoveset->begin(),partialMoveset->end()); // add the filtered moveset to the complete moveset
                }
                else { // add one of the regular unpinned movesets from pieces
                    
                    auto partialMoveset = tempPiece.getMoves(m_Boardstate, currentTurn);
                    completePlayerMoveset->insert(completePlayerMoveset->end(), partialMoveset->begin(),partialMoveset->end());
                    
                    //cout << endl;
                }
            }
            else if (tempPiece.getType() == 'K') { // add king's viable moves depending on what would put it in check
                kingUnfilteredMoveset = *tempPiece.getMoves(m_Boardstate, currentTurn);
                    for (auto it = kingUnfilteredMoveset.begin(); it != kingUnfilteredMoveset.end();) {
                        string move = *it;
                        
                        char pieceType, prevFile, newFile;
                        int prevRank, newRank;
                        
                        parseMove(move, &pieceType, &prevFile, &prevRank, &newFile, &newRank);
                        
                        if (countNumAttackers(*enemyMoveset, static_cast<int>(newFile-'A'), newRank) != 0) {// CAN IMPROVE SPEED BY GETTING THE MOVES AND DOING COMPARISON
                            kingUnfilteredMoveset.erase(it);
                        }
                        else {
                            ++it;
                        }
                        
                    }
                
                kingFilteredMoveset = kingUnfilteredMoveset; // create an updated variable
                completePlayerMoveset->insert(completePlayerMoveset->end(), kingFilteredMoveset.begin(),kingFilteredMoveset.end()); // add filtered moveset to complete moveset
                
            }
        }
        
        if (checksOnKing == 1) { // if there is only one attacker on the king then
            
            for (auto pMove = completePlayerMoveset->begin(); pMove != completePlayerMoveset->end();) {
                string move = *pMove;
                char pieceType = move[0];
                int testSquareFile = move[3] - 'A';
                int testSquareRank = move[4] - '1';
                char pieceAttackingKingType = attackingKingMoves[0][0]; // get the attacking enemy piece type
                if (pieceType == 'K'){ // IF WE'RE LOOKING AT KING MOVES, THEN IT CAN MOVE OUT OF CHECK, or capture the attacker
                    ++pMove; // this move is fine, move on to the next move in the list
                }
                else if (pieceAttackingKingType == 'N') { // if the attacker is a knight
                    int pieceAttackingKingFile = attackingKingMoves[0][1]-'A'; // attackingKingMoves is a string vector of moves
                    int pieceAttackingKingRank = attackingKingMoves[0][2]-'1';
                    if (testSquareRank == pieceAttackingKingRank && testSquareFile == pieceAttackingKingFile) { // if we can capture the knight
                        ++pMove; // then keep the move as valid move
                    }
                    else{ // if we can't capture the knight
                        completePlayerMoveset->erase(pMove); // then erase the potential move
                    }
                }
                else if (!squareIsBetweenSquares(attackingKingMoves[0], testSquareFile, testSquareRank)) { // if the piece is not a knight and your possible move cannot get between the attacker or capture it
                    completePlayerMoveset->erase(pMove); // throw out the move
                }
                else {
                    ++pMove;
                }
                
                
            }
            
            
            
        }
        // Writes out list of moves:
        //cout << "Moveset List for " << playerColor << endl;
        //for (auto it = completePlayerMoveset->begin(); it != completePlayerMoveset->end(); it++) {
        //    cout << *it << ", ";
        //}
        //cout << endl;
        
        if (addCastling) {
            completePlayerMoveset->insert(completePlayerMoveset->end(), castlingMoveset.begin(),castlingMoveset.end()); // add filtered moveset to complete moveset
        }
        
        
        
        
        //Check the moves and remove invalid ones here
        // to know if move is invalid
    } // Fast move calculation
    
    
    
    int Board::countNumAttackers(vector<string> playerMoveset, int forFile, int forRank){
        int attackerCount = 0;
        for (auto it = playerMoveset.begin(); it != playerMoveset.end(); it++) {
            string move = *it;
            int file = Piece::cFileToIndex(move[3]);
            int rank = move[4] - '0'-1;
            
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
            return;
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
    
    bool Board::squareIsBetweenSquares(string move, int testSquareFile, int testSquareRank){
        
        char pieceType, prevFile, newFile;
        int prevRank, newRank;
        
        parseMove(move, &pieceType, &prevFile, &prevRank, &newFile, &newRank);
        
        if (pieceType == 'N') {
            return false;
        }
        
        
        int square2File = newFile - 'A';
        int square2Rank = newRank;
        int square1File = prevFile - 'A';
        int square1Rank = prevRank;
        
        int x1 = 0, x2=0, y1=0, y2=0;
        
        int y, x;
        
        x = testSquareFile;
        y = testSquareRank;
        
        if (square1Rank >= square2Rank) {
            y2 = square1Rank;
            x2 = square1File;
            y1 = square2Rank;
            x1 = square2File;
        }
        else if (square2Rank > square1Rank){
            y2 = square2Rank;
            x2 = square2File;
            y1 = square1Rank;
            x1 = square1File;
        }
        
        // if it's a vertical line, if the file is the same, and the square is inbetween or on the endpoints, return true
        if (x2 == x1) {
            if (x == x1 && y1 <= y && y <= y2) {
                return true;
            }
            else {
                return false;
            }
        }
        // if it's a horizontal line, if the test rank is the same, and the square is inbetween or on the endpoints, return true
        if (y2 == y1) {
            if (y == y1) {
                if (x1 <= x2) {
                    if (x1 <= x && x <= x2) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else if (x2<=x1){
                    if (x2<=x && x <= x1) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
            }
            else {
                return false;
            }
        }
        
        float slope = (y2-y1)/(x2-x1); // calculate slope of line  (the only valid slope is 1 or -1)
        
        int b = y1-slope*x1; // calculate the y intercept
        
        if (y == slope*x+b && y1 <= y && y <= y2) { // if the point is on the line and the y value is on or between endpoints, return true
            return true;
        }
        
        return false; // all other cases the test square isn't on the line between the pieces
        
        
    }
    
    void Board::getSquaresBetweenSquares(string move, vector<string>* returnVectorPointer){
        
        char pieceType, prevFile, newFile;
        int prevRank, newRank;
        
        parseMove(move, &pieceType, &prevFile, &prevRank, &newFile, &newRank);
        
        
        
        int square2File = newFile - 'A';
        int square2Rank = newRank;
        int square1File = prevFile - 'A';
        int square1Rank = prevRank;
        
        int x1 = 0, x2=0, y1=0, y2=0;
        vector<string> holderVector;
        
        if (square1Rank >= square2Rank) {
            y2 = square1Rank;
            y1 = square2Rank;
        }
        else if (square2Rank > square1Rank){
            y2 = square2Rank;
            y1 = square1Rank;
        }
        
        if (square1File >= square2File) {
            x2 = square1File;
            x1 = square2File;
        }
        else if (square2File > square1File){
            x2 = square2File;
            x1 = square1File;
        }
        
        // if it's a vertical line, if the file is the same, and the square is inbetween or on the endpoints, return true
        if (x2 == x1) {
            for (int i = 0; i < y2-y1; i++) {
                char y, x;
                y = static_cast<char>(y1+i);
                x = static_cast<char>(x1);
                string sX(1, x);
                string sY(1,y);
                
                holderVector.push_back(sX+sY);
            }
            *returnVectorPointer = holderVector;
            return;
        }
        // if it's a horizontal line, if the test rank is the same, and the square is inbetween or on the endpoints, return true
        if (y2 == y1) {
            for (int i = 0; i < x2-x1; i++) {
                char y, x;
                y = static_cast<char>(y1);
                x = static_cast<char>(x1+i);
                string sX(1, x);
                string sY(1,y);
                
                holderVector.push_back(sX+sY);
            }
            *returnVectorPointer = holderVector;
            return;
        }
        
        float slope = (y2-y1)/(x2-x1); // calculate slope of line  (the only valid slope is 1 or -1)
        //test the slope
        if (abs(slope) != 1) {
            std::cout << "two non linear squares given, invalid inputs" << endl;
        }
        
        for (int i = 0; i < x2-x1; i++) {
            char y, x;
            y = static_cast<char>(y1+i);
            x = static_cast<char>(x1+i);
            string sX(1, x);
            string sY(1,y);
            
            holderVector.push_back(sX+sY);
        }
        *returnVectorPointer = holderVector;
        return;
        
    }
    
    void Board::parseMove( string move, char &pieceTypeAddress, char &prevFileAddress, int &prevRankAddress, char &newFileAddress, int &newRankAddress){
        pieceTypeAddress = move[0];
        prevFileAddress = move[1];
        prevRankAddress = move[2] - '0'-1;
        newFileAddress = move[3];
        newRankAddress = move[4] -'0'-1;    // Convert string to int
    }
    
    int Board::containsFriendlyPiece(string playerColor, int nFile, int rank){
        Piece* boardState = getBoardstate();
        if (nFile >= 0 && nFile <= 7 && rank >= 0 && rank <= 7) {
            if(boardState[Piece::convertCoordinateToBoardIndex(nFile, rank)].getColor() == playerColor){
                //  cout << "Friendly piece on square " << indexTo_cFile(nFile) << rank << endl;
                return 1;
            }
            else if (boardState[Piece::convertCoordinateToBoardIndex(nFile, rank)].getColor() == "Empty"){
                //  cout << "Square " << indexTo_cFile(nFile) << rank+1 << " is empty" << endl;
                return 0;
            }
            else {
                //  cout << "Enemy piece on square " << indexTo_cFile(nFile) << rank << endl;
                
                return -1;
            }
        }
        
        cout << "Warning, illegal rank and file numbers" << endl;
        
        return -2;
    }
    
}
