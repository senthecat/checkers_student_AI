#include "Node.h"

// Constructor used for root
Node::Node(int player)
{
    this->player = player;
}

// Constructor used for iterations
Node::Node(const Node & parent, vector<Move> untried)
{
    this->visits = 1;
    this->wins   = 0;
    this->parent = &parent;
    this->untried = untried;
}

double Node::uct()
{
    return (this->wins / this->visits) + 
        explorationP * sqrt(log(this->parent->visits) / this->visits);
}