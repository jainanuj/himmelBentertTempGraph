#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "graph.h"
#include <string.h>
#include <stdio.h>
#include <climits>
#include <fstream>
#include "cxxopts.hpp"

using namespace std;

unsigned long beta = 2e9;

//create a file with [amount] many sources for the given input file in range of [0,|nodes|]
void create_list(char* filePath, unsigned long amount){
    FILE* file = fopen(filePath,"r");
    unsigned long n,m,x;
   	x=fscanf(file, "%lu %lu",&n, &m);
   	fclose(file);
   	string s;
   	s = filePath;

   	string path;
   	string tmp;

   	char *token = std::strtok(filePath, "/");
   	while (token != NULL) {
   		tmp = token;
   		token = std::strtok(NULL, "/");
   		if(token!=NULL){
   			path += tmp + "/";
   			s = token;
   		}
   	}

   	string str2 = s.substr(9);
   	s = path + "source_" + std::to_string(amount) + "." + str2; // @suppress("Function cannot be resolved")

   	FILE* file2 = fopen(s.c_str(),"w+");
   	for(unsigned long i=0;i<amount;i++){
   		unsigned long num=rand()%n;
   		fprintf(file2, "%lu\n", num);
   	}
   	fclose(file2);
}

void print_help(){
	cout<<" h/help:"<<endl;
	cout<<" 	Print the usable flags."<<endl;
	cout<<" "<<endl;
	cout<<"	a/algo:"<<endl;
	cout<<"		This flag expects a string containing the name of the single optimality criteria. The default for input is \"foremost\""<<endl;
	cout<<"		The list of optimality criteria:"<<endl;
	cout<<"			foremost"<<endl;
	cout<<"			fastest"<<endl;
	cout<<"			reverse"<<endl;
	cout<<"			minhop"<<endl;
	cout<<"			cheapest"<<endl;
    cout<<"	l/linear:"<<endl;
	cout<<"		If flag is active, then optimal walks for a linear combination of the optimality criteria are computed."<<endl;
	cout<<"		The constants for the optimality criteria have to be set (default value 1)."<<endl;
	cout<<"			-u,foremost"<<endl;
	cout<<"			-v,reverse"<<endl;
	cout<<"			-w,fastest"<<endl;
	cout<<"			-x,weight"<<endl;
	cout<<"			-y,minhop"<<endl;
	cout<<"			-z,waiting"<<endl;
	cout<<"	"<<endl;
	cout<<"	e/edge:"<<endl;
	cout<<"		The path to the file containing an edge list. All algorithms need a edge file to run."<<endl;
	cout<<" "<<endl;
	cout<<"The algorithm expects either a node file or a beta value. Otherwise we use a default beta value of 2e9."<<endl;
	cout<<"	b/beta:"<<endl;
	cout<<"		Integer indicating the dwell time of all vertices of the input."<<endl;
	cout<<" "<<endl;
	cout<<"	n/node:"<<endl;
	cout<<"		The path to the file containing dwell times for all verticex"<<endl;
	cout<<" "<<endl;
	cout<<"	p/backtracking:"<<endl;
	cout<<"		If flag active, we use backtracking. For each reached vertex, we compute a path to the source."<<endl;
	cout<<" "<<endl;
	cout<<"The algorithm expects either a file containing sources, a fixed source node or a number indicating the number of sources:"<<endl;
	cout<<" f/sourcefile:"<<endl;
	cout<<"		This flag expects a file afterwards which contains a list of sources."<<endl;
	cout<<" "<<endl;
	cout<<"	s/source:"<<endl;
	cout<<"		This flag expects a integer indicating a specific node of the input graph as source."<<endl;
	cout<<" "<<endl;
	cout<<" c/createList"<<endl;
	cout<<" 	This flag expects an integer as input and creates a list of sources for a given input file."<<endl;
	cout<<" "<<endl;
	cout<<"	b/numsource:"<<endl;
	cout<<"		This flag expects a integer indicating the number of sources for the algorithm. We randomly generate this many"<<endl;
	cout<<"		sources and run the algorithm on each of these."<<endl;
	cout<<" "<<endl;
	cout<<"print:"<<endl;
	cout<<"Output all [opt] values after running through algorithm"<<endl;
	cout<<""<<endl;
	cout<<"Output Format:"<<endl;
	cout<<"filename"<<endl;
	cout<<"algo n M avg_Degree t_start t_end |T| compression beta readintime"<<endl;
	cout<<"#sources, visit_min, max, median, first, third, medium"<<endl;
	cout<<"runtime_min, max, median, first, third, medium"<<endl;
// // 	cout<<"backtracking, cycles_min, max, median, first, third, medium"<<endl;
	cout<<"pathruntime_min, max, median, first, third, medium"<<endl;
	cout<<"	"<<endl;
}

int main(int argc, char* argv[]){
	cxxopts::Options options("MyProgram", "One line description of MyProgram");
	options.allow_unrecognised_options();
	options.add_options()
	  ("b,beta", "Number to indicate the dwell time of all vertices", cxxopts::value<unsigned long>()->default_value("2147483647"))  // @suppress("Symbol is not resolved") // @suppress("Invalid arguments") // @suppress("Method cannot be resolved") // @suppress("Method cannot be resolved")
	  ("h,help", "Use the help list", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))  // @suppress("Symbol is not resolved") // @suppress("Invalid arguments") // @suppress("Invalid overload") // @suppress("Method cannot be resolved")
	  ("p,backtracking", "Enable pathing for the graph", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))  // @suppress("Symbol is not resolved") // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
	  ("e,edge", "File containing all edges of a graph", cxxopts::value<std::string>())  // @suppress("Symbol is not resolved") // @suppress("Invalid arguments")
	  ("n,node", "File containing all dwell times of a graph", cxxopts::value<std::string>())  // @suppress("Symbol is not resolved") // @suppress("Invalid arguments")
	  ("f,sourcefile", "File containing a set of sources for a graph", cxxopts::value<std::string>())  // @suppress("Symbol is not resolved") // @suppress("Invalid arguments")
	  ("s,source", "Source node of the graph", cxxopts::value<unsigned long>()->default_value("0"))  // @suppress("Symbol is not resolved") // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
	  ("numsource", "Random number of sources", cxxopts::value<unsigned long>())  // @suppress("Symbol is not resolved") // @suppress("Invalid arguments")
	  ("a,algo", "String containing the flag of the algorithm to use.", cxxopts::value<std::string>()->default_value("algo1"))  // @suppress("Symbol is not resolved") // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
	  ("c,createList", "Create a list of sources.", cxxopts::value<unsigned long>())  // @suppress("Symbol is not resolved") // @suppress("Invalid arguments")
	  ("l,linear", "Linear Combination", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))  // @suppress("Symbol is not resolved") // 
	  ("u,foremost", "Weight Foremost", cxxopts::value<unsigned long>()->default_value("1"))  // @suppress("Symbol is not resolved") //
	  ("v,reverse", "Weight Reverse-Foremost", cxxopts::value<unsigned long>()->default_value("1"))  // @suppress("Symbol is not resolved") //
	  ("w,fastest", "Weight Fastest", cxxopts::value<unsigned long>()->default_value("1"))  // @suppress("Symbol is not resolved") //
	  ("x,weight", "Weight Weight", cxxopts::value<unsigned long>()->default_value("1"))  // @suppress("Symbol is not resolved") //
	  ("y,minhop", "Weight Minhop", cxxopts::value<unsigned long>()->default_value("1"))  // @suppress("Symbol is not resolved") //
	  ("z,waiting", "Weight Waiting", cxxopts::value<unsigned long>()->default_value("1"))  // @suppress("Symbol is not resolved") //
     ("t,print", "Prints optimum values of algorithm as output [Scales badly]", cxxopts::value<bool>()->default_value("false")->implicit_value("true")) // @suppress("Symbol is not resolved") // @suppress("Invalid arguments") // @suppress("Method cannot be resolved")
	;

	auto result = options.parse(argc, argv);
	if(result.arguments().size() == 0){
		cout<<"Run ./tempath --help for a documentation."<<endl;
		return 0;
	}
	if(result["help"].as<bool>()){
		print_help();
		return 0;
	}
	if(result.count("beta") != 0){
		beta = result["beta"].as<unsigned long>();
	}else{
		beta = 2e9;
	}

	if(result.count("edge") == 0){
		cout<<"No input file detected! Run ./tempath -e edgefile."<<endl;
		return 0;
	}

	//create List if flag is given [don't run algorithm then]
	if(result.count("createList")){
		if(result.count("edge")){
			create_list(strdup(result["edge"].as<std::string>().c_str()), result["createList"].as<unsigned long>());
		}else{
			cout<<"Need filename in order to create a list with sources!"<<endl;
		}
		return 0;
	}
	Graph g(1);


	//Read input
	Timer readin;
	readin.start();

    g.read_input(result["edge"].as<std::string>().c_str(),0,result["backtracking"].as<bool>());
    if(result.count("node")){
        g.dwell_for_file(result["node"].as<std::string>().c_str());
    }else{
        g.dwell_for_number(beta);
    }
	readin.stop();
    cout << "Read time: " << readin.GetRuntime() << endl;

	//Initialize sources
	if(result.count("sourcefile")){
		g.initial_file_query(result["sourcefile"].as<std::string>().c_str());
	}else if(result.count("numsource")){
		g.initial_query(result["numsource"].as<unsigned long>());
	}else{
		g.sources.push_back(result["source"].as<unsigned long>());
	}
	Timer t;
	vector<double> pathruntime;
    g.max_runtime = 0;
    g.max_src = -1;
	//For each source
    std::ofstream timings_file("./timings");
	for(unsigned long source: g.sources){
		//run algorithm
        if(result.count("linear") != 0) {
            g.run_algo_linear_combination(source, result["foremost"].as<unsigned long>(), result["reverse"].as<unsigned long>(), result["fastest"].as<unsigned long>(), result["weight"].as<unsigned long>(), result["minhop"].as<unsigned long>(), result["waiting"].as<unsigned long>(), timings_file);
        } else {
            g.run_algo(strdup(result["algo"].as<std::string>().c_str()), source);
        }
		//Print opt values if enabled
		if(result["print"].as<bool>()){
			for(unsigned long i=0;i<g.original_V;i++){
				cout<<"i: "<<i<<" opt: "<<g.arr_time[i]<<endl;
			}
		}

                //added by sanaz (for debugging):
                //g.print_earliest(strdup(result["algo"].as<std::string>().c_str()));

		//use backtracking from source
		if(result["backtracking"].as<bool>()){
			unsigned long num_cycle=0;
			unsigned long num_visited=1;

			//for each reached vertex besides source...
			for(unsigned long j=0;j<g.original_V;j++){
				if(j!=source && g.arr_time[j] != infinity && g.arr_time[j] != -infinity){
					num_visited++;

					t.start();
					//run backtracking
					g.pathing(j,source);

					if(result["print"].as<bool>()){
						g.print_path();
					}
					t.stop();
					pathruntime.push_back(t.GetRuntime());


					if(g.cycle != 0){
						num_cycle++;
						g.cycle = 0;
					}
				}
			}
			g.cycles.push_back(num_cycle);
		}
	}


	//OUTPUT
	//FORMAT:
	/*
	 * filename
	 * algo n M avg_Degree t_start t_end |T| compression beta readintime
	 * #sources, visit_min, max, median, first, third, medium
	 * runtime_min, max, median, first, third, medium
	 * backtracking, cycles_min, max, median, first, third, medium
	 * pathruntime_min, max, median, first, third, medium
	 */

	/*cout<<result["edge"].as<std::string>()<<","<<flush;
	cout<<result["algo"].as<std::string>()<<","<<flush;
	cout<<g.V<<","<<flush;
	cout<<g.dynamic_E<<","<<flush;
	cout<<(float)g.dynamic_E/(float)g.V<<","<<flush;
	cout<<g.t_start<<","<<flush;
	cout<<g.t_last<<","<<flush;
	cout<<g.timestamps.size()<<","<<flush;
	cout<<result["beta"].as<unsigned long>()<<","<<flush;
	cout<<readin.GetRuntime()<<","<<flush;
	cout<<g.sources.size()<<","<<flush;
	g.boxplot(g.visited);
	g.boxplot(g.run_time);
	cout<<result["backtracking"].as<bool>()<<","<<flush;
	if(result["backtracking"].as<bool>()){
		g.boxplot(g.cycles);
		g.boxplot(pathruntime);
	}
	cout<<","<<endl;*/

	//print the average runtime 
	g.print_avg_time();

	return 0;
}



