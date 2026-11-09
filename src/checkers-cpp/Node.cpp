#include "Node.h"

// Constructor used for iterations
Node::Node(int player, const Move& move, Node * parent)
    : player(player), parent(parent), move(move)   // same order as declared in Node.h
{
}

// Deletes every child; each child's destructor deletes its own children, so `delete root` frees the tree.
Node::~Node()
{
    for (Node* child : children)
    {
        delete child;
    }
}

bool Node::isTerminal() const 
{
    return terminal != 0;
}

bool Node::isFullyExpanded() const 
{
    return untried.empty();
}

int Node::toMove() const 
{
    return 3 - player;
}

double Node::uct(double c) const
{
    return (this->wins / this->visits) + 
        c * sqrt(log(this->parent->visits) / this->visits);
}

Node* Node::bestUCTChild(double c) const
{
    double maxUCT = -1;
    Node* res = nullptr;
    for (Node* child : children)
    {
        double uct = child->uct(c);
        if (uct > maxUCT)
        {
            maxUCT = uct;
            res = child;
        }
    }
    return res;
}

Node* Node::mostVisitedChild() const
{
    int maxVisits = -1;
    Node* res = nullptr;
    for (Node* child : children)
    {
        if (child->visits > maxVisits)
        {
            maxVisits = child->visits;
            res = child;
        }
    }
    return res;
}

Node* Node::addChild(const Move& m, int mover) 
{
    Node* child = new Node(mover, m, this);
    this->children.push_back(child);

    return child;
}

void Node::update(double scoreForPlayer1) 
{
    this->visits++;
    if(this->player == 1)
    {
        this->wins += scoreForPlayer1; 
    } else 
    {
        this->wins += 1 - scoreForPlayer1;
    }
}