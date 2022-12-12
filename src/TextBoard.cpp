//
//  TextBoard.cpp
//  Chess
//
//  Created by Mack Ragland on 10/12/22.
//  Copyright © 2022 MackRagland. All rights reserved.
//

#include "TextBoard.hpp"

TextBoard::TextBoard()
{
    // Init the board itself
    m_board[0][CONSTANTS::A1] = CONSTANTS::Piece::WROOK;
    m_board[0][CONSTANTS::B1] = CONSTANTS::Piece::WKNIGHT;
    m_board[0][CONSTANTS::C1] = CONSTANTS::Piece::WBISHOP;
    m_board[0][CONSTANTS::D1] = CONSTANTS::Piece::WKING;
    m_board[0][CONSTANTS::E1] = CONSTANTS::Piece::WQUEEN;
    m_board[0][CONSTANTS::F1] = CONSTANTS::Piece::WBISHOP;
    m_board[0][CONSTANTS::G1] = CONSTANTS::Piece::WKNIGHT;
    m_board[0][CONSTANTS::H1] = CONSTANTS::Piece::WROOK;

    for (int i = 8; i < 16; ++i)
    {
        m_board[0][i] = CONSTANTS::Piece::WPAWN;
    }
    for (int i = 16; i < 48; ++i)
    {
        m_board[0][i] = CONSTANTS::Piece::EMPTY;
    }
    for (int i = 48; i < 56; ++i)
    {
        m_board[0][i] = CONSTANTS::Piece::BPAWN;
    }
    m_board[0][CONSTANTS::A8] = CONSTANTS::Piece::BROOK;
    m_board[0][CONSTANTS::B8] = CONSTANTS::Piece::BKNIGHT;
    m_board[0][CONSTANTS::C8] = CONSTANTS::Piece::BBISHOP;
    m_board[0][CONSTANTS::D8] = CONSTANTS::Piece::BKING;
    m_board[0][CONSTANTS::E8] = CONSTANTS::Piece::BQUEEN;
    m_board[0][CONSTANTS::F8] = CONSTANTS::Piece::BBISHOP;
    m_board[0][CONSTANTS::G8] = CONSTANTS::Piece::BKNIGHT;
    m_board[0][CONSTANTS::H8] = CONSTANTS::Piece::BROOK;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // // Custom Board setup
    // //Rank 8
    // m_board[0][CONSTANTS::A8] = CONSTANTS::Piece::BROOK;
    // m_board[0][CONSTANTS::B8] = CONSTANTS::Piece::BKNIGHT;
    // m_board[0][CONSTANTS::C8] = CONSTANTS::Piece::BBISHOP;
    // m_board[0][CONSTANTS::D8] = CONSTANTS::Piece::BKING;
    // m_board[0][CONSTANTS::E8] = CONSTANTS::Piece::BQUEEN;
    // m_board[0][CONSTANTS::F8] = CONSTANTS::Piece::BBISHOP;
    // m_board[0][CONSTANTS::G8] = CONSTANTS::Piece::BKNIGHT;
    // m_board[0][CONSTANTS::H8] = CONSTANTS::Piece::BROOK;
    // // Rank 7
    // m_board[0][CONSTANTS::A7] = CONSTANTS::Piece::BPAWN;
    // m_board[0][CONSTANTS::B7] = CONSTANTS::Piece::BPAWN;
    // m_board[0][CONSTANTS::C7] = CONSTANTS::Piece::BPAWN;
    // m_board[0][CONSTANTS::D7] = CONSTANTS::Piece::BPAWN;
    // m_board[0][CONSTANTS::E7] = CONSTANTS::Piece::BPAWN;
    // m_board[0][CONSTANTS::F7] = CONSTANTS::Piece::BPAWN;
    // m_board[0][CONSTANTS::G7] = CONSTANTS::Piece::BPAWN;
    // m_board[0][CONSTANTS::H7] = CONSTANTS::Piece::BPAWN;

    // for(int rank = 2; rank < 6; ++rank){
    //     for (int file = 0; file < 8; ++file){
    //         m_board[rank][file] = CONSTANTS::Piece::EMPTY;
    //     }
    // }

    // // Rank 2
    // m_board[0][CONSTANTS::A2] = CONSTANTS::Piece::WPAWN;
    // m_board[0][CONSTANTS::B2] = CONSTANTS::Piece::WPAWN;
    // m_board[0][CONSTANTS::C2] = CONSTANTS::Piece::WPAWN;
    // m_board[0][CONSTANTS::D2] = CONSTANTS::Piece::WPAWN;
    // m_board[0][CONSTANTS::E2] = CONSTANTS::Piece::WPAWN;
    // m_board[0][CONSTANTS::F2] = CONSTANTS::Piece::WPAWN;
    // m_board[0][CONSTANTS::G2] = CONSTANTS::Piece::WPAWN;
    // m_board[0][CONSTANTS::H2] = CONSTANTS::Piece::WPAWN;

    // // Rank 1
    // m_board[0][CONSTANTS::A1] = CONSTANTS::Piece::WROOK;
    // m_board[0][CONSTANTS::B1] = CONSTANTS::Piece::WKNIGHT;
    // m_board[0][CONSTANTS::C1] = CONSTANTS::Piece::WBISHOP;
    // m_board[0][CONSTANTS::D1] = CONSTANTS::Piece::WKING;
    // m_board[0][CONSTANTS::E1] = CONSTANTS::Piece::WQUEEN;
    // m_board[0][CONSTANTS::F1] = CONSTANTS::Piece::WBISHOP;
    // m_board[0][CONSTANTS::G1] = CONSTANTS::Piece::WKNIGHT;
    // m_board[0][CONSTANTS::H1] = CONSTANTS::Piece::WROOK;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Create the first board state in the m_boardHistory stack
    std::vector<CONSTANTS::Piece> v_board(64);
    for (int idx = 0; idx < 64; ++idx){
        v_board[idx] = m_board[0][idx];
    }
    m_boardHistory.push(v_board);

    // Init the supporting variables
    m_playerTurn = CONSTANTS::Color::WHITE;
    // m_turnNum = 1;
    m_blackKingMoved = false;
    m_whiteKingMoved = false;
    m_whiteARookMoved = false;
    m_whiteHRookMoved = false;
    m_blackARookMoved = false;
    m_blackHRookMoved = false;

    findPlayerPieces(false);
    calcPlayerMovesetV2(CONSTANTS::Color::WHITE, true); // At the start of every game, calculate all white's moves.
}

void TextBoard::findPlayerPieces(bool update)
{   // Function should only be used once on init, then other functions should keep piece lists updated
    // This commented out section below is faster, but it is only called once, so whats the point
    // if (regularSetup){
    // // This is the special case of a normal board setup
    // for (int i = 0; i < 16; ++i){ // Populating the list with all initial indexes of pieces it owns
    //     m_whitePieceIndices.push_back(i);
    //     m_blackPieceIndices.push_back(i+48);
    // }
    // }
    // else{
        if(update){
           m_whitePieceIndices.clear();
           m_blackPieceIndices.clear(); 
        }

    for (int index = 0; index < 64; index++)
    {
        CONSTANTS::Piece piece = m_board[0][index];
        if (isWhite(piece))
        {
            m_whitePieceIndices.push_back(index);
        }
        else if (isBlack(piece))
        {
            m_blackPieceIndices.push_back(index);
        }
    }
    // m_whiteIndexHistory.push(m_whitePieceIndices);
    // m_blackIndexHistory.push(m_blackPieceIndices);
    // }

    return;
}

TextBoard::~TextBoard() {}

inline bool TextBoard::isWhite(CONSTANTS::Piece piece) { return (piece >= CONSTANTS::Piece::WPAWN && piece <= CONSTANTS::Piece::WKING); }
inline bool TextBoard::isBlack(CONSTANTS::Piece piece) { return (piece >= CONSTANTS::Piece::BPAWN && piece <= CONSTANTS::Piece::BKING); }
inline bool TextBoard::isEmpty(CONSTANTS::Piece piece) { return (piece == CONSTANTS::Piece::EMPTY); }

char TextBoard::getPieceType(int nFile, int rank)
{
    if (nFile < 0 || nFile > 7 || rank < 0 || rank > 7)
    {
        std::cout << "Error, invalid rank or file input" << std::endl;
        return 'E';
    }

    CONSTANTS::Piece piece = m_board[rank][nFile];
    switch (piece)
    {
    case CONSTANTS::Piece::WPAWN:
        return 'P';
        break;
    case CONSTANTS::Piece::BPAWN:
        return 'P';
        break;
    case CONSTANTS::Piece::WROOK:
        return 'R';
        break;
    case CONSTANTS::Piece::BROOK:
        return 'R';
        break;
    case CONSTANTS::Piece::WKNIGHT:
        return 'N';
        break;
    case CONSTANTS::Piece::BKNIGHT:
        return 'N';
        break;
    case CONSTANTS::Piece::WBISHOP:
        return 'B';
        break;
    case CONSTANTS::Piece::BBISHOP:
        return 'B';
        break;
    case CONSTANTS::Piece::WQUEEN:
        return 'Q';
        break;
    case CONSTANTS::Piece::BQUEEN:
        return 'Q';
        break;
    case CONSTANTS::Piece::WKING:
        return 'K';
        break;
    case CONSTANTS::Piece::BKING:
        return 'K';
        break;
    case CONSTANTS::Piece::EMPTY: // std::cout << "Error, passed empty square to getPieceType()" << std::endl;
        return 'E';
        break;
    default:
        return 'E';
        break;
    }
    return 'X';
}

CONSTANTS::Color TextBoard::getPieceColor(int file, int rank)
{
    if (file < 0 || file > 7 || rank < 0 || rank > 7)
    {
        std::cout << "Error, invalid rank or file input" << std::endl;
        return CONSTANTS::Color::INVALID;
    }

    CONSTANTS::Piece piece = m_board[rank][file];
    if (isEmpty(piece))
        return CONSTANTS::Color::EMPTY;
    else if (isWhite(piece))
        return CONSTANTS::Color::WHITE;
    else
        return CONSTANTS::Color::BLACK;
}
char TextBoard::getPieceType(int index)
{
    if (index < 0 || index > 63)
    {
        std::cout << "Error, invalid rank or file input" << std::endl;
        return 'E';
    }

    CONSTANTS::Piece piece = m_board[0][index];
    switch (piece)
    {
    case CONSTANTS::Piece::WPAWN:
        return 'P';
        break;
    case CONSTANTS::Piece::BPAWN:
        return 'P';
        break;
    case CONSTANTS::Piece::WROOK:
        return 'R';
        break;
    case CONSTANTS::Piece::BROOK:
        return 'R';
        break;
    case CONSTANTS::Piece::WKNIGHT:
        return 'N';
        break;
    case CONSTANTS::Piece::BKNIGHT:
        return 'N';
        break;
    case CONSTANTS::Piece::WBISHOP:
        return 'B';
        break;
    case CONSTANTS::Piece::BBISHOP:
        return 'B';
        break;
    case CONSTANTS::Piece::WQUEEN:
        return 'Q';
        break;
    case CONSTANTS::Piece::BQUEEN:
        return 'Q';
        break;
    case CONSTANTS::Piece::WKING:
        return 'K';
        break;
    case CONSTANTS::Piece::BKING:
        return 'K';
        break;
    case CONSTANTS::Piece::EMPTY: // std::cout << "Error, passed empty square to getPieceType()" << std::endl;
        return 'E';
        break;
    default:
        return 'E';
        break;
    }
    return 'X';
}

CONSTANTS::Color TextBoard::getPieceColor(int index)
{
    if (index < 0 || index > 63)
    {
        std::cout << "Error, invalid rank or file input" << std::endl;
        return CONSTANTS::Color::INVALID;
    }

    CONSTANTS::Piece piece = m_board[0][index];
    if (isEmpty(piece))
        return CONSTANTS::Color::EMPTY;
    else if (isWhite(piece))
        return CONSTANTS::Color::WHITE;
    else
        return CONSTANTS::Color::BLACK;
}

CONSTANTS::Piece TextBoard::getPiece(int file, int rank)
{
    if (file < 0 || file > 7 || rank < 0 || rank > 7)
    {
        std::cout << "Error, segmentation fault" << std::endl;
        return CONSTANTS::Piece::EMPTY;
    }
    return m_board[rank][file];
}

CONSTANTS::Piece TextBoard::getPiece(int index)
{
    if (index < 0 || index > 63)
    {
        std::cout << "Error, segmentation fault" << std::endl;
        return CONSTANTS::Piece::EMPTY;
    }
    return m_board[0][index];
}

// std::list<int8_t>* getPieceLocations(CONSTANTS::Color color){

// }
void TextBoard::addPiece(int file, int rank, CONSTANTS::Piece piece)
{
    if (file < 0 || file > 7 || rank < 0 || rank > 7)
    {
        std::cout << "Error, segmentation fault" << std::endl;
        return;
    }
    if (isEmpty(piece))
    {
        std::cout << "Error: Attempting to place empty piece on board" << std::endl;
        return;
    }
    if (getPieceColor(file, rank) != CONSTANTS::Color::EMPTY)
    {
        std::cout << "Error: Placing piece ontop of other piece" << std::endl;
        return;
    }
    m_board[rank][file] = piece;
    if (isWhite(piece))
    {
        m_whitePieceIndices.push_back(rank * 8 + file);
    }
    else if (isBlack(piece))
    {
        m_blackPieceIndices.push_back(rank * 8 + file);
    }
    return;
}

void TextBoard::addPiece(int index, CONSTANTS::Piece piece)
{
    if (index < 0 || index > 63)
    {
        std::cout << "Error, segmentation fault" << std::endl;
        return;
    }
    if (isEmpty(piece))
    {
        std::cout << "Error: Attempting to place empty piece on board" << std::endl;
        return;
    }
    if (getPieceColor(index) != CONSTANTS::Color::EMPTY)
    {
        std::cout << "Error: Placing piece ontop of other piece" << std::endl;
        return;
    }
    m_board[0][index] = piece;
    if (isWhite(piece))
    {
        m_whitePieceIndices.push_back(index);
    }
    else if (isBlack(piece))
    {
        m_blackPieceIndices.push_back(index);
    }
    return;
}

void TextBoard::removePiece(int index)
{
    CONSTANTS::Piece piece = getPiece(index);
        m_captureHistory.push(piece); // Store the piece in capture history before removing. All I need is the piece.

    if (isEmpty(piece))
    {
        std::cout << "Error, attempting to remove empty piece from board" << std::endl;
        return;
    }
    m_board[0][index] = CONSTANTS::Piece::EMPTY;
    if (isWhite(piece))
    {
        m_whitePieceIndices.remove(index);
    }
    else if (isBlack(piece))
    {
        m_blackPieceIndices.remove(index);
    }
    return;
}
void TextBoard::removePiece(int file, int rank)
{
    CONSTANTS::Piece piece = getPiece(file, rank);
    m_captureHistory.push(piece); // Store the piece in capture history before removing
    if (isEmpty(piece))
    {
        std::cout << "Error, attempting to remove empty piece from board" << std::endl;
        return;
    }
    m_board[rank][file] = CONSTANTS::Piece::EMPTY;
    if (isWhite(piece))
    {
        m_whitePieceIndices.remove(rank * 8 + file);
    }
    else if (isBlack(piece))
    {
        m_blackPieceIndices.remove(rank * 8 + file);
    }
    return;
}
void TextBoard::movePiece(int prevFile, int prevRank, int newFile, int newRank, CONSTANTS::Piece promotion)
{
    CONSTANTS::Color occupyingPieceColor = getPieceColor(newFile, newRank);
    CONSTANTS::Color movingPieceColor = getPieceColor(prevFile, prevRank);

    // If we are moving a white piece, update the white index list with new location
    if (movingPieceColor == CONSTANTS::Color::WHITE)
    {
        auto it = std::find(m_whitePieceIndices.begin(), m_whitePieceIndices.end(), prevRank * 8 + prevFile);
        *it = newRank * 8 + newFile;
    }
    // If we are moving a black piece, update the black index list with new location
    else if (movingPieceColor == CONSTANTS::Color::BLACK)
    {
        auto it = std::find(m_blackPieceIndices.begin(), m_blackPieceIndices.end(), prevRank * 8 + prevFile);
        *it = newRank * 8 + newFile;
    }

    if (occupyingPieceColor != CONSTANTS::Color::EMPTY)
    {
        removePiece(newFile, newRank);
    }
    // Update the board
    if (promotion != CONSTANTS::Piece::EMPTY)
    {
        m_board[newRank][newFile] = promotion;
    }
    else
    {
        m_board[newRank][newFile] = m_board[prevRank][prevFile];
    }
    m_board[prevRank][prevFile] = CONSTANTS::Piece::EMPTY;
}

void TextBoard::movePiece(int prevIndex, int newIndex, CONSTANTS::Piece promotion)
{
    CONSTANTS::Color movingPieceColor = getPieceColor(prevIndex);
    CONSTANTS::Color occupyingPieceColor = getPieceColor(newIndex);

    // If we are moving a white piece, update the white index list with new location
    if (movingPieceColor == CONSTANTS::Color::WHITE)
    {
        auto it = std::find(m_whitePieceIndices.begin(), m_whitePieceIndices.end(), prevIndex);
        *it = newIndex;
    }
    // If we are moving a black piece, update the black index list with new location
    else if (movingPieceColor == CONSTANTS::Color::BLACK)
    {
        auto it = std::find(m_blackPieceIndices.begin(), m_blackPieceIndices.end(), prevIndex);
        *it = newIndex;
    }

    if (occupyingPieceColor != CONSTANTS::Color::EMPTY)
    {
        removePiece(newIndex);
    }
    // Update the board
    if (promotion == CONSTANTS::Piece::EMPTY)
    {
        m_board[0][newIndex] = m_board[0][prevIndex];
    }
    else
    {
        m_board[0][newIndex] = promotion;
    }
    m_board[0][prevIndex] = CONSTANTS::Piece::EMPTY;
}


std::string TextBoard::buildMoveString(char cFileOld, int rankOld, char cFileNew, int rankNew)
{
    std::string sFilePrev(1, cFileOld);
    std::string sFileNew(1, cFileNew);
    std::string sRankOld = std::to_string(rankOld + 1);
    std::string sRankNew = std::to_string(rankNew + 1);
    int nFileOld = cFileOld - 'A';
    char type = getPieceType(nFileOld, rankOld);
    std::string moveString = type + sFilePrev + sRankOld + sFileNew + sRankNew;
    // cout << "Building position from (char, int): " << moveString << std::endl;
    return moveString;
}

std::string TextBoard::buildMoveString(int nFilePrev, int rankPrev, int nFileNew, int rankNew)
{

    std::string sFilePrev(1, static_cast<char>(nFilePrev + 'A'));
    std::string sFileNew(1, static_cast<char>(nFileNew + 'A'));
    std::string sRankPrev = std::to_string(rankPrev + 1);
    std::string sRankNew = std::to_string(rankNew + 1);
    char type = getPieceType(nFilePrev, rankPrev);
    std::string moveString = type + sFilePrev + sRankPrev + sFileNew + sRankNew;
    // cout << "Building position from (char, int): " << moveString << std::endl;
    return moveString;
}

CONSTANTS::Status TextBoard::checkSquareStatus(CONSTANTS::Color playerColor, int nFile, int rank)
{
    if (nFile < 0 || nFile > 7 || rank < 0 || rank > 7)
    {
        // std::cout << "Rank and file out of bounds: nFile - " << nFile << " rank - " << rank << std::endl;
        if (rank < -1 || rank > 8 || nFile < -1 || nFile > 8)
        {
            // std::cout << "Houston we have a real problem, or it's just a knight..." << std::endl;
        }
        return CONSTANTS::Status::INVALID;
    }
    if (playerColor != CONSTANTS::Color::WHITE && playerColor != CONSTANTS::Color::BLACK)
    {
        std::cout << "ERROR: returning -2, invalid playerColor input: checkSquareStatus() Color: " << static_cast<int>(playerColor) << std::endl;
        return CONSTANTS::Status::INVALID;
    }

    CONSTANTS::Color squareContentsColor = getPieceColor(nFile, rank);
    if (squareContentsColor == CONSTANTS::Color::EMPTY)
    {
        //  std::cout << "Square " << indexTo_cFile(nFile) << rank+1 << " is empty" << std::endl;
        return CONSTANTS::Status::EMPTY;
    }
    else if (squareContentsColor == playerColor)
    {
        //  std::cout << "Friendly piece on square " << indexTo_cFile(nFile) << rank << std::endl;
        return CONSTANTS::Status::FRIENDLY;
    }
    else
    {
        //  std::cout << "Enemy piece on square " << indexTo_cFile(nFile) << rank << std::endl;
        return CONSTANTS::Status::ENEMY;
    }
}

int TextBoard::cFileToIndex(char cFile)
{
    return cFile - 'A';
}
char TextBoard::indexTo_cFile(int nFile)
{
    return static_cast<char>(nFile + 'A');
}

int TextBoard::convertCoordinateToBoardIndex(int nFile, int rank)
{

    if (nFile >= 0 && nFile <= 7 && rank >= 0 && rank <= 7)
    {
        return rank * 8 + nFile;
    }
    // cout << "Warning, illegal rank and file numbers" << std::endl;

    return -1;
}

void TextBoard::parseMove(std::string move, char &pieceType, char &prevFile, int &prevRank, char &newFile, int &newRank)
{
    pieceType = move[0];
    prevFile = move[1];
    prevRank = move[2] - '0' - 1;
    newFile = move[3];
    newRank = move[4] - '0' - 1; // Convert std::string to int
}
void TextBoard::parseMove(std::string move, char &pieceType, int &prevFile, int &prevRank, int &newFile, int &newRank)
{
    pieceType = move[0];
    prevFile = move[1] - 'A';
    prevRank = move[2] - '0' - 1;
    newFile = move[3] - 'A';
    newRank = move[4] - '0' - 1; // Convert std::string to int
}

void TextBoard::calcPawnMoves(int file, int rank, std::list<std::string> &movesResults, bool attacksOnly)
{
    // Init variables and flags for calculation
    CONSTANTS::Color pieceColor = getPieceColor(file, rank);
    CONSTANTS::Color enemyColor = CONSTANTS::Color::EMPTY;
    CONSTANTS::Color leftDiagonalColor = CONSTANTS::Color::EMPTY;
    CONSTANTS::Color rightDiagonalColor = CONSTANTS::Color::EMPTY;
    int leftFile = -1;
    int rightFile = -1;
    int forwardRank = -1;
    int doubleJumpRank = -1;
    int friendlyPawnStartingRank = -1;
    int enemyPawnStartingRank = -1;
    int enPassantFile = -1;
    int enPassantRank = -1;
    int prePromotionRank = -1;

    // Set flag for if we should check for enpassant
    bool findEnPassant = false;
    if (!m_moveHistory.empty() && (m_moveHistory.top()[0] == 'P') && (abs(m_moveHistory.top()[4] - m_moveHistory.top()[2]) > 1))
    {
        findEnPassant = true;
        enPassantFile = m_moveHistory.top()[3] - 'A'; // Convert from char to int
        enPassantRank = m_moveHistory.top()[4] - '1'; // Convert from char to int
    }

    // Configure variables for moving depending on the location and color of pawn
    //  White pieces
    if (pieceColor == CONSTANTS::Color::WHITE)
    {
        enemyColor = CONSTANTS::Color::BLACK;
        friendlyPawnStartingRank = 1;
        enemyPawnStartingRank = 6;
        prePromotionRank = 6;
        if (rank != 7)
        {
            forwardRank = rank + 1;
        }
        if (file != 0 && rank != 7)
        {
            leftFile = file - 1;
            leftDiagonalColor = getPieceColor(leftFile, forwardRank);
        }
        if (file != 7 && rank != 7)
        {
            rightFile = file + 1;
            rightDiagonalColor = getPieceColor(rightFile, forwardRank);
        }
        if (rank == friendlyPawnStartingRank)
        { // Pawn hasn't moved, enable double jump
            doubleJumpRank = rank + 2;
        }
    }
    // Black Pieces
    else if (pieceColor == CONSTANTS::Color::BLACK)
    {
        enemyColor = CONSTANTS::Color::WHITE;
        friendlyPawnStartingRank = 6;
        enemyPawnStartingRank = 1;
        prePromotionRank = 1;
        if (rank != 0)
        {
            forwardRank = rank - 1;
        }
        if (file != 0 && rank != 0)
        {
            leftFile = file - 1;
            leftDiagonalColor = getPieceColor(leftFile, forwardRank);
        }
        if (file != 7 && rank != 0)
        {
            rightFile = file + 1;
            rightDiagonalColor = getPieceColor(rightFile, forwardRank);
        }
        if (rank == friendlyPawnStartingRank)
        { // Pawn hasn't moved, enable double jump
            doubleJumpRank = rank - 2;
        }
    }
    // Empty squares.  Should never reach this condition
    else
    {
        std::cout << "No piece at location pawnMoves( int " << file << ", int " << rank << ")" << std::endl;
    }

    // All flags and variables have been set, time to do the calculations

    // Search for enpassant and add moves
    if (findEnPassant && !attacksOnly)
    {
        if (rank == enPassantRank)
        { // En passant possible on this rank, caused by black (opponent) performing double jump

            bool enPassantLeft = false;

            if (file != 0)
            {

                if (leftFile == enPassantFile)
                {
                    enPassantLeft = true; // set flag that we added the enPassant move for this pawn
                    movesResults.push_back(buildMoveString(file, rank, enPassantFile, forwardRank));
                }
            }

            if (file != 7 && enPassantLeft == false)
            { // If we're not on the far right, and enpassant wasn't already added, check right

                if (rightFile == enPassantFile)
                {
                    movesResults.push_back(buildMoveString(file, rank, enPassantFile, forwardRank));
                }
            }
        }
    }

    // Add the possible moves for going forward, including promotion if appropriate
    if (forwardRank != -1 && !attacksOnly)
    {

        if (getPiece(file, forwardRank) == CONSTANTS::Piece::EMPTY)
        {
            if (rank != prePromotionRank)
            {
                movesResults.push_back(buildMoveString(file, rank, file, forwardRank));
            }
            else
            { // Pawn promotion by going straight
                movesResults.push_back(buildMoveString(file, rank, file, forwardRank) + "R");
                movesResults.push_back(buildMoveString(file, rank, file, forwardRank) + "N");
                movesResults.push_back(buildMoveString(file, rank, file, forwardRank) + "B");
                movesResults.push_back(buildMoveString(file, rank, file, forwardRank) + "Q");
            }
        }
    }
    // Add double jump moves if appropriate
    if (doubleJumpRank != -1 && !attacksOnly)
    {
        if (getPiece(file, doubleJumpRank) == CONSTANTS::Piece::EMPTY && getPiece(file, forwardRank) == CONSTANTS::Piece::EMPTY)
        {
            movesResults.push_back(buildMoveString(file, rank, file, doubleJumpRank));
        }
    }
    // Calculate capture moves for the left diagonal including promotion
    // If findAttacks then ADD, if !A then ADD if diag = enemy
    if ((attacksOnly && leftFile != -1) || (!attacksOnly && leftDiagonalColor == enemyColor))
    {
        if (rank != prePromotionRank)
        {
            movesResults.push_back(buildMoveString(file, rank, leftFile, forwardRank));
        }
        else
        { // Pawn promotion Left diagonal capture
            movesResults.push_back(buildMoveString(file, rank, leftFile, forwardRank) + "R");
            movesResults.push_back(buildMoveString(file, rank, leftFile, forwardRank) + "N");
            movesResults.push_back(buildMoveString(file, rank, leftFile, forwardRank) + "B");
            movesResults.push_back(buildMoveString(file, rank, leftFile, forwardRank) + "Q");
        }
    }
    // Calculate capture moves for the right diagonal including promotion
    // If findAttacks then ADD, if !A then ADD if diag = enemy
    if ((attacksOnly && rightFile != -1) || (!attacksOnly && rightDiagonalColor == enemyColor))
    {
        if (rank != prePromotionRank)
        {
            movesResults.push_back(buildMoveString(file, rank, rightFile, forwardRank));
            // if (buildMoveString(file, rank, rightFile, forwardRank) == "PD5E4"){
            //     std::cout << "Watch out" << std::endl;
            // }
        }
        else
        { // Pawn promotion right diagonal capture
            movesResults.push_back(buildMoveString(file, rank, rightFile, forwardRank) + "R");
            movesResults.push_back(buildMoveString(file, rank, rightFile, forwardRank) + "N");
            movesResults.push_back(buildMoveString(file, rank, rightFile, forwardRank) + "B");
            movesResults.push_back(buildMoveString(file, rank, rightFile, forwardRank) + "Q");
        }
    }

    return;
}
void TextBoard::calcRookMoves(int file, int rank, std::list<std::string> &movesResults, bool isProtecting)
{

    CONSTANTS::Color pieceColor = getPieceColor(file, rank);
    bool stopFlag[4] = {false, false, false, false};

    for (int i = 1; i < 8; i++)
    {

        if (stopFlag[0] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[0] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank));
                stopFlag[0] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[0] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file + i, rank));
                }
            }
        }
        if (stopFlag[1] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[1] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank));
                stopFlag[1] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[1] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file - i, rank));
                }
            }
        }
        if (stopFlag[2] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file, rank + i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[2] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file, rank + i));
                stopFlag[2] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file, rank + i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[2] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file, rank + i));
                }
            }
        }
        if (stopFlag[3] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file, rank - i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[3] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file, rank - i));
                stopFlag[3] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file, rank - i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[3] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file, rank - i));
                }
                
            }
        }
    }

    return;
}
void TextBoard::calcKnightMoves(int file, int rank, std::list<std::string> &movesResults, bool isProtecting)
{

    CONSTANTS::Color pieceColor = getPieceColor(file, rank);

    std::list<std::pair<int, int>> legalSquares;
    legalSquares.push_back(std::pair<int, int>(file + 2, rank + 1));
    legalSquares.push_back(std::pair<int, int>(file + 2, rank - 1));
    legalSquares.push_back(std::pair<int, int>(file - 2, rank + 1));
    legalSquares.push_back(std::pair<int, int>(file - 2, rank - 1));
    legalSquares.push_back(std::pair<int, int>(file + 1, rank + 2));
    legalSquares.push_back(std::pair<int, int>(file + 1, rank - 2));
    legalSquares.push_back(std::pair<int, int>(file - 1, rank + 2));
    legalSquares.push_back(std::pair<int, int>(file - 1, rank - 2));

    for (auto squarePair : legalSquares)
    {
        CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, squarePair.first, squarePair.second);

        if ((squareStatus != CONSTANTS::Status::FRIENDLY || isProtecting) && squareStatus != CONSTANTS::Status::INVALID)
        {
            movesResults.push_back(buildMoveString(file, rank, squarePair.first, squarePair.second));
        }
    }

    // moves to 8 spots except off board

    return;
}
void TextBoard::calcBishopMoves(int file, int rank, std::list<std::string> &movesResults, bool isProtecting)
{

    CONSTANTS::Color pieceColor = getPieceColor(file, rank);
    bool stopFlag[4] = {false, false, false, false};

    for (int i = 1; i < 8; i++)
    {
        if (stopFlag[0] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank + i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[0] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank + i));
                stopFlag[0] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank + i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[0] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file + i, rank + i));
                }
            }
        }
        if (stopFlag[1] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank - i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[1] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank - i));
                stopFlag[1] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank - i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[1] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file + i, rank - i));
                }
            }
        }
        if (stopFlag[2] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank + i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[2] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank + i));
                stopFlag[2] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank + i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[2] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file - i, rank + i));
                }
            }
        }
        if (stopFlag[3] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank - i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[3] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank - i));
                stopFlag[3] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank - i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[3] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file - i, rank - i));
                }
            }
        }
    }

    return;
}
void TextBoard::calcQueenMoves(int file, int rank, std::list<std::string> &movesResults, bool isProtecting)
{

    CONSTANTS::Color pieceColor = getPieceColor(file, rank);

    bool stopFlag[8] = {false, false, false, false, false, false, false, false};

    for (int i = 1; i < 8; i++)
    {
        if (stopFlag[0] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank + i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[0] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank + i));
                stopFlag[0] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank + i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[0] = true;

                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file + i, rank + i));
                }
            }
        }
        if (stopFlag[1] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank - i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[1] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank - i));
                stopFlag[1] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank - i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[1] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file + i, rank - i));
                }
            }
        }
        if (stopFlag[2] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank + i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[2] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank + i));
                stopFlag[2] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank + i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[2] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file - i, rank + i));
                }
            }
        }
        if (stopFlag[3] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank - i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[3] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank - i));
                stopFlag[3] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank - i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[3] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file - i, rank - i));
                }
            }
        }
        if (stopFlag[4] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[4] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank));
                stopFlag[4] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file + i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[4] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file + i, rank));
                }
            }
        }
        if (stopFlag[5] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[5] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank));
                stopFlag[5] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file - i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[5] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file - i, rank));
                }
            }
        }
        if (stopFlag[6] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file, rank + i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[6] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file, rank + i));
                stopFlag[6] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file, rank + i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[6] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file, rank + i));
                }
            }
        }
        if (stopFlag[7] == false)
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file, rank - i);
            if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[7] = true;
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                movesResults.push_back(buildMoveString(file, rank, file, rank - i));
                stopFlag[7] = true;
            }
            else if (squareStatus == CONSTANTS::Status::EMPTY)
            {
                movesResults.push_back(buildMoveString(file, rank, file, rank - i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[7] = true;
                if(isProtecting){
                    movesResults.push_back(buildMoveString(file, rank, file, rank - i));
                }
            }
        }
    }

    return;
}
void TextBoard::calcKingMoves(int file, int rank, std::list<std::string> &movesResults, bool isProtecting)
{
    CONSTANTS::Color pieceColor = getPieceColor(file, rank);
    for (int row = -1; row <= 1; ++row)
    {
        for (int col = -1; col <= 1; ++col)
        {
            CONSTANTS::Status squareStatus = CONSTANTS::Status::INVALID;
            if (row == 0 && col == 0)
                continue; // Skip the position the king is already sitting in
            else
            {
                squareStatus = checkSquareStatus(pieceColor, file + col, rank + row);
                if (squareStatus == CONSTANTS::Status::EMPTY || squareStatus == CONSTANTS::Status::ENEMY || (squareStatus == CONSTANTS::Status::FRIENDLY && isProtecting))
                {
                    movesResults.push_back(buildMoveString(file, rank, file + col, rank + row));
                }
            }
        }
    }
    return;
}

void TextBoard::calcPieceMoves(int file, int rank, std::list<std::string> &resultsList, bool attacksOnly)
{
    char pieceType = getPieceType(file, rank);
    switch (pieceType)
    {
    case 'P':
        calcPawnMoves(file, rank, resultsList, attacksOnly);
        break;
    case 'R':
        calcRookMoves(file, rank, resultsList, attacksOnly);
        break;
    case 'N':
        calcKnightMoves(file, rank, resultsList, attacksOnly);
        break;
    case 'B':
        calcBishopMoves(file, rank, resultsList, attacksOnly);
        break;
    case 'Q':
        calcQueenMoves(file, rank, resultsList, attacksOnly);
        break;
    case 'K':
        calcKingMoves(file, rank, resultsList, attacksOnly);
        break;
    case 'E':
        std::cout << "Error, passed empty square to calcPieceMoves()" << std::endl;
        break;
    }
    return;
}

int8_t TextBoard::findKingIndex(CONSTANTS::Color color)
{ // this function is only needed for kings // such as myself :D
    std::list<int8_t> *pieceListIndexes = nullptr;
    if (color == CONSTANTS::Color::WHITE)
    {
        pieceListIndexes = &m_whitePieceIndices; // Set my array pointer to point to the first row of m_whitePieceIndices
    }
    else if (color == CONSTANTS::Color::BLACK)
    {
        pieceListIndexes = &m_blackPieceIndices; // Set my array pointer to point to the first row of m_blackPieceIndices
    }
    else
    {
        std::cout << "Invalid player color, returning -1" << std::endl;
        return -1;
    }
    for (auto it = pieceListIndexes->begin(); it != pieceListIndexes->end(); ++it)
    { //
        int8_t testPieceIndex = *it;
        // if (testPieceIndex % 8 == 3 && (testPieceIndex / 8 == 0 || testPieceIndex / 8 == 7))
        // {
        //     std::cout << "Condition found" << std::endl;
        // }
        char pieceType = getPieceType(testPieceIndex % 8, testPieceIndex / 8);
        if (pieceType == 'K')
        {
            return testPieceIndex;
        }
    }

    std::cout << "King not found (captured/non-present), returning -1" << std::endl;
    return -1;
}

void TextBoard::rookScreenMoves(int file, int rank, std::list<std::string> &resultsList)
{
    resultsList.clear();

    CONSTANTS::Color pieceColor = getPieceColor(file, rank);

    bool stopFlag[4] = {false, false, false, false}; // tells the pieces when to stop adding additional distance to their possible move direction (4 for rook)
    int screenNum[4] = {0, 0, 0, 0};                 // Counts the number of pieces we've looked through to try and find the king behind

    for (int i = 1; i < 8; i++)
    {

        if (!stopFlag[0])
        { // if we can keep adding move distance to the right of a piece

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[0] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[0] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[0]; // Add one to the count of attackers in the way of the piece
                if (screenNum[0] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[0] == 2 && getPieceType(file + i, rank) == 'K')
                {                                                                       // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildMoveString(file, rank, file + i, rank)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[0] = true;                                                 // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[1])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            {
            } // if the space is empty then do nothing
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[1] = true;
            } // Stop checking for screen moves
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[1] = true;
            } // You're looking off the board. Stop adding moves to the right
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[1]; // Add one to the count of attackers in the way of the piece
                if (screenNum[1] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[1] == 2 && getPieceType(file - i, rank) == 'K')
                {                                                                       // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildMoveString(file, rank, file - i, rank)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[1] = true;                                                 // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[2])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file, rank + i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[2] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[2] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[2]; // Add one to the count of attackers in the way of the piece
                if (screenNum[2] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[2] == 2 && getPieceType(file, rank + i) == 'K')
                {                                                                       // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildMoveString(file, rank, file, rank + i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[2] = true;                                                 // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[3])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file, rank - i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[3] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[3] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[3]; // Add one to the count of attackers in the way of the piece
                if (screenNum[3] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[3] == 2 && getPieceType(file, rank - i) == 'K')
                {                                                                       // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildMoveString(file, rank, file, rank - i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[3] = true;                                                 // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
    }

    return;
}

void TextBoard::bishopScreenMoves(int file, int rank, std::list<std::string> &resultsList)
{
    resultsList.clear();

    CONSTANTS::Color pieceColor = getPieceColor(file, rank);

    bool stopFlag[4] = {false, false, false, false}; // tells the pieces when to stop adding additional distance to their possible move direction (4 for bishop)
    int screenNum[4] = {0, 0, 0, 0};                 // Counts the number of pieces we've looked through to try and find the king behind

    for (int i = 1; i < 8; i++)
    {

        if (!stopFlag[0])
        { // if we can keep adding move distance to the right of a piece

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank + i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[0] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[0] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[0]; // Add one to the count of attackers in the way of the piece
                if (screenNum[0] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[0] == 2 && getPieceType(file + i, rank + i) == 'K')
                {                                                                           // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildMoveString(file, rank, file + i, rank + i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[0] = true;                                                     // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[1])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank + i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[1] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[1] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[1]; // Add one to the count of attackers in the way of the piece
                if (screenNum[1] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[1] == 2 && getPieceType(file - i, rank + i) == 'K')
                {                                                                           // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildMoveString(file, rank, file - i, rank + i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[1] = true;                                                     // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[2])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank - i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[2] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[2] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[2]; // Add one to the count of attackers in the way of the piece
                if (screenNum[2] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[2] == 2 && getPieceType(file + i, rank - i) == 'K')
                {                                                                           // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildMoveString(file, rank, file + i, rank - i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[2] = true;                                                     // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[3])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank - i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[3] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[3] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[3]; // Add one to the count of attackers in the way of the piece
                if (screenNum[3] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[3] == 2 && getPieceType(file - i, rank - i) == 'K')
                {                                                                           // if there are two attackers being hit, is the second one their king?
                    resultsList.push_back(buildMoveString(file, rank, file - i, rank - i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[3] = true;                                                     // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
    }

    return;
}

void TextBoard::queenScreenMoves(int file, int rank, std::list<std::string> &movesHolder)
{
    movesHolder.clear(); // Make sure to give a vector you don't care about

    CONSTANTS::Color pieceColor = getPieceColor(file, rank);

    bool stopFlag[8] = {false, false, false, false, false, false, false, false};
    int screenNum[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 1; i < 8; i++)
    {
        if (!stopFlag[0])
        { // if we can keep adding move distance to the right of a piece

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank + i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[0] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[0] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[0]; // Add one to the count of attackers in the way of the piece
                if (screenNum[0] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[0] == 2 && getPieceType(file + i, rank + i) == 'K')
                {                                                                           // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildMoveString(file, rank, file + i, rank + i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[0] = true;                                                     // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[1])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank + i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[1] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[1] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[1]; // Add one to the count of attackers in the way of the piece
                if (screenNum[1] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[1] == 2 && getPieceType(file - i, rank + i) == 'K')
                {                                                                           // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildMoveString(file, rank, file - i, rank + i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[1] = true;                                                     // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[2])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank - i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[2] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[2] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[2]; // Add one to the count of attackers in the way of the piece
                if (screenNum[2] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[2] == 2 && getPieceType(file + i, rank - i) == 'K')
                {                                                                           // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildMoveString(file, rank, file + i, rank - i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[2] = true;                                                     // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[3])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank - i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[3] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[3] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[3]; // Add one to the count of attackers in the way of the piece
                if (screenNum[3] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[3] == 2 && getPieceType(file - i, rank - i) == 'K')
                {                                                                           // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildMoveString(file, rank, file - i, rank - i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[3] = true;                                                     // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[4])
        { // if we can keep adding move distance to the right of a piece

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[4] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[4] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[4]; // Add one to the count of attackers in the way of the piece
                if (screenNum[4] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[4] == 2 && getPieceType(file + i, rank) == 'K')
                {                                                                       // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildMoveString(file, rank, file + i, rank)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[4] = true;                                                 // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[5])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[5] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[5] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[5]; // Add one to the count of attackers in the way of the piece
                if (screenNum[5] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[5] == 2 && getPieceType(file - i, rank) == 'K')
                {                                                                       // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildMoveString(file, rank, file - i, rank)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[5] = true;                                                 // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[6])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file, rank + i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[6] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[6] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[6]; // Add one to the count of attackers in the way of the piece
                if (screenNum[6] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[6] == 2 && getPieceType(file, rank + i) == 'K')
                {                                                                       // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildMoveString(file, rank, file, rank + i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[6] = true;                                                 // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
        if (!stopFlag[7])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file, rank - i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                // moves -> push_back(buildMoveString(file,rank,file +i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[7] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[7] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                ++screenNum[7]; // Add one to the count of attackers in the way of the piece
                if (screenNum[7] == 1)
                    continue; // Does this break from the local if statement, or the larger stopFlag[0] if statement?
                else if (screenNum[7] == 2 && getPieceType(file, rank - i) == 'K')
                {                                                                       // if there are two attackers being hit, is the second one their king?
                    movesHolder.push_back(buildMoveString(file, rank, file, rank - i)); // It is a king!  so add the "screen" attack to the moves list
                    stopFlag[7] = true;                                                 // stop looking further for more pieces in the way
                }
                // screenedPiece = boardState[convertCoordinateToBoardIndex(file +i, rank)].getType();
            }
        }
    }

    return;
}

// This is a special function for handling a case where the king moves out of check
void TextBoard::queenCheckScreenMoves(int file, int rank, std::list<std::string> &movesHolder)
{
    movesHolder.clear(); // Make sure to give a vector you don't care about

    CONSTANTS::Color pieceColor = getPieceColor(file, rank);

    bool stopFlag[8] = {false, false, false, false, false, false, false, false};
    bool passedKing[8] = {false, false, false, false, false, false, false, false};

    for (int i = 1; i < 8; ++i)
    {
        if (!stopFlag[0])
        { // if we can keep adding move distance to the right of a piece

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank + i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                movesHolder.push_back(buildMoveString(file,rank,file +i, rank+i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[0] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[0] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                stopFlag[0] = true;
                if (getPieceType(file + i, rank + i) == 'K')
                {   
                    squareStatus = checkSquareStatus(pieceColor, file + (i+1), rank + (i+1)); // check the space i squares to the right of the piece                                                               // if there are two attackers being hit, is the second one their king? 
                    if(squareStatus == CONSTANTS::Status::EMPTY){
                        movesHolder.push_back(buildMoveString(file,rank,file +(i+1), rank+(i+1)));                                            // stop looking further for more pieces in the way
                    }
                }
            }
        }
        if (!stopFlag[1])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank + i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                movesHolder.push_back(buildMoveString(file,rank,file - i, rank + i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[1] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[1] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {
                stopFlag[1] = true;
                if (getPieceType(file - i, rank + i) == 'K')
                {   
                    squareStatus = checkSquareStatus(pieceColor, file - (i+1), rank + (i+1)); // check the space i squares to the right of the piece                                                               // if there are two attackers being hit, is the second one their king? 
                    if(squareStatus == CONSTANTS::Status::EMPTY){
                        movesHolder.push_back(buildMoveString(file,rank,file -(i+1), rank+(i+1)));                                            // stop looking further for more pieces in the way
                    }
                }
            }
        }
        if (!stopFlag[2])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank - i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                movesHolder.push_back(buildMoveString(file,rank,file + i, rank - i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[2] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[2] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                stopFlag[2] = true;
                if (getPieceType(file + i, rank - i) == 'K')
                {   
                    squareStatus = checkSquareStatus(pieceColor, file + (i+1), rank - (i+1)); // check the space i squares to the right of the piece                                                               // if there are two attackers being hit, is the second one their king? 
                    if(squareStatus == CONSTANTS::Status::EMPTY){
                        movesHolder.push_back(buildMoveString(file,rank,file +(i+1), rank-(i+1)));                                            // stop looking further for more pieces in the way
                    }
                }
            }
        }
        if (!stopFlag[3])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank - i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                movesHolder.push_back(buildMoveString(file,rank,file - i, rank - i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[3] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[3] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                stopFlag[3] = true;
                if (getPieceType(file - i, rank - i) == 'K')
                {   
                    squareStatus = checkSquareStatus(pieceColor, file - (i+1), rank - (i+1)); // check the space i squares to the right of the piece                                                               // if there are two attackers being hit, is the second one their king? 
                    if(squareStatus == CONSTANTS::Status::EMPTY){
                        movesHolder.push_back(buildMoveString(file,rank,file -(i+1), rank-(i+1)));                                            // stop looking further for more pieces in the way
                    }
                }
            }
        }
        if (!stopFlag[4])
        { // if we can keep adding move distance to the right of a piece

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file + i, rank); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                movesHolder.push_back(buildMoveString(file,rank,file + i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[4] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[4] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                stopFlag[4] = true;
                if (getPieceType(file + i, rank) == 'K')
                {   
                    squareStatus = checkSquareStatus(pieceColor, file + (i+1), rank); // check the space i squares to the right of the piece                                                               // if there are two attackers being hit, is the second one their king? 
                    if(squareStatus == CONSTANTS::Status::EMPTY){
                        movesHolder.push_back(buildMoveString(file,rank,file +(i+1), rank));                                            // stop looking further for more pieces in the way
                    }
                }
            }
        }
        if (!stopFlag[5])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file - i, rank); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                movesHolder.push_back(buildMoveString(file,rank,file - i, rank));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[5] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[5] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                stopFlag[5] = true;
                if (getPieceType(file - i, rank) == 'K')
                {   
                    squareStatus = checkSquareStatus(pieceColor, file - (i+1), rank); // check the space i squares to the right of the piece                                                               // if there are two attackers being hit, is the second one their king? 
                    if(squareStatus == CONSTANTS::Status::EMPTY){
                        movesHolder.push_back(buildMoveString(file,rank,file -(i+1), rank));                                            // stop looking further for more pieces in the way
                    }
                }
            }
        }
        if (!stopFlag[6])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file, rank + i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                movesHolder.push_back(buildMoveString(file,rank,file, rank + i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[6] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[6] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                stopFlag[6] = true;
                if (getPieceType(file, rank + i) == 'K')
                {   
                    squareStatus = checkSquareStatus(pieceColor, file, rank + (i+1)); // check the space i squares to the right of the piece                                                               // if there are two attackers being hit, is the second one their king? 
                    if(squareStatus == CONSTANTS::Status::EMPTY){
                        movesHolder.push_back(buildMoveString(file,rank,file, rank+(i+1)));                                            // stop looking further for more pieces in the way
                    }
                }
            }
        }
        if (!stopFlag[7])
        {

            CONSTANTS::Status squareStatus = checkSquareStatus(pieceColor, file, rank - i); // check the space i squares to the right of the piece

            if (squareStatus == CONSTANTS::Status::EMPTY)
            { // if the space is empty then do nothing
                movesHolder.push_back(buildMoveString(file,rank,file, rank - i));
            }
            else if (squareStatus == CONSTANTS::Status::FRIENDLY)
            {
                stopFlag[7] = true;
            }
            else if (squareStatus == CONSTANTS::Status::INVALID)
            {
                stopFlag[7] = true; // Stop adding moves to the right
            }
            else if (squareStatus == CONSTANTS::Status::ENEMY)
            {

                stopFlag[7] = true;
                if (getPieceType(file, rank - i) == 'K')
                {   
                    squareStatus = checkSquareStatus(pieceColor, file, rank - (i+1)); // check the space i squares to the right of the piece                                                               // if there are two attackers being hit, is the second one their king? 
                    if(squareStatus == CONSTANTS::Status::EMPTY){
                        movesHolder.push_back(buildMoveString(file,rank,file, rank-(i+1)));                                            // stop looking further for more pieces in the way
                    }
                }
            }
        }
    }

    return;
}

void TextBoard::getScreenMoves(int pieceFile, int pieceRank, std::list<std::string> &resultsList)
{

    char pieceType = getPieceType(pieceFile, pieceRank);

    switch (pieceType)
    { // check the type of piece being calculated for and runs its move calc function with results contained in movesHolder
    case 'R':
        rookScreenMoves(pieceFile, pieceRank, resultsList);
        return;
    case 'B':
        bishopScreenMoves(pieceFile, pieceRank, resultsList);
        return;
    case 'Q':
        queenScreenMoves(pieceFile, pieceRank, resultsList);
        return;
    case 'P':
        return;
    case 'N':
        return;
    case 'K':
        return;
    case 'E':
        return;
    default:
        std::cout << "Passed bad piece type to function getScreenMoves()";
        return;
    }
}

void TextBoard::calcScreenMovesets()
{ // The pointers in this are very obscure, but I might have done them right, fingers crossed

    // cout << "Calculating moveset for player: " << playerColor << std::endl;

    // Piece kingPiece = *findPiece(playerColor, 'K');  // commented this out because think i dont need it

    m_whiteScreenMoves.clear();
    m_blackScreenMoves.clear();
    typedef enum
    {
        BLACK,
        WHITE
    } Colors;
    std::list<int8_t> *pieceIndicesList;
    std::list<std::string> *pieceScreenMoveHolder;

    for (int color = BLACK; color < WHITE + 1; color++)
    {
        if (color == BLACK)
        {
            pieceIndicesList = &m_blackPieceIndices;
            pieceScreenMoveHolder = &m_blackScreenMoves;
        }
        else if (color == WHITE)
        {
            pieceIndicesList = &m_whitePieceIndices;
            pieceScreenMoveHolder = &m_whiteScreenMoves;
        }
        else
        {
            std::cout << "Error with for loop or if statement in calcScreenMovesets()" << std::endl;
            return;
        }

        for (auto it = pieceIndicesList->begin(); it != pieceIndicesList->end(); ++it)
        { // go through all the pieces
            int8_t index = *it;
            int8_t tempRank = index / 8;
            int8_t tempFile = index % 8;

            // get the moves for white pieces on the board that are "screen" attacks  (pinning attacks)
            std::list<std::string> partialScreenMoveset;
            getScreenMoves(tempFile, tempRank, partialScreenMoveset);
            if (!partialScreenMoveset.empty())
            {
                std::string();
            }
            pieceScreenMoveHolder->insert(pieceScreenMoveHolder->end(), partialScreenMoveset.begin(), partialScreenMoveset.end()); // add them all up
        }
    }

    //    for(auto it = m_blackPieceIndices.begin(); it != m_blackPieceIndices.end(); ++it){ // do the same for black
    //        int8_t tempPiece = m_board[*it];
    //        int8_t index = *it;
    //        int8_t tempPieceRank = index/8;
    //        int8_t tempPieceFile = index%8;
    //
    //        // get the piece moves from the pieces that are on the board that are your color
    //        std::list<std::string> partialScreenMoveset;
    //        getScreenMoves(tempPieceFile, tempPieceIndex, partialScreenMoveset);
    //        m_blackScreenMoves.insert(m_blackScreenMoves.end(), partialScreenMoveset->begin(),partialScreenMoveset->end());
    //    }
}

int TextBoard::countNumAttackers(std::list<std::string> *pplayerMoveset, int forFile, int forRank)
{
    std::string move;
    char type;
    int beginFile, endFile, beginRank, endRank; // converts from one char within a std::string to an int, and decrements from rank number to array index num
    int attackerCount = 0;
    for (auto it = pplayerMoveset->begin(); it != pplayerMoveset->end(); ++it)
    {
        move = *it;
        parseMove(move, type, beginFile, beginRank, endFile, endRank);

        if (endFile == forFile && endRank == forRank)
        {
            // if(!(type == 'P' && beginFile == endFile)){ // Covers exception that pawns cannot capture forward
            attackerCount++;
            // }
        }
    }
    return attackerCount;
}

void TextBoard::getAttackingMoves(std::list<std::string> playerMoves, int forFile, int forRank, std::list<std::string> *attackingMoves)
{
    for (auto it = playerMoves.begin(); it != playerMoves.end(); ++it)
    {
        std::string simple = it->c_str();
        int file = TextBoard::cFileToIndex(simple[3]);
        int rank = simple[4] - '0' - 1;

        if (file == forFile && rank == forRank)
        {
            attackingMoves->push_back(simple);
        }
    }
}

void TextBoard::makeColinearSquaresVector(std::string pieceMove, std::list<std::string> *pColinearSquaresResultVector)
{
    pColinearSquaresResultVector->clear();
    char pieceType;
    char cPrevFile;
    int prevRank;
    char cNewFile;
    int newRank;

    parseMove(pieceMove, pieceType, cPrevFile, prevRank, cNewFile, newRank);

    if (pieceType == 'N')
    {
        std::cout << "Cannot make colinear squares vector from knight moves" << std::endl;
        return;
    }

    int8_t fileDiff = cNewFile - cPrevFile;
    int8_t rankDiff = newRank - prevRank; // gets difference between squares
    int8_t fileDirection = 0;
    int8_t rankDirection = 0;
    int8_t loopIterations = 0;

    if (fileDiff != 0)
    {
        fileDirection = fileDiff / abs(fileDiff);
        loopIterations = abs(fileDiff);
    }
    if (rankDiff != 0)
    {
        rankDirection = rankDiff / abs(rankDiff); // if rankDiff is negative/positive, it increments negative/positive respectively
        loopIterations = abs(rankDiff);
    }

    for (int i = 0; i < loopIterations; i++)
    {

        char colinearSquareFile = static_cast<char>(cPrevFile + i * fileDirection);
        int colinearSquareRank = prevRank + i * rankDirection;

        std::string sColinearSquareFile(1, colinearSquareFile);
        std::string sColinearSquareRank = std::to_string(colinearSquareRank);

        pColinearSquaresResultVector->push_back(sColinearSquareFile + sColinearSquareRank);
    }

    std::cout << "Colinear squares created" << std::endl;

} // Not as useful when you just use squareIsBetweenSquares in order to check for getting between two pieces. Still, using this might be faster with storage.

bool TextBoard::squareIsBetweenSquares(std::string move, int testSquareFile, int testSquareRank)
{

    char pieceType, prevFile, newFile;
    int prevRank, newRank;

    parseMove(move, pieceType, prevFile, prevRank, newFile, newRank);

    if (pieceType == 'N')
    {
        return false;
    }

    int square2File = newFile - 'A';
    int square2Rank = newRank;
    int square1File = prevFile - 'A';
    int square1Rank = prevRank;

    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

    int y, x;

    x = testSquareFile;
    y = testSquareRank;

    if (square1Rank >= square2Rank)
    {
        y2 = square1Rank;
        x2 = square1File;
        y1 = square2Rank;
        x1 = square2File;
    }
    else if (square2Rank > square1Rank)
    {
        y2 = square2Rank;
        x2 = square2File;
        y1 = square1Rank;
        x1 = square1File;
    }

    // if it's a vertical line, if the file is the same, and the square is inbetween or on the endpoints, return true
    if (x2 == x1)
    {
        if (x == x1 && y1 <= y && y <= y2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // if it's a horizontal line, if the test rank is the same, and the square is inbetween or on the endpoints, return true
    if (y2 == y1)
    {
        if (y == y1)
        {
            if (x1 <= x2)
            {
                if (x1 <= x && x <= x2)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (x2 <= x1)
            {
                if (x2 <= x && x <= x1)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            return false;
        }
    }

    float slope = (y2 - y1) / (x2 - x1); // calculate slope of line  (the only valid slope is 1 or -1)

    int b = y1 - slope * x1; // calculate the y intercept

    if (y == slope * x + b && y1 <= y && y <= y2)
    { // if the point is on the line and the y value is on or between endpoints, return true
        return true;
    }

    return false; // all other cases the test square isn't on the line between the pieces
}
// Good function, but unused
// void TextBoard::getSquaresBetweenSquares(std::string move, std::list<std::string> *returnVectorPointer)
// {

//     char pieceType, prevFile, newFile;
//     int prevRank, newRank;

//     parseMove(move, pieceType, prevFile, prevRank, newFile, newRank);
//     returnVectorPointer->clear();

//     int square2File = newFile - 'A';
//     int square2Rank = newRank;
//     int square1File = prevFile - 'A';
//     int square1Rank = prevRank;

//     int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

//     if (square1Rank >= square2Rank)
//     {
//         y2 = square1Rank;
//         y1 = square2Rank;
//     }
//     else if (square2Rank > square1Rank)
//     {
//         y2 = square2Rank;
//         y1 = square1Rank;
//     }

//     if (square1File >= square2File)
//     {
//         x2 = square1File;
//         x1 = square2File;
//     }
//     else if (square2File > square1File)
//     {
//         x2 = square2File;
//         x1 = square1File;
//     }

//     // if it's a vertical line, if the file is the same, and the square is inbetween or on the endpoints, return true
//     if (x2 == x1)
//     {
//         for (int i = 0; i < y2 - y1; i++)
//         {
//             char y, x;
//             y = static_cast<char>(y1 + i);
//             x = static_cast<char>(x1);
//             std::string sX(1, x);
//             std::string sY(1, y);

//             returnVectorPointer->push_back(sX + sY); // Dereference the data storage location of variable and add a square to the list
//         }
//         return;
//     }
//     // if it's a horizontal line, if the test rank is the same, and the square is inbetween or on the endpoints, return true
//     if (y2 == y1)
//     {
//         for (int i = 0; i < x2 - x1; i++)
//         {
//             char y, x;
//             y = static_cast<char>(y1);
//             x = static_cast<char>(x1 + i);
//             std::string sX(1, x);
//             std::string sY(1, y);

//             returnVectorPointer->push_back(sX + sY);
//         }
//         return;
//     }

//     float slope = (y2 - y1) / (x2 - x1); // calculate slope of line  (the only valid slope is 1 or -1)
//     // test the slope
//     if (abs(slope) != 1)
//     {
//         std::cout << "two non linear squares given, invalid inputs" << std::endl;
//     }

//     for (int i = 0; i < x2 - x1; i++)
//     {
//         char y, x;
//         y = static_cast<char>(y1 + i);
//         x = static_cast<char>(x1 + i);
//         std::string sX(1, x);
//         std::string sY(1, y);

//         returnVectorPointer->push_back(sX + sY);
//     }
//     return;
// }

void TextBoard::calcPlayerMovesetV2(CONSTANTS::Color playerColor, bool validateMoveset)
{

    // This function is called recursively, so we need to clear it when we first start calculation, but not on recursive calls.
    if (validateMoveset)
    {
        m_whiteMoves.clear();
        m_blackMoves.clear();
    }

    std::list<int8_t> *pPlayerPieceIndices;
    std::list<std::string> *pcompletePlayerMoveset;
    std::list<std::string> *penemyMoveset;
    std::list<std::string> *penemyScreenMoveset;
    CONSTANTS::Color enemyColor;
    bool kingHasMoved;
    bool aRookHasMoved;
    bool hRookHasMoved;
    bool addCastling = false;

    if (playerColor == CONSTANTS::Color::WHITE)
    {
        pPlayerPieceIndices = &m_whitePieceIndices;
        pcompletePlayerMoveset = &m_whiteMoves;
        penemyMoveset = &m_blackMoves;
        penemyScreenMoveset = &m_blackScreenMoves;
        enemyColor = CONSTANTS::Color::BLACK;
        kingHasMoved = m_whiteKingMoved;
        aRookHasMoved = m_whiteARookMoved;
        hRookHasMoved = m_whiteHRookMoved;
    }
    else if (playerColor == CONSTANTS::Color::BLACK)
    {
        pPlayerPieceIndices = &m_blackPieceIndices;
        pcompletePlayerMoveset = &m_blackMoves;
        penemyMoveset = &m_whiteMoves;
        penemyScreenMoveset = &m_whiteScreenMoves;
        enemyColor = CONSTANTS::Color::WHITE;
        kingHasMoved = m_blackKingMoved;
        aRookHasMoved = m_blackARookMoved;
        hRookHasMoved = m_blackHRookMoved;
    }
    else
        return;


    // find king, calculate checks on king, calculate screen attackers on king

    int8_t kingIndex = findKingIndex(playerColor); // might have issue with this if comp is simulating taking enemy king
    this;

    if (kingIndex == -1){
        return; // Error, did not find the king on the board
    }

    int8_t kingFile = kingIndex % 8;
    int8_t kingRank = kingIndex / 8;
    std::list<std::string> attackingKingMoves;
    std::list<std::string> apparentScreenAttackingKingMoves;
    std::list<std::string> screenAttackingKingMoves;
    std::list<std::string> kingUnfilteredMoveset;
    std::list<std::string> kingFilteredMoveset;
    std::list<std::string> castlingMoveset;
    int8_t checksOnKing = 0;

    calcScreenMovesets(); // calculate all the screening attacks (attacks that hit the king through another piece). Can optimize by making it color specific

    if (!validateMoveset)
    { // if we don't need to apply normal move rules with regards to the king then just get all the unabridged moves
        for (auto it = pPlayerPieceIndices->begin(); it != pPlayerPieceIndices->end(); ++it)
        {
            int8_t tempPieceIndex = *it;
            int8_t rank = tempPieceIndex / 8;
            int8_t file = tempPieceIndex % 8;
            std::list<std::string> partialMoveset;
            calcPieceMoves(file, rank, partialMoveset, true);
            pcompletePlayerMoveset->insert(pcompletePlayerMoveset->end(), partialMoveset.begin(), partialMoveset.end());
        }
        return;
    }

    calcPlayerMovesetV2(enemyColor, false); // calculate all the possible places the opponents pieces can move at the moment

    getAttackingMoves(*penemyMoveset, kingFile, kingRank, &attackingKingMoves);             // get all the moves that attack the king
    getAttackingMoves(*penemyScreenMoveset, kingFile, kingRank, &screenAttackingKingMoves); // get all the screen moves on the king

    checksOnKing = attackingKingMoves.size(); // find the number of attacks on king

    if (checksOnKing == 2)
    { // If there are two pieces attacking the king, the king must move to survive -  shortest: linear time
        // only get king moves and don't do moveset creation below

        calcKingMoves(kingFile, kingRank, kingUnfilteredMoveset); // Finds the general squares a king can move to
        std::list<std::string>::iterator checkIt = attackingKingMoves.begin(); // point to beginning of attacking moves

        for (auto it = kingUnfilteredMoveset.begin(); it != kingUnfilteredMoveset.end();)
        {
            std::string move = *it;

            char pieceType;
            int prevRank, newRank, prevFile, newFile;
            char pieceAttackingKingType;
            int attackerFile;
            int attackerRank;

            parseMove(move, pieceType, prevFile, prevRank, newFile, newRank);

            if (countNumAttackers(penemyMoveset, newFile, newRank) != 0)
            {                                         // CAN IMPROVE SPEED BY GETTING THE attacking moves and performing comparison to the king moves, as soon as one attacking move hits a king move square, it removes the king move and goes to next. This would be an optimization custom function
                it = kingUnfilteredMoveset.erase(it); // The enemy is attacking our observed square.
            }
            else if (checkIt != attackingKingMoves.end())
                {
                    pieceAttackingKingType = (*checkIt)[0];
                    if (pieceAttackingKingType == 'P' || pieceAttackingKingType == 'N'){ // We don't care about knight or pawn attacks in this way
                        ++checkIt;
                    pieceAttackingKingType = (*checkIt)[0];
                    }
                    attackerFile = (*checkIt)[1]-'A';
                    attackerRank = (*checkIt)[2]-'1';
                    if(pieceAttackingKingType == 'R'){
                        // If we are trying to move directly horizontal or vertical of the queen or rook, don't accept the move
                        if (newFile == attackerFile || newRank == attackerRank)
                        {
                            it = pcompletePlayerMoveset->erase(it);
                            ++checkIt;
                        }
                        else {++it;}
                    }
                    else if (pieceAttackingKingType == 'B')
                    {
                        // If we are trying to move directly diagonal to a queen or bishop, don't accept the move
                        if (newFile - attackerFile != 0 && abs((newRank - attackerRank) / (newFile - attackerFile)) == 1)
                        {
                            it = pcompletePlayerMoveset->erase(it);
                            ++checkIt;
                        }
                        else {++it;}
                    }
                    else if (pieceAttackingKingType == 'Q')
                    {
                        std::list<std::string> movesHolder, newSquareAttackers;
                        queenCheckScreenMoves(attackerFile,attackerRank,movesHolder); // SPEED OPPORTUNITY This calculates every time. Better to store and lookup. 

                        int attacksOnNewSquare = countNumAttackers(&movesHolder,newFile,newRank);
                        if (attacksOnNewSquare != 0)
                        {
                            it = pcompletePlayerMoveset->erase(it);
                            ++checkIt;
                        }
                        else {++it;}
                    }    
                }
            else
            {
                ++it;
            }
        }
        kingFilteredMoveset = kingUnfilteredMoveset; // create an updated variable
        pcompletePlayerMoveset->insert(pcompletePlayerMoveset->end(), kingFilteredMoveset.begin(), kingFilteredMoveset.end());

        return; // Return because the only viable moves are ones where the king moves

    } // if there are two attacks on the king, the king must move

    for (auto it = pPlayerPieceIndices->begin(); it != pPlayerPieceIndices->end(); ++it)
    {
        int8_t tempPieceIndex = *it;
        int tempFile = tempPieceIndex % 8;
        int tempRank = tempPieceIndex / 8;
        char tempPieceType = getPieceType(tempFile, tempRank);

        if (tempPieceType != 'K')
        { // if a piece is not the king
            if (!screenAttackingKingMoves.empty())
            { // if there is still an unremoved pinning move
                std::list<std::string> partialMoveset;
                calcPieceMoves(tempFile, tempRank, partialMoveset);

                for (auto it = screenAttackingKingMoves.begin(); it != screenAttackingKingMoves.end();)
                { // for each of the pinning moves
                    std::string move = *it;

                    if (squareIsBetweenSquares(move, tempFile, tempRank))
                    { // if the piece is the one being pinned (in between king && attacker)
                        for (auto it1 = partialMoveset.begin(); it1 != partialMoveset.end();)
                        { // for each of the moves in the single piece's moveset
                            auto possibleMove = *it1;
                            int testFile = possibleMove[3] - 'A';
                            int testRank = possibleMove[4] - '1';
                            if (!squareIsBetweenSquares(move, testFile, testRank))
                            {                                    // if the move is not between the king and the attacker
                                it1 = partialMoveset.erase(it1); // erase the possible move from the vector as invalid
                            }
                            else
                            {
                                ++it1;
                            }
                        }
                        it = screenAttackingKingMoves.erase(it); // erase the pinning move from the pinning move moveset because we've found the piece being pinned and have modified its moveset to account for being pinned
                    }
                    else
                    {
                        ++it;
                    }
                }

                pcompletePlayerMoveset->insert(pcompletePlayerMoveset->end(), partialMoveset.begin(), partialMoveset.end()); // add the filtered moveset to the complete moveset
            }
            else
            { // add one of the regular unpinned movesets from pieces

                std::list<std::string> partialMoveset;
                calcPieceMoves(tempFile, tempRank, partialMoveset);
                pcompletePlayerMoveset->insert(pcompletePlayerMoveset->end(), partialMoveset.begin(), partialMoveset.end());

                // cout << std::endl;
            }
        }
        else if (tempPieceType == 'K')
        { // add king's viable moves depending on what would put it in check
            std::list<std::string> kingUnfilteredMoveset;
            calcPieceMoves(tempFile, tempRank, kingUnfilteredMoveset);
            char pieceAttackingKingType;
            int attackerFile;
            int attackerRank;
            std::list<std::string>::iterator checkIt = attackingKingMoves.begin(); // point to beginning of moves attacking the king
            for (std::list<std::string>::iterator it = kingUnfilteredMoveset.begin(); it != kingUnfilteredMoveset.end();)
            {
                std::string move = *it;

                char pieceType;
                int prevRank, newRank, prevFile, newFile;

                parseMove(move, pieceType, prevFile, prevRank, newFile, newRank);


                if (countNumAttackers(penemyMoveset, static_cast<int>(newFile), newRank) != 0) 
                { // CAN IMPROVE SPEED BY GETTING THE MOVES AND DOING COMPARISON
                    it = kingUnfilteredMoveset.erase(it);
                }
                // In the case being checked by queen, rook, bishop.  The king can't run away in a straight line.
                // When we get to a move that involves the king running away in a straight line or diagonal, remove the move
                // and increment the number of moves we've removed. If the king is in double check, it has to remove 2 moves.
                else if (checkIt != attackingKingMoves.end())
                {   
                    pieceAttackingKingType = (*checkIt)[0];
                    if (pieceAttackingKingType == 'P' || pieceAttackingKingType == 'N'){ // We don't care about knight or pawn attacks in this way
                        ++checkIt;
                        // if(checkIt == attackingKingMoves.end()){ // Since there's only 1 checking move, this should always be true
                            ++it;
                            continue;
                        // }
                    pieceAttackingKingType = (*checkIt)[0];
                    }
                    attackerFile = (*checkIt)[1]-'A';
                    attackerRank = (*checkIt)[2]-'1';
                    if(pieceAttackingKingType == 'R'){
                        // If we are trying to move directly horizontal or vertical of the queen or rook, don't accept the move
                        if (newFile == attackerFile || newRank == attackerRank)
                        {
                            it = pcompletePlayerMoveset->erase(it);
                            ++checkIt;
                        }
                        else {++it;}
                    }
                    else if (pieceAttackingKingType == 'B')
                    {
                        // If we are trying to move directly diagonal to a queen or bishop, don't accept the move
                        if (newFile - attackerFile != 0 && abs((newRank - attackerRank) / (newFile - attackerFile)) == 1)
                        {
                            it = pcompletePlayerMoveset->erase(it);
                            ++checkIt;
                        }
                        else {++it;}
                    }
                    else if (pieceAttackingKingType == 'Q')
                    {
                        std::list<std::string> movesHolder, newSquareAttackers;
                        queenCheckScreenMoves(attackerFile,attackerRank,movesHolder); // SPEED OPPORTUNITY This calculates every time. Better to store and lookup. 

                        int attacksOnNewSquare = countNumAttackers(&movesHolder,newFile,newRank);
                        if (attacksOnNewSquare != 0)
                        {
                            it = pcompletePlayerMoveset->erase(it);
                            ++checkIt;
                        }
                        else {++it;}
                    }    
                }
                else
                {
                    ++it;
                }
            }

            kingFilteredMoveset = kingUnfilteredMoveset;                                                                           // create an updated variable because moveset has been filtered/cleaned
            pcompletePlayerMoveset->insert(pcompletePlayerMoveset->end(), kingFilteredMoveset.begin(), kingFilteredMoveset.end()); // add filtered moveset to complete moveset

            // INSERT HERE
            // Add castling to the move choices if it is viable
            if (tempPieceType == 'K' && kingHasMoved == false)
            { // if the king hasn't moved
                bool check = false;
                castlingMoveset.clear();
                int attackerNum = countNumAttackers(penemyMoveset, tempFile, tempRank);
                if (attackerNum != 0)
                {
                    check = true;
                }

                // All conditions for castling

                // Mutual:
                bool notInCheck = !check;
                kingHasMoved;

                // Kingside:
                aRookHasMoved;
                const bool cSquareAdjacentKingIsEmpty = checkSquareStatus(playerColor, tempFile - 1, tempRank) == CONSTANTS::Status::EMPTY;
                const bool bSquareAdjacentKingIsEmpty = checkSquareStatus(playerColor, tempFile - 2, tempRank) == CONSTANTS::Status::EMPTY;
                const bool cSquareAdjacentKingUnderAttack = countNumAttackers(penemyMoveset, tempFile - 1, tempRank) > 0;
                const bool bSquareAdjacentKingUnderAttack = countNumAttackers(penemyMoveset, tempFile - 2, tempRank) > 0;

                // Queenside:
                hRookHasMoved;
                const bool eSquareAdjacentKingIsEmpty = checkSquareStatus(playerColor, tempFile + 1, tempRank) == CONSTANTS::Status::EMPTY;
                const bool fSquareAdjacentKingIsEmpty = checkSquareStatus(playerColor, tempFile + 2, tempRank) == CONSTANTS::Status::EMPTY;
                const bool gSquareAdjacentKingIsEmpty = checkSquareStatus(playerColor, tempFile + 3, tempRank) == CONSTANTS::Status::EMPTY;
                const bool eSquareAdjacentKingUnderAttack = countNumAttackers(penemyMoveset, tempFile + 1, tempRank) > 0;
                const bool fSquareAdjacentKingUnderAttack = countNumAttackers(penemyMoveset, tempFile + 2, tempRank) > 0;

                if (notInCheck && cSquareAdjacentKingIsEmpty && bSquareAdjacentKingIsEmpty && !kingHasMoved && !aRookHasMoved && !cSquareAdjacentKingUnderAttack && !bSquareAdjacentKingUnderAttack)
                {

                    castlingMoveset.push_back("O-O");
                    addCastling = true;
                    // std::cout << "Player may castle king-side" << std::endl;
                }
                if (notInCheck && eSquareAdjacentKingIsEmpty && fSquareAdjacentKingIsEmpty && gSquareAdjacentKingIsEmpty && !kingHasMoved && !hRookHasMoved && !eSquareAdjacentKingUnderAttack && !fSquareAdjacentKingUnderAttack)
                {
                    castlingMoveset.push_back("O-O-O");
                    addCastling = true;
                    // std::cout << "Player may castle queen-side" << std::endl;
                }
            } // add castling if viable
        }
    }

    if (checksOnKing == 1)
    { // if there is only one attacker on the king then

        for (auto pMove = pcompletePlayerMoveset->begin(); pMove != pcompletePlayerMoveset->end();)
        {
            std::string move = *pMove;
            char pieceType = move[0];
            int moveEndFile = move[3] - 'A';
            int moveEndRank = move[4] - '1';
            char pieceAttackingKingType = (*attackingKingMoves.begin())[0]; // get the attacking enemy piece type
            if (pieceType == 'K')
            { // IF WE'RE LOOKING AT KING MOVES, THEN IT CAN MOVE OUT OF CHECK, or capture the attacker. 
              // We've already filtered the king moves for legality, so all the king moves are okay.
                ++pMove; // this move is fine, move on to the next move in the list
            }
            else if (pieceAttackingKingType == 'N')
            {                                                                         // if the attacker is a knight
                int knightAttackingKingFile = (*attackingKingMoves.begin())[1] - 'A'; // attackingKingMoves is a std::string vector of moves
                int knightAttackingKingRank = (*attackingKingMoves.begin())[2] - '1';
                if (moveEndRank == knightAttackingKingRank && moveEndFile == knightAttackingKingFile)
                {            // if we can capture the knight
                    ++pMove; // then keep the move as valid move, because we are removing the knight threat
                }
                else
                {                                                 // if we can't capture the knight
                    pMove = pcompletePlayerMoveset->erase(pMove); // the move won't get us out of check. Erase it.
                }
            }
            else if (!squareIsBetweenSquares(*attackingKingMoves.begin(), moveEndFile, moveEndRank))
            {                                                 // if the piece is not a knight and your possible move cannot get between the attacker or capture it
                pMove = pcompletePlayerMoveset->erase(pMove); // throw out the move
            }
            else
            {
                ++pMove; // The move goes between king and the attacking piece, blocking the check. So leave it in the moveset
            }
        }
    }
    // Writes out list of moves:
    // cout << "Moveset List for " << playerColor << std::endl;
    // for (auto it = completePlayerMoveset->begin(); it != completePlayerMoveset->end(); ++it) {
    //    std::cout << *it << ", ";
    //}
    // cout << std::endl;

    if (addCastling)
    {
        pcompletePlayerMoveset->insert(pcompletePlayerMoveset->end(), castlingMoveset.begin(), castlingMoveset.end()); // add filtered moveset to complete moveset
    }

    return;

    // Check the moves and remove invalid ones here
    //  to know if move is invalid
} // Fast move calculation

// Public functions excluding constructor and destructor
// const CONSTANTS::Piece  ()[8][8] TextBoard::getBoardState(){
//    return &m_board;
//}

TextBoard::board* TextBoard::getBoardState()
{ // This one is hard!  I want this to return a read only pointer to board state
    return &m_board;
}
bool TextBoard::editBoard(int file, int rank, CONSTANTS::Piece newPiece)
{
    m_board[rank][file] = newPiece;
    return true;
};
// This function is a big deal. It executes thousands of times. Can optimize. BUG: Not resetting castling
void TextBoard::undoLastMove(){ 
    // Reset to the previous player turn
    m_playerTurn = (m_playerTurn == CONSTANTS::Color::WHITE) ? CONSTANTS::Color::BLACK : CONSTANTS::Color::WHITE;

    // m_boardHistory.pop();
    std::string move = m_moveHistory.front();
    if(move[0] == 'O'){ // equivalent to if(move == "O-O" || move == "O-O-O"){ for castling
        if(move.size() == 3){ // equivalent to if(move.size = "O-O")
            if(m_playerTurn == CONSTANTS::Color::WHITE){
                movePiece(CONSTANTS::Square::B1, CONSTANTS::Square::D1);
                movePiece(CONSTANTS::Square::C1, CONSTANTS::Square::A1);
                m_whiteKingMoved = false;
                m_whiteARookMoved = false;
            }
            else if(m_playerTurn == CONSTANTS::Color::BLACK){
                movePiece(CONSTANTS::Square::B8, CONSTANTS::Square::D8);
                movePiece(CONSTANTS::Square::C8, CONSTANTS::Square::A8);
                m_blackKingMoved = false;
                m_blackARookMoved = false;
            }
        }
        else if(move.size() == 5){ // equivalent to if(move.size = "O-O-O")
            if(m_playerTurn == CONSTANTS::Color::WHITE){
                movePiece(CONSTANTS::Square::F1, CONSTANTS::Square::D1);
                movePiece(CONSTANTS::Square::E1, CONSTANTS::Square::H1);
                m_whiteKingMoved = false;
                m_whiteHRookMoved = false;
            }
            else if(m_playerTurn == CONSTANTS::Color::BLACK){
                movePiece(CONSTANTS::Square::F8, CONSTANTS::Square::D8);
                movePiece(CONSTANTS::Square::E8, CONSTANTS::Square::H8);
                m_blackKingMoved = false;
                m_blackHRookMoved = false;
            }
        }
    }
    else if(move.size() == 5){ // If the move was a normal move without pawn promotion
        char piece;
        int prevFile, prevRank, newFile, newRank;
        parseMove(move, piece, prevFile, prevRank, newFile, newRank); // Get the move variables in usable terms
        movePiece(newFile, newRank, prevFile, prevRank); // Move the capturing piece back to where it was
        if(m_captureHistory.front() != CONSTANTS::Piece::EMPTY){ // Check if a piece was captured in the capture history
            addPiece(newFile, newRank, m_captureHistory.front()); // Put the piece back on the board
        }
        if(piece == 'K'){
            for(auto move : m_moveHistory.front(), m_moveHistory.()){

            }
        }
    }
    else if(move.size() == 6){ // If the move was a pawn promotion move
        char piece, promotion;
        int prevFile, prevRank, newFile, newRank;
        parseMove(move, piece, prevFile, prevRank, newFile, newRank); // Get the move variables in usable terms
        CONSTANTS::Piece demotion = (m_playerTurn == CONSTANTS::Color::WHITE) ? CONSTANTS::Piece::WPAWN : CONSTANTS::Piece::BPAWN;
        movePiece(newFile, newRank, prevFile, prevRank, demotion); // Move the capturing piece back to where it was and demote it to a pawn
        if(m_captureHistory.top() != CONSTANTS::Piece::EMPTY){ // Check if a piece was captured in the capture history
            addPiece(newFile, newRank, m_captureHistory.top()); // Put the piece back on the board
        }
    }
    m_moveHistory.pop(); // We've handled the move, remove it from history
    m_captureHistory.pop(); // Remove the capture from the history
    // m_whiteIndexHistory.pop();
    // m_whitePieceIndices = m_whiteIndexHistory.top();
    // m_blackIndexHistory.pop();
    // m_blackPieceIndices = m_blackIndexHistory.top();
    // for (int idx = 0; idx < 64; ++idx){
    //     m_board[0][idx] = m_boardHistory.top()[idx];
    // }
    findPlayerPieces(true); // We need to re-update the location of all the pieces after moving them back. Slow but accomplishes goals for now.
    return;
};
std::list<std::string> *TextBoard::getLegalMoves(CONSTANTS::Color color)
{
    switch (color)
    {
    case CONSTANTS::Color::WHITE:
        return &m_whiteMoves;
    case CONSTANTS::Color::BLACK:
        return &m_blackMoves;
    default:
        std::cout << "ERROR: Invalid color give to function getLegalMoves(color)" << std::endl;
        return nullptr;
    }
}
// bool TextBoard::isWinner(CONSTANTS::Color winnerColor){
//     //If current players moveset is empty. Report winner is the opposing player!
// } // Reports if white or black is the winner when asked

// Reports if move was successful and board was modified
bool TextBoard::makeMove(std::string move)
{

    int prevFile = 0;
    int prevRank = 0;
    int newFile = 0;
    int newRank = 0;
    char pieceType;
    CONSTANTS::Color pieceColor = CONSTANTS::Color::INVALID;
    char promotionChoice = 'P';
    bool promotePawn = false;
    // int prevKingIndex = 0;
    // int newKingIndex = 0;
    // int prevRookIndex = 0;
    // int newRookIndex = 0;

    if (!(move.size() == 5 || move.size() == 6 || move == "O-O" || move == "O-O-O"))
    {
        std::cout << "Function passed bad move" << std::endl;
        return false;
    }

    if (!(move == "O-O" || move == "O-O-O"))
    { // If we are not castling

        TextBoard::parseMove(move, pieceType, prevFile, prevRank, newFile, newRank); // parse the move
        pieceColor = getPieceColor(prevFile, prevRank);

        if (move.size() == 6)
        { // If we are promoting a piece, set the flag.
            promotionChoice = move[5];
            promotePawn = true;
        }

        // ################### modifies the board with the move the player chose #########################

        /*
         Piece destination = m_board[newRank][newFile];
         int destinationColor = getPieceColor(newFile, newRank);
         char destinationType = getPieceType(newFile, newRank);
         prevIndex = Piece::convertCoordinateToBoardIndex(prevFile, prevRank);
         newIndex = Piece::convertCoordinateToBoardIndex(newFile, newRank);
         */
        if (pieceType == 'P' && abs(newFile - prevFile) == 1)
        { // If a pawn is capturing
            if (getPieceColor(newFile, newRank) == CONSTANTS::Color::EMPTY)
            {                                   // Check for enpassant
                removePiece(newFile, prevRank); // Enpassant confirmed, delete the enemy pawn
            }
        }

        CONSTANTS::Color capturedPieceColor = getPieceColor(newFile, newRank);

        if (!promotePawn)
        { // If we are not promoting a pawn

            movePiece(prevFile, prevRank, newFile, newRank); // Move the piece to the new location
            if (!m_whiteKingMoved || !m_blackKingMoved)
            {
                if (pieceColor == CONSTANTS::Color::WHITE)
                {
                    if (pieceType == 'K')
                    {
                        m_whiteKingMoved = true;
                    }
                    else if (pieceType == 'R')
                    {
                        if (!m_whiteHRookMoved || !m_whiteARookMoved)
                        {
                            if (prevFile == 0)
                            {
                                m_whiteARookMoved = true;
                            }
                            if (prevFile == 7)
                            {
                                m_whiteHRookMoved = true;
                            }
                        }
                    }
                }
                else if (pieceColor == CONSTANTS::Color::BLACK)
                {
                    if (pieceType == 'K')
                    {
                        m_blackKingMoved = true;
                    }
                    else if (pieceType == 'R')
                    {
                        if (!m_blackHRookMoved || !m_blackARookMoved)
                        {
                            if (prevFile == 0)
                            {
                                m_blackARookMoved = true;
                            }
                            if (prevFile == 7)
                            {
                                m_blackHRookMoved = true;
                            }
                        }
                    }
                }
            }
        }
        else
        { // If we are promoting a pawn
            if (pieceColor == CONSTANTS::Color::WHITE)
            {
                switch (promotionChoice)
                { // Check the pieces from most commonly moved to least common and assign the piece
                case 'Q':
                    movePiece(prevFile, prevRank, newFile, newRank, CONSTANTS::Piece::WQUEEN);
                    break;
                case 'N':
                    movePiece(prevFile, prevRank, newFile, newRank, CONSTANTS::Piece::WKNIGHT);
                    break;
                case 'B':
                    movePiece(prevFile, prevRank, newFile, newRank, CONSTANTS::Piece::WBISHOP);
                    break;
                case 'R':
                    movePiece(prevFile, prevRank, newFile, newRank, CONSTANTS::Piece::WROOK);
                    break;
                default:
                    std::cout << "ERROR: Attempting to assign invalid piece type to square" << std::endl;
                    return false;
                }
            }
            else if (pieceColor == CONSTANTS::Color::BLACK)
            {
                switch (promotionChoice)
                { // Check the pieces from most commonly moved to least common
                case 'Q':
                    movePiece(prevFile, prevRank, newFile, newRank, CONSTANTS::Piece::BQUEEN);
                    break;
                case 'N':
                    movePiece(prevFile, prevRank, newFile, newRank, CONSTANTS::Piece::BKNIGHT);
                    break;
                case 'B':
                    movePiece(prevFile, prevRank, newFile, newRank, CONSTANTS::Piece::BBISHOP);
                    break;
                case 'R':
                    movePiece(prevFile, prevRank, newFile, newRank, CONSTANTS::Piece::BROOK);
                    break;
                default:
                    std::cout << "ERROR: Attempting to assign invalid piece type to square" << std::endl;
                    return false;
                }
            }
        }
    }
    else if (move == "O-O")
    {

        m_board;
        // finds the king on the board
        int prevKingIndex = findKingIndex(m_playerTurn); // This could be simplified because if we are doing a castling, we already know where the king is, we just have to know which king
        int newKingIndex = prevKingIndex - 2;
        int prevRookIndex = prevKingIndex - 3;
        int newRookIndex = newKingIndex + 1;

        movePiece(prevKingIndex, newKingIndex);
        movePiece(prevRookIndex, newRookIndex);

        if (pieceColor == CONSTANTS::Color::WHITE)
        {
            m_whiteKingMoved = true;
            m_whiteARookMoved = true;
        }
        else if (pieceColor == CONSTANTS::Color::BLACK)
        {
            m_blackKingMoved = true;
            m_blackARookMoved = true;
        }
    }
    else if (move == "O-O-O")
    {

        // finds the king on the board
        int prevKingIndex = findKingIndex(m_playerTurn); // This could be simplified because if we are doing a castling, we already know where the king is, we just have to know which king
        int newKingIndex = prevKingIndex + 2;
        int prevRookIndex = prevKingIndex + 4;
        int newRookIndex = newKingIndex - 1;

        movePiece(prevKingIndex, newKingIndex);
        movePiece(prevRookIndex, newRookIndex);

        if (pieceColor == CONSTANTS::Color::WHITE)
        {
            m_whiteKingMoved = true;
            m_whiteHRookMoved = true;
        }
        else if (pieceColor == CONSTANTS::Color::BLACK)
        {
            m_blackKingMoved = true;
            m_blackHRookMoved = true;
        }
    }

    CONSTANTS::Color nextPlayer;
    if (m_playerTurn == CONSTANTS::Color::WHITE)
    {
        nextPlayer = CONSTANTS::Color::BLACK;
    }
    else
        nextPlayer = CONSTANTS::Color::WHITE;

    // Need to update the move history for the move
    // std::vector<CONSTANTS::Piece> v_board(64);
    // for (int idx = 0; idx < 64; ++idx){
    //     v_board[idx] = m_board[0][idx];
    // }
    // m_boardHistory.push(v_board);
    m_moveHistory.push(move);
    calcPlayerMovesetV2(nextPlayer, true); // Update the potential moves lists for proper color

    m_playerTurn = nextPlayer;
    return true;
}

////Archived Function:
// bool updatePiecesArray(std::string move, bool performedCapture = false, int capturedFile = -1, int capturedRank = -1){ // The updating process of the indexes is just too simplified in some cases and complex in others to warrant an effective function that can handle all conditions. It could be done, but is impractical.
//
//     char pieceType;
//     int prevRank, newRank, prevFile, newFile;
//
//     int capturedIndex = -1;
//     CONSTANTS::Color capturedPieceColor = -1;
//
//
//     parseMove(move, &pieceType,  &prevFile, &prevRank, &newFile, &newRank);
//
//     int prevPieceIndex = convertCoordinateToBoardIndex(prevFile, prevRank);
//     int newPieceIndex = convertCoordinateToBoardIndex(newFile, newRank)
//     CONSTANTS::Color pieceColor = getPieceColor(newFile, newRank);
//
//
//     if(performedCapture){
//     capturedIndex = convertCoordinateToBoardIndex(capturedFile, capturedRank);
//     capturedPieceColor = getPieceColor(capturedFile,capturedRank);
//     }
//
//     if(pieceColor == CONSTANTS::Color::WHITE){
//         auto it = std::find(m_whitePieceIndices.begin(), m_whitePieceIndices.end(),prevPieceIndex);
//         *it = newPieceIndex; // Once we find the pointer to the old index, point to the new index
//         if(performedCapture){
//             auto itB = std::find(m_blackPieceIndices.begin(), m_blackPieceIndices.end(),capturedIndex);
//             //if(itB != m_blackPiecesIndicies.end()){
//                 itB.erase(); THIS HAS TO BE FIXED TO itB = list.erase(itB)
//             //}
//         }
//     }
//     else if (pieceColor == CONSTANTS::Color::BLACK){
//         auto it = std::find(m_blackPieceIndices.begin(), m_blackPieceIndices.end(),prevPieceIndex);
//         *it = newPieceIndex;
//         if(performedCapture){
//             auto itW = std::find(m_whitePieceIndicies.begin(), m_whitePieceIndicies.end(),capturedIndex);
//             //if(itW != m_whitePiecesIndicies.end()){ // It's okay to comment this out because I told it that it would be there
//                 itW.erase();  THIS HAS TO BE FIXED TO itW = list.erase(itW)
//             //}
//
//         }
//     }
//     else {
//         std::cout << "ERROR, invalid piece location (EMPTY color) given in updatePiecesArray() board" << std::endl;
//         return false;
//     }
//
//     return true; // Successfully updated both array of piece indexes
//
// }
