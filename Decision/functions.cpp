#include <sstream>

#include "functions.h"
#include "structures.h"
#include <iostream>

bool evaluate_lt( double a, double b ){
	return a < b;
}

bool evaluate_gt( double a, double b ){
	return a > b;
}


std::shared_ptr<Node> build_tree( std::string& errors, std::ifstream& file, std::map<std::string, int>& attributes_order ){
	std::map<int, std::shared_ptr<Node> > tree;
	int line = 0;
	std::string current_line;
	tree[0] = nullptr ;
    while (std::getline(file, current_line)) {
    	line++;
        std::string line_without_comment = current_line.substr(0, current_line.find("%"));
        if( line_without_comment.find_first_not_of( " \t" ) == std::string::npos )
        	continue;
        std::stringstream line_stream(line_without_comment);
        
        std::shared_ptr<Node> current_node( new Node );
        char comparison;
        std::string attribute;

        line_stream >> current_node->index >> attribute >> comparison  >> current_node->value
                    >> current_node->output_false >> current_node->output_true;

        if( line_stream.fail() ){
        	//syntax error
        	errors+="line " + std::to_string( line ) + ": syntax error\n";
        	continue;
        }

        switch( comparison ){
        	case '<':
        		current_node->evaluate = evaluate_lt;
        		break;
        	case '>':
        		current_node->evaluate = evaluate_gt;
        		break;
        	default:
        		errors+= "line " + std::to_string( line ) + ": incorrect comparison\n";
        		continue;

        }

        current_node->attribute_idx = attributes_order[attribute];

        tree[current_node->index] = current_node;

    }

    std::shared_ptr<Node> root = tree[0];
    if( root == nullptr ){
    	errors+= "error: no root node\n";
    	return root;
    }
    set_children_pointers( tree, root );
    return root;
}

void set_children_pointers( std::map<int, std::shared_ptr<Node> >& tree, std::shared_ptr<Node> node ){
	if( node == nullptr )
		return;
	try{
    	int idx = std::stoi( node->output_false );
    	if( tree.find( idx ) != tree.end() ){
    		node->child_false = tree[idx];
    		node->output_false = "";
    	}else{
    		node->child_false = nullptr;
    	}
    }
    catch( std::invalid_argument& e ){
    	node->child_false = nullptr;
    };

    try{
    	int idx = std::stoi( node->output_true );
    	if( tree.find( idx ) != tree.end() ){
    		node->child_true = tree[idx];
    		node->output_true = "";
    	}else{
    		node->child_true = nullptr;
    	}
    }
    catch( std::invalid_argument& e ){
    	node->child_true = nullptr;
    };

    set_children_pointers( tree, node->child_false );
    set_children_pointers( tree, node->child_true );
}

std::map<std::string, int> read_attributes_list( std::ifstream& file ){
	std::string line;
	getline( file, line );
	std::string line_without_comment = line.substr(0, line.find("%"));
	std::stringstream line_stream( line_without_comment );
	std::string attribute;
	std::map<std::string, int> attributes;
	while( line_stream>>attribute ){
		attributes[attribute] = attributes.size();
	}
	return attributes;
}

std::string evaluate( std::shared_ptr<Node> node, std::vector<double>& attributes ){
	if( node == nullptr )
		return "";
	if( node->evaluate( attributes[node->attribute_idx], node->value ) ){
		if( node->child_true != nullptr )
			return evaluate( node->child_true, attributes );
		else
			return node->output_true;
	}else{
		if( node->child_false != nullptr )
			return evaluate( node->child_false, attributes );
		else
			return node->output_false;
	}
}

void print_to_file( std::map<std::string, std::vector<std::vector<double> > >& results, std::ostream& file ){
	for( const auto& discipline : results ){
    	file<<discipline.first<<std::endl;
    	for( const auto& record : discipline.second ){
    		file<<"\t";
    		for( const auto& attribute : record ){
    			file<<attribute<<" ";
    		}
    		file<<std::endl;
    	}
    }
}