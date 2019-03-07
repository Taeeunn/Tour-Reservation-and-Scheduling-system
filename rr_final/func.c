#define _CRT_SECURE_NO_WARNINGS
#include "final.h"

extern route_p travel_route[1000];


void printTitle() {
	printf("-------------------------------------------------\n");
	printf("\t\tSungkyunkwan Tour\n\tReservation and Scheduling System\n");
	printf("-------------------------------------------------\n");
	Sleep(1);
}
void menu() {
	printf("\nTell me what you want to do!!\n\n");
	printf("1. Make Travel Plan\n\n");
	printf("2. Check Travel Plan\n\n");
	printf("3. Cancel Travel Plan\n\n");
	printf("4. View all Travel Route\n\n");
	printf("5. View all Hotel\n\n");
	printf("0. Quit\n");
}

void get_int(int *target_ptr) {
	//Get input and return int.
	//If not proper input, return -1.
	//Else return valid input.
	char *buffer = (char *)malloc(BUFSIZE * sizeof(char));
	int temp, result = 0;
	int i = 0;
	while ((temp = getchar()) != '\n') {
		buffer[i++] = (char)temp;
		if (!(buffer[i - 1] >= '0'&&buffer[i - 1] <= '9')) {
			while ((temp = getchar()) != '\n');
			*target_ptr = -1;
			free(buffer);
			return;
		}
		if (i == BUFSIZE) {
			printf("Too long input. Maximum: %d\n", BUFSIZE);
			while ((temp = getchar()) != '\n');
			*target_ptr = -1;
			free(buffer);
			return;
		}
	}
	if (buffer[0] == '\0') {
		*target_ptr = -1;
		free(buffer);
		return;
	}
	buffer[i] = '\0';
	for (i = 0; buffer[i] != '\0'; i++) {
		result = result * 10 + buffer[i] - '0';
	}
	*target_ptr = result;
	free(buffer);
	return;
}

void get_str(char *target_ptr) {
	//get input and write it on target_ptr.
	//If input invalid, then the first char in target_ptr will be '\0'.
	int temp;
	int i = 0;
	while ((temp = getchar()) != '\n') {
		target_ptr[i++] = (char)temp;
		if (i == BUFSIZE) {
			printf("Too long input. Maximum: %d\n", BUFSIZE);
			while ((temp = getchar()) != '\n');
			target_ptr[0] = -1;
			return;
		}
	}
	if (target_ptr[0] == '\0') {
		target_ptr[0] = -1;
		return;
	}
	target_ptr[i] = '\0';
	return;
}


hotel_p search_hotel(hotel_p root, int hotel_num, hotel_p nil_node) {
	//Search hotel and return hotel
	char hotelname[50], num[4];
	strcpy(hotelname, "hotel-");
	_itoa(hotel_num, num, 10);
	strcat(hotelname, num);
	hotel_p left, right;

	if (root != nil_node) {
		if (strcmp(root->name, hotelname)==0) return root;
		left = search_hotel(root->left, hotel_num, nil_node);
		if (left != NULL) return left;
		right = search_hotel(root->right, hotel_num, nil_node);
		if (right != NULL) return right;
		return NULL;
	}
	else
		return NULL;
}


void Make_plan(site_p * graph, hotel_p root, reserve_p* r_T) {
	// Make reservation according to user input. 
	reserve_p new = (reserve_p)malloc(sizeof(reservation));

	printf("Enter your id: "), get_str(new->id);
	if ((new->id)[0] == -1) {
		printf("\nInvalid Input! Please verify and try again.\n\n");
		return;
	}
	if (search_id(r_T, new->id) != NULL) {
		printf("\nThis ID is already in reservation table.\n\n");
		return;
	}

	printf("Enter your departure: "), get_str(new->departure);
	if ((new->id)[0] == -1 || index_vertex(graph, new->departure) == -1) {
		printf("\nInvalid Input! Please verify and try again.\n\n");
		return;
	}
	printf("Enter your destination: "), get_str(new->destination);
	if ((new->id)[0] == -1 || index_vertex(graph, new->destination) == -1) {
		printf("\nInvalid Input! Please verify and try again.\n\n");
		return;
	}
	printf("Enter your budget: "), get_int(&new->cost);
	if (new->cost == -1) {
		printf("\nInvalid Input! Please verify and try again.\n\n");
		return;
	}
	printf("Enter your period: "), get_int(&new->period);
	if (new->period == -1) {
		printf("\nInvalid Input! Please verify and try again.\n\n");
		return;
	}

	find_route(graph, new->departure, new->destination, new->period, new->cost);

	int i, k = 0, hotel_num, d = 0, hotelcost = 0;
	hotel_p h;
	if (travel_route[0] == NULL) printf("\n경로가 없습니다.\n\n");
	else {
		for (i = 0; travel_route[i] != NULL; i++) {
			printf("\n*******Travel Route %d*******\n\n", i + 1);
			print_route(graph, travel_route[i]);
		}
		printf("Select travel route: "); get_int(&d);
		printf("\n");
		if(( d<1 || d>1000 ) || ( travel_route[d-1] == NULL)){
			printf("\nInvalid Input! Please verify and try again.\n\n");
			return;
		}
		route_p temp = travel_route[d - 1];
		while (travel_route[d - 1] != NULL) {
			print_avail_hotels(root, root->nil, new->cost-get_cost_of_route(graph, temp));
			
			printf("\nIn %s, Select Hotel number (Enter 0 if not wanted or unavailable): ", travel_route[d - 1]->name); get_int(&hotel_num);
			puts("");
			if(hotel_num==0){
				printf("You have skipped the hotel.\n");
			}
			else if(( hotel_num<1 || hotel_num>100) || (search_hotel(root, hotel_num, root->nil)->cost > new->cost-get_cost_of_route(graph, temp))){
				printf("\nInvalid Input! Please verify and try again.\n\n");
				return;
			}
			else{
				h = search_hotel(root, hotel_num, root->nil);
				hotelcost += h->cost;
				travel_route[d - 1]->hotel_ = h;
			}
			travel_route[d - 1] = travel_route[d - 1]->link;
		}
		travel_route[d - 1] = temp;


		make_reservation(r_T, new->id, get_dist_of_route(graph, travel_route[d - 1]),
			get_cost_of_route(graph, travel_route[d - 1]), get_period_of_route(graph, travel_route[d - 1]),
			travel_route[d - 1], new->departure, new->destination);

		printf("Reservation is successfully completed!\n\n");
		free(new);
	}
}




void Reservation_confirm(site_p * graph, reserve_p rc) {
	//Confirm reservation (print user reservation information) 
	printf("Please confirm your reservation!\n");
	printf("------------------------------------------\n");
	printf("ID : %s\n", rc->id);
	printf("Departure : %s\n", rc->departure);
	printf("Destination : %s\n", rc->destination);
	print_route(graph, rc->route_);
	printf("------------------------------------------\n");
	
}


void Check_Plan(site_p * graph, reserve_p* T) {
	// Check reservation
	if ((*T) == (*T)->nil) {
		printf("Reservation table is empty..\n\n");
		return;
	}
	char id[BUFSIZE];
	reserve_p temp;
	
	while (1) {
		printf("Enter your ID : ");
		get_str(id);
		if(id[0]==-1)
			return;
		temp = search_id(T, id);
		if (temp == NULL) {
			printf("\n-- Can't find your ID. --\n");
			printf("-- Go to the main menu. --\n\n");
		}
		else {
			printf("\n");
			Reservation_confirm(graph, temp);
			break;
		}
		break;
	}
}



void Cancel_Plan(reserve_p* T) {
	// Cancel reservation
	if ((*T) == (*T)->nil) {
		printf("Reservation Table is empty..\n\n");
		return;
	}

	char id[BUFSIZE];
	printf("Please Enter your ID: "), get_str(id);
	if(id[0]==-1)
			return;
	if (search_id(T, id) == NULL) {
		printf("\n-- Can't find your ID. --\n");
		printf("-- Go to the main menu. --\n\n");
	}

	if(Reserve_DELETE(T, id)==1) 
		printf("\nReservation is canceled.\n\n");
}

void Print_Route(site_p * site_Graph) {
	// Print routes from 100 sites.  
	site_p w;
	for (int i = 0; i < 100; i++) {
		printf("");
		for (w = site_Graph[i]; w; w = w->edge_link) {
			printf("[%8s]  ", w->name);
			if (w->edge_link != NULL)printf("→   ");
		}
		printf("\n");
	}
	printf("\n\n");
}

void inorder(hotel_p* h_T, hotel_p s) {
	// Print hotel inorder of cost (ascending order). 
	if (s != (*h_T)->nil) {
		inorder(h_T, s->left);
		printf("%s \t[%d]\n", s->name, s->cost);
		inorder(h_T, s->right);
	}
}

void Print_Hotel(hotel_p root, hotel_p *hotel) {
	// Print hotel and cost 
	printf("  Hotel   \t[cost]\n");
	printf("-----------------------\n");
	inorder(hotel, root);
	printf("\n\n");
}