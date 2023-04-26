/**@file functions.h
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <memory>
#include <fstream>
#include <map>
#include <vector>

#include "structures.h"

/**Function builds decision tree based on the definition
*@param errors Reference to string where to put errors
*@param file Input strean with tree definition
*
*@return Function returns pointer to root
*/
std::shared_ptr<Node> build_tree( std::string& errors, std::ifstream& file, std::map<std::string, int>& attributes_order );
void set_children_pointers( std::map<int, std::shared_ptr<Node> >& tree, std::shared_ptr<Node> node );
std::map<std::string, int> read_attributes_list( std::ifstream& file );
std::string evaluate( std::shared_ptr<Node> node, std::vector<double>& attributes );
void print_to_file( std::map<std::string, std::vector<std::vector<double> > >& results, std::ostream& file );

#endif