#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

#define _CRT_SECURE_NO_WARNINGS
#define BUFSIZE 50

typedef struct trans { //graph weight
	int dist; //50 - 200 random select
	int cost; //dist * 100
	int travel_time; // dist / 50 
}trans;


typedef struct hotel* hotel_p;
typedef struct hotel {
	hotel_p left; //Left child within RB tree.
	hotel_p right; //Right child within RB tree.
	hotel_p parent; //Parent within RB tree.
	hotel_p nil;
	char clr; // r,b color

	char name[BUFSIZE];
	int cost; //50 - 300 random select ÈÄ * 1000

}hotel;

typedef struct site* site_p;
typedef struct site {
	char name[BUFSIZE];
	int stay_time; //4~10 random select
	int cost; // travel cost -> stay_time *  10000
	trans linked_transes; //Transportations which connect this site to other sites. graph edge weight

	site_p edge_link; // edge(trans) info
	hotel_p head; // link to hotel tree root
}site;

typedef struct map {
	hotel hotels[100];
}map;

typedef struct route* route_p;
typedef struct route {
	char name[BUFSIZE];
	hotel_p hotel_;
	trans trans_info;
	route_p link;
}route;

typedef struct reservation* reserve_p;
typedef struct reservation {
	reserve_p left; //Left child within RB tree.
	reserve_p right; //Right child within RB tree.
	reserve_p parent; //Parent within RB tree.
	reserve_p nil;
	char clr;
	route_p route_; //From the start site to destination site.
	char id[BUFSIZE];
	int dist;
	int cost;
	int period;
	char departure[10];
	char destination[10];
}reservation;



//***functions***

//Middle level functions

//Initialize a map that contains sites, trans, hotels.
void init_map(site_p* site_Graph, hotel_p root);
//Three functions below return period, cost, dist of one target route ignoring hotel.
int get_period_of_route(site_p* site_Graph, route_p target_route_p);
int get_cost_of_route(site_p* site_Graph, route_p target_route_p);
int get_dist_of_route(site_p* site_Graph, route_p target_route_p);
//Finds routes in terms of period and budget. Results are stored in 'travel_route', an array of route_p.
//You dont need to reset visit array! This function handles it.
void find_route(site_p* site_Graph, char* start, char* dest, int period, int budget);
//Returns index of cheapest, shortest dist, shortest period route within 'travel_route' array.
//If these functions returns -1, something is really wrong.
int get_cheapest_route_index(site_p* site_Graph);
int get_shortest_dist_route_index(site_p* site_Graph);
int get_shortest_period_route_index(site_p* site_Graph);
//Make reservation given infomation.
void make_reservation(reserve_p* T, char* id, int dist, int cost, int period, route_p route_, char* dept, char* dest);
//Prints a hotel infomation.
void print_hotel(hotel_p target_hotel_p);
//Prints every cheaper than 'cost' hotel. Call this function with 'temp_hotel_p' as hotel tree's head.
void print_avail_hotels(hotel_p temp_hotel_p, hotel_p nil_node, int cost);
//Prints a route.
void print_route(site_p* site_Graph, route_p target_route_p);

//graph
void make_graph(site_p*, hotel_p);
void add_vertex(site_p*, char*, hotel_p);
void add_edge(site_p, site_p, trans);
int index_vertex(site_p*, char*);
void DFS(site_p*, char*, int*);
void DFS_route(site_p*, char*, char*, int*);

//hotel
hotel_p make_hotel(hotel_p*);
void RB_INSERT(hotel_p*, char*, int);
void insert_fixup(hotel_p*, hotel_p);
void Right_Rotate(hotel_p*, hotel_p);
void Left_Rotate(hotel_p*, hotel_p);
void create_nilnode(hotel_p*);

//reservation
void Reserve_INSERT(reserve_p*, char*, int, int, int, route_p, char*, char*);
void r_insert_fixup(reserve_p*, reserve_p);
void r_Right_Rotate(reserve_p*, reserve_p);
void r_Left_Rotate(reserve_p*, reserve_p);
void r_create_nilnode(reserve_p*);
int Reserve_DELETE(reserve_p*, char*);
reserve_p search_id(reserve_p*, char*);


