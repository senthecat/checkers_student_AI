#include "GameLogic.h"

// Stores the board size, piece rows, mode and turn order, and creates the empty list of players.
GameLogic::GameLogic(int col, int row, int p, string mode,int order)
{
	this->col = col;
	this->row = row;
	this->p = p;
	this->mode = mode;
	this->order = order;
	this->aiList = new vector<AI*>();
}

// Referee loop for manual and self-play modes. Asks the current player's AI for a move, applies it
// to the referee's own board (an invalid move loses), prints the board, checks isWin, then switches
// players. Prints the winner or "Tie" at the end.
void GameLogic::Manual()
{
	int player = 1;
	int winPlayer = 0;
	bool init = true;
	Move move;
	Board board(col, row, p);
        board.initializeGame();
        board.showBoard();
	while (true)
	{
		move = ((*aiList)[player - 1])->GetMove(move);
		cout << move.toString() << endl;
		try
		{
			board.makeMove(move, player);
		}
		catch (InvalidMoveError)
		{
			cerr << "Invalid Move!" << endl;
			winPlayer = player == 1 ? 2 : 1;
			break;
		}
		winPlayer = board.isWin(player);
		board.showBoard();
		if (winPlayer != 0)
		{
			break;
		}
		player = player == 1 ? 2 : 1;

	}
	if (winPlayer == -1)
	{
	    cout<< "Tie"<<endl;
	}
	else
	{
	    cout << "Player " << winPlayer << " wins!" << endl;
	}

}

// Tournament mode: runs a StudentAI over stdin/stdout. Each loop reads the opponent's move as one
// token ("-1" = no move yet, so we go first; otherwise "(r,c)-(r,c)..."), and prints StudentAI's reply.
void GameLogic::TournamentInterface()
{
	StudentAI ai(col, row, p);
	while (true)
	{
		string instr;
		cin >> instr;
		Move result = ai.GetMove(Move(instr));
		cout << result.toString()<< endl;
	}
}

// Picks the two players based on mode/order (StudentAI + ManualAI, or two StudentAIs), then starts
// Manual(); in mode "t" it starts TournamentInterface() instead.
void GameLogic::Run()
{
	if (mode == "m" or mode == "manual")
	{
        AI* studentai = new StudentAI(col, row, p);
        AI* manualai = new ManualAI(col, row, p);
		if (order == 1)
        	{
            		aiList->push_back(manualai);
            		aiList->push_back(studentai);
        	}
        	else
        	{
            		aiList->push_back(studentai);
          	  	aiList->push_back(manualai);
        	}

		Manual();
	}
	else if (mode == "s" or mode == "self")
	{
		AI* studentai = new StudentAI(col, row, p);
        AI* manualai = new StudentAI(col, row, p);
		if (order == 1)
        	{
            		aiList->push_back(manualai);
            		aiList->push_back(studentai);
        	}
        	else
        	{
            		aiList->push_back(studentai);
          	  	aiList->push_back(manualai);
        	}

		Manual();
	}
	else if (mode == "t")
	{
		TournamentInterface();
	}
}



// Deletes the AI players created in Run().
GameLogic::~GameLogic()
{
	while(!aiList->empty())
	{
		delete aiList->back();
		aiList->pop_back();
	}
}
