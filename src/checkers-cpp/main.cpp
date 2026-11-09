//#include "GameLogic.h"

#include "GameLogic.h"

// Program entry point. Usage: ./main {col} {row} {p} {mode} [order]
//   mode: m/manual (you vs StudentAI), s/self (StudentAI vs itself), t (tournament: moves over stdin/stdout)
//   order (m/s only): 0 = StudentAI is player 1, 1 = the other AI is player 1
// Builds a GameLogic with those settings and runs it.
int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		cout << "Invalid Parameters" << endl;
		return 0;
	}
	//mode="m"->manual/"t"->tournament
	int col = atoi(argv[1]);
	int row = atoi(argv[2]);
	int p = atoi(argv[3]);
	string mode = argv[4];
	int order = 0;
    if (mode == "m" || mode == "manual"|| mode == "s"|| mode == "self")
    {
        order = atoi(argv[5]);
    }
	GameLogic main(col,row,p, mode, order);//col,row,p,g,mode,debug
	main.Run();

	return 0;
}
