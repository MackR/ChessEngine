//
//  Player.cpp
//  Chess
//
//  Created by Mack Ragland on 10/27/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include "Player.hpp"

Player::Player(CONSTANTS::Color color)
{
    m_player_color = color;
    m_pCompleteMoveset = nullptr;
}

string Player::askPlayerForValidMove()
{
    std::cout << "Moves: ";
    int counter = 0;
    for (auto it = m_pCompleteMoveset->begin(); it != m_pCompleteMoveset->end(); ++it)
    {
        std::cout << *it << " ";
        if (counter < 15)
        {
            ++counter;
        }
        else
        {
            std::cout << endl;
            counter = 0;
        }
    }
    string input;
    std::cout << "Please select a move: ";
    std::cin >> input;
    std::cout << endl
              << endl;
    while (1)
    {

        for (auto it = m_pCompleteMoveset->begin(); it != m_pCompleteMoveset->end(); it++)
        {
            if (input == *it)
            {
                // std::cout << "Valid move" << endl;
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

bool Player::achievedCheckmateOnEnemy(TextBoard *board)
{ // Just have to check next player moveList size. If their list of possible moves is == 0, they have lost.
    const std::list<std::string> *nextPlayerMoves;

    if (m_player_color == CONSTANTS::Color::WHITE)
    { // if player is white, calculate all possible moves for them
        nextPlayerMoves = board->getLegalMoves(CONSTANTS::Color::BLACK);
        if (nextPlayerMoves->empty())
        {
            std::cout << "White wins!" << std::endl;
            return true;
        }
    }
    else if (m_player_color == CONSTANTS::Color::BLACK)
    {
        nextPlayerMoves = board->getLegalMoves(CONSTANTS::Color::WHITE);
        if (nextPlayerMoves->empty())
        {
            std::cout << "Black wins!" << std::endl;
            return true;
        }
    }
    else
    {
        std::cout << "ERROR: Player color invalid in achievedCheckmate()" << std::endl;
        return false;
    }

    return false;
}

bool Player::takeTurn(TextBoard *board)
{
    // We already should have moves calculated and prepared in the board member variables
    // ask the player for move and test if the move is valid, if the move isn't valid, discard their choice and ask again

    std::string move;
    cout << "It is " << (int)m_player_color * 255 << "'s turn" << endl; // announce who's turn it is
    if (humanPlayer == true)
    {

        if (m_player_color == CONSTANTS::Color::WHITE)
        {
            m_pCompleteMoveset = board->getLegalMoves(CONSTANTS::Color::WHITE);
        }
        else if (m_player_color == CONSTANTS::Color::BLACK)
        {
            m_pCompleteMoveset = board->getLegalMoves(CONSTANTS::Color::BLACK);
        }
        else
        {
            cout << "Player doesn't have a valid color" << endl;
        }
        move = askPlayerForValidMove();
    }
    else
    {
        // time check of evaluate board function // Commented out because we don't need it yet
        auto start = std::chrono::system_clock::now();
        move = computerBeginThinking(board);
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        std::cout << "elapsed comp think time: " << elapsed_seconds.count() << "s\n";

        if (move == "")
        {
            std::cout << "Error: Computer function is passing a blank move" << std::endl; // passed a bad move
        }
    }

    // Modifies the given board with a move, function will do invalid moves, so only give it valid ones
    board->makeMove(move);

    if (achievedCheckmateOnEnemy(board))
    {
        return true; // Checkmate!
    }
    return false; // Not checkmate
}

// bool Player::OLDachievedCheckmateOnEnemy(TextBoard* board){  Might not need this function. Just have to check next player moveList size. If their list of possible moves is == 0, they have lost.
//
//         auto checkmateBoard = *board;  make a copy of current board
//         CONSTANTS::Pieces enemyKing;
//
//         if (m_player_color == CONSTANTS::WHITE) {  if player is white, calculate all possible moves for them
//             checkmateBoard.calcPlayerMovesetV2("White", true);
//             m_completeMoveset = *checkmateBoard.getWhiteMoves();
//             enemyKing = *checkmateBoard.findKing("Black");
//         }
//         if (m_player_color == CONSTANTS::BLACK) {
//             checkmateBoard.calcPlayerMovesetV2("Black", true);
//             m_completeMoveset = *checkmateBoard.getBlackMoves();
//             enemyKing = *checkmateBoard.findKing("White");
//
//         }
//
//         int enemyKingAttackers = enemyKing.countNumAttackers(m_completeMoveset, enemyKing.getFile(), enemyKing.getRank());  count number of attackers on king
//
//         if (enemyKingAttackers > 0) {  if enemy king is in check
//
//             Piece* boardState = checkmateBoard.getBoardstate();  copy checkmate board to begin testing moves
//             Piece* originalBoardState = boardState;
//             vector<string> enemyMoveset;
//
//             if (m_player_color == "White") {
//                 checkmateBoard.calcPlayerMovesetV2("Black",turnCounter, true);
//                 enemyMoveset = *checkmateBoard.getBlackMoves();  get all enemy moves
//             }
//             if (m_player_color == "Black") {
//                 checkmateBoard.calcPlayerMovesetV2("White",turnCounter+1, true);
//                 enemyMoveset = *checkmateBoard.getWhiteMoves();  get all enemy moves
//
//             }
//
//             int prevFile = 0;
//             int prevRank = 0;
//             int newFile = 0;
//             int newRank = 0;
//             int prevIndex = 0;
//             int newIndex = 0;
//
//             for (auto it = enemyMoveset.begin(); it != enemyMoveset.end(); it++) {  test enemy moves
//                 string move = *it;
//                 prevFile = Piece::cFileToIndex(move[1]);
//                 prevRank = move[2] - '0'-1;
//                 newFile = Piece::cFileToIndex(move[3]);
//                 newRank = move[4] -'0'-1;     Convert string to int
//
//                  makes the move the player chose on this hypothetical board
//                 prevIndex = Piece::convertCoordinateToBoardIndex(prevFile, prevRank);
//                 newIndex = Piece::convertCoordinateToBoardIndex(newFile, newRank);
//
//                 boardState[newIndex].setColor(boardState[prevIndex].getColor());
//                 boardState[newIndex].setType(boardState[prevIndex].getType());
//                 boardState[prevIndex].setColor("Empty");
//                 boardState[prevIndex].setType('E');
//
//                  perform en passant if the conditions apply
//                 if (boardState[newIndex].getColor() == "Empty" && boardState[prevIndex].getType() == 'P' && abs(newFile-prevFile) == 1) {
//                     int enPassantedPieceIndex = Piece::convertCoordinateToBoardIndex(newFile, prevRank);
//                     boardState[enPassantedPieceIndex].setColor("Empty");
//                     boardState[enPassantedPieceIndex].setType('E');
//                 }
//
//                 checkmateBoard.findPlayerPieces();  update the pieces list after capturing one
//
//                 if (m_player_color == "White") {
//                     checkmateBoard.calcValidatedPlayerMoveset("Black",turnCounter);
//                     enemyMoveset = *checkmateBoard.getBlackMoves();   enemyMoveset was already calculated above
//                     cout << "Enemy move is: " << move << endl;
//                     enemyKing = *checkmateBoard.findKing("Black");
//                     checkmateBoard.calcPlayerMovesetV2("White", turnCounter+1, true);
//                     m_completeMoveset = *checkmateBoard.getWhiteMoves();
//                 }
//                 if (m_player_color == "Black") {
//                     checkmateBoard.calcValidatedPlayerMoveset("White",turnCounter+1);
//                     enemyMoveset = *checkmateBoard.getWhiteMoves();   enemyMoveset was already calculated above
//                     enemyKing = *checkmateBoard.findKing("White");
//                     checkmateBoard.calcPlayerMovesetV2("Black", turnCounter+1, true);
//                     m_completeMoveset = *checkmateBoard.getBlackMoves();
//
//                 }
//
//
//
//                 enemyKingAttackers = enemyKing.countNumAttackers(m_completeMoveset, enemyKing.getFile(), enemyKing.getRank());
//                 if (enemyKingAttackers == 0) {
//                     cout << move << " saves the day " << endl;
//                     incrementTurn();  Increase turn counter
//                     cout << "Check!" << endl;
//                     return false;
//                 }
//                 *boardState = *originalBoardState;
//             }
//             return true;
//
//         }
//
//
//         return false;
//
//     }

string Player::computerBeginThinking(TextBoard *board)
{

    // setup get the board and copy it to a simulation board, and calculate possible moves for the position
    static int maxStopDepth = 3;
    currentTurnBoardScore = computerEvaluateBoard(board, m_player_color); // Eventually we want the computer to prune bad branches

    // time check of evaluate board function
    auto start = std::chrono::system_clock::now();
    computerEvaluateBoard(board, m_player_color);
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "elapsed board eval time: " << elapsed_seconds.count() << "s\n";

    list<string> simMoveset;
    list<string> simEnemyMoveset;

    // time check of calc moveset function
    //         start = std::chrono::system_clock::now();
    // Thing to be timed here
    //         end = std::chrono::system_clock::now();

    //        elapsed_seconds = end-start;
    //        end_time = std::chrono::system_clock::to_time_t(end);

    //        std::cout << "move calc computation elapsed time" << elapsed_seconds.count() << "s\n";
    //        const double OVERHEADCOMPENSATION = 1.06875;
    //        long double maxMoveTime = 0.00021*pow((simulationBoard.getWhiteMoves()->size()+simulationBoard.getBlackMoves()->size())/2, maxStopDepth)*1.5;
    //
    //        cout << "Max possible move time calculation: " << maxMoveTime << " seconds" << endl;;
    //
    //
    //
    map<string, float> moveScores;
    string move;
    auto simulationBoard = *board; // make ONE copy of the board to work with when starting thinking

    if (m_player_color == CONSTANTS::Color::WHITE)
    {
        simMoveset = *simulationBoard.getLegalMoves(CONSTANTS::Color::WHITE); // Points to location of moveset in board object

        for (auto it = simMoveset.begin(); it != simMoveset.end(); it++)
        {
            string move = *it;
            if (move == "")
            {
                cout << "Warning, bad move selected" << endl;
            }

            // time check of modify board function
            //                start = std::chrono::system_clock::now();
            //                end = std::chrono::system_clock::now();
            //
            //                elapsed_seconds = end-start;
            //                end_time = std::chrono::system_clock::to_time_t(end);

            // std::cout << "finished modify board with move calc computation at " << std::ctime(&end_time)
            //<< "elapsed time: " << elapsed_seconds.count() << "s\n";

            // do a breadth calculation of depth 2 and find all the moves that suck and remove them from here, then feed those moves into next layer

            simulationBoard.makeMove(move);
            float moveScore = computerMaxMin(&simulationBoard, 1, maxStopDepth,CONSTANTS::Color::BLACK); // Need a filtered moveset parameter // it's returning the overall value of the board some moves into the future
            moveScores.insert({move, moveScore});                                   // Many ways of inserting a pair into map (emplace, make_pair, {key,value})
            simulationBoard.undoLastMove();
            // In line above, we are resetting to the previous board state to make another move.
            // This function goes through the entire board and sets every piece, but I can speedup by 64x
            // if I make it so that it only fixes the pieces that were removed before.
            // Redesign the make move function so that it returns what piece type is captured
            // and accounts for promotions
        }

        // Now we go through all the moves and pick the one with the best score (highest # for white)
        string largestKey;
        float largestScore = -2000;
        for (std::pair<std::string, float> element : moveScores)
        {
            // Accessing KEY from element
            std::string key = element.first;
            // Accessing VALUE from element.
            int score = element.second;

            if (score >= largestScore)
            {
                largestScore = score;
                largestKey = key;
            }
            if (key == "")
            {
                std::cout << "Returning bad (empty) move for comp think" << std::endl;
            }
        }
        move = largestKey;
    }
    else
    {
        simMoveset = *simulationBoard.getLegalMoves(CONSTANTS::Color::BLACK);
        int counter = 0;
        for (std::list<std::string>::iterator it = simMoveset.begin(); it != simMoveset.end(); ++it)
        {
            string move = *it;
            simulationBoard.makeMove(move);
            float moveScore = computerMaxMin(&simulationBoard, 1, maxStopDepth,CONSTANTS::Color::WHITE); // Need a filtered moveset parameter // it's returning the overall value of the board some moves into the future
            moveScores.insert({move, moveScore});                                   // Many ways of inserting a pair into map (emplace, make_pair, {key,value})
            simulationBoard.undoLastMove();
            ++counter;
        }

        string smallestKey;
        float smallestScore = 2000;
        for (std::pair<std::string, float> element : moveScores)
        {
            // Accessing KEY from element
            std::string key = element.first;
            // Accessing VALUE from element.
            int score = element.second;

            if (score <= smallestScore)
            {
                smallestScore = score;
                smallestKey = key;
            }
            if (key == "")
            {
                std::cout << "Returning bad (empty) move for comp think" << std::endl;
            }
        }
        move = smallestKey;
    }

    if (move == "")
    {
        cout << "Warning, bad move selected" << endl;
    }

    // need to grab the largest value out of the map to finish this

    return move;
}

float Player::computerEvaluateBoard(TextBoard *board, CONSTANTS::Color playerTurn)
{ // currently only evaluates the board on material advantage

    float pawnValue = 1;
    float knightValue = 3;
    float bishopValue = 3.25;
    float rookValue = 5;
    float queenValue = 9;
    float kingValue = 100;

    float moveCount = 0;

    const int MAX_MOVE_NUM_BISHOP = 13; // maximum number moves a bishop can have at any point in game
    const int MAX_MOVE_NUM_KNIGHT = 8;
    const int MAX_MOVE_NUM_QUEEN = 27;
    const int MAX_MOVE_NUM_ROOK = 14;

    float score = 0;

    TextBoard::board* boardState = board->getBoardState(); // gets the address of the actual board member in object;
    for (int rank = 0; rank < 8; ++rank)
    {
        for (int file = 0; file < 8; ++file)
        {
            CONSTANTS::Piece piece = *boardState[rank][file];
            switch (piece)
            {
            case CONSTANTS::Piece::WKING:
                score += kingValue;

            case CONSTANTS::Piece::WQUEEN:
                score += queenValue;
                //    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                //    score += 0.10*(moveCount/MAX_MOVE_NUM_QUEEN); // apply weighting to incentivize getting queen out third and rook 4th
                break;
            case CONSTANTS::Piece::WROOK:
                score += rookValue;
                //    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                //    score += 0.05*(moveCount/MAX_MOVE_NUM_ROOK);
                break;
            case CONSTANTS::Piece::WKNIGHT:
                score += knightValue;
                //    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                //    score += 0.25*(moveCount/MAX_MOVE_NUM_KNIGHT);

                break;
            case CONSTANTS::Piece::WBISHOP:
                score += bishopValue;
                //    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                //    score += 0.25*(moveCount/MAX_MOVE_NUM_BISHOP);
                /*if (evalPiece.getRank() == 0) {
                    score -= 0.1;
                }
                 */
                break;
            case CONSTANTS::Piece::WPAWN:
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

            case CONSTANTS::Piece::BKING:
                score -= kingValue;
                break;

            case CONSTANTS::Piece::BQUEEN:
                score -= queenValue;
                //    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                //    score -= 0.10*((moveCount/MAX_MOVE_NUM_QUEEN));
                break;
            case CONSTANTS::Piece::BROOK:
                score -= rookValue;
                //    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                //    score -= 0.05*(moveCount/MAX_MOVE_NUM_ROOK);
                break;
            case CONSTANTS::Piece::BKNIGHT:
                score -= knightValue;
                //    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                //    score -= 0.25*(moveCount/MAX_MOVE_NUM_KNIGHT);
                break;
            case CONSTANTS::Piece::BBISHOP:
                score -= bishopValue;
                //    moveCount = evalPiece.getMoves(boardState, turnCounter)->size();
                //    score -= 0.25*(moveCount/MAX_MOVE_NUM_BISHOP);
                break;
            case CONSTANTS::Piece::BPAWN:
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
    }

    return score;
}


float Player::computerMaxMin(TextBoard *board, int currentDepth, int stopDepth, CONSTANTS::Color color)
{   
    CONSTANTS::Color enemyColor;
    int badCheckmateValue;
    std::list<std::string> *moves = board->getLegalMoves(color);

    if(color == CONSTANTS::Color::WHITE){ // Set parameters for score maximization
        badCheckmateValue = -1000;
        enemyColor = CONSTANTS::Color::BLACK;
    }
    else if(color == CONSTANTS::Color::BLACK){ // Set parameters for score minimization
        badCheckmateValue = 1000;
        enemyColor = CONSTANTS::Color::WHITE;
    }

    const int nMoves = moves->size();
    if (nMoves == 0)
    {
        return badCheckmateValue; // The king was captured for your side and it's checkmate
    }

    std::vector<float> scores(nMoves); // We need our array to be the same size as the number of moves
    

    for (int i = 0; i < nMoves; ++i)
    {
        scores[i] = 0; // Initialize all elements to zero in scores
    }

    float singleScore = 0;
    float maxMinScore;

    // int moveCountBeforeLoop = moves->size();
    auto movesMem = *moves; // remember the possible moves before modifying the board
    auto memBoard = *board;
    int i = 0;

    for (auto it = movesMem.begin(); it != movesMem.end(); ++it) // start modifying the board
    {
        string move = *it;

        if (move == "")
        {
            int moveSize = moves->size();
            std::cout << "Move in moveset is empty" << std::endl;
        }

        board->makeMove(move);

        
        // if (currentDepth >= 2 && scores[i]-currentTurnBoardScore > 0.95) {
        //     continue;
        // }
        
        if (currentDepth == stopDepth - 1) // If we are one layer from finished, evaluate the board and return
        {
            computerEvaluateBoard(board, enemyColor);
            scores[i] = computerEvaluateBoard(board, enemyColor);
        }
        else
        {
            scores[i] = computerMaxMin(board, currentDepth + 1, stopDepth, enemyColor);
        }

        ++i;                    // increment the scores array index to
        board->undoLastMove(); // reset the board to before the move was tested and prepare for new one
    }

    if (color == CONSTANTS::Color::WHITE){
        maxMinScore = *std::max_element(scores.begin(),scores.end()); // Get the minimum score from the list
    }
    else if(color == CONSTANTS::Color::BLACK){
        maxMinScore = *std::min_element(scores.begin(),scores.end()); // Get the minimum score from the list
    }
    return maxMinScore; // return the minimum score

}
// float Player::computerMaximizer(TextBoard *board, int currentDepth, int stopDepth)
// {
//         std::list<std::string> *moves = board->getLegalMoves(CONSTANTS::Color::WHITE);

//     const int nMoves = moves->size();
//     if (nMoves == 0)
//     {
//         return -1000; // The king was captured for your side and it's checkmate
//     }

//     std::vector<float> scores(nMoves); // We need our array to be the same size as the number of moves
    

//     for (int i = 0; i < nMoves; ++i)
//     {
//         scores[i] = 0; // Initialize all elements to zero in scores
//     }

//     float singleScore = 0;
//     float maxScore;

//     float currentBoardVal = 0;
//     // int pieceNumBlackPrev = 0;
//     // if (currentDepth == stopDepth - 1)
//     // {
//     //     // currentBoardVal = computerEvaluateBoard(&simBoard, "White");
//     //     pieceNumBlackPrev = simBoard->getPieceLocations("Black")->size();
//     // }

//     int moveCountBeforeLoop = moves->size();
//     auto movesMem = *moves; // remember the possible moves before modifying the board
//     int i = 0;

//     for (auto it = moves->begin(); it != moves->end(); ++it) // start modifying the board
//     {
//         string move = *it;

//         if (move == "")
//         {
//             int moveSize = moves->size();
//             std::cout << "Move in moveset is empty" << std::endl;
//         }

//         board->makeMove(move);

//         /*
//         if (currentDepth >= 2 && scores[i]-currentTurnBoardScore < -0.95) {
//             continue;
//         }
//         */
//         if (currentDepth == stopDepth - 1) // If we are one layer from finished, evaluate the board and return
//         {
//             scores[i] = computerEvaluateBoard(board, CONSTANTS::Color::BLACK);
//             // auto modifiedBoardCurrentScore = scores[i];
//             /*
//             int pieceNumBlackNew = simBoard.getPieces("Black")->size();
//             if (pieceNumBlackPrev-pieceNumBlackNew != 0) { // if there is a piece capture somewhere or the board sees a big swing in score
//                 if (!((currentDepth+1) % 2 == 0 && currentDepth > 4)) { // if we're not at an even depth stopping point
//                     scores[i] = computerMinimizer(&simBoard, currentDepth+1, stopDepth+1); // increase the depth of search by one
//                 }
//             }
//              */
//         }
//         /*
//         else if (currentDepth == 2){
//             singleScore = computerEvaluateBoard(&simBoard, "Black");
//             if (singleScore < -1) {
//                 return singleScore;
//             }
//         }
//          */
//         else
//         {
//             scores[i] = computerMinimizer(board, currentDepth + 1, stopDepth);
//         }

//         ++i;                    // increment the scores array index to
//         board->undoLastMove(); // reset the board to before the move was tested
//     }
//     maxScore = std::max_element(scores.begin(),scores.end());
//     return maxScore;
// }

// float Player::computerMinimizer(TextBoard *board, int currentDepth, int stopDepth)
// {
//     chess::Board simBoard = *board;
//     vector<string> moves;
//     // simBoard.calcValidatedPlayerMoveset("Black", turnCounter+currentDepth, true);  // Old move calculator
//     simBoard.calcPlayerMovesetV2("Black", turnCounter + currentDepth, true); // NEW MOVE CALCULATOR
//     if (pMoves == NULL)
//     {
//         moves = *simBoard.getBlackMoves();
//     }
//     else
//     {
//         moves = *pMoves;
//     }

//     int moveVectorSize = moves.size();

//     if (moveVectorSize == 0)
//     {
//         return 1000;
//     }

//     float *scores = new float[moveVectorSize];
//     for (int i = 0; i < moveVectorSize; i++)
//     {
//         scores[i] = 0; // Initialize all elements to zero.
//     }
//     float singleScore = 0;
//     float minScore;

//     int i = 0;
//     auto holderBoard = simBoard;
//     float currentBoardVal = computerEvaluateBoard(&simBoard, "Black");
//     int pieceNumWhitePrev = 0;
//     if (currentDepth == stopDepth - 1)
//     {
//         pieceNumWhitePrev = simBoard.getPieces("White")->size();
//         // pieceNumBlackPrev = simBoard.getPieces("Black")->size();
//         // currentBoardVal = computerEvaluateBoard(&simBoard, "White");
//     }
//     // kingPiece = *simBoard.findPiece(m_player_color, 'K'); // REMOVE ME AFTER DEBUGGING
//     for (auto it = moves.begin(); it != moves.end(); it++)
//     {
//         string move = *it;
//         ///// saved piece = function (save move and piece captured) goes here /////
//         modifyBoardWithMove(&simBoard, move);

//         /*
//         if (currentDepth >= 2 && scores[i]-currentTurnBoardScore > 0.95) {
//             continue;
//         }
//          */

//         if (currentDepth == stopDepth - 1)
//         {

//             scores[i] = computerEvaluateBoard(&simBoard, "Black");

//             /*
//             //auto currentScore = scores[i]; // readout value for current board situation
//             int pieceNumWhiteNew = simBoard.getPieces("White")->size();
//             if (pieceNumWhitePrev-pieceNumWhiteNew != 0) { // if there is a piece capture somewhere
//                 if (!((currentDepth+1) % 2 == 0 && currentDepth > 4)) { // if we're not at an even depth stopping point
//                     scores[i] = computerMaximizer(&simBoard, currentDepth+1, stopDepth+1); // increase the depth of search by one
//                 }

//             }
//              */
//         }
//         /*
//          else if (currentDepth == 2){
//          singleScore = computerEvaluateBoard(&simBoard, "Black");
//          if (singleScore < -1) {
//          return singleScore;
//          }
//          }
//          */
//         else
//         {
//             scores[i] = computerMaximizer(&simBoard, currentDepth + 1, stopDepth);
//         }

//         ++i; // increment the scores array index to

//         // undo move
//         // undo capture
//         simBoard = holderBoard; // reset the board to before the move was tested
//     }
//     minScore = *std::min_element(scores, scores + moveVectorSize);
//     delete[] scores;
//     return minScore;
// }
