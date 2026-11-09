#include "StudentAI.h"
#include <random>

//The following part should be completed by students.
//The students can modify anything except the class name and exisiting functions and varibles.
// Your AI. Keeps its own copy of the board, set up for a new game. Defaults to player 2 until
// GetMove sees it's going first.
StudentAI::StudentAI(int col,int row,int p)
	:AI(col, row, p)
{
    board = Board(col,row,p);
    board.initializeGame();
    player = 2;
}

// Called once per turn with the opponent's last move (empty on the very first turn = we're player 1).
// Applies that move to our board, gets all legal moves, and picks one. Right now it picks at random;
// replace this with your search. Applies our move to our board and returns it.
Move StudentAI::GetMove(Move move)
{
    if (move.seq.empty())
    {
        player = 1;
    } else{
        board.makeMove(move,player == 1?2:1);
    }

    vector<vector<Move> > moves = board.getAllPossibleMoves(player);
    vector<Move> flat_moves = flatten(moves);

    Node * root = new Node(player, flat_moves);

    auto start_time = chrono::steady_clock::now();
    auto duration   = chrono::seconds(1);
 


    /*
    int i = rand() % (moves.size());
    vector<Move> checker_moves = moves[i];
    int j = rand() % (checker_moves.size());
    Move res = checker_moves[j];
    board.makeMove(res,player);
    return res;
    */

}

vector<Move> StudentAI::flatten(const vector<vector<Move> > & groups)
{
    vector<Move> ret;
    size_t size = accumulate(groups.begin(), groups.end(), 0ULL,
        [](size_t sum, const vector<Move>& inner) 
        {
            return sum + inner.size();
        });

    ret.reserve(size);
    for (const auto& move : groups)
    {
       ret.insert(ret.end(), move.begin(), move.end());
    }
    return ret;
}

void StudentAI::undoMoves(int movesMade) {
    while (movesMade > 0) 
    {
        board.Undo();
        movesMade--;
    }
}

