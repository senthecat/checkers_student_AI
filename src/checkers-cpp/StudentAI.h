#ifndef STUDENTAI_H
#define STUDENTAI_H
#include <chrono>
#include <numeric>
#include <random>
#include "AI.h"
#include "Board.h"
#include "Node.h"
#pragma once

//The following part should be completed by students.
//Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI :public AI
{
public:
    Board board;
	StudentAI(int col, int row, int p);
	virtual Move GetMove(Move board);

private:
	mt19937 rng;            // seeded once in the constructor
	double C = 1.5;
	double timeLimit = 1.0;
	int maxRollout = 80;

	Node* select(Node* root, int& movesMade);
	Node* expand(Node* node, int& movesMade);
	double simulate(Node* node, int& movesMade);
	void backpropagate(Node* node, double scoreForPlayer1);

	vector<Move> flatten(const vector<vector<Move> > & groups);
	double heuristicScore();
	void undoMoves(int movesMade);
};

#endif //STUDENTAI_H
