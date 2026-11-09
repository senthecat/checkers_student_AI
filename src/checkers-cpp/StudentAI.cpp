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
    rng.seed(random_device{}()); 
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

    Node * root = new Node(3 - player, Move(), nullptr);
    root->untried = flatten(board.getAllPossibleMoves(player));

    Move ret;
    if (root->untried.size() == 1)
    {
        // only one legal move (common with forced captures): nothing to search
        ret = root->untried[0];
    }
    else
    {
        auto start_time = chrono::steady_clock::now();
        auto duration   = chrono::duration<double>(timeLimit);
        while (chrono::steady_clock::now() - start_time < duration)
        {
            int movesMade = 0;
            Node * n = select(root, movesMade);
            n = expand(n, movesMade);
            double s = simulate(n, movesMade);
            backpropagate(n, s);
            undoMoves(movesMade);
        }
        ret = root->mostVisitedChild()->move;
    }

    delete root;
    board.makeMove(ret, player);
    return ret;

}

Node* StudentAI::select(Node* root, int& movesMade)
{
    Node* node = root;
    while (!(node->isTerminal()) && node->isFullyExpanded() && !(node->children.empty()))
    {
        node = node->bestUCTChild(C);
        board.makeMove(node->move, node->player);
        movesMade++;
    }
    return node;
}

Node* StudentAI::expand(Node* node, int& movesMade)
{
    if (node->isTerminal() || node->untried.empty())
    {
        return node;
    }

    int i = rng() % node->untried.size();
    Move m = (node->untried)[i];
    node->untried[i] = node->untried.back();
    node->untried.pop_back();

    int mover = node->toMove();
    board.makeMove(m, mover);   
    movesMade++;
    Node * child = node->addChild(m, mover);
    child->terminal = board.isWin(mover);
    
    if (child->terminal == 0)
    {
        child->untried = flatten(board.getAllPossibleMoves(child->toMove()));
    }
    return child;
}

double StudentAI::simulate(Node* node, int& movesMade)
{
    int result = node->terminal;
    int toMove = node->toMove();
    int depth = 0;
    
    while (result == 0 && depth < maxRollout) 
    {
        auto moves = flatten(board.getAllPossibleMoves(toMove));
        if (moves.empty())
        {
            result = 3 - toMove;
            break;
        }
        auto m = moves[rng() % moves.size()];
        board.makeMove(m, toMove); movesMade++;
        result = board.isWin(toMove);
        toMove = 3 - toMove;
        depth++;
    }
    if (result == 1) return 1.0;
    if (result == 2) return 0.0;
    if (result == -1) return 0.5;
    return heuristicScore();    
}

void StudentAI::backpropagate(Node* node, double scoreForPlayer1)
{
    while (node != nullptr)
    {
        node->update(scoreForPlayer1);
        node = node->parent;
    }
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

// Estimate of how good the current board is for PLAYER 1 (black), from 0 to 1: black's share of the
// material, with kings worth more. Used when a rollout hits maxRollout without the game ending. This was suggested by Gen AI, but I understand why it would be important.
double StudentAI::heuristicScore()
{
    const double KING = 1.5;
    double b = 0, w = 0;
    for (int r = 0; r < board.row; r++)
    {
        for (int c = 0; c < board.col; c++)
        {
            const Checker& piece = board.board[r][c];
            double value = piece.isKing ? KING : 1.0;
            if (piece.color == "B") b += value;
            else if (piece.color == "W") w += value;
        }
    }
    if (b + w == 0) return 0.5;
    return b / (b + w);
}

void StudentAI::undoMoves(int movesMade) {
    while (movesMade > 0) 
    {
        board.Undo();
        movesMade--;
    }
}

