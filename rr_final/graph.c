#define _CRT_SECURE_NO_WARNINGS
#include "final.h"

route_p travel_route[1000] = { 0 }; //경유지를 링크드리스트에 저장
char stack[1000][20]; int top = -1, r = 0;

void add_vertex(site_p* arr, char* site_name, hotel_p root) {
	
	int stay = rand() % 6 + 5; // 체류시간을 4시간~10시간 사이 랜덤 추출
	int i = 0;
	site_p newnode = (site_p)malloc(sizeof(site));
	strcpy(newnode->name, site_name);
	newnode->stay_time = stay;
	newnode->cost = stay * 10000; // 여행비용은 여행시간 * 10000
	newnode->head = (root);
	newnode->linked_transes.cost = 0;
	newnode->linked_transes.dist = 0;
	newnode->linked_transes.travel_time = 0;

	newnode->edge_link = NULL;
	if (*arr == NULL)
		*arr = newnode;
	else {
		while (arr[i] != 0) {
			if (strcmp(arr[i]->name, site_name) == 0)
				return;
			i++;
		}
		arr[i] = newnode;
	}
}

//site 간 연결, 그래프 edge의 weight는 trans 정보
void add_edge(site_p v1, site_p v2, trans weight) {
	site_p new = (site_p)malloc(sizeof(site));
	strcpy(new->name, v2->name);
	new->cost = v2->cost;
	new->stay_time = v2->stay_time;
	new->head = v2->head;

	new->linked_transes = weight;

	new->edge_link = NULL;

	if (v1->edge_link == NULL)
		v1->edge_link = new;
	else {
		site_p temp = v1;
		while (temp->edge_link != NULL)
			temp = temp->edge_link;
		temp->edge_link = new;
	}
}

int index_vertex(site_p* arr, char *site_name) {
	int i = 0;
	while (strcmp(arr[i]->name, site_name) != 0) {
		i++;
		if (i >= 100)
			return -1;
	}
	return i;
}

void DFS(site_p* site_Graph, char* vertex, int* Visited) {
	int v = index_vertex(site_Graph, vertex);
	site_p w;
	Visited[v] = 1;
	printf("%s", vertex);
	for (w = site_Graph[v]; w; w = w->edge_link) {
		int temp = index_vertex(site_Graph, w->name);
		if (Visited[temp] == 0) {
			printf("[%d] ", w->linked_transes.dist);
			DFS(site_Graph, w->name, Visited);
		}
	}
}

void DFS_route(site_p* site_Graph, char* v, char* goal, int* Visited) {
	int v_index = index_vertex(site_Graph, v);
	site_p w = site_Graph[v_index];
	Visited[v_index] = 1;
	if (top > 8) return;
	strcpy(stack[++top], v);
	if (strcmp(v, goal) == 0) {
		if (r < 1000 && top <= 8) { //저장할 최대 경로 수
			for (int i = 0; i <= top; i++) {
				route_p newNode = (route_p)malloc(sizeof(route));
				strcpy(newNode->name, stack[i]);
				newNode->link = NULL;
				newNode->hotel_ = NULL;
				int tmp_end = index_vertex(site_Graph, stack[i]); //교통정보를 가진 노드를 탐색
				if (travel_route[r] == NULL) {
					newNode->trans_info = site_Graph[tmp_end]->linked_transes;
					travel_route[r] = newNode;
				}
				else {
					int tmp_start = index_vertex(site_Graph, stack[i - 1]);
					site_p temp = site_Graph[tmp_start];
					while (strcmp(temp->name, stack[i]) != 0) { //trans 정보를 얻기 위해 경유지간 엣지노드를 확인
						temp = temp->edge_link;
					}
					newNode->trans_info = temp->linked_transes;
					route_p temp_route = travel_route[r];

					while (temp_route->link != NULL) {
						temp_route = temp_route->link;
					}
					temp_route->link = newNode;
				}
			}
			r++;
		}
		top--;
		return;
	}

	while (w != NULL) {
		int temp = index_vertex(site_Graph, w->name);
		if (Visited[temp] == 0) {
			DFS_route(site_Graph, w->name, goal, Visited);
			Visited[temp] = 0;
		}
		w = w->edge_link;
	}
	top--;
}

void make_graph(site_p* site_Graph, hotel_p root) {
	srand((unsigned int)time(NULL));
	char site_name[10]; strcpy(site_name, "site-");
	//그래프 vertex 생성, site-1 ~ site-100
	for (int i = 1; i <= 100; i++) {
		int j = 0;
		char num[10];
		_itoa(i, num, 10);
		for (j = 0; num[j] != '\0'; j++) {
			site_name[j + 5] = num[j]; // ex) site-1, site-78
		}
		site_name[j + 5] = '\0';
		add_vertex(site_Graph, site_name, root);
	}


	// site 간 edge(trans) 연결
	int site_1, site_2; trans trans_info;
	//for (int i = 0; i < 100; i++) {
	//	//trans를 랜덤으로 설정
	//	trans_info.dist = rand() % 150 + 51; //50~200 사이 랜덤 추출
	//	trans_info.cost = trans_info.dist * 100;
	//	trans_info.travel_time = trans_info.dist / 50;

	//	if (i == 99)
	//		add_edge(site_Graph[i], site_Graph[0], trans_info);
	//	else
	//		add_edge(site_Graph[i], site_Graph[i + 1], trans_info);
	//	
	//}

	for (int i = 0; i < 300; i++) {
		// site 두 개를 랜덤으로 추출
		site_1 = rand() % 100;
		site_2 = rand() % 100;

		while (1) {
			if (site_1 != site_2)
				break;
			else
				site_2 = rand() % 100;
		}

		while (1) {
			site_p temp = site_Graph[site_1];
			while (temp != NULL) {
				if (strcmp(site_Graph[site_2]->name, temp->name) == 0) {
					site_2 = rand() % 100;
					break;
				}
				temp = temp->edge_link;
			}
			if (temp == NULL)
				break;
		}


		//trans를 랜덤으로 설정
		trans_info.dist = rand() % 150 + 51; //50~200 사이 랜덤 추출
		trans_info.cost = trans_info.dist * 100;
		trans_info.travel_time = trans_info.dist / 50;

		add_edge(site_Graph[site_1], site_Graph[site_2], trans_info);
	}
}
