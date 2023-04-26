#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <memory>

typedef bool (*evaluation_function)( double, double );

/**
	Structure describing one node of the tree
*/
struct Node {
    int index;///<index of a node
    int attribute_idx;
    evaluation_function evaluate;
    double value;

    std::string output_false;
    std::shared_ptr<Node> child_false;

    std::string output_true;
    std::shared_ptr<Node> child_true;
};




#endif