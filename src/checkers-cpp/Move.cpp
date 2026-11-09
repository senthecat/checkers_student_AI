#include "Move.h"


// Empty move (no positions). Used to mean "no previous move" / "you go first".
Move::Move()
{
}

// Copy constructor: copies the position sequence.
Move::Move(const Move & move)
{
    this->seq = move.seq;
}

// Builds a move from a list of positions: start square, then each landing square.
Move::Move(const vector<Position> seq)
{
    this->seq = seq;
}

// Parses move text like "(5,0)-(4,1)" or "(2,1)-(4,3)-(6,1)" into positions.
// "-1" gives an empty move. Expects no spaces.
Move::Move(const string & input)
{
    int len = input.length();
    if (input == "-1"){
        return;
    }

    string delimiter = "-";
    vector<string> points = split(input,delimiter);
    for (int i = 0; i<points.size();++i)
    {
        string point = points[i];
        string result;
        for (int j = 1; j<point.length()-1;++j)
        {
            result += point[j];
        }
        vector<string> xy = split(result,",");
        int x = stoi(xy[0]);
        int y = stoi(xy[1]);
        Position coordinate{x,y};
        seq.push_back(coordinate);


    }

}

// Helper: splits a string on a delimiter, e.g. "a-b-c" on "-" gives {"a","b","c"}.
vector<string> Move::split(string input,const string delimiter){
    vector<string> result;
    size_t pos = 0;
    string token;
    while ((pos = input.find(delimiter)) != string::npos) {
        token = input.substr(0, pos);
        result.push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    result.push_back(input);
    return result;
}

// Formats the move as "(r,c)-(r,c)-...", the same text format the parser above reads.
string Move::toString()
{
    string result;
    for (int i = 0;i<seq.size();++i)
    {
        result += "("+to_string(seq[i][0])+","+to_string(seq[i][1])+")";
        if (i != seq.size()-1)
        {
            result += "-";
        }
    }
    return result;
}

// True if the move jumps a piece: more than 2 positions (multi-jump), or a first step of 2 rows.
bool Move::isCapture()
{
    if (this->seq.size()>2)
        return true;
    return abs(seq[0][0]-seq[1][0]) > 1;
}