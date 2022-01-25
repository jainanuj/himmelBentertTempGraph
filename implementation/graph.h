#ifndef GRAPH_H_HHWU
#define GRAPH_H_HHWU

#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <set>
#include <queue>
#include <iostream>
#include "Timer.h"
#include <iomanip>
#include <numeric>
#include <climits>
#include <list>
#include <float.h>
#include <map>
#include <math.h>
#include <random>

using namespace std;
//namespace po = boost::program_options;

const unsigned long infinity = 4000000000000000000;

struct tuples {
    unsigned long d;	//use index, weight for queue
    unsigned long a;
    bool operator < (const tuples that) const{
    	if(d !=that.d)
    		return d < that.d;
    	return a < that.a;
    }
    bool operator > (const tuples that) const{
    	if(d !=that.d)
    		return d > that.d;
    	return a > that.a;
    }
};

struct Edge
{
    unsigned long u, v, t, w, o;       //Changed from int to unsigned long. Anuj
    bool operator < (const Edge that) const
    {
        if (t != that.t)
            return t < that.t;
        if (u != that.u)
        	return u < that.u;
        if (v != that.v)
        	return v < that.v;
        return w < that.w;  
    }
};

class Graph{
public:
	Graph(unsigned long i);
    void read_input(const char* edge, bool compression, bool backtrack);
    void initial_file_query(const char* file);
    void initial_query(unsigned long amount);
	void print_avg_time();
	void boxplot( vector<double> input);
	void print_sources();
	void update_edge(Edge e, queue<unsigned long> q, unsigned long time);
    
    //additional function
    void print_path();
    void pathing(unsigned long end, unsigned long source);
    void print_earliest(char* algo);
    void print_time(string s);
    void count_visited();

    //algo1
    void initial_algo1(unsigned long tmp);
    void init_algo1();
    void run_algo1(unsigned long source);
    void single_source_optimal_walk(unsigned long source);
    void mod_bfs(unsigned long time, unsigned long source,unsigned long counter, queue<unsigned long> q);

    //algo2
    void initial_algo2(unsigned long source);
    void run_algo2(unsigned long source);
    void single_source_fastest_walk(unsigned long source);
    void mod_dijkstra(unsigned long time,unsigned long source,unsigned long counter, bool cheapest, bool minwait, long (Graph::*fptr)(unsigned long, unsigned long,unsigned long));
    //added by sanaz:
    void mod_dijkstra_minhop(unsigned long time, unsigned long source, unsigned long counter, long (Graph::*fptr)(unsigned long, unsigned long, unsigned long));

    //variation
    void run_reverse_foremost(unsigned long source);
    void single_source_reverse_foremost_walk(unsigned long source);

    void initial_hop_count(unsigned long var);
    void run_hop_count(unsigned long source);
    void min_hop_count(unsigned long source);

    void run_cheapest(unsigned long source);
    void initial_cheapest(unsigned long source);
    void cheapest(unsigned long var);
    
    void initial_min_waiting_time(unsigned long var);
    void run_min_waiting_time(unsigned long source);
    void min_waiting_time(unsigned long source);
    

    //lambda function
    long fastest_function(unsigned long opt, unsigned long curr, unsigned long time);
    long reverse_foremost_function(unsigned long opt, unsigned long curr,unsigned long time);
    long min_hop_count_function(unsigned long opt, unsigned long curr, unsigned long time);
    long cheapest_function(unsigned long opt, unsigned long curr, unsigned long time);                   //Anuj
    long min_waiting_time_function(unsigned long opt, unsigned long curr, unsigned long time);
    long linear_combination_function(unsigned long opt, unsigned long curr, unsigned long time);

    void read_graph(FILE* file, unsigned long x, bool no_compression, bool backtrack);
    void dwell_for_file(const char* filePath);
    void dwell_for_number(unsigned long x);
    void run_algo(char* algo, unsigned long source);
    
    void run_algo_linear_combination(unsigned long source, unsigned long c1, unsigned long c2, unsigned long c3, unsigned long c4, unsigned long c5, unsigned long c6);
    void run_linear_combination(unsigned long source);
    void initial_linear_combination(unsigned long source);
    void linear_combination(unsigned long source);
    
    void initial_variations(unsigned long var);

public:
    vector< Edge > edge_list;
    vector< unsigned long > sources;
    vector <unsigned long> edge_track;
    unsigned long V, static_E, dynamic_E, original_V;
    unsigned long t_start, t_end; //beginning point and ending point in temp graphs
    unsigned long t_last;			//time of last edge instead of inf for t_end
    vector <double> run_time;
	vector <unsigned long> arr_time, f_time, dwell_time, min_time_vertex;      //Anuj Changed from int to double
    //vector < std::tuple <unsigned long, unsigned long, unsigned long> > backtracking_opt;
    //vector < unsigned long > backtracking_tmp;
	vector < vector <Edge> > edge_matrix;
	vector < vector < std::tuple <unsigned long,unsigned long,unsigned long> > > predecessor;
	vector < set < pair <unsigned long,unsigned long > > > ft_timepair; // arrival time, starting time
	vector < set < pair <unsigned long,unsigned long > > > st_timepair; // arrival time, shortest distance
	vector <vector <unsigned long> > node_list;					 //contains nodes with outgoing arcs on each timestep
	vector < std::tuple <long,unsigned long,unsigned long> > reached_algo1;
	vector < std::tuple <unsigned long,unsigned long/*,unsigned long*/> > reached_algo2;       //Changed from int to unsigned long
	vector <unsigned long> distance;       //Anuj changed from int to unsigned long
	set <unsigned long> times;							 // set of all times
	vector <unsigned long> timestamps;
	unsigned long nodes_to_reach;
	unsigned long source_length;
	vector < std::list < tuple <unsigned long,unsigned long> > > d_list;
	vector <unsigned long> path;
	unsigned long cycle;
	vector <unsigned long> visited;
	vector <unsigned long> cycles;
	std::map <unsigned long,unsigned long> timemap;
	bool backtracking = false;
//    unsigned long c_foremost, c_reverse, c_fastest, c_weight, c_minhop, c_waiting;
    unsigned long c_foremost, c_reverse, c_fastest, c_weight, c_minhop, c_waiting;     //Anuj changes

};
/**
 * u startnode
 * v endnode
 * t time
 * w cost
 */
//init graph structure [no information yet in this structure]
Graph::Graph(unsigned long tmp){ // @suppress("Class members should be properly initialized")
	tmp=1;
}


void Graph::read_graph(FILE* file, unsigned long x, bool no_compression, bool backtrack){

	arr_time.resize(V);				                    //first arrival time for vertex
	dwell_time.resize(V);		                        //dwell time for vertex
	edge_matrix.resize(V, vector <Edge>());		        //contains for each vertex the edges sorted
	predecessor.resize(V, vector < std::tuple <unsigned long,unsigned long,unsigned long> > ());	    //list of predecessors for backtracking
	edge_track.resize(V);			                   //list to keep track of visited edges from all nodes
    //backtracking_tmp.resize(V);
    //backtracking_opt.resize(V, std::tuple <unsigned long,unsigned long,unsigned long> ());
	t_end = 2e9;

	unsigned long num_timestamps = 0;

	Edge e;
	x=fscanf(file, "%lu %lu %lu %lu",&e.u, &e.v, &e.t, &e.w);

	if(!no_compression){
		e.t = 0;
	}

	backtracking = backtrack;

	t_start = (unsigned long)e.t;
	unsigned long curr_time = e.t;

	timemap[curr_time] = num_timestamps;
	timestamps.push_back(e.t);
	edge_matrix[e.u].push_back(e);

	for(unsigned long i = 1; i < dynamic_E; i ++){
		x=fscanf(file, "%lu %lu %lu %lu",&e.u, &e.v, &e.t, &e.w);

		if(curr_time<e.t){

			num_timestamps++;
			if(!no_compression){
				e.t = num_timestamps;
			}
			curr_time = e.t;
			timemap[e.t] = num_timestamps;
			timestamps.push_back(e.t);
			vector <unsigned long> curr_nodes;
			node_list.push_back(curr_nodes);

		}
		if(!no_compression){
			e.t = num_timestamps;
		}
		edge_matrix[e.u].push_back(e);
	}
	vector <unsigned long> curr_nodes;
	node_list.push_back(curr_nodes);
	t_last = (unsigned long) e.t;
	fclose(file);
}

//intialize graph for our fastest and variations
void Graph::read_input(const char* edge, bool compression, bool backtrack){ // @suppress("Class members should be properly initialized")

	FILE* file = fopen(edge,"r");
	unsigned long x;
	x=fscanf(file, "%lu %lu %lu",&V, &dynamic_E, &original_V);

	reached_algo1.resize(V, std::tuple <long,unsigned long,unsigned long> ());
	f_time.resize(V);
	reached_algo2.resize(V, std::tuple <unsigned long,unsigned long/*,unsigned long*/> ());
	distance.resize(V);
	d_list.resize(V,  std::list < tuple <unsigned long,unsigned long> > () );
	read_graph(file, x, !compression, backtrack);
}

void Graph::initial_query(unsigned long amount){
	unsigned long s;
	for(unsigned long i = 0 ;i < amount ;i ++){
    	s=rand()%V;
        sources.push_back(s);
    }
}

/**
 * push starting point to sources
 */
void Graph::initial_file_query(const char* filePath){

	unsigned long source;
	FILE* file = fopen(filePath,"r");
	unsigned long x;
	x=fscanf(file, "%lu",&source);
	while(x!=-1){
		sources.push_back(source%V);
		x=fscanf(file, "%lu",&source);
	}
}

/*
 * Extract dwell times from file
 * num_node: Number of Nodes (Error when not equal to number of nodes from other file
 * mode    : Not important. Reflects the process of creating dwell time for graph
 */
void Graph::dwell_for_file(const char* filePath){
	FILE* file = fopen(filePath,"r");
	unsigned long num_nodes;
	unsigned long mode;
	unsigned long x;
	x=fscanf(file, "%lu %lu",&num_nodes, &mode);
	if(num_nodes==V){
		for(unsigned long i=0;i<num_nodes;i++){
			x = fscanf(file,"%lu",&dwell_time[i]);      //Changed by Anuj
		}
	}else{
		cout<<"Error: #Nodes of file do not correspond to #Nodes in graph!"<<endl;
	}
}

void Graph::dwell_for_number(unsigned long x){
 	for(unsigned long i=0; i<original_V;i++){
 		dwell_time[i] = x;
 	}
 	for(unsigned long i=original_V; i<V;i++){
 		dwell_time[i] = 2147483647;
 	}
//	for(unsigned long i=0;i<V;i++){
//		dwell_time[i] = x;
//	}
}


void Graph::run_algo(char* algo, unsigned long source){

	if(!strcmp(algo,"foremost")){
		run_algo1(source);
	}
	if(!strcmp(algo,"fastest")){
		run_algo2(source);
	}
	if(!strcmp(algo,"reverse")){
		run_reverse_foremost(source);
	}
	if(!strcmp(algo,"minhop")){
		run_hop_count(source);
	}
	if(!strcmp(algo,"cheapest")){
		run_cheapest(source);
	}
	if(!strcmp(algo,"minwait")){
		run_min_waiting_time(source);
	}
	count_visited();
}

void Graph::run_algo_linear_combination(unsigned long source, unsigned long c1, unsigned long c2, unsigned long c3, unsigned long c4, unsigned long c5, unsigned long c6){
    c_foremost = c1;
    c_reverse = c2;
    c_fastest = c3;
    c_weight = c4;
    c_minhop = c5;
    c_waiting = c6;
    
    run_linear_combination(source);
    count_visited();
}


/*
 * Initial Algorithm1
 */
void Graph::initial_algo1(unsigned long var){
	for(unsigned long i=0; i<V;i++){
		arr_time[i] =  infinity;
		reached_algo1[i] = std::make_tuple (0,0,0);
		edge_track[i] = 0;
		predecessor[i].clear();
        f_time[i] = infinity;
        //backtracking_opt[i] = std::make_tuple (0,0,0);
        //backtracking_tmp[i] = 0;
	}

	unsigned long min = 2e9;
	for(Edge e: edge_matrix[var]){
		if(e.t<min){
			min = e.t;
		}
	}

	for(unsigned long i=0;i<node_list.size();i++){
		node_list[i].clear();
	}

	//arr_time[var] = min;
	arr_time[var] = 0; //modified by sanaz
	reached_algo1[var] = std::make_tuple (min,var,min);
	nodes_to_reach = V-1;
	source_length = edge_matrix[var].size();
}

/**
 * Inital call of foremost computation
 * Runs for each source
 */
void Graph::run_algo1(unsigned long source){

//    char* algo = new char[15];
//    strcpy(algo,"foremost");
	initial_algo1(source);
	Timer t;
	t.start();
	single_source_optimal_walk(source);
	t.stop();
	double time_sum = t.GetRuntime();
	run_time.push_back(time_sum);
//    print_earliest(algo);
}

/**
 * Run algorithm foremost
 */
void Graph::single_source_optimal_walk(unsigned long source){

	unsigned long counter = 0;
	queue<unsigned long> q;
	for(unsigned long time: timestamps){
		if(time>=arr_time[source]){
			mod_bfs(time,source,counter,q);
			if(nodes_to_reach==0){
				return;
			}
		}
		counter++;
	}
}

/**
 * Run modified BFS
 */
void Graph::mod_bfs(unsigned long time, unsigned long source, unsigned long counter, queue<unsigned long> q){

	for(unsigned long i = edge_track[source];i<source_length;i++){		//iterate over edges outgoing from source vertex
		Edge e = edge_matrix[source][i];
        
		if(e.t==time){
            //printf("Edge from source: (%lu, %lu, %lu, %lu)\n", e.u, e.v, e.t, e.w);
            std::tuple<unsigned long,unsigned long,unsigned long> pre (time, source, time); //new line
			reached_algo1[e.v] = pre;
			/*if(backtracking){
				predecessor[e.v].push_back(pre);
			}*/
			if(arr_time[e.v]==infinity){
				nodes_to_reach--;
				arr_time[e.v] = time;
                /*if (backtracking){ //new line
                    backtracking_opt[e.v] = std::make_tuple (time, source, time);//new line
                }*/ //new line
				f_time[e.v] = time;
				if(nodes_to_reach==0){
					return;
				}
				unsigned long length2 = edge_matrix[e.v].size();
				for(unsigned long j=edge_track[e.v];j<length2;j++){
					Edge e2 = edge_matrix[e.v][j];
					if(e2.t>time){
                        node_list[timemap[e2.t]].push_back(e2.u);
					}else if(e2.t==time){
                        q.push(e2.u);  
					}else{
						edge_track[e.v]=j;
					}
				}
			}
		}else if(e.t>time){
			edge_track[source] = i;
			break;
        }
    }

	for(unsigned long i: node_list[counter]){		//iterate over vertices with outgoing edges which were visited already
		if( (long)time <= (long)std::get<0>(reached_algo1[i]) + (long)dwell_time[i] ){
			q.push(i);
        }
    }
	node_list[counter].clear();

	while(!q.empty()){					//iterate over vertices with potential improvements
		unsigned long cur = q.front();
		q.pop();
        //printf("Current vertex in queue: %lu at time %lu\n", cur, time);
		unsigned long length = edge_matrix[cur].size();
        
		for(unsigned long i = edge_track[cur];i<length;i++){
            
			Edge e = edge_matrix[cur][i];
            
			if(e.t==time && std::get<0>(reached_algo1[e.v]) < (long)time){
                
                std::tuple<unsigned long,unsigned long,unsigned long> pre (time,e.u,std::get<0>(reached_algo1[e.u])); //new line
				reached_algo1[e.v] = pre;
                
				/*if(backtracking){
                    predecessor[e.v].push_back(pre);
				}*/
				q.push(e.v);
				if(arr_time[e.v]==infinity){	//we only need to update opt the first time, we find a vertex
                    
					nodes_to_reach--;
					arr_time[e.v] = time;
                    /*if (backtracking){
                        
                        backtracking_opt[e.v] = pre;
                    }*/
					f_time[e.v] = time;
					if(nodes_to_reach==0){
                        
						return;
					}
					unsigned long length2 = edge_matrix[e.v].size();	//if visiting a vertex the first time then check outgoing edges and fill to node list
					for(unsigned long j=edge_track[e.v];j<length2;j++){
                        
						Edge e2 = edge_matrix[e.v][j];
						if(e2.t>time){ 
                            
                            node_list[timemap[e2.t]].push_back(e2.u);
						}else if(e2.t<time){
                            
							edge_track[e.v] = j;
						}
					}
				}
			}else if(e.t>time){
				edge_track[cur] = i;
				break;
	
            }
        }
    }
}

void Graph::initial_variations(unsigned long var){

	for(unsigned long i=0;i<V;i++){
		arr_time[i] =  infinity;		//set optimum to inf
		edge_track[i] = 0;				//set to 0 -> no edges visited yet
		distance[i] = infinity;			//distance to inf       //probly should be double ANUJ
		f_time[i] = infinity;			//first arrival set to inf
		reached_algo2[i] = std::make_tuple (infinity,0/*,0*/);	//reached to inf
        //backtracking_tmp[i] = 0;
		d_list[i].clear();
        predecessor[i] = vector < std::tuple <unsigned long,unsigned long,unsigned long> > ();
        //backtracking_opt[i] = std::make_tuple (0,0,0);
	}

	unsigned long length = edge_matrix[var].size();
	unsigned long min = 2e9;
	for(Edge e: edge_matrix[var]){		//find minimum timestamp with outgoing edge from source [earlier timestamps yield no result]
		if(e.t<min){
			min = e.t;
		}
	}

	distance[var] = 0;
	arr_time[var] = 0;
	f_time[var] = min;
	nodes_to_reach = V-1;
	source_length = edge_matrix[var].size();

	for(unsigned long i=0;i<node_list.size();i++){	//reset node list
		node_list[i].clear();
	}

}
//preprocessing for fastest
void Graph::initial_algo2(unsigned long var){

	for(unsigned long i=0;i<edge_matrix.size();i++){
		for(unsigned long j=0;j<edge_matrix[i].size();j++){
			if(edge_matrix[i][j].u==var){
				edge_matrix[i][j].o = t_last - edge_matrix[i][j].t;
			}else{
				edge_matrix[i][j].o = 0;
			}
		}
	}
}

//preprocessing for hop count
void Graph::initial_hop_count(unsigned long var){
	for(unsigned long i=0;i<edge_matrix.size();i++){
		for(unsigned long j=0;j<edge_matrix[i].size();j++){
			edge_matrix[i][j].o = 1;
		}
	}
}
//preprocessing for minimum waiting time
void Graph::initial_min_waiting_time(unsigned long var){
	for(unsigned long i=0;i<edge_matrix.size();i++){
		for(unsigned long j=0;j<edge_matrix[i].size();j++){
			edge_matrix[i][j].o = 0;
        }
	}
}
//preprocessing for cheapest
void Graph::initial_cheapest(unsigned long var){
	for(unsigned long i=0;i<edge_matrix.size();i++){
		for(unsigned long j=0;j<edge_matrix[i].size();j++){
			edge_matrix[i][j].o = edge_matrix[i][j].w ;
        }
	}
}
//preprocessing for linear combination
void Graph::initial_linear_combination(unsigned long var){
	for(unsigned long i=0;i<edge_matrix.size();i++){
		for(unsigned long j=0;j<edge_matrix[i].size();j++){
			if(edge_matrix[i][j].u==var){
				edge_matrix[i][j].o = ( c_fastest + c_reverse ) * ((unsigned long) t_last - edge_matrix[i][j].t ) +  c_weight * edge_matrix[i][j].w  + c_minhop;
			}else{
				edge_matrix[i][j].o = c_weight * edge_matrix[i][j].w + c_minhop;
			}
		}
	}
}

//function for fastest
long Graph::fastest_function(unsigned long opt, unsigned long curr, unsigned long time){
	return std::min((long) opt,  (long)time - ((long)t_last - (long)curr));
}
//function for reverse foremost
long Graph::reverse_foremost_function(unsigned long opt, unsigned long curr, unsigned long time){
	return std::max((long) opt, (long)t_last - (long)curr);
}
//function for min hop count
long Graph::min_hop_count_function(unsigned long opt, unsigned long curr, unsigned long time){
	return std::min((long)opt, (long)curr);
}
//function for cheapest
long Graph::cheapest_function(unsigned long opt, unsigned long curr, unsigned long time){
	return std::min((long)opt, (long)curr);
}
//function for minimum waiting time
long Graph::min_waiting_time_function(unsigned long opt, unsigned long curr, unsigned long time){
	return std::min((long)opt, (long)curr);
}
//function for lienar combination
long Graph::linear_combination_function(unsigned long opt, unsigned long curr, unsigned long time){
	return std::min((unsigned long)opt, (unsigned long)curr +  (unsigned long)c_foremost * (unsigned long)time + (unsigned long)c_fastest * ( (unsigned long)time - (unsigned long)t_last ) - (unsigned long)c_reverse * ( (unsigned long)t_last ));
}
/**
 * Run algorithm fastest
 */
void Graph::single_source_fastest_walk(unsigned long source){
	unsigned long counter = 0;
	for(unsigned long time: timestamps){  //run over all timestamps
		if(time>=arr_time[source]){  //only for necessary timestamps
			mod_dijkstra(time,source,counter,false, false, &Graph::fastest_function); //run fastest with given input function for fastest
			node_list[counter].clear();
		}
		counter++;
	}
}

/*
 * Run algorithm min hop count
 */
void Graph::min_hop_count(unsigned long source){
	unsigned long counter = 0;
	//unsigned long t = 0;
	for(unsigned long time: timestamps){
		if(time>=arr_time[source]){
			//printf("Current Time: %lu ---- Difference: %lu \n",time, time - t );
			mod_dijkstra_minhop(time, source, counter, &Graph::min_hop_count_function);
			node_list[counter].clear();
		}
		counter++;
		//t =time;
	}
}

/*
 * Run algorithm cheapest
 */
void Graph::cheapest(unsigned long source){
	unsigned long counter = 0;
	for(unsigned long time: timestamps){
		if(time>=arr_time[source]){
			mod_dijkstra(time,source,counter, true, false, &Graph::cheapest_function);
			node_list[counter].clear();
		}
		counter++;
	}
}

/*
 * Run reverse foremost
 */
void Graph::single_source_reverse_foremost_walk(unsigned long source){
	unsigned long counter = 0;
	for(unsigned long time: timestamps){
		if(time>=arr_time[source]){
			mod_dijkstra(time,source,counter, false, false, &Graph::reverse_foremost_function);
			node_list[counter].clear();
		}
		counter++;
	}
}
/*
 * Run minimum waiting time
 */
void Graph::min_waiting_time(unsigned long source){
	unsigned long counter = 0;
	for(unsigned long time: timestamps){
		if(time>=arr_time[source]){
			mod_dijkstra(time,source,counter, false, true, &Graph::min_waiting_time_function);
			node_list[counter].clear();
		}
		counter++;
	}
}
/*
 * Run minimum waiting time
 */
void Graph::linear_combination(unsigned long source){
	unsigned long counter = 0;
	for(unsigned long time: timestamps){
		if(time>=arr_time[source]){
			mod_dijkstra(time,source,counter, true, true, &Graph::linear_combination_function);
			node_list[counter].clear();
		}
		counter++;
	}
}
/**
 * Run modified Dijkstra
 * tuple: (node_index,distance)
 */
void Graph::mod_dijkstra(unsigned long time, unsigned long source, unsigned long counter, bool cheapest, bool minwait, long (Graph::*fptr)(unsigned long, unsigned long, unsigned long)){

    vector<unsigned long> reset;
    vector<unsigned long> nodes;
    std::priority_queue<tuples,std::vector<tuples>, std::greater<tuples>> q;
    tuples t;
    distance[source] = arr_time[source];
    // go through all out going arcs of the source vertex at time time
    //printf("Start with time step %lu \n",time);
	for(unsigned long i = edge_track[source];i<source_length;i++){	//run over edges from source
		Edge e = edge_matrix[source][i];
		if(e.t==time){
			/*
			 * We added this part, because for the cheapest algorithm there may be a difference
			 * between using the edge from E_T or continuing with E_R, because there are costs involved.
			 * In comparison to minhop and the other algorithms we do not favor any of both choices, thus
			 * we need to check both.
			 * For the other algorithms, we priorize using the edge from E_T
			 */
            //for cheapest a case distinction is needed
			if(cheapest){
                std::tuple <unsigned long,unsigned long> curr;
                while(!d_list[e.v].empty()){
                    
                    curr = d_list[e.v].front();
                    if((unsigned long) std::get<0> (curr) + dwell_time[e.v] >= (unsigned long) time){     //Changed from long to double
                        
                        if(minwait && ( e.v < original_V ) ){
                            if(e.o > ( std::get<1> (curr) + c_waiting * ( time - std::get<0>(curr) ) ) ){	
                            
                                distance[e.v] =  std::get<1> (curr) + c_waiting * ( time - std::get<0>(curr) ) ;
                            } else {
                            
                                distance[e.v] = e.o;
                            }
                        } else {
                            if(e.o > std::get<1> (curr)){	
                                
                                distance[e.v] = std::get<1> (curr);
                            } else {
                                
                                distance[e.v] = e.o;
                            }
                        }
                        t.a = e.v;
                        t.d = distance[e.v];
                        q.push(t);
                            
                        reached_algo2[e.v] = std::make_tuple ( e.o, source/*, time*/);
                        /*if (backtracking){
                            backtracking_tmp[e.v] = std::get<0> (curr);
                        }*/
                        reset.push_back(e.v);
                        nodes.push_back(e.v);
                        break;

                    }else{
                        d_list[e.v].pop_front();
                    }
                }
                if(distance[e.v] < infinity){
                    continue;
                }
            }
            distance[e.v] = e.o;
            reached_algo2[e.v] = std::make_tuple ( e.o, source/*, time*/);
            t.a = e.v;
            t.d = e.o;
            q.push(t);
            nodes.push_back(e.v);
            reset.push_back(e.v);
			
		       /*	if(backtracking){
                backtracking_tmp[e.v] = time;
			}*/

			if(f_time[e.v]==infinity){	//node visited first time -> fill node_list
				f_time[e.v] = time;
				unsigned long length2 = (unsigned long) edge_matrix[e.v].size();
				for(unsigned long j=edge_track[e.v];j<length2;j++){
					Edge e2 = edge_matrix[e.v][j];
					if(e2.t>time){
                        node_list[timemap[e2.t]].push_back(e2.u);
					}else if(e2.t<time){
						edge_track[e.v]=j;
					}
				}
			}
		}else if(e.t>time){
			edge_track[source] = i;
			break;
        }
    }
	for(unsigned long i: node_list[counter]){	//push node to queue if reached before and has outgoing edges
        
        while(!d_list[i].empty() && distance[i]==infinity){
            
            std::tuple <unsigned long,unsigned long> curr = d_list[i].front();

			if((unsigned long) std::get<0> (curr) + (unsigned long)dwell_time[i] >= (unsigned long)time){	//if list element inside range [t,t+dwell_time]
                if (minwait && ( i < original_V ) ){
                    distance[i] = std::get<1>(curr) + c_waiting * ( (unsigned long)time - std::get<0>(curr) );
                } else {
                    distance[i] = std::get<1>(curr);
                }
				t.a = i;
				t.d = distance[i];
				q.push(t);
				reset.push_back(i);
               /*if (backtracking){
                    backtracking_tmp[i] = std::get<0> (curr);
                }*/
				break;
			}else{
				d_list[i].pop_front();		//remove first element -> not needed anymore
            }
        }
    }

	while(!q.empty()){			//dijsktra with priority queue
		tuples cur = q.top();
		q.pop();
		if(distance[cur.a]==cur.d){			//only use element if not found better path in this iteration
			
            unsigned long length = edge_matrix[cur.a].size();
            
			for(unsigned long i = edge_track[cur.a];i<length;i++){
				
                Edge e = edge_matrix[cur.a][i];
				if(e.t==time){
                    if(distance[e.v] > (distance[e.u] + e.o)){
						
                        if(distance[e.v] == infinity){
						    reset.push_back(e.v);
						}
						/*if(backtracking){
                            backtracking_tmp[e.v] = time;
                        }*/
                        distance[e.v] = distance[e.u] + e.o;
						t.a = e.v;
						t.d = distance[e.v];
						q.push(t);
					}
                    if (std::get<0>(reached_algo2[e.v]) > distance[e.u]+ e.o){
                        reached_algo2[e.v] = std::make_tuple( distance[e.u]+ e.o, e.u/*, backtracking_tmp[e.u]*/);
                        nodes.push_back(e.v);
                    }
				}else if(e.t>time){	//found node after current timestamp [next iteration]
					edge_track[cur.a] = i;
					break;
                }
            }
        }
    }
    // for all reached vertices exactly at time step time
	for(unsigned long i: nodes){
        // updat optimality values
        if ( arr_time[i] != (this->*fptr)(arr_time[i], std::get<0>(reached_algo2[i]), time)){
              
            arr_time[i] = (this->*fptr)(arr_time[i], std::get<0>(reached_algo2[i]), time);
            //backtracking_opt[i] =  std::make_tuple(time, std::get<1>(reached_algo2[i]), std::get<2>(reached_algo2[i]));
        }
        // update list of time steps a vertex was reached 
        /*if (backtracking){
            std::tuple<unsigned long,unsigned long,unsigned long> pre = std::make_tuple(time, std::get<1>(reached_algo2[i]),std::get<2>(reached_algo2[i]));
            predecessor[i].push_back(pre);
        }*/
        std::tuple <unsigned long,unsigned long> tmp = std::make_tuple(time, std::get<0>(reached_algo2[i]));
        while(!d_list[i].empty()){
            
            std::tuple <unsigned long,unsigned long> curr = d_list[i].back();
            
            if (minwait && i < original_V){
                
                if( std::get<1>(curr) + c_waiting * ( (unsigned long)time - std::get<0>(curr) ) >= std::get<1>(tmp) ){
                    d_list[i].pop_back();
                }else{
                    break;
                }
                
            } else {
                
                if(std::get<1>(curr) >= std::get<1>(tmp) ){
                    d_list[i].pop_back();
                }else{
                    break;
                }
            }
        }
        d_list[i].push_back(tmp);

		
		if(f_time[i]==infinity){	//node visited first time -> fill node_list
            f_time[i] = time;
			unsigned long length2 = edge_matrix[i].size();
			for(unsigned long j=edge_track[i];j<length2;j++){
                Edge e2 = edge_matrix[i][j];
                if(e2.t>time){
                    if ( node_list[timemap[e2.t]].empty() || node_list[timemap[e2.t]].back() != e2.u ){
                        node_list[timemap[e2.t]].push_back(e2.u);
                    }
                }else if(e2.t<time){
                    edge_track[i]=j;
                }
            }
        }
		
		reached_algo2[i] = std::make_tuple (infinity,0/*,0*/);
		distance[i] = infinity;
	}
	for(unsigned long i: reset){	//reset distance for vertices which werent found but used as E_r vertices
		distance[i] = infinity;			//reset distance of vertices used by source vertex
	}
}

/*
 * Run Our Fastest for every source
 */
void Graph::run_algo2(unsigned long source){

	initial_variations(source);
	initial_algo2(source);
	Timer t;
	t.start();
	single_source_fastest_walk(source);
	t.stop();
	double time_sum = t.GetRuntime();
	run_time.push_back(time_sum);
}

/*
 * Run our cheapest for every source
 */
void Graph::run_cheapest(unsigned long source){
	initial_variations(source);
    initial_cheapest(source);
	Timer t;
	t.start();
	cheapest(source);
	t.stop();
	double time_sum = t.GetRuntime();
	run_time.push_back(time_sum);
}

/*
 * Run our min hop count for every source
 */
void Graph::run_hop_count(unsigned long source){
	initial_variations(source);
	initial_hop_count(source);
	Timer t;
	t.start();
	min_hop_count(source);
	t.stop();
	double time_sum = t.GetRuntime();
	run_time.push_back(time_sum);
}
/*
 * Run our min waiting time for every source
 */
void Graph::run_min_waiting_time(unsigned long source){
    c_waiting = 1;
	initial_variations(source);
	initial_min_waiting_time(source);
	Timer t;
	t.start();
	min_waiting_time(source);
	t.stop();
	double time_sum = t.GetRuntime();
	run_time.push_back(time_sum);
}
/*
 * Run our reverse foremost for every source
 */
void Graph::run_reverse_foremost(unsigned long source){
	initial_variations(source);
    for(unsigned long i=0;i<V;i++){
		arr_time[i] = -infinity;
    }
    arr_time[source]=0;
	initial_algo2(source);
	Timer t;
	t.start();
	single_source_reverse_foremost_walk(source);
	t.stop();
	double time_sum = t.GetRuntime();
	run_time.push_back(time_sum);
}
/*
 * Run our linear combination for every source
 */
void Graph::run_linear_combination(unsigned long source){
    initial_variations(source);
    char* algo = new char[10];
    strcpy(algo,"linear");
    initial_linear_combination(source);
    Timer t;
	t.start();
	linear_combination(source);
	t.stop();
	double time_sum = t.GetRuntime();
	run_time.push_back(time_sum);
    print_earliest(algo);       //Anuj
    delete [] algo;
}


void Graph::print_path(){
	cout<<"Path: "<<endl;
	for(unsigned long i=0;i<path.size()-1;i++){
		cout<<path[i]<<"->";
		cout<<flush;
	}
	cout<<path[path.size()-1]<<endl;
}


void Graph::print_avg_time(){
	std::cout << std::setprecision(6) << std::showpoint << std::fixed;
	double time_sum=0;
	for(unsigned long i=0;i<run_time.size();i++){
		time_sum+=run_time[i];
	}
	cout<<"Average time: "<< time_sum/sources.size() <<endl;
}


//create a boxplot for a given vector of values [mostly running times]
void Graph::boxplot(vector<double> values){

	std::sort (values.begin(), values.end());
	double min = DBL_MAX;
	double max = DBL_MIN;
	double mean = 0;
	double median, first, third;
	unsigned long length = values.size();
	for(unsigned long i=0;i<length;i++){
		if(values[i]<min){
			min = values[i];
		}
		if(values[i]>max){
			max = values[i];
		}
		mean+= values[i];
	}

	if(length==1){
		median = values[0];
		first= values[0];
		third = values[0];
		min = values[0];
		max = values[0];
		mean = values[0];
	}else if(length==2){
		first= values[0];
		third= values[1];
		median = (values[0]+values[1])/2;
	}else if(length==3){
		median = values[1];
		first= values[0];
		third = values[2];
	}else{
		if(length%2==0){
			median = (values[length/2] + values[(length/2)-1]) /2;
		}else{
			median = values[length/2];
		}if((length+1)%4==0){
			first = values[(length+1)/4];
		}else{
			first = (values[((length+1)/4) -1] + values[(length+1)/4]) /2;
		}if(3*(length+1)%4==0){
			third = values[3*(length+1)/4];
		}else{
			third = (values[3*(length+1)/4 -1] + values[3*(length+1)/4]) /2;
		}
	}

	cout<<min<<","<<flush;
	cout<<max<<","<<flush;
	cout<<median<<","<<flush;
	cout<<first<<","<<flush;
	cout<<third<<","<<flush;
	cout<<mean/length<<","<<flush;;
}

void Graph::print_time(string s){
	double mean = 0;
	unsigned long length = run_time.size();
	for(unsigned long i=0;i<length;i++){
		mean+= run_time[i];
	}
	cout<<s<<"Average: "<<mean/length<<endl;
}

void Graph::print_earliest(char* algo){
	for(unsigned long i=0;i<original_V;i++){
//        if(arr_time[i]!=infinity && arr_time[i]!=-infinity){
            cout<< i << ": " << arr_time[i]<<endl;           //Anuj
  //      }
	}
}

void Graph::print_sources(){
	cout<<"Sources:"<<endl;
	unsigned long length  = sources.size();
	for(unsigned long i=0;i<length;i++){
		cout<<sources[i]<<" "<<endl;
	}
}

void Graph::count_visited(){
	unsigned long count = 0;
	for(unsigned long i=0;i<original_V;i++){
		if(arr_time[i]!=infinity && arr_time[i]!=-infinity){
			count++;
		}
	}
	visited.push_back(count);
}

//run backtracking for a graph from an endpoint to source and save path
void Graph::pathing(unsigned long target, unsigned long source){

/*	path.clear();
    cycle = 0;
    vector<bool> visit (V,false);
    bool change = true;
    unsigned long next;
	unsigned long next_time;
    
    if (f_time[target] != infinity){
        
        cycle = 0;
        path.push_back(target);
        next = std::get<1> (backtracking_opt[target]);
        next_time = std::get<2> (backtracking_opt[target]);
        path.push_back(next);
        
        while (next != source){
            if ( visit[next] ) {
                cycle++;
            }
            visit[next] = true;
            
            for(unsigned long i=0; i<predecessor[next].size(); i++){
                if (std::get<0> (predecessor[next][i]) == next_time) {
                    
                    next_time = std::get<2> (predecessor[next][i]);
                    next = std::get<1> (predecessor[next][i]) ;
                    path.push_back(next);
                    break;
                }
            }
        }
    }*/
}

/*----------------------------------------------------------------------*/
//added by sanaz: to run minhop ignoring the newly added nodes
void Graph::mod_dijkstra_minhop(unsigned long time, unsigned long source, unsigned long counter, long (Graph::*fptr)(unsigned long, unsigned long, unsigned long)){
   
    vector<unsigned long> reset;
    vector<unsigned long> nodes;
    std::priority_queue<tuples,std::vector<tuples>, std::greater<tuples>> q;
    tuples t;
    distance[source] = arr_time[source];
    // go through all out going arcs of the source vertex at time time
    //printf("Start with time step %lu \n",time);
	for(unsigned long i = edge_track[source];i<source_length;i++){	//run over edges from source
		Edge e = edge_matrix[source][i];
		if(e.t==time){
		   /*if(e.u >= original_V){
		       distance[e.v] = distance[e.u];
		       reached_algo2[e.v] = std::make_tuple (distance[e.u], source, time);
		   }else{*/
		       distance[e.v] = e.o;
                       reached_algo2[e.v] = std::make_tuple (e.o, source/*, time*/);
		   //}
                   t.a = e.v;
                   t.d = e.o;
                   q.push(t);
                   nodes.push_back(e.v);
                   reset.push_back(e.v);
			
		   /*if(backtracking){
                      backtracking_tmp[e.v] = time;
		   }*/

		   if(f_time[e.v]==infinity){	//node visited first time -> fill node_list
			f_time[e.v] = time;
			unsigned long length2 = edge_matrix[e.v].size();
			for(unsigned long j=edge_track[e.v];j<length2;j++){
			     Edge e2 = edge_matrix[e.v][j];
			     if(e2.t>time){
                        	  node_list[timemap[e2.t]].push_back(e2.u);
			     }else if(e2.t<time){
				  edge_track[e.v]=j;
			     }
			}
		   }
		}else if(e.t>time){
		    edge_track[source] = i;
		    break;
                }
        }

	for(unsigned long i: node_list[counter]){	//push node to queue if reached before and has outgoing edges
            while(!d_list[i].empty() && distance[i]==infinity){            
            	   std::tuple <unsigned long,unsigned long> curr = d_list[i].front();
		   if((long) std::get<0> (curr) + (long)dwell_time[i] >= (long)time){	//if list element inside range [t,t+dwell_time]
                        distance[i] = std::get<1>(curr);
			t.a = i;
			t.d = distance[i];
			q.push(t);
			reset.push_back(i);
                	/*if (backtracking){
                    	    backtracking_tmp[i] = std::get<0> (curr);
                	}*/
			break;
		   }else{
			d_list[i].pop_front();		//remove first element -> not needed anymore
            	   }
            }
        }

	while(!q.empty()){			//dijsktra with priority queue
		tuples cur = q.top();
		q.pop();
		if(distance[cur.a]==cur.d){	//only use element if not found better path in this iteration			
            	    unsigned long length = edge_matrix[cur.a].size();
		    for(unsigned long i = edge_track[cur.a];i<length;i++){
                	Edge e = edge_matrix[cur.a][i];
			if(e.t==time){
			    if(e.u >= original_V){
		            	    if(distance[e.v] > (distance[e.u])){						
		                	if(distance[e.v] == infinity){
					   reset.push_back(e.v);	
					}
					/*if(backtracking){
		                    	   backtracking_tmp[e.v] = time;
		                	}*/
		                	distance[e.v] = distance[e.u];
					t.a = e.v;
					t.d = distance[e.v];
					q.push(t);
				     }
			     }else{
		            	    if(distance[e.v] > (distance[e.u] + e.o)){						
		                	if(distance[e.v] == infinity){
					   reset.push_back(e.v);	
					}
					/*if(backtracking){
		                    	   backtracking_tmp[e.v] = time;
		                	}*/
		                	distance[e.v] = distance[e.u] + e.o;
					t.a = e.v;
					t.d = distance[e.v];
					q.push(t);
				     }
			     }
			     if(e.u >= original_V){
		                     if (std::get<0>(reached_algo2[e.v]) > distance[e.u]){
		                        reached_algo2[e.v] = std::make_tuple(distance[e.u], e.u/*, backtracking_tmp[e.u]*/);
		                        nodes.push_back(e.v);
		                     }
			     }else{
		                     if (std::get<0>(reached_algo2[e.v]) > distance[e.u]+ e.o){
		                        reached_algo2[e.v] = std::make_tuple( distance[e.u]+ e.o, e.u/*, backtracking_tmp[e.u]*/);
		                        nodes.push_back(e.v);
		                     }
			     }
			}else if(e.t>time){	//found node after current timestamp [next iteration]
				edge_track[cur.a] = i;
				break;
                        }
                   }
               }
         }

        // for all reached vertices exactly at time step time
	for(unsigned long i: nodes){
		// updat optimality values
		if ( arr_time[i] != (this->*fptr)(arr_time[i], std::get<0>(reached_algo2[i]), time)){		      
		    arr_time[i] = (this->*fptr)(arr_time[i], std::get<0>(reached_algo2[i]), time);
		    //backtracking_opt[i] =  std::make_tuple(time, std::get<1>(reached_algo2[i]), std::get<2>(reached_algo2[i]));
		}
		// update list of time steps a vertex was reached 
		/*if (backtracking){
		    std::tuple<unsigned long,unsigned long,unsigned long> pre = std::make_tuple(time, std::get<1>(reached_algo2[i]),std::get<2>(reached_algo2[i]));
		    predecessor[i].push_back(pre);
		}*/
        	std::tuple <unsigned long,unsigned long> tmp = std::make_tuple(time, std::get<0>(reached_algo2[i]));
		while(!d_list[i].empty()){            
		    std::tuple <unsigned long,unsigned long> curr = d_list[i].back();
		    if(std::get<1>(curr) >= std::get<1>(tmp)){
		       d_list[i].pop_back();
		    }else{
		       break;
		    }		    
		}
        	d_list[i].push_back(tmp);		
		if(f_time[i]==infinity){	//node visited first time -> fill node_list
                    f_time[i] = time;
		    unsigned long length2 = edge_matrix[i].size();
		    for(unsigned long j=edge_track[i];j<length2;j++){
                	Edge e2 = edge_matrix[i][j];
                	if(e2.t>time){
                    	   if ( node_list[timemap[e2.t]].empty() || node_list[timemap[e2.t]].back() != e2.u ){
                        	node_list[timemap[e2.t]].push_back(e2.u);
                    	   }
                	}else if(e2.t<time){
                    	   edge_track[i]=j;
                	}
            	    }
        	}		
		reached_algo2[i] = std::make_tuple (infinity,0/*,0*/);
		distance[i] = infinity;
	}
	for(unsigned long i: reset){	//reset distance for vertices which werent found but used as E_r vertices
		distance[i] = infinity;			//reset distance of vertices used by source vertex
	}
}


#endif
