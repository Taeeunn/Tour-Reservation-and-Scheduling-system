#define _CRT_SECURE_NO_WARNINGS
#include "final.h"

void r_create_nilnode(reserve_p *T) {
	reserve_p Null = (reserve_p)malloc(sizeof(reservation));
	Null->clr = 'B';
	(*T) = Null;
	(*T)->nil = Null;
}

void r_Left_Rotate(reserve_p* T, reserve_p z) {
	reserve_p temp;

	temp = z->right;
	z->right = temp->left;

	if (temp->left != (*T)->nil) {
		temp->left->parent = z;
	}

	temp->parent = z->parent;

	if (z->parent == (*T)->nil) {
		(*T) = temp;
	}
	else if (z == z->parent->left) {
		z->parent->left = temp;
	}
	else {
		z->parent->right = temp;
	}

	temp->left = z;
	z->parent = temp;
}

void r_Right_Rotate(reserve_p* T, reserve_p z) {
	reserve_p temp;

	temp = z->left;
	z->left = temp->right;

	if (temp->right != (*T)->nil) {
		temp->right->parent = z;
	}

	temp->parent = z->parent;

	if (z->parent == (*T)->nil) {
		(*T) = temp;
	}
	else if (z == z->parent->left) {
		z->parent->left = temp;
	}
	else {
		z->parent->right = temp;
	}
	temp->right = z;
	z->parent = temp;
}

void r_insert_fixup(reserve_p* T, reserve_p z) {
	reserve_p y = NULL;
	while (z->parent->clr == 'R') {
		if (z->parent == z->parent->parent->left) {
			y = z->parent->parent->right;
			if (y->clr == 'R') {
				z->parent->clr = 'B';
				y->clr = 'B';
				z->parent->parent->clr = 'R';
				z = z->parent->parent;
			}
			else {
				if (z->parent->right == z) {
					z = z->parent;
					r_Left_Rotate(T, z);
				}
				z->parent->clr = 'B';
				z->parent->parent->clr = 'R';
				r_Right_Rotate(T, z->parent->parent);
			}
		}

		else {
			y = z->parent->parent->left;

			if (y->clr == 'R') {
				z->parent->clr = 'B';
				y->clr = 'B';

				z->parent->parent->clr = 'R';

				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {
					z = z->parent;
					r_Right_Rotate(T, z);
				}

				z->parent->clr = 'B';
				z->parent->parent->clr = 'R';
				r_Left_Rotate(T, z->parent->parent);
			}
		}
	}
	(*T)->clr = 'B';
}

void Reserve_INSERT(reserve_p* T, char* id, int dist, int cost, int period, route_p route_, char* dept, char* dest) { 
	reserve_p new_Node = (reserve_p)malloc(sizeof(reservation));
	strcpy(new_Node->id, id);
	new_Node->cost = cost;
	new_Node->dist = dist;
	new_Node->period = period;
	new_Node->route_=route_;
	strcpy(new_Node->departure, dept);
	strcpy(new_Node->destination, dest);
	new_Node->left = (*T)->nil; new_Node->right = (*T)->nil; new_Node->parent = NULL;
	new_Node->clr = 'R';
	new_Node->nil = (*T)->nil;
	reserve_p y = (*T)->nil;
	reserve_p x = *T;

	while (x != (*T)->nil) {
		y = x;
		if (strcmp(id, x->id) < 0)
			x = x->left;
		else if (strcmp(id, x->id) > 0)
			x = x->right;
		else
			return;
	}
	new_Node->parent = y;

	if (y == (*T)->nil)
		*T = new_Node;
	if (strcmp(id, y->id) < 0)
		y->left = new_Node;
	else
		y->right = new_Node;
	r_insert_fixup(T, new_Node);
}


void r_tree_transplant(reserve_p* T, reserve_p t, reserve_p c)
{
	if (t->parent == (*T)->nil)
	{
		(*T) = c;
	}
	else if (t == (t->parent)->left)
	{
		(t->parent)->left = c;
	}
	else
	{
		(t->parent)->right = c;
	}

	c->parent = t->parent;
}

void r_delete_fixup(reserve_p* T, reserve_p x)
{
	reserve_p s = NULL;

	while ((x != *T) && (x->clr == 'B'))
	{
		if (x == (x->parent)->left)
		{
			s = (x->parent)->right;


			if (s->clr == 'R')
			{
				s->clr = 'B';
				(x->parent)->clr = 'R';
				r_Left_Rotate(T, x->parent);

				s = (x->parent)->right;
			}


			if ((s->left)->clr == 'B' && (s->right)->clr == 'B')
			{
				s->clr = 'R';
				x = x->parent;
			}

			else if ((s->left)->clr == 'R' && (s->right)->clr == 'B')
			{
				s->clr = 'R';
				(s->left)->clr = 'B';
				r_Right_Rotate(T, s);

				s = (x->parent)->right;
			}


			if ((s->right)->clr == 'R')
			{
				s->clr = (x->parent)->clr;
				(s->right)->clr = 'B';
				(x->parent)->clr = 'B';
				r_Left_Rotate(T, x->parent);

				x = *T;
			}

		}
		else
		{
			s = (x->parent)->left;


			if (s->clr == 'R')
			{
				s->clr = 'B';
				(x->parent)->clr = 'R';
				r_Right_Rotate(T, x->parent);
				// update x's sibling
				s = (x->parent)->left;
			}


			if ((s->left)->clr == 'B' && (s->right)->clr == 'B')
			{
				s->clr = 'R';
				x = x->parent;
			}

			else if ((s->right)->clr == 'R' && (s->left)->clr == 'B')
			{
				s->clr = 'R';
				(s->right)->clr = 'B';
				r_Left_Rotate(T, s);
				// update x's sibling
				s = (x->parent)->left;
			}


			if ((s->left)->clr == 'R')
			{
				s->clr = (x->parent)->clr;
				(s->left)->clr = 'B';
				(x->parent)->clr = 'B';
				r_Right_Rotate(T, x->parent);

				x = *T;
			}
		}
	}

	x->clr = 'B';
}

int Reserve_DELETE(reserve_p *T, char* id)
{
	reserve_p target = *T;
	reserve_p temp = NULL;
	reserve_p x = NULL;
	char t_clr;


	while (strcmp(id, target->id) != 0)
	{
		if (target == (*T)->nil)
			return 0;
		if (strcmp(target->id, id) > 0)
		{
			target = target->left;
		}
		else if (strcmp(target->id, id) < 0)
		{
			target = target->right;
		}
	}

	t_clr = target->clr;

	if (target->left == (*T)->nil)
	{
		x = target->right;
		r_tree_transplant(T, target, target->right);
	}

	else if (target->right == (*T)->nil)
	{
		x = target->left;
		r_tree_transplant(T, target, target->left);
	}
	else
	{
		temp = target->right;


		while (temp->left != (*T)->nil)
		{
			temp = temp->left;
		}

		t_clr = temp->clr;
		x = temp->right;

		r_tree_transplant(T, temp, temp->right);
		temp->right = target->right;
		(temp->right)->parent = temp;

		r_tree_transplant(T, target, temp);
		temp->left = target->left;
		(temp->left)->parent = temp;
		temp->clr = target->clr;
	}

	if (t_clr == 'B')
	{
		r_delete_fixup(T, x);
	}

	free(target);
	return 1;
}


reserve_p search_id(reserve_p* r_T, char* id) {
	reserve_p x = *(r_T);
	while (x != (*r_T)->nil) {
		if (strcmp(id, x->id) < 0)
			x = x->left;
		else if (strcmp(id, x->id) > 0)
			x = x->right;
		else
			return x;
	}
	return NULL;
}