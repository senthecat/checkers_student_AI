#include "ManualAI.h"
#include <iostream>

// Human player. Keeps its own copy of the board, set up for a new game. Defaults to player 2
// until GetMove sees it's going first.
ManualAI::ManualAI(int col, int row, int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}

// Applies the opponent's last move to its own board (an empty move means we go first, as player 1),
// prints every legal move as "group : [index : move, ...]", reads two ints from cin (group, then
// index) until they're valid, plays that move on its own board and returns it.
Move ManualAI::GetMove(Move move)
{

    if (move.seq.empty())
    {
        player = 1;
    } else{
        board.makeMove(move, (player == 1) ? 2 : 1);
    }
    vector<vector<Move> > moves = board.getAllPossibleMoves(player);

    for (int i = 0; i < moves.size();i++)
    {
        cout<<i<<" : [";
        for (int j = 0; j < moves[i].size();j++)
        {
            cout<<j<<" : "<<moves[i][j].toString()<<", ";
        }
        cout<<"]"<<endl;
    }
    cout<<"Waiting input {int} {int}: ";
    int n = -1;
    int m = -1;
    do {
        cin >> n;
        cin >> m;
        if ((n < 0 || n >= moves.size()) || (m < 0 || m >= moves[n].size()))
            cout << "Invalid move" << "\n" << "Waiting for input {int} {int}: ";
    } while ((n < 0 || n >= moves.size()) || (m < 0 || m >= moves[n].size()));
    Move res = moves[n][m];
    board.makeMove(res,player);
    return res;
}

