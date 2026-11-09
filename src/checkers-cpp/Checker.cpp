//
// Created by Ziyang Zhang on 2019-05-01.
//

#include "Checker.h"
#include "Board.h"

// Lookup table from a color to the opposing color.
const map<string , string> Checker::opponent = {{"W","B"},{"B","W"}};

// One square's contents: color ("B", "W" or "." for empty; lowercase is turned into uppercase),
// its position, and not a king.
Checker::Checker(string color, int row, int col) {
    color = (int)color[0] <= 90? color : string("")+(char) ((int)color[0]-32);
    this->color = color;
    this->row = row;
    this->col = col;
    this->isKing = false;

}

// Promotes this piece to a king (can then move both ways).
void Checker::becomeKing(){
    this->isKing = true;
};

// Turns this piece back into a normal piece (also used when clearing a square).
void Checker::becomeMan(){
    this->isKing = false;
};

// Display character: uppercase B/W for a king, lowercase b/w for a normal piece, "." for empty.
string Checker::toString() {
    if (isKing){
        return color;
    }
    else
    {
        return color == "." ? color :  string("")+(char) ((int)color[0]+32);
    }
}

// All moves for this piece. Collects single diagonal steps into empty squares, then uses
// binary_tree_traversal to find every jump chain. If any jump exists, the simple steps are dropped
// (a piece that can capture must). Blanks its own square during the search so a king can loop back
// through where it started.
vector<Move> Checker::getPossibleMoves(Board* board)
{
    Direction direction;
    vector<Move> result;
    if (this->color == ".")
    {
        return result;
    }
    vector<vector<Position>> multiple_jump;

    vector<Position> explore_direction = direction[color];
    if (isKing)
    {
        for  (int i = 0; i<direction[opponent.at(color)].size(); i++)
        {
            explore_direction.push_back(direction[opponent.at(color)][i]);
        }
    }
    for (auto i = explore_direction.begin(); i != explore_direction.end(); i++) {
        int pos_x = row+(*i)[0];
        int pos_y = col+(*i)[1];
        if (board->isInBoard(pos_x,pos_y))
        {
            if (board->board[pos_x][pos_y].color == ".")
            {
                result.emplace_back(Move(vector<Position>{Position(row,col),Position(pos_x,pos_y)}));
            }
        }
    }
    vector<Position> temp_v;
    string self_color =  board->board[row][col].color;
    board->board[row][col].color = ".";
    binary_tree_traversal(row,col,multiple_jump, *board, explore_direction, temp_v,self_color);
    if (!multiple_jump.empty())
    {
        result.clear();
    }

    for (auto jump = multiple_jump.begin(); jump != multiple_jump.end(); jump++) {
        (*jump).insert((*jump).begin(),Position(row,col));
        result.emplace_back(Move(*jump));
    }

    board->board[row][col].color = self_color;
    return result;

}

// Recursive depth-first search for jump chains from (pos_x, pos_y). If no jump is possible here and
// we've jumped at least once, the path so far is a complete chain and is added to multiple_jump.
// Otherwise, for each direction with an enemy next to us and an empty square behind it: remove that
// enemy, add the landing square, recurse from there, then undo both (backtracking). Only full chains
// are recorded, so a multi-jump can't stop early.
void Checker::binary_tree_traversal(int pos_x,int pos_y,vector<vector<Position>> & multiple_jump,Board& board,vector<Position> direction,vector<Position>move, string self_color) {
    bool flag = true;
    for (auto i = direction.begin(); i != direction.end(); i++) {
        int temp_x = pos_x + (*i)[0];
        int temp_y = pos_y + (*i)[1];
        Position temp(temp_x, temp_y);
        if (board.isInBoard(temp_x, temp_y)
        && board.board[temp_x][temp_y].color == opponent.at(self_color)
            && board.isInBoard(temp_x + (*i)[0], temp_y + (*i)[1])
            && board.board[temp_x + (*i)[0]][temp_y + (*i)[1]].color == ".")
        {
            flag = false;
            break;
        }

    }
    if (flag) {
        if (!move.empty()) {
            multiple_jump.push_back(move);
            return;
        }

    }

    for (auto i = direction.begin(); i != direction.end(); i++) {
        int temp_x = pos_x + (*i)[0];
        int temp_y = pos_y + (*i)[1];
        Position temp(temp_x, temp_y);
        if (board.isInBoard(temp_x, temp_y) && board.board[temp_x][temp_y].color == opponent.at(self_color)){
            if (board.isInBoard(pos_x + (*i)[0] + (*i)[0], pos_y + (*i)[1] + (*i)[1]) &&
                board.board[pos_x + (*i)[0] + (*i)[0]][pos_y + (*i)[1] + (*i)[1]].color == ".") {
                Position temptemp{pos_x + (*i)[0] + (*i)[0], pos_y + (*i)[1] + (*i)[1]};
                string backup = board.board[pos_x + (*i)[0]][pos_y + (*i)[1]].color;
                board.board[pos_x + (*i)[0]][pos_y + (*i)[1]].color = ".";
                move.push_back(temptemp);
                this->binary_tree_traversal(temptemp[0], temptemp[1], multiple_jump, board, direction, move,self_color);
                move.pop_back();
                board.board[pos_x + (*i)[0]][pos_y + (*i)[1]].color = backup;
            }

        }

    }

}