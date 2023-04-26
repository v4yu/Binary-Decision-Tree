#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <memory>

#include "functions.h"
#include "structures.h"

int main(int argc,
         char *argv[]) {// zawsze int argc, char *argv!!! argc - liczba argumentów atgv - talica zawierająca kolejne argumenty
    std::string data_path, tree_description_path, results_path;
    const std::string input_switch = "-i", tree_switch = "-t", output_switch = "-o";
    const std::string executable = argv[0];
    const std::string usage = "usage: " + executable + " [options]\n\
options:\n\
\t" + input_switch + " <filename> - specifies input file \n\
\t" + tree_switch + " <filename> - specifies file with tree description\n\
\t" + output_switch + " <filename> - specifies output file";


    if( argc < 7 ){
    	std::cout<<usage<<std::endl<<std::flush;
    	return 0;
    }

    for (int i = 1; i < argc; i++) { // pętla z warunkiem póki i jest mniejsze od argc
        std::string current_argument = argv[i];
        //std::cout << argv[i] << std::endl;
        if (current_argument == input_switch) {
        	if( i != argc - 1 ){
	            data_path = argv[i + 1];
	            i++;
	        }
	        else{
	        	std::cout<<"error: no filename after "<<input_switch<<std::endl<<std::flush;
	        	return 0;
	        }
        }else if (current_argument == "-t") {
        	if( i != argc - 1 ){
            	tree_description_path = argv[i + 1];
            	i++;
        	}
            else{
            	std::cout<<"error: no filename after "<<tree_switch<<std::endl<<std::flush;
            	return 0;
            }
        }else if (current_argument == "-o") {
        	if( i != argc - 1 ){
	            results_path = argv[i + 1];
	            i++;
	        }
	        else{
	        	std::cout<<"error: no filename after "<<output_switch<<std::endl<<std::flush;
            	return 0;
	        }
        }else{
        	std::cout<<"error: invalid parameter"<<std::endl<<std::flush;
        	return 0;
        }

    }
    if (data_path == "" || tree_description_path == "" || results_path == "") {
        std::cout << usage <<std::endl<<std::flush;
        return 0;
    }


    std::ifstream tree_description_file(tree_description_path);
    if (!tree_description_file.good()) {
        std::cout << "error: couldn't open tree description file: \""<<tree_description_path<<"\"" << std::endl<<std::flush;
        return 0;
    }

    std::ifstream data_file(data_path);
    if (!data_file.good()) {
        std::cout << "error: couldn't open input file: \"" << data_path<<"\"" << std::endl; //zmieniłem "data_file" na "data_path".
        return 0;
    }

    std::string errors = "";
    std::map<std::string, int> attributes = read_attributes_list( data_file );


    std::shared_ptr<Node> root = build_tree( errors, tree_description_file, attributes );
    tree_description_file.close();

    if( errors != "" ){
    	std::cout<<errors<<std::endl;
    	return 0;
    }

    size_t attribute_count = attributes.size();

    std::map<std::string, std::vector<std::vector<double> > > results;

    int line_no = 1;
    std::string line;
    while( getline( data_file, line ) ){
    	line_no++;
    	std::string line_without_comment = line.substr(0, line.find("%"));
    	if( line_without_comment == "" || line_without_comment.find_first_not_of( " \t" ) == std::string::npos )
        	continue;
		std::stringstream line_stream( line_without_comment );
		std::vector<double> current_attributes;
		double tmp;
		while( line_stream>>tmp ){
			current_attributes.push_back(tmp);
		}
		if( current_attributes.size() != attribute_count ){
			std::cout<<data_path<<": line: "<<std::to_string( line_no )<<" error: incorrect number of attributes"<<std::endl;
			continue;
		}
		results[ evaluate( root, current_attributes ) ].push_back( current_attributes );
    }

    data_file.close();

    std::ofstream output_file( results_path );
    if( output_file ){
    	print_to_file( results, output_file );
    	output_file.close();
    }else{
    	std::cout<<"error: couldn't open output file: \""<<results_path<<"\""<<std::endl;
    }
    

    return 0;
}
