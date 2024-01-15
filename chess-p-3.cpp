#include <windows.h>
#include <iostream>
using namespace std;

const int BOARD_SIZE = 8;
bool makeMove(char board[BOARD_SIZE][BOARD_SIZE], int startX, int startY, int endX, int endY, int turn);


int switchTurn(int turn)
 {
    return (turn + 1) % 2;
}
bool isValidCoordinate(int x, int y) 
{
    return (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE);
}

char getPiece(const char board[BOARD_SIZE][BOARD_SIZE], int x, int y) {
    if (isValidCoordinate(x, y))
    {
        return board[y][x];
    }
    return ' ';
}

bool getMove(const char board[BOARD_SIZE][BOARD_SIZE], int& startX, int& startY, int& endX, int& endY, int turn) 
{
    cout << "Player " << (turn + 1) << ", enter your move (from-to notation, e.g., e4-f5): ";
    string input;
    cin >> input;

    if (input.length() != 5) 
    {
        return false; 
    }

    startX = tolower(input[0]) - 'a';
    startY = input[1] - '1';
    endX = tolower(input[3]) - 'a';
    endY = input[4] - '1';

    if (!isValidCoordinate(startX, startY) || !isValidCoordinate(endX, endY))
    {
        return false; 
    }

    char selectedPiece = getPiece(board, startX, startY);
    if (tolower(selectedPiece) != ' ' && (turn == 0 && islower(selectedPiece)) || (turn == 1 && isupper(selectedPiece))) 
    {
        return false;
    }

    return true;
}



void init(char board[8][8])
{
    char initialPieces[8] = { 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R' };
    for (int i = 0; i < 8; ++i)
    {
        board[0][i] = initialPieces[i];
        board[1][i] = 'P';
        board[6][i] = 'p';
        board[7][i] = tolower(initialPieces[i]);
        for (int j = 2; j < 6; ++j)
        {
            board[j][i] = ' ';
        }
    }
}

void displayBoard(const char board[BOARD_SIZE][BOARD_SIZE]) 
{
    cout << "   a   b   c   d   e   f   g   h" << endl;
    cout << "  ---------------------------------" << endl;
    for (int i = BOARD_SIZE - 1; i >= 0; --i) {
        cout << i + 1 << " |";
        for (int j = 0; j < BOARD_SIZE; ++j) {
            cout << " " << board[i][j] << " |";
        }
        cout << " " << i + 1 << endl;
        cout << "  ---------------------------------" << endl;
    }
    cout << "    a   b   c   d   e   f   g   h" << endl;
}

bool isHorizontalMove(int startX, int startY, int endX, int endY)
{
    return (startX == endX);
}

bool isVerticalMove(int startX, int startY, int endX, int endY)
{
    return (startY == endY);
}

bool isDiagonalMainMove(int startX, int startY, int endX, int endY)
{
    return (abs(startX - endX) == abs(startY - endY)) && (startX != endX);
}

bool isDiagonalSecMove(int startX, int startY, int endX, int endY)
{
    return (startX + startY == endX + endY) && (startX != endX);
}

bool isLegalPawn(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char pawn = board[startX][startY];
    int direction = (turn == 0) ? 1 : -1;
    if (startY == endY)
    {
        if (startX + direction == endX && board[endX][endY] == ' ')
        {
            return true;
        }
        if (startX + 2 * direction == endX && startX == (turn == 0 ? 1 : 6) && board[startX + direction][endY] == ' ' && board[endX][endY] == ' ')
        {
            return true;
        }
    }

    if (abs(startY - endY) == 1 && startX + direction == endX)
    {
        if (islower(board[endX][endY]) && turn == 0)
        {
            return true;
        }
        if (isupper(board[endX][endY]) && turn == 1)
        {
            return true;
        }
    }
    if (abs(startY - endY) == 1 && startX + direction == endX) 
    {
        if ((islower(board[endX][endY]) && turn == 0) || (isupper(board[endX][endY]) && turn == 1))
        {
            return true; 
        }
    }

    return false;
}


bool isLegalKing(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char king = board[startX][startY];

    if (abs(startX - endX) <= 1 && abs(startY - endY) <= 1)
    {
        if (isValidCoordinate(endX, endY)) 
        {
            if ((turn == 0 && (!islower(board[endX][endY]) || board[endX][endY] == ' ')) ||
                (turn == 1 && (!isupper(board[endX][endY]) || board[endX][endY] == ' ')))
            {
                return true; 
            }
        }
    }

    return false; 
}



bool isLegalQueen(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char queen = board[startX][startY];

    if ((startX == endX || startY == endY || abs(startX - endX) == abs(startY - endY)) && (startX != endX || startY != endY))
    {
        int dx = (endX - startX > 0) ? 1 : (endX - startX < 0) ? -1 : 0;
        int dy = (endY - startY > 0) ? 1 : (endY - startY < 0) ? -1 : 0;

        int x = startX + dx;
        int y = startY + dy;

        while (x != endX || y != endY)
        {
            if (board[x][y] != ' ')
            {
                return false; 
            }
            x += dx;
            y += dy;
        }

        if (isValidCoordinate(endX, endY))
        {
            if ((turn == 0 && (!islower(board[endX][endY]) || board[endX][endY] == ' ')) ||
                (turn == 1 && (!isupper(board[endX][endY]) || board[endX][endY] == ' ')))
            {
                return true; 
            }
        }
    }

    return false;
}



bool isLegalBishop(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char bishop = board[startX][startY];

    if (abs(startX - endX) == abs(startY - endY) && (startX != endX || startY != endY))
    {
        int dx = (endX - startX > 0) ? 1 : -1;
        int dy = (endY - startY > 0) ? 1 : -1;

        int x = startX + dx;
        int y = startY + dy;

        while (x != endX || y != endY)
        {
            if (board[x][y] != ' ')
            {
                return false; 
            }
            x += dx;
            y += dy;
        }

        if (isValidCoordinate(endX, endY))
        {
            if ((turn == 0 && (!islower(board[endX][endY]) || board[endX][endY] == ' ')) ||
                (turn == 1 && (!isupper(board[endX][endY]) || board[endX][endY] == ' '))) 
            {
                return true; 
            }
        }
    }

    return false; 
}



bool isLegalRook(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char rook = board[startX][startY];

    if ((startX == endX || startY == endY) && (startX != endX || startY != endY))
    {
        int dx = (endX - startX > 0) ? 1 : (endX - startX < 0) ? -1 : 0;
        int dy = (endY - startY > 0) ? 1 : (endY - startY < 0) ? -1 : 0;

        if (dx != 0) {
            int x = startX + dx;

            while (x != endX) {
                if (board[x][startY] != ' ')
                {
                    return false; 
                }
                x += dx;
            }
        }
        else 
        {
            int y = startY + dy;

            while (y != endY) 
            {
                if (board[startX][y] != ' ')
                {
                    return false; 
                }
                y += dy;
            }
        }

        if (isValidCoordinate(endX, endY))
        {
            if ((turn == 0 && (!islower(board[endX][endY]) || board[endX][endY] == ' ')) ||
                (turn == 1 && (!isupper(board[endX][endY]) || board[endX][endY] == ' ')))
            {
                return true; 
            }
        }
    }

    return false; 
}


bool isLegalKnight(int startX, int startY, int endX, int endY, const char board[8][8], int turn)
{
    char knight = board[startX][startY];

    if ((abs(startX - endX) == 2 && abs(startY - endY) == 1) || (abs(startX - endX) == 1 && abs(startY - endY) == 2))
    {
        if (isValidCoordinate(endX, endY)) 
        {
            if ((turn == 0 && (!islower(board[endX][endY]) || board[endX][endY] == ' ')) ||
                (turn == 1 && (!isupper(board[endX][endY]) || board[endX][endY] == ' ')))
            {
                return true; 
            }
        }
    }

    return false; 
}
bool isValidMove(int startX, int startY, int endX, int endY, const char board[BOARD_SIZE][BOARD_SIZE], int turn)
{
    char piece = board[startY][startX];
    switch (tolower(piece))
    {
    case 'p':
        return isLegalPawn(startY, startX, endY, endX, board, turn);
    case 'r':
        return isLegalRook(startY, startX, endY, endX, board, turn);
    case 'n':
        return isLegalKnight(startY, startX, endY, endX, board, turn);
    case 'b':
        return isLegalBishop(startY, startX, endY, endX, board, turn);
    case 'q':
        return isLegalQueen(startY, startX, endY, endX, board, turn);
    case 'k':
        return isLegalKing(startY, startX, endY, endX, board, turn);
    default:
        return false;
    }
}

bool isKingCaptured(const char board[8][8])
{
    bool whiteKing = false;
    bool blackKing = false;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j] == 'k')
            {
                blackKing = true;
            }
            else if (board[i][j] == 'K')
            {
                whiteKing = true;
            }
        }
    }

    return !blackKing || !whiteKing;
}

bool validateMove(char piece, int startX, int startY, int endX, int endY, const char board[BOARD_SIZE][BOARD_SIZE], int turn) {
    switch (tolower(piece)) {
    case 'p':
        return isLegalPawn(startY, startX, endY, endX, board, turn);
    case 'r':
        return isLegalRook(startY, startX, endY, endX, board, turn);
    case 'n':
        return isLegalKnight(startY, startX, endY, endX, board, turn);
    case 'b':
        return isLegalBishop(startY, startX, endY, endX, board, turn);
    case 'q':
        return isLegalQueen(startY, startX, endY, endX, board, turn);
    case 'k':
        return isLegalKing(startY, startX, endY, endX, board, turn);
    default:
        return false;
    }
}
//pawn promotion!!!!!!!!
void executeMove(char board[BOARD_SIZE][BOARD_SIZE], int startX, int startY, int endX, int endY, char promotionPiece)
{
    char piece = board[startY][startX];

    if ((piece == 'p' && endY == BOARD_SIZE - 1) || (piece == 'P' && endY == 0))
    {
        board[endY][endX] = promotionPiece;
    }
    else {
        board[endY][endX] = piece;
    }

    board[startY][startX] = ' ';
}

bool isKingInCheck(const char board[BOARD_SIZE][BOARD_SIZE], int kingX, int kingY, int turn)
{
    char opponentKingSymbol = (turn == 0) ? 'K' : 'k';

    for (int i = 0; i < BOARD_SIZE; ++i) 
    {
        for (int j = 0; j < BOARD_SIZE; ++j) 
        {
            char piece = board[i][j];

            if ((turn == 0 && islower(piece)) || (turn == 1 && isupper(piece))) 
            {
          
                bool canAttack = false;
                switch (tolower(piece))
                {
                case 'p':
                    if (turn == 0) 
                    {
                        if ((i + 1 == kingY && (j + 1 == kingX || j - 1 == kingX)) ||
                            (i - 1 == kingY && (j + 1 == kingX || j - 1 == kingX)))
                        {
                            canAttack = true;
                        }
                    }
                    else
                    {
                        if ((i - 1 == kingY && (j + 1 == kingX || j - 1 == kingX)) ||
                            (i + 1 == kingY && (j + 1 == kingX || j - 1 == kingX)))
                        {
                            canAttack = true;
                        }
                    }
                    break;
                case 'r':
                    if (i == kingY || j == kingX)
                    {
                        canAttack = true;
                    }
                    break;
                case 'n':
                    if ((std::abs(i - kingY) == 2 && std::abs(j - kingX) == 1) ||
                        (std::abs(i - kingY) == 1 && std::abs(j - kingX) == 2))
                    {
                        canAttack = true;
                    }
                    break;
                case 'b':
                    if (std::abs(i - kingY) == std::abs(j - kingX))
                    {
                        canAttack = true;
                    }
                    break;
                case 'q':
                    if (i == kingY || j == kingX || std::abs(i - kingY) == std::abs(j - kingX)) 
                    {
                        canAttack = true;
                    }
                    break;
                default:
                    break;
                }

                if (canAttack)
                {
                    int dx = (j > kingX) ? -1 : (j < kingX) ? 1 : 0;
                    int dy = (i > kingY) ? -1 : (i < kingY) ? 1 : 0;
                    int x = j + dx;
                    int y = i + dy;
                    bool blocked = false;

                    while (x != kingX || y != kingY)
                    {
                        if (board[y][x] != ' ')
                        {
                            blocked = true;
                            break;
                        }
                        x += dx;
                        y += dy;
                    }

                    if (!blocked) 
                    {
                        return true; 
                    }
                }
            }
        }
    }

    return false; 
}

bool isCheck(const char board[BOARD_SIZE][BOARD_SIZE], int turn)
{
    int kingX = -1, kingY = -1;
    char kingSymbol = (turn == 0) ? 'k' : 'K';

    for (int i = 0; i < BOARD_SIZE; ++i) 
    {
        for (int j = 0; j < BOARD_SIZE; ++j) 
        {
            if (board[i][j] == kingSymbol)
            {
                kingX = j;
                kingY = i;
                break;
            }
        }
        if (kingX != -1)
        {
            break;
        }
    }

    if (kingX == -1 || kingY == -1)
    {
        return false;
    }

    return isKingInCheck(board, kingX, kingY, turn);
}

bool isCheckmate(const char board[BOARD_SIZE][BOARD_SIZE], int turn) 
{
    char kingSymbol = (turn == 0) ? 'K' : 'k';
    int kingX = -1, kingY = -1;

    for (int i = 0; i < BOARD_SIZE; ++i) 
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j] == kingSymbol) 
            {
                kingX = j;
                kingY = i;
                break;
            }
        }
        if (kingX != -1)
        {
            break;
        }
    }

    if (isKingInCheck(board, kingX, kingY, turn))
    {
        for (int startY = 0; startY < BOARD_SIZE; ++startY)
        {
            for (int startX = 0; startX < BOARD_SIZE; ++startX)
            {
                if ((turn == 0 && islower(board[startY][startX])) || (turn == 1 && isupper(board[startY][startX])))
                {
                    for (int endY = 0; endY < BOARD_SIZE; ++endY)
                    {
                        for (int endX = 0; endX < BOARD_SIZE; ++endX)
                        {
                            if (isValidMove(startX, startY, endX, endY, board, turn))
                            {
                                char tempBoard[BOARD_SIZE][BOARD_SIZE];
                                std::memcpy(tempBoard, board, sizeof(tempBoard)); 

                                makeMove(tempBoard, startX, startY, endX, endY, turn);

                                if (!isKingInCheck(tempBoard, kingX, kingY, turn))
                                {
                                    return false; 
                                }
                            }
                        }
                    }
                }
            }
        }
        return true; 
    }

    return false; 
}


bool isStalemate(const char board[BOARD_SIZE][BOARD_SIZE], int turn)
{
    char kingSymbol = (turn == 0) ? 'K' : 'k';
    int kingX = -1, kingY = -1;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j] == kingSymbol) 
            {
                kingX = j;
                kingY = i;
                break;
            }
        }
        if (kingX != -1) 
        {
            break;
        }
    }

    if (!isKingInCheck(board, kingX, kingY, turn))
    {
        for (int startY = 0; startY < BOARD_SIZE; ++startY) 
        {
            for (int startX = 0; startX < BOARD_SIZE; ++startX) 
            {
                if ((turn == 0 && islower(board[startY][startX])) || (turn == 1 && isupper(board[startY][startX]))) 
                {
                    for (int endY = 0; endY < BOARD_SIZE; ++endY) 
                    {
                        for (int endX = 0; endX < BOARD_SIZE; ++endX) 
                        {
                            if (isValidMove(startX, startY, endX, endY, board, turn))
                            {
                                char tempBoard[BOARD_SIZE][BOARD_SIZE];
                                std::memcpy(tempBoard, board, sizeof(tempBoard)); 

                                makeMove(tempBoard, startX, startY, endX, endY, turn);

                                if (!isKingInCheck(tempBoard, kingX, kingY, turn))
                                {
                                    return false; 
                                }
                            }
                        }
                    }
                }
            }
        }
        return true; 
    }

    return false; 
}

bool makeMove(char board[BOARD_SIZE][BOARD_SIZE], int startX, int startY, int endX, int endY, int turn) 
{
    char selectedPiece = board[startY][startX];

    if (!isValidMove(startX, startY, endX, endY, board, turn)) 
    {
        return false;
    }

    char targetPiece = board[endY][endX];
    board[endY][endX] = selectedPiece;
    board[startY][startX] = ' '; 

    if (targetPiece != ' ')
    {
        cout << "Captured " << targetPiece << " at " << char(endX + 'a') << endY + 1 << "!" << endl;
    }

    return true;
}

int main()
{
    char board[BOARD_SIZE][BOARD_SIZE];
    init(board);

    bool gameOver = false;
    int turn = 0;

    while (!gameOver) {
        system("cls");
        displayBoard(board);

        cout << "Player " << (turn + 1) << "'s turn." << endl; 

        int startX, startY, endX, endY;
        while (!getMove(board, startX, startY, endX, endY, turn))
        {
            cout << "Invalid move. Please try again." << endl;
        }

        if (!makeMove(board, startX, startY, endX, endY, turn)) 
        {
            cout << "Illegal move. Please try again." << endl;
            continue;
        }

        gameOver = isKingCaptured(board) || isCheckmate(board, turn) || isStalemate(board, turn);

        if (!gameOver) 
        {
            turn = (turn + 1) % 2; 
        }
    }

    system("cls");
    displayBoard(board);

    if (gameOver) 
    {
        cout << "Game Over! Player " << ((turn + 1) % 2 + 1) << " wins!" << endl;
    }

    return 0;
}
