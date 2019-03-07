#define _CRT_SECURE_NO_WARNINGS
#include "final.h"


int main() {

	hotel_p root = NULL;
	hotel_p* h_T = &root;
	root = make_hotel(h_T);
	site_p site_Graph[100] = { 0 };
	make_graph(site_Graph, root);
	reserve_p r_root = NULL;
	reserve_p* r_T = &r_root;
	r_create_nilnode(r_T);

	char str[100];
	int ch;
	while (1) {
		system("cls");
		printTitle();
		menu();
		printf("\nselect number: ");

		get_int(&ch); 

		while (ch==-1 || (ch != 1 && ch != 2 && ch != 3 && ch != 4 && ch != 5 && ch != 0)) {
			printf("Wrong Input. Select again: ");
			get_int(&ch);
		}

		switch (ch) {
		case 1: system("cls");
			printf("-¡Ú Make Travel Plan ¡Ú-\n\n");
			Make_plan(site_Graph, root, r_T);
			break;
		case 2: system("cls");
			printf("-¡Ú Check Travel Plan ¡Ú-\n\n");
			Check_Plan(site_Graph, r_T);
			break;
		case 3: system("cls");
			printf("-¡Ú Cancel Travel Plan ¡Ú-\n\n");
			Cancel_Plan(r_T);
			break;
		case 4: system("cls");
			printf("- ¡Ú View all Travel Route ¡Ú -\n\n");
			Print_Route(site_Graph);
			break;
		case 5: system("cls");
			printf("- ¡Ú View all Hotel ¡Ú -\n\n");
			Print_Hotel(root, h_T);
			break;
		case 0: exit(0);
		default:
			break;
		}
		system("pause");

	}
	for (int i = 0; i < 100; i++) str[i] = '\0';
}