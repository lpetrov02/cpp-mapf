#include "astarNodes.h"


int manhattan(int i1, int j1, int i2, int j2) {
    return std::abs(i1 - i2) + std::abs(j1 - j2);
}


// BASE NODE
BaseNode::BaseNode(int i, int j) : _i{i}, _j{j} {}

std::tuple<int, int> BaseNode::getTuple() const {
    return std::tuple<int, int>(_i, _j);
}

bool operator==(BaseNode const& first, BaseNode const& second) {
    return first._i == second._i && first._j == second._j;
}

bool operator<(BaseNode const& first, BaseNode const& second) { 
    if (first._i == second._i)
        return first._j < second._j;
    return first._i < second._i;
}

std::ostream& operator<<(std::ostream& os, BaseNode const& baseNode) {
    os << baseNode._i << " " << baseNode._j;
    return os;
}

std::size_t hashBaseNode::operator()(BaseNode const& s) const {
     auto ij = s.getTuple();
     return std::hash<int>{}(std::get<0>(ij)) ^ (std::hash<int>{}(std::get<1>(ij)) << 1);
}

std::size_t hashBaseNodeInt::operator()(std::tuple<BaseNode, int> const& s) const {
     return hashBaseNode{}(std::get<0>(s)) ^ (std::hash<int>{}(std::get<1>(s)) << 1);
}


// NODE
Node::Node(int i, int j, int h, std::tuple<int, int, int> parentTuple) 
    : _i{i}, _j{j}, _h{h}, _t{0}, _node{BaseNode(i, j)}, _parentTuple{parentTuple} {
    if (std::get<2>(_parentTuple) != -1) {
        _t = std::get<2>(_parentTuple) + 1;
        _f = _t + _h;
    }
}

std::tuple<int, int, int> Node::getTuple() const {
    return std::tuple<int, int, int>(_i, _j, _t);
}

std::tuple<int, int, int> Node::getParentTuple() const {
    return _parentTuple;
}

BaseNode Node::getBaseNode() const {
    return _node;
}

bool operator==(Node const& first, Node const& second) {
    return first._i == second._i && first._j == second._j && first._t == second._t;
}

bool operator<(Node const& first, Node const& second) { 
    if (first._f == second._f)
        return first._t > second._t;
    return first._f < second._f;
}

std::ostream& operator<<(std::ostream& os, Node const& node) {
    os << "(" << node._i << ", " << node._j << ") t=" << node._t << ", h=" << node._h << ", f=" << node._f;
    return os;
}

std::size_t hashNode::operator()(Node const& s) const {
    auto ijt = s.getTuple();
    return std::hash<int>{}(std::get<0>(ijt)) ^ (std::hash<int>{}(std::get<1>(ijt)) << 1) ^ (std::hash<int>{}(std::get<2>(ijt)) << 2);
}

std::size_t hashTuple3::operator()(std::tuple<int, int, int> const& s) const {
    return std::hash<int>{}(std::get<0>(s)) ^ (std::hash<int>{}(std::get<1>(s)) << 1) ^ (std::hash<int>{}(std::get<2>(s)) << 2);
}

bool InversedComparator::operator()(Node const& first, Node const& second) const { 
    if (first._f == second._f)
        return first._t <= second._t;
    return first._f > second._f;
}