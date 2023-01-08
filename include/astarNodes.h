#pragma once


#include <tuple>
#include <iostream>


struct BaseNode {
    friend bool operator==(BaseNode const& first, BaseNode const& second);
    friend bool operator<(BaseNode const& first, BaseNode const& second);
    friend std::ostream& operator<<(std::ostream& os, BaseNode const& baseNode);
public:
    BaseNode(int i = 0, int j = 0);
    std::tuple<int, int> getTuple() const;

public:
    int _i;
    int _j;
};

bool operator==(BaseNode const& first, BaseNode const& second);
bool operator<(BaseNode const& first, BaseNode const& second);
std::ostream& operator<<(std::ostream& os, BaseNode const& node);

struct hashBaseNode {
    typedef BaseNode argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};


class Node {
    friend bool operator==(Node const& first, Node const& second);
    friend bool operator<(Node const& first, Node const& second);
    friend std::ostream& operator<<(std::ostream& os, const Node& node);
public:
    Node(int i, int j, int h = 0, std::tuple<int, int, int> parentTuple = std::tuple<int, int, int>(-1, -1, -1));
    BaseNode getBaseNode() const;
    std::tuple<int, int, int> getTuple() const;
    std::tuple<int, int, int> getParentTuple() const;

public:
    int _i;
    int _j;
    int _h;
    int _t;
    int _f;

private:
    BaseNode _node;
    std::tuple<int, int, int> _parentTuple;
};

bool operator==(Node const& first, Node const& second);
bool operator<(Node const& first, Node const& second);
std::ostream& operator<<(std::ostream& os, Node const& node);

struct hashNode {
    typedef Node argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};

struct hashTuple3 {
    typedef std::tuple<int, int, int> argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};

struct InversedComparator {
    typedef Node argument_type;
    typedef bool result_type;

    result_type operator()(argument_type const& f, argument_type const& s) const;
};