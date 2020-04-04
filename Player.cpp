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


        
        // ask the player for move and test if the move is valid, if the move isn't valid, discard their choice and ask again
        
        string move;
        string enemyColor;
        cout << "It is " << m_player_color << "'s turn" << endl; // announce who's turn it is
        if (humanPlayer == true){
            
            //board->calcValidatedPlayerMoveset(m_player_color, turnCounter, true);  // calculates player moves
            board->calcPlayerMovesetV2(m_player_color, turnCounter, true);  // calculates player moves
            
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
        else {
            // time check of evaluate board function
            auto start = std::chrono::system_clock::now();
            move = computerBeginThinking(board);
            auto end = std::chrono::system_clock::now();
            
            std::chrono::duration<double> elapsed_seconds = end-start;
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            
            std::cout << "elapsed comp think time: " << elapsed_seconds.count() << "s\n";
            
            
            if (move == "") {
                SDL_Delay(1); // passed a bad move
            }
        }
        
        // Modifies the a given board with a move, function will do invalid moves, so only give it valid ones
        modifyBoardWithMove(board, move);
        

        
        if (m_player_color == "White") {
            
            enemyColor = "Black";
            //board->calcValidatedPlayerMoveset("White", turnCounter+1, true);
            //board->calcValidatedPlayerMoveset("Black", turnCounter, true);
            board->calcPlayerMovesetV2("White", turnCounter+1, true);  // can decrease number of calls here
            board->calcPlayerMovesetV2("Black", turnCounter, true);
            m_completeMoveset = *board->getWhiteMoves();
            
        }
        else if (m_player_color == "Black"){
            
            enemyColor = "White";
            //board->calcValidatedPlayerMoveset("White", turnCounter+1, true);
            //board->calcValidatedPlayerMoveset("Black", turnCounter+1, true);
            board->calcPlayerMovesetV2("White", turnCounter, true);
            board->calcPlayerMovesetV2("Black", turnCounter, true);
            m_completeMoveset = *board->getBlackMoves();
        }
        
        auto kingPiece = board->findKing(enemyColor);
        if (kingPiece == NULL) {
            return true;
        }
        int numAttackers = kingPiece->countNumAttackers(m_completeMoveset, kingPiece->getFile(), kingPiece->getRank());
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
                boardState[enPassantedPieceIndex].setColor("Empty");
                boardState[enPassantedPieceIndex].setType('E');
            }
            
            boardState[newIndex].setColor(boardState[prevIndex].getColor());
            if (!promotePawn) {
                boardState[newIndex].setType(boardState[prevIndex].getType());
            }
            else {
                boardState[newIndex].setType(promotionChoice);
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
            auto kingPiece = *board->findKing(m_player_color);
            
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
            auto kingPiece = *board->findKing(m_player_color);
            
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
        
        board->findPlayerPieces(); // after making the move, update the lists containing the locations of all player pieces
        
    }
    
    bool Player::achievedCheckmateOnEnemy(chess::Board* board){
        
        auto checkmateBoard = *board;
        Piece enemyKing;
        
        if (m_player_color == "White") {
            checkmateBoard.calcPlayerMovesetV2("White",turnCounter+1, true);
            m_completeMoveset = *checkmateBoard.getWhiteMoves();
            enemyKing = *checkmateBoard.findKing("Black");
        }
        if (m_player_color == "Black") {
            checkmateBoard.calcPlayerMovesetV2("Black",turnCounter+1, true);
            m_completeMoveset = *checkmateBoard.getBlackMoves();
            enemyKing = *checkmateBoard.findKing("White");
            
        }
        
        int enemyKingAttackers = enemyKing.countNumAttackers(m_completeMoveset, enemyKing.getFile(), enemyKing.getRank());
        
        if (enemyKingAttackers > 0) { // if enemy king is in check
            
            Piece* boardState = checkmateBoard.getBoardstate();
            Piece* originalBoardState = boardState;
            vector<string> enemyMoveset;
            
            if (m_player_color == "White") {
                checkmateBoard.calcPlayerMovesetV2("Black",turnCounter, true);
                enemyMoveset = *checkmateBoard.getBlackMoves();
            }
            if (m_player_color == "Black") {
                checkmateBoard.calcPlayerMovesetV2("White",turnCounter+1, true);
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
                    boardState[enPassantedPieceIndex].setColor("Empty");
                    boardState[enPassantedPieceIndex].setType('E');
                }
                
                if (m_player_color == "White") {
                    //checkmateBoard.calcValidatedPlayerMoveset("Black",turnCounter);
                    //enemyMoveset = *checkmateBoard.getBlackMoves();  // enemyMoveset was already calculated above
                    //cout << "Enemy move is: " << move << endl;
                    enemyKing = *checkmateBoard.findKing("Black");
                    checkmateBoard.calcPlayerMovesetV2("White", turnCounter+1, true);
                    m_completeMoveset = *checkmateBoard.getWhiteMoves();
                }
                if (m_player_color == "Black") {
                    //checkmateBoard.calcValidatedPlayerMoveset("White",turnCounter+1);
                    //enemyMoveset = *checkmateBoard.getWhiteMoves();  // enemyMoveset was already calculated above
                    enemyKing = *checkmateBoard.findKing("White");
                    checkmateBoard.calcPlayerMovesetV2("Black", turnCounter+1, true);
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
        static int maxStopDepth = 4;
        currentTurnBoardScore = computerEvaluateBoard(board, m_player_color);
        auto simulationBoard = *board;
        
        // time check of evaluate board function
        auto start = std::chrono::system_clock::now();
        computerEvaluateBoard(board, m_player_color);
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        
        std::cout << "elapsed board eval time: " << elapsed_seconds.count() << "s\n";
        
        vector<string> simMoveset;
        vector<string> simEnemyMoveset;
        
        
        // time check of calc moveset function
         start = std::chrono::system_clock::now();
        simulationBoard.calcPlayerMovesetV2(m_player_color, turnCounter, true);
         end = std::chrono::system_clock::now();
        
        elapsed_seconds = end-start;
        end_time = std::chrono::system_clock::to_time_t(end);
        
        std::cout << "move calc computation elapsed time" << elapsed_seconds.count() << "s\n";
        const double OVERHEADCOMPENSATION = 1.06875;
        long double maxMoveTime = 0.00021*pow((simulationBoard.getWhiteMoves()->size()+simulationBoard.getBlackMoves()->size())/2, maxStopDepth)*1.5;
        
        cout << "Max possible move time calculation: " << maxMoveTime << " seconds" << endl;;
        
        
        
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
                
                //std::cout << "finished modify board with move calc computation at " << std::ctime(&end_time)
                //<< "elapsed time: " << elapsed_seconds.count() << "s\n";
                
                // do a breadth calculation of depth 2 and find all the moves that suck and remove them from here, then feed those moves into next layer
                
                float moveScore = computerMinimizer(&simulationBoard, 1, maxStopDepth); // Need a filtered moveset parameter // it's returning the overall value of the board some moves into the future
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
            modifyBoardWithMove(&simulationBoard, move);  // makes test move for black
            float moveScore = computerMaximizer(&simulationBoard, 1, maxStopDepth); // inputs test move into calculator
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
        
        vector<Piece*> whitePieces = *evaluationBoard.getPieces("White");
        vector<Piece*> blackPieces = *evaluationBoard.getPieces("Black");
        if (blackPieces.empty()) {
            cout << "Warning: Black's pieces have not been found yet";
        }
        
        auto playerMoveset = m_completeMoveset;
        
        Piece* boardState = evaluationBoard.getBoardstate();
        
        for (auto it = whitePieces.begin(); it != whitePieces.end(); it++) { // evaluate white's pieces for material and slight position
            auto evalPiece = **it;
            
            switch (evalPiece.getType()) {
                case 'K':
                    score += kingValue;
                    
                case 'Q':
                    score += queenValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score += 0.10*(moveCount/MAX_MOVE_NUM_QUEEN); // apply weighting to incentivize getting queen out third and rook 4th
                    break;
                case 'R':
                    score += rookValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score += 0.05*(moveCount/MAX_MOVE_NUM_ROOK);
                    break;
                case 'N':
                    score += knightValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score += 0.25*(moveCount/MAX_MOVE_NUM_KNIGHT);
                    
                    break;
                case 'B':
                    score += bishopValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score += 0.25*(moveCount/MAX_MOVE_NUM_BISHOP);
                    /*if (evalPiece.getRank() == 0) {
                        score -= 0.1;
                    }
                     */
                    break;
                case 'P':
                    score += pawnValue;
                    /*
                    if (boardState[Piece::convertCoordinateToBoardIndex(evalPiece.getRank()-1, evalPiece.getFile()-1)].getType() == 'P') {
                        score += 0.4;
                    }
                    if (boardState[Piece::convertCoordinateToBoardIndex(evalPiece.getRank()-1, evalPiece.getFile()+1)].getType() == 'P') {
                        score += 0.1;
                    }
                     */
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
                    score -= 0.10*((moveCount/MAX_MOVE_NUM_QUEEN));
                    break;
                case 'R':
                    score -= rookValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score -= 0.05*(moveCount/MAX_MOVE_NUM_ROOK);
                    break;
                case 'N':
                    score -= knightValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score -= 0.25*(moveCount/MAX_MOVE_NUM_KNIGHT);
                    break;
                case 'B':
                    score -= bishopValue;
                    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                    score -= 0.25*(moveCount/MAX_MOVE_NUM_BISHOP);
                    break;
                case 'P':
                    score -= pawnValue;
                    /*if (boardState[Piece::convertCoordinateToBoardIndex(evalPiece.getRank()+1, evalPiece.getFile()-1)].getType() == 'P') {
                        score -= 0.1;
                    }
                    if (boardState[Piece::convertCoordinateToBoardIndex(evalPiece.getRank()+1, evalPiece.getFile()+1)].getType() == 'P') {
                        score -= 0.1;
                    }
                     */
                    break;
                default:
                    break;
            }
            
        }
        
        
        
        
        return score;
    }
    
    float Player::computerMaximizer(chess::Board* board, int currentDepth, int stopDepth, vector<string>* pMoves){
        chess::Board simBoard = *board;
        
        //simBoard.calcValidatedPlayerMoveset("White", turnCounter+currentDepth, true); // Old move calculator
        simBoard.calcPlayerMovesetV2("White", turnCounter+currentDepth, true); // NEW MOVE CALCULATOR
        vector<string> moves;
        if (pMoves == NULL) {
            moves = *simBoard.getWhiteMoves();
        }
        else {
            moves = *pMoves;
        }
        
        chess::Board memBoard = simBoard;
        int moveVectorSize = moves.size();
        if (moveVectorSize == 0) {
            return -1000;
        }
        
        float* scores;
        
        scores = new float[moveVectorSize];
        for (int i=0; i<moveVectorSize; i++) {
            scores[i] = 0;    // Initialize all elements to zero.
        }
        
        float singleScore = 0;
        float maxScore;
        
        
        int i = 0;
        auto holderBoard = simBoard;
        float currentBoardVal = 0;
        int pieceNumBlackPrev = 0;
        if (currentDepth == stopDepth-1) {
            //currentBoardVal = computerEvaluateBoard(&simBoard, "White");
            pieceNumBlackPrev = simBoard.getPieces("Black")->size();
        }
        
        int moveCountBeforeLoop = moves.size();
        auto movesMem = moves;
        
        
        
        for (auto it = moves.begin(); it != moves.end(); it++) {
            string move = *it;
            
            if (moves.size() == 0 || move == "") {
                int moveSize = moves.size();
                SDL_Delay(1);
            }
            
            modifyBoardWithMove(&simBoard, move);
            scores[i] = computerEvaluateBoard(&simBoard, "Black"); // avoids additional function call and just evaluates and returns evaluation
            
            
            if (currentDepth >= 2 && scores[i]-currentTurnBoardScore < -0.95) {
                continue;
            }
            
            if (currentDepth == stopDepth-1) {
                
                //auto modifiedBoardCurrentScore = scores[i];
                int pieceNumBlackNew = simBoard.getPieces("Black")->size();
                if (pieceNumBlackPrev-pieceNumBlackNew != 0) { // if there is a piece capture somewhere or the board sees a big swing in score
                    if (!((currentDepth+1) % 2 == 0 && currentDepth > 4)) { // if we're not at an even depth stopping point
                        scores[i] = computerMinimizer(&simBoard, currentDepth+1, stopDepth+1); // increase the depth of search by one
                    }
                }
            }
            /*
            else if (currentDepth == 2){
                singleScore = computerEvaluateBoard(&simBoard, "Black");
                if (singleScore < -1) {
                    return singleScore;
                }
            }
             */
            else {
                scores[i] = computerMinimizer(&simBoard, currentDepth+1, stopDepth);
            }
            
            
                ++i; // increment the scores array index to
                simBoard = holderBoard; // reset the board to before the move was tested
            
        }
        maxScore = *std::max_element(scores, scores+moveVectorSize);
        delete[] scores;
        return maxScore;
        
    }
    
    
    float Player::computerMinimizer(chess::Board* board, int currentDepth, int stopDepth, vector<string>* pMoves){
        chess::Board simBoard = *board;
        vector<string> moves;
        //simBoard.calcValidatedPlayerMoveset("Black", turnCounter+currentDepth, true);  // Old move calculator
        simBoard.calcPlayerMovesetV2("Black", turnCounter+currentDepth, true); // NEW MOVE CALCULATOR
        if (pMoves == NULL) {
            moves = *simBoard.getBlackMoves();
        }
        else {
            moves = *pMoves;
        }
        
        int moveVectorSize = moves.size();
        
        if (moveVectorSize == 0) {
            return 1000;
        }
        
        float* scores = new float[moveVectorSize];
        for (int i=0; i<moveVectorSize; i++) {
            scores[i] = 0;    // Initialize all elements to zero.
        }
        float singleScore = 0;
        float minScore;
        
        
        int i = 0;
        auto holderBoard = simBoard;
        float currentBoardVal = computerEvaluateBoard(&simBoard, "Black");
        int pieceNumWhitePrev = 0;
        if (currentDepth == stopDepth-1) {
            pieceNumWhitePrev = simBoard.getPieces("White")->size();
            //pieceNumBlackPrev = simBoard.getPieces("Black")->size();
            //currentBoardVal = computerEvaluateBoard(&simBoard, "White");
        }
        //kingPiece = *simBoard.findPiece(m_player_color, 'K'); // REMOVE ME AFTER DEBUGGING
        for (auto it = moves.begin(); it != moves.end(); it++) {
            string move = *it;
            ///// saved piece = function (save move and piece captured) goes here /////
            modifyBoardWithMove(&simBoard, move);
            scores[i] = computerEvaluateBoard(&simBoard, "White"); // avoids additional function call and just evaluates and returns evaluation
            
            if (currentDepth >= 2 && scores[i]-currentTurnBoardScore > 0.95) {
                continue;
            }
            
            if (currentDepth == stopDepth-1) {
                
                //auto currentScore = scores[i]; // readout value for current board situation
                int pieceNumWhiteNew = simBoard.getPieces("White")->size();
                if (pieceNumWhitePrev-pieceNumWhiteNew != 0) { // if there is a piece capture somewhere
                    if (!((currentDepth+1) % 2 == 0 && currentDepth > 4)) { // if we're not at an even depth stopping point
                        scores[i] = computerMaximizer(&simBoard, currentDepth+1, stopDepth+1); // increase the depth of search by one
                    }
                    
                }
            }
            /*
             else if (currentDepth == 2){
             singleScore = computerEvaluateBoard(&simBoard, "Black");
             if (singleScore < -1) {
             return singleScore;
             }
             }
             */
            else {
                scores[i] = computerMaximizer(&simBoard, currentDepth+1, stopDepth);
            }
            
            
            ++i; // increment the scores array index to
            
            // undo move
            // undo capture 
            simBoard = holderBoard; // reset the board to before the move was tested

        }
        minScore = *std::min_element(scores, scores+moveVectorSize);
        delete[] scores;
        return minScore;
    }

    
    
}


