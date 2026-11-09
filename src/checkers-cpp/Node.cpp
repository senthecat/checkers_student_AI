#include "Node.h"


Node::Node(int player, vector<Move> untried)
{
    this->player    = player;
    this->untried   = untried;
}

// Constructor used for iterations
Node::Node(int player, vector<Move> untried, const Node & parent)
{
    this->player    = player;
    this->parent    = &parent;
    this->untried   = untried;
}

double Node::uct()
{
    return (this->wins / this->visits) + 
        explorationParam * sqrt(log(this->parent->visits) / this->visits);
}