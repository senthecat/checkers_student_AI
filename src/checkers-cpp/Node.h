#ifndef NODE_H
#define NODE_H

#pragma once

#include <vector>
#include <cmath>
#include <Move.h>

using namespace std;
const double explorationP = 1.5;
class Node
{
public:
    int visits  = 0;
    int wins    = 0;
    int player;
    const Node * parent {nullptr};
    vector<Node *> children;
    Move move;
    vector<Move> untried;

    Node(int player);
    // Node(const Node & node); // copy constructor
    Node(const Node & parent, vector<Move> untried);
    ~Node();

    double uct();
};
#endif