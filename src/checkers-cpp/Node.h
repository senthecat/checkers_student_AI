#ifndef NODE_H
#define NODE_H

#pragma once

#include <vector>
#include <cmath>
#include "Move.h"

using namespace std;
class Node
{
public:
    int visits      = 0;
    double wins     = 0;    // double: ties add 0.5 and heuristic scores are fractions
    int terminal    = 0;
    int player;
    Node * parent;          
    vector<Node *> children;
    const Move move;
    vector<Move> untried;

    Node(int player, const Move& move, Node * parent);
    ~Node();

    bool isTerminal() const;
    bool isFullyExpanded() const;
    int toMove() const;

    double uct(double c) const;

    Node* bestUCTChild(double c) const;
    Node* mostVisitedChild() const;

    Node* addChild(const Move& m, int mover);
    void update(double scoreForPlayer1);

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
};
#endif