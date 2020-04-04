//
//  Player.cpp
//  Chess
//
//  Created by Mack Ragland on 10/27/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include "Player.hpp"

namespace Player{
    
    Player::Player(string color){
        m_player_color = color;
    }
    void Player::init(chess::Board* board){
        
        Piece* boardState = board->getBoardstate();
        
        if (m_player_color == "White") {
            for (int i = 0; i< 16; i++) {
                m_pieces.push_back(&boardState[i]);
            }
        }
        else if (m_player_color == "Black"){
            for (int i = 48; i< 64; i++) {
                m_pieces.push_back(&boardState[i]);
            }
        }
        else{
            std::cout << "Player init() function detected an invalid color" << endl;
        }
        
        std::cout << "Player pieces have been initialized for " << m_player_color << endl << endl;
        
    }
    
    
    string Player::askPlayerForValidMove(vector<string>* validMoveset){
        
        string input;
        std::cout << "What move would you like to make? ";
        std::cin >> input;
        std::cout << endl << endl;
        while (1) {
            
            for (auto it = m_completeMoveset.begin(); it != m_completeMoveset.end(); it++) {
                
                if (input == *it) {
                    //std::cout << "Valid move" << endl;
                    return input;
                }
                
                
            }
            cout << "You have entered an invalid move, please try again: ";
            cin.clear();
            cin.ignore();
            std::cin >> input;
            std::cout << endl;
        }
        
        
    }
    
    
    bool Player::makeMove(chess::Board* board){
        
        //cout << "Real board moveset calculation" << endl;
        
        // Calculate player's possible moves and assign movesets for each player to variables


        
        // ask the player for move and test if the moves is valid, if the move isn't valid, discard their choice and ask again
        
        string move;
        string enemyColor;
        cout << "It is " << m_player_color << "'s turn" << endl; // announce who's turn it is
        if (humanPlayer == true){
            
            board->calcPlayerMoveset(m_player_color, turnCounter, true);  // calculates player moves
            
            if (m_player_color == "White") {
                m_completeMoveset = *board->getWhiteMoves();
            }
            else if (m_player_color == "Black"){
                m_completeMoveset = *board->getBlackMoves();
            }
            else {
                cout << "Player doesn't have a valid color" << endl;
            }
            move = askPlayerForValidMove(&m_completeMoveset);
        }
        else { move = computerBeginThinking(board);
            if (move == "") {
                SDL_Delay(1); // passed a bad move
            }
        }
        
        // Modifies the a given board with a move, function will do invalid moves, so only give it valid ones
        modifyBoardWithMove(board, move);
        

        
        if (m_player_color == "White") {
            
            enemyColor = "Black";
            board->calcPlayerMoveset("White", turnCounter+1, true);
            board->calcPlayerMoveset("Black", turnCounter, true);
            m_completeMoveset = *board->getWhiteMoves();
            
        }
        else if (m_player_color == "Black"){
            
            enemyColor = "White";
            board->calcPlayerMoveset("White", turnCounter+1, true);
            board->calcPlayerMoveset("Black", turnCounter+1, true);
            m_completeMoveset = *board->getBlackMoves();
        }
        
        auto kingPiece = *board->findPiece(enemyColor, 'K');
        int numAttackers = kingPiece.countNumAttackers(m_completeMoveset, kingPiece.getFile(), kingPiece.getRank());
        if(numAttackers != 0){
            return achievedCheckmateOnEnemy(board);
        }
        
        incrementTurn(); // Increase turn counter
        return false;
        
    }
    
    
    
    void Player::modifyBoardWithMove(chess::Board* board, string move){
        
        auto boardState = board->getBoardstate();
        
        int prevFile = 0;
        int prevRank = 0;
        int newFile = 0;
        int newRank = 0;
        int prevIndex = 0;
        int newIndex = 0;
        char promotionChoice = 'P';
        bool promotePawn = false;
        int prevKingIndex = 0;
        int newKingIndex = 0;
        int prevRookIndex = 0;
        int newRookIndex = 0;
        
        if (move == "") {
            cout << "Function passed bad move" << endl;
            return;
        }
        
        if (!(move == "O-O" || move == "O-O-O")) {
            
            prevFile = Piece::cFileToIndex(move[1]);
            prevRank = move[2] - '0'-1;
            newFile = Piece::cFileToIndex(move[3]);
            newRank = move[4] -'0'-1;    // Convert string to int
            if (move.size() == 6) {
                promotionChoice = move[5];
                promotePawn = true;
            }
            
            // ################### modifies the board with the move the player chose #########################
            prevIndex = Piece::convertCoordinateToBoardIndex(prevFile, prevRank);
            newIndex = Piece::convertCoordinateToBoardIndex(newFile, newRank);
            
            if (boardState[newIndex].getColor() == "Empty" && boardState[prevIndex].getType() == 'P' && abs(newFile-prevFile) == 1) {
                int enPassantedPieceIndex = Piece::convertCoordinateToBoardIndex(newFile, prevRank);
                Piece enPassantedPiece = boardState[enPassantedPieceIndex];
                boardState[enPassantedPieceIndex].setColor("Empty");
                boardState[enPassantedPieceIndex].setType('E');
            }
            
            boardState[newIndex].setColor(boardState[prevIndex].getColor());
            if (promotePawn) {
                boardState[newIndex].setType(promotionChoice);
                promotePawn = false;
            }
            else {
                boardState[newIndex].setType(boardState[prevIndex].getType());
            }
            boardState[prevIndex].setColor("Empty");
            boardState[prevIndex].setType('E');
            // SETTING ENPASSANT FLAG HERE
            if (boardState[newIndex].getType() == 'P' && (abs(newRank - prevRank) == 2)) {
                boardState[newIndex].setDoubleJumpTurn(turnCounter);
            }
            
            boardState[prevIndex].Moved();
            
        }
        else if (move == "O-O"){
            
            // finds the king on the board
            auto kingPiece = *board->findPiece(m_player_color, 'K');
            
            // makes the move the player chose on this hypothetical board
            prevKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile(), kingPiece.getRank());
            newKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()+2, kingPiece.getRank());
            prevRookIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()+3, kingPiece.getRank());
            newRookIndex = newKingIndex-1;
            
            boardState[newKingIndex].setColor(boardState[prevKingIndex].getColor());
            boardState[newKingIndex].setType(boardState[prevKingIndex].getType());
            boardState[prevKingIndex].setColor("Empty");
            boardState[prevKingIndex].setType('E');
            boardState[prevKingIndex].Moved();
            
            boardState[newRookIndex].setColor(boardState[prevRookIndex].getColor());
            boardState[newRookIndex].setType(boardState[prevRookIndex].getType());
            boardState[prevRookIndex].setColor("Empty");
            boardState[prevRookIndex].setType('E');
            boardState[prevRookIndex].Moved();
            
        }
        else if (move == "O-O-O"){
            // Check conditions for the player to castle Queenside
            
            // finds the king on the board
            auto kingPiece = *board->findPiece(m_player_color, 'K');
            
            // makes the move the player chose on this hypothetical board
            prevKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile(), kingPiece.getRank());
            newKingIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()-2, kingPiece.getRank());
            prevRookIndex = Piece::convertCoordinateToBoardIndex(kingPiece.getFile()-4, kingPiece.getRank());
            newRookIndex = newKingIndex+1;
            
            boardState[newKingIndex].setColor(boardState[prevKingIndex].getColor());
            boardState[newKingIndex].setType(boardState[prevKingIndex].getType());
            boardState[prevKingIndex].setColor("Empty");
            boardState[prevKingIndex].setType('E');
            boardState[prevKingIndex].Moved();
            
            boardState[newRookIndex].setColor(boardState[prevRookIndex].getColor());
            boardState[newRookIndex].setType(boardState[prevRookIndex].getType());
            boardState[prevRookIndex].setColor("Empty");
            boardState[prevRookIndex].setType('E');
            boardState[prevRookIndex].Moved();
            
        }
        
    }
    
    bool Player::achievedCheckmateOnEnemy(chess::Board* board){
        
        auto checkmateBoard = *board;
        Piece enemyKing;
        
        if (m_player_color == "White") {
            checkmateBoard.calcPlayerMoveset("White",turnCounter+1, true);
            m_completeMoveset = *checkmateBoard.getWhiteMoves();
            enemyKing = *checkmateBoard.findPiece("Black", 'K');
        }
        if (m_player_color == "Black") {
            checkmateBoard.calcPlayerMoveset("Black",turnCounter+1, true);
            m_completeMoveset = *checkmateBoard.getBlackMoves();
            enemyKing = *checkmateBoard.findPiece("White", 'K');
            
        }
        
        int enemyKingAttackers = enemyKing.countNumAttackers(m_completeMoveset, enemyKing.getFile(), enemyKing.getRank());
        
        if (enemyKingAttackers > 0) { // if enemy king is in check
            
            Piece* boardState = checkmateBoard.getBoardstate();
            Piece* originalBoardState = boardState;
            vector<string> enemyMoveset;
            
            if (m_player_color == "White") {
                checkmateBoard.calcPlayerMoveset("Black",turnCounter, true);
                enemyMoveset = *checkmateBoard.getBlackMoves();
            }
            if (m_player_color == "Black") {
                checkmateBoard.calcPlayerMoveset("White",turnCounter+1, true);
                enemyMoveset = *checkmateBoard.getWhiteMoves();
                
            }
            
            int prevFile = 0;
            int prevRank = 0;
            int newFile = 0;
            int newRank = 0;
            int prevIndex = 0;
            int newIndex = 0;
            
            for (auto it = enemyMoveset.begin(); it != enemyMoveset.end(); it++) {
                string move = *it;
                prevFile = Piece::cFileToIndex(move[1]);
                prevRank = move[2] - '0'-1;
                newFile = Piece::cFileToIndex(move[3]);
                newRank = move[4] -'0'-1;    // Convert string to int
                
                // makes the move the player chose on this hypothetical board
                prevIndex = Piece::convertCoordinateToBoardIndex(prevFile, prevRank);
                newIndex = Piece::convertCoordinateToBoardIndex(newFile, newRank);
                
                boardState[newIndex].setColor(boardState[prevIndex].getColor());
                boardState[newIndex].setType(boardState[prevIndex].getType());
                boardState[prevIndex].setColor("Empty");
                boardState[prevIndex].setType('E');
                
                // perform en passant if the conditions apply
                if (boardState[newIndex].getColor() == "Empty" && boardState[prevIndex].getType() == 'P' && abs(newFile-prevFile) == 1) {
                    int enPassantedPieceIndex = Piece::convertCoordinateToBoardIndex(newFile, prevRank);
                    Piece enPassantedPiece = boardState[enPassantedPieceIndex];
                    boardState[enPassantedPieceIndex].setColor("Empty");
                    boardState[enPassantedPieceIndex].setType('E');
                }
                
                if (m_player_color == "White") {
                    //checkmateBoard.calcPlayerMoveset("Black",turnCounter);
                    //enemyMoveset = *checkmateBoard.getBlackMoves();  // enemyMoveset was already calculated above
                    //cout << "Enemy move is: " << move << endl;
                    enemyKing = *checkmateBoard.findPiece("Black", 'K');
                    checkmateBoard.calcPlayerMoveset("White", turnCounter+1, true);
                    m_completeMoveset = *checkmateBoard.getWhiteMoves();
                }
                if (m_player_color == "Black") {
                    //checkmateBoard.calcPlayerMoveset("White",turnCounter+1);
                    //enemyMoveset = *checkmateBoard.getWhiteMoves();  // enemyMoveset was already calculated above
                    enemyKing = *checkmateBoard.findPiece("White", 'K');
                    checkmateBoard.calcPlayerMoveset("Black", turnCounter+1, true);
                    m_completeMoveset = *checkmateBoard.getBlackMoves();
                    
                }
                
                
                
                enemyKingAttackers = enemyKing.countNumAttackers(m_completeMoveset, enemyKing.getFile(), enemyKing.getRank());
                if (enemyKingAttackers == 0) {
                    cout << move << " saves the day " << endl;
                    incrementTurn(); // Increase turn counter
                    cout << "Check!" << endl;
                    return false;
                }
                *boardState = *originalBoardState;
            }
            return true;
            
        }
        
        
        return false;
        
    }
    
    void Player::incrementTurn(){
        turnCounter += 1;
    }
    
    string Player::computerBeginThinking(chess::Board *board){
        
        // setup get the board and copy it to a simulation board, and calculate possible moves for the position
        int stopDepth = 2;
        auto simulationBoard = *board;
        
        // time check of evaluate board function
        auto start = std::chrono::system_clock::now();
        computerEvaluateBoard(board, m_player_color);;
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        
        std::cout << "finished board evaluation computation at " << std::ctime(&end_time)
        << "elapsed time: " << elapsed_seconds.count() << "s\n";
        
        vector<string> simMoveset;
        vector<string> simEnemyMoveset;
        
        
        // time check of calc moveset function
         start = std::chrono::system_clock::now();
        simulationBoard.calcPlayerMoveset(m_player_color, turnCounter, true);
         end = std::chrono::system_clock::now();
        
        elapsed_seconds = end-start;
        end_time = std::chrono::system_clock::to_time_t(end);
        
        std::cout << "finished move calc computation at " << std::ctime(&end_time)
        << "elapsed time: " << elapsed_seconds.count() << "s\n";
        
        
        
        map<string, float> moveScores;
        auto holderBoard = simulationBoard;
        string move;
        if (m_player_color == "White") {
            simMoveset = *simulationBoard.getWhiteMoves();
            
            for (auto it = simMoveset.begin(); it != simMoveset.end(); it++) {
                string move = *it;
                if (move == "") {
                    cout << "Warning, bad move selected" << endl;
                }
                
                // time check of modify board function
                start = std::chrono::system_clock::now();
                modifyBoardWithMove(&simulationBoard, move);
                end = std::chrono::system_clock::now();
                
                elapsed_seconds = end-start;
                end_time = std::chrono::system_clock::to_time_t(end);
                
                std::cout << "finished modify board with move calc computation at " << std::ctime(&end_time)
                << "elapsed time: " << elapsed_seconds.count() << "s\n";
                
                
                float moveScore = computerMinimizer(&simulationBoard, 1, stopDepth); // it's returning the overall value of the board some moves into the future
                moveScores.insert (std::pair<string,float>(move,moveScore) );
                simulationBoard = holderBoard;
                
            }
            
            string largestKey;
            float largestScore = -200;
            for (std::pair<std::string, float> element : moveScores) {
                // Accessing KEY from element
                std::string key = element.first;
                // Accessing VALUE from element.
                int score = element.second;
                
                if (score >= largestScore) {
                    largestScore = score;
                    largestKey = key;
                }
                if (key == "") {
                    SDL_Delay(1); // passed a bad move
                }
                
            }
            move  = largestKey;
        }
        else {
            simMoveset = *simulationBoard.getBlackMoves();
            
            for (auto it = simMoveset.begin(); it != simMoveset.end(); it++) {
            string move = *it;
            modifyBoardWithMove(&simulationBoard, move);
            float moveScore = computerMaximizer(&simulationBoard, 1, stopDepth);
            moveScores.insert (std::pair<string,float>(move,moveScore) );
            simulationBoard = holderBoard;
            }
            
            
            string smallestKey;
            float smallestScore = 200;
            for (std::pair<std::string, float> element : moveScores) {
                // Accessing KEY from element
                std::string key = element.first;
                // Accessing VALUE from element.
                int score = element.second;
                
                if (score <= smallestScore) {
                    smallestScore = score;
                    smallestKey = key;
                }
                if (key == "") {
                    SDL_Delay(1); // passed a bad move
                }
                
            }
            move  = smallestKey;
        }
        
        
        if (move == "") {
            cout << "Warning, bad move selected" << endl;
        }
        
        
// need to grab the largest value out of the map to finish this

        
        return move;

    }
    
    float Player::computerEvaluateBoard(chess::Board* board, string playerTurn){  // currently only evaluates the board on material advantage
        
        float pawnValue = 1;
        float knightValue = 3;
        float bishopValue = 3.25;
        float rookValue = 5;
        float queenValue = 9;
        float kingValue = 100;
        
        float moveCount = 0;
        
        const int MAX_MOVE_NUM_BISHOP = 13;  // maximum number moves a bishop can have at any point in game
        const int MAX_MOVE_NUM_KNIGHT = 8;
        const int MAX_MOVE_NUM_QUEEN = 27;
        const int MAX_MOVE_NUM_ROOK = 14;
        
        float score = 0;
        auto evaluationBoard = *board;
        
        vector<Piece*> whitePieces = *evaluationBoard.findPlayerPieces("White");
        vector<Piece*> blackPieces = *evaluationBoard.findPlayerPieces("Black");
        if (blackPieces.size() == 0) {
            cout << "Warning: Black's pieces have not been found yet";
        }
        Piece* boardState = evaluationBoard.getBoardstate();
        
        for (auto it = whitePieces.begin(); it != whitePieces.end(); it++) { // evaluate white's pieces for material and slight position
            auto evalPiece = **it;
            
            switch (evalPiece.getType()) {
                case 'K':
                    score += kingValue;
                case 'Q':
                    score += queenValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score += ((moveCount/MAX_MOVE_NUM_QUEEN)-0.5)*0.95; // apply weighting to incentivize getting queen out third and rook 4th
                    break;
                case 'R':
                    score += rookValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score += ((moveCount/MAX_MOVE_NUM_ROOK)-0.5)*0.9;
                    break;
                case 'N':
                    score += knightValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score += (moveCount/MAX_MOVE_NUM_KNIGHT)-0.5;
                    break;
                case 'B':
                    score += bishopValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score += (moveCount/MAX_MOVE_NUM_BISHOP)-0.5;
                    break;
                case 'P':
                    score += pawnValue;
                    break;
                default:
                    break;
            }
            
        }
        
        for (auto it = blackPieces.begin(); it != blackPieces.end(); it++) { // evaluate black's pieces for material and slight position
            auto evalPiece = **it;
            
            switch (evalPiece.getType()) {
                case 'K':
                    score -= kingValue;
                case 'Q':
                    score -= queenValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score -= ((moveCount/MAX_MOVE_NUM_QUEEN)+0.5)*0.95;
                    break;
                case 'R':
                    score -= rookValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score -= ((moveCount/MAX_MOVE_NUM_ROOK)+0.5)*0.9;
                    break;
                case 'N':
                    score -= knightValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score -= (moveCount/MAX_MOVE_NUM_KNIGHT)+0.5;
                    break;
                case 'B':
                    score -= bishopValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score -= (moveCount/MAX_MOVE_NUM_BISHOP)+0.5;
                    break;
                case 'P':
                    score -= pawnValue;
                    break;
                default:
                    break;
            }
            
        }
        
        
        
        
        return score;
    }
    
    float Player::computerMaximizer(chess::Board* board, int currentDepth, int stopDepth){
        chess::Board simBoard = *board;
        
        simBoard.calcPlayerMoveset("White", turnCounter+currentDepth, true);
        vector<string> moves = *simBoard.getWhiteMoves();
        
        float* scores;
        scores = new float[moves.size()];
        for (int i=0; i<moves.size(); i++) {
            scores[i] = 0;    // Initialize all elements to zero.
        }
        
        float singleScore = 0;
        float maxScore;
        
        if (currentDepth == stopDepth) {
            singleScore = computerEvaluateBoard(&simBoard, "White");
            
            return singleScore;
        }
        
        
        int i = 0;
        auto holderBoard = simBoard;
        for (auto it = moves.begin(); it != moves.end(); it++) {
            string move = *it;
            modifyBoardWithMove(&simBoard, move);
            scores[i] = computerMinimizer(&simBoard, currentDepth+1, stopDepth);
            ++i;
            simBoard = holderBoard;
        }
        maxScore = *std::max_element(scores, scores+moves.size());
        delete[] scores;
        return maxScore;
        
    }
    
    float Player::computerMinimizer(chess::Board* board, int currentDepth, int stopDepth){
        chess::Board simBoard = *board;
        simBoard.calcPlayerMoveset("Black", turnCounter+currentDepth, true);
        vector<string> moves = *simBoard.getBlackMoves();
        //Piece kingPiece = *simBoard.findPiece(m_player_color, 'K'); // REMOVE ME AFTER DEBUGGING
        float* scores;
        scores = new float[moves.size()];
        for (int i=0; i<moves.size(); i++) {
            scores[i] = 0;    // Initialize all elements to zero.
        }
        float singleScore = 0;
        float minScore;
        
        if (currentDepth == stopDepth) {
            singleScore = computerEvaluateBoard(&simBoard, "Black");
            
            return singleScore;
        }
        
        
        int i = 0;
        auto holderBoard = simBoard;
        //kingPiece = *simBoard.findPiece(m_player_color, 'K'); // REMOVE ME AFTER DEBUGGING
        for (auto it = moves.begin(); it != moves.end(); it++) {
            string move = *it;
            modifyBoardWithMove(&simBoard, move);
            
            scores[i] = computerMaximizer(&simBoard, currentDepth+1, stopDepth);
            ++i;
            simBoard = holderBoard;
        }
        minScore = *std::min_element(scores, scores+moves.size());
        delete[] scores;
        return minScore;
    }
    
    
    
    
    
}


