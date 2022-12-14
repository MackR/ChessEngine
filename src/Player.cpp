//
//  Player.cpp
//  Chess
//
//  Created by Mack Ragland on 10/27/19.
//  Copyright © 2019 KattisPracticeProblems. All rights reserved.
//

#include "Player.hpp"

Player::Player(CONSTANTS::Color color, const bool isHuman)
{
    m_player_color = color;
    m_isHuman = isHuman;
    m_pCompleteMoveset = nullptr;
}

string Player::askPlayerForMove()
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
        for (auto it = m_pCompleteMoveset->begin(); it != m_pCompleteMoveset->end(); ++it)
        {   
            if(input == "UNDO"){
                return input;
            }
            else if (input == *it)
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

bool Player::achievedCheckmateOnEnemy(TextBoard *pboard)
{ // Just have to check next player moveList size. If their list of possible moves is == 0, they have lost.
    const std::list<std::string> *nextPlayerMoves;

    if (m_player_color == CONSTANTS::Color::WHITE)
    { // if player is white, calculate all possible moves for them

        if (pboard->getLegalMoves(CONSTANTS::Color::BLACK)->empty())
        {
            std::cout << "White wins!" << std::endl;
            return true;
        }
    }
    else if (m_player_color == CONSTANTS::Color::BLACK)
    {
        if (pboard->getLegalMoves(CONSTANTS::Color::WHITE)->empty())
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

bool Player::takeTurn(TextBoard *pboard)
{
    // We already should have moves calculated and prepared in the board member variables
    // ask the player for move and test if the move is valid, if the move isn't valid, discard their choice and ask again

    std::string move;

    cout << "It is " << (int)m_player_color * 255 << "'s turn" << endl; // announce who's turn it is
    if (m_isHuman == true)
    {
        m_pCompleteMoveset = pboard->getLegalMoves(m_player_color);
        move = askPlayerForMove();
    }
    else
    {
        // time check of computer think function
        auto start = std::chrono::system_clock::now();
        move = computeMove(pboard);
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "Elapsed comp think time: " << elapsed_seconds.count() << "s\n";

        if (move == "")
        {
            std::cout << "Error: Computer is picking a blank move" << std::endl; // passed a bad move
        }
    }

    // Modifies the given board with a move, function will do invalid moves, so only give it valid ones
    // time check of computer think function
    auto start = std::chrono::system_clock::now();
    if(move == "UNDO"){
        
        pboard->undoLastMove(true);
    }
    else{
        pboard->makeMove(move);
    }
    
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "Elapsed makeMove()/undoMove() time: " << elapsed_seconds.count() << "s\n";

    if (achievedCheckmateOnEnemy(pboard))
    {
        return true; // Checkmate!
    }
    return false; // Not checkmate
}

string Player::computeMove(TextBoard *pboard)
{

    CONSTANTS::Color enemyColor;
    list<string> simMoveset; // Holds the moveset in memory before modifying board
    map<string, float> moveScores;
    string move;
    auto simulationBoard = *pboard; // make ONE copy of the board to work with when starting thinking

    // Set the variables for each color
    if (m_player_color == CONSTANTS::Color::WHITE)
    {
        enemyColor = CONSTANTS::Color::BLACK;
    }
    else if (m_player_color == CONSTANTS::Color::BLACK)
    {
        enemyColor = CONSTANTS::Color::WHITE;
    }

    // time check of evaluate board function
    auto start = std::chrono::system_clock::now();
    float currentTurnBoardScore = computerEvaluateBoard(pboard, m_player_color); // Eventually we want the computer to prune bad branches
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "elapsed board eval time: " << elapsed_seconds.count() << "s\n";

    // time check of calc moveset function
    //         start = std::chrono::system_clock::now();
    // Thing to be timed here
    //         end = std::chrono::system_clock::now();

    //        elapsed_seconds = end-start;
    //        end_time = std::chrono::system_clock::to_time_t(end);

    //        std::cout << "move calc computation elapsed time" << elapsed_seconds.count() << "s\n";
    //        const double OVERHEADCOMPENSATION = 1.06875;
    //        long double maxMoveTime = 0.00021*pow((simulationBoard.getWhiteMoves()->size()+simulationBoard.getBlackMoves()->size())/2, m_computerStopDepth)*1.5;
    //
    //        cout << "Estimated maximum think time: " << maxMoveTime << " seconds" << endl;;
    //
    //
    //

    simMoveset = *(simulationBoard.getLegalMoves(m_player_color)); // Save moveset before the pointer changes

    for (auto it = simMoveset.begin(); it != simMoveset.end(); ++it)
    {
        move = *it;
        if (move == "")
        {
            cout << "Warning, bad move selected" << endl;
        }

        simulationBoard.makeMove(move);

        start = std::chrono::system_clock::now();
        float moveScore = computerMaxMin(&simulationBoard, 1, m_computerStopDepth, enemyColor); // Need a filtered moveset parameter
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "MaxMin elapsed board eval time: " << elapsed_seconds.count() << "s\n";

        moveScores.insert({move, moveScore}); // Many ways of inserting a pair into map (emplace, make_pair, {key,value})
        simulationBoard.undoLastMove(false);
        // In line above, we are resetting to the previous board state to make another move.
        // This function goes through the entire board and sets every piece, but I can speedup by 64x
        // if I make it so that it only fixes the pieces that were removed before.
        // Redesign the make move function so that it returns what piece type is captured
        // and accounts for promotions
    }

    // Now we go through all the moves and pick the one with the best score (highest # for white)
    if (m_player_color == CONSTANTS::Color::WHITE)
    {
        string largestKey;
        float largestScore = -2000;
        for (std::pair<std::string, float> pair : moveScores)
        {
            // Get the KEY & VALUE
            std::string key = pair.first;
            int score = pair.second;
            if (score >= largestScore)
            {
                largestScore = score;
                largestKey = key;
            }
            if (key == "")
            {
                std::cout << "Received empty move from computeMove()" << std::endl;
            }
        }
        move = largestKey;
    }
    else
    {
        string smallestKey;
        float smallestScore = 2000;
        for (std::pair<std::string, float> pair : moveScores)
        {
            // Get the KEY & VALUE
            std::string key = pair.first;
            int score = pair.second;

            if (key == "")
            {
                std::cout << "Received empty move from computeMove()" << std::endl;
            }
            if (score <= smallestScore)
            {
                smallestScore = score;
                smallestKey = key;
            }
        }
        move = smallestKey;
    }
    if (move == "")
    {
        cout << "Warning, bad move selected" << endl;
    }

    return move;
}

float Player::computerEvaluateBoard(TextBoard *pboard, CONSTANTS::Color playerTurn)
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

    // TextBoard* board = pboard; // gets the address of the actual board member in object;
    TextBoard::board* pboardState = pboard->getBoardState(); // gets the address of the actual board member in object;
    for (int rank = 0; rank < 8; ++rank)
    {
        for (int file = 0; file < 8; ++file)
        {
            CONSTANTS::Piece piece = (*pboardState)[rank][file];
            switch (piece)
            {
            case CONSTANTS::Piece::WKING:
                score += kingValue;
                break;
            case CONSTANTS::Piece::WQUEEN:
                score += queenValue;
                break;
            case CONSTANTS::Piece::WROOK:
                score += rookValue;
                break;
            case CONSTANTS::Piece::WKNIGHT:
                score += knightValue;
                break;
            case CONSTANTS::Piece::WBISHOP:
                score += bishopValue;
                break;
            case CONSTANTS::Piece::WPAWN:
                score += pawnValue;
                break;
            case CONSTANTS::Piece::BKING:
                score -= kingValue;
                break;
            case CONSTANTS::Piece::BQUEEN:
                score -= queenValue;
                break;
            case CONSTANTS::Piece::BROOK:
                score -= rookValue;
                break;
            case CONSTANTS::Piece::BKNIGHT:
                score -= knightValue;
                break;
            case CONSTANTS::Piece::BBISHOP:
                score -= bishopValue;
                break;
            case CONSTANTS::Piece::BPAWN:
                score -= pawnValue;
                break;
            default: break;
            }
        }
    }

    return score;
}

float Player::computerMaxMin(TextBoard *board, int currentDepth, int stopDepth, CONSTANTS::Color color)
{
    CONSTANTS::Color enemyColor;
    int badCheckmateValue;
    std::list<std::string> *pMoves = board->getLegalMoves(color);

    if (color == CONSTANTS::Color::WHITE)
    { // Set parameters for score maximization
        badCheckmateValue = -1000;
        enemyColor = CONSTANTS::Color::BLACK;
    }
    else if (color == CONSTANTS::Color::BLACK)
    { // Set parameters for score minimization
        badCheckmateValue = 1000;
        enemyColor = CONSTANTS::Color::WHITE;
    }

    if (pMoves->empty())
    {
        return badCheckmateValue; // The king was captured for your side and it's checkmate
    }
    const int nMoves = pMoves->size();
    if(nMoves != pMoves->size()){
        std::cout << "moves-> size didn't perform correctly, big error" << std::endl;
    }

    std::vector<float> scores(nMoves); // We need our array to be the same size as the number of moves

    for (int i = 0; i < nMoves; ++i)
    {
        scores[i] = 0; // Initialize all elements to zero in scores
    }

    float singleScore = 0;
    float maxMinScore;

    // int moveCountBeforeLoop = moves->size();
    auto movesMem = *pMoves; // remember the possible moves before modifying the board
    auto memBoard = *board;
    int i = 0;

    for (auto it = movesMem.begin(); it != movesMem.end(); ++it) // start modifying the board
    {
        string move = *it;

        if (move == "")
        {
            int moveSize = pMoves->size();
            std::cout << "Move in moveset is empty" << std::endl;
        }

        board->makeMove(move);

        // if (currentDepth >= 2 && scores[i]-currentTurnBoardScore > 0.95) {
        //     continue;
        // }

        if (currentDepth == stopDepth - 1) // If we are one layer from finished, evaluate the board and return
        {
            int holderScore = computerEvaluateBoard(board, enemyColor);
            scores[i] = holderScore;
        }
        else
        {
            scores[i] = computerMaxMin(board, currentDepth + 1, stopDepth, enemyColor);
        }

        ++i;                   // increment the scores array index to
        board->undoLastMove(false); // reset the board to before the move was tested and prepare for new one
    }

    if (color == CONSTANTS::Color::WHITE)
    {
        maxMinScore = *std::max_element(scores.begin(), scores.end()); // Get the minimum score from the list
    }
    else if (color == CONSTANTS::Color::BLACK)
    {
        maxMinScore = *std::min_element(scores.begin(), scores.end()); // Get the minimum score from the list
    }
    return maxMinScore; // return the minimum score
}
// ARCHIVED
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
//     for (auto it = moves.begin(); it != moves.end(); ++it)
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
