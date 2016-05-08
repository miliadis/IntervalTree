#include "IntervalTree.h"
#include <stddef.h>
#include <iostream>

using namespace std;

// Utility function to insert nodes in the Interval tree
TreeNode* insert(TreeNode *root, TreeNode *p)
{
	if (root == NULL){
		return p;
	}

	float start = root->span->start_time;

	// Check for duplicates. If so, just increase the counter of this node - useful for Red-Black Trees
	if (p->span->start_time == root->span->start_time && p->span->end_time == root->span->end_time){
		std::cout << "Time Span " << p->span->start_time << "," << p->span->end_time << " exists" << endl;
		(p->count)++;
		return p;
	}

	if (p->span->start_time < start){
		root->left = insert(root->left, p);
		root->left->parent = root;
	}
	else{
		root->right = insert(root->right, p);
		root->right->parent = root;
	}

	if (root->max < p->span->end_time)
		root->max = p->span->end_time;

	return root;
}

// Find max between two floats
float findMax(float a, float b){
	return ((a > b) ? a : b);
}

// Rotate tree left - needed for Red-Black Trees 
void IntervalTree::rotateLeft(TreeNode *&root, TreeNode *&pt)
{
	TreeNode *pt_right = pt->right;

	pt->right = pt_right->left;

	if (pt->right != NULL)
		pt->right->parent = pt;

	pt_right->parent = pt->parent;

	if (pt->parent == NULL)
		root = pt_right;

	else if (pt == pt->parent->left)
		pt->parent->left = pt_right;

	else
		pt->parent->right = pt_right;

	pt_right->left = pt;
	pt->parent = pt_right;

	// If statements to update the max value of the interval tree after rotation
	if (pt->left == NULL && pt->right != NULL){
		pt->max = findMax(pt->right->max, pt->span->end_time);
	}

	if (pt->left != NULL && pt->right != NULL){
		pt->max = findMax(pt->left->max, findMax(pt->right->max, pt->span->end_time));
	}

	if (pt->left == NULL && pt->right == NULL){
		pt->max = pt->span->end_time;
	}

	if (pt->left != NULL && pt->right == NULL){
		pt->max = findMax(pt->left->max, pt->span->end_time);
	}

	if (pt_right->right == NULL){
		pt_right->max = findMax(pt->max, pt_right->span->end_time);
	}
	else{
		pt_right->max = findMax(pt->max, findMax(pt_right->right->max, pt_right->span->end_time));
	}

}
// Rotate tree right - needed for Red-Black Trees 
void IntervalTree::rotateRight(TreeNode *&root, TreeNode *&pt)
{
	TreeNode *pt_left = pt->left;

	pt->left = pt_left->right;

	if (pt->left != NULL)
		pt->left->parent = pt;

	pt_left->parent = pt->parent;

	if (pt->parent == NULL)
		root = pt_left;

	else if (pt == pt->parent->left)
		pt->parent->left = pt_left;

	else
		pt->parent->right = pt_left;

	pt_left->right = pt;
	pt->parent = pt_left;

	// If statements to update the max value of the interval tree after rotation
	if (pt->left == NULL && pt->right != NULL){
		pt->max = findMax(pt->right->max, pt->span->end_time);
	}

	if (pt->left != NULL && pt->right != NULL){
		pt->max = findMax(pt->left->max, findMax(pt->right->max, pt->span->end_time));
	}

	if (pt->left == NULL && pt->right == NULL){
		pt->max = pt->span->end_time;
	}

	if (pt->left != NULL && pt->right == NULL){
		pt->max = findMax(pt->left->max, pt->span->end_time);
	}

	if (pt_left->left == NULL){
		pt_left->max = findMax(pt->max, pt_left->span->end_time);
	}
	else{
		pt_left->max = findMax(pt_left->left->max, findMax(pt->max, pt_left->span->end_time));
	}
}

// This function fixes violations caused by Interval Tree insertion
void IntervalTree::balanceTree(TreeNode *&root, TreeNode *&pt)
{
	TreeNode *parent_pt = NULL;
	TreeNode *grand_parent_pt = NULL;

	while ((pt != root) && (pt->color != BLACK) &&
		(pt->parent->color == RED))
	{

		parent_pt = pt->parent;
		grand_parent_pt = pt->parent->parent;

		/*  Case : A
		Parent of pt is left child of Grand-parent of pt */
		if (parent_pt == grand_parent_pt->left)
		{

			TreeNode *uncle_pt = grand_parent_pt->right;

			/* Case : 1
			The uncle of pt is also red
			Only Recoloring required */
			if (uncle_pt != NULL && uncle_pt->color == RED)
			{
				grand_parent_pt->color = RED;
				parent_pt->color = BLACK;
				uncle_pt->color = BLACK;
				pt = grand_parent_pt;
			}

			else
			{
				/* Case : 2
				pt is right child of its parent
				Left-rotation required */
				if (pt == parent_pt->right)
				{
					rotateLeft(root, parent_pt);
					pt = parent_pt;
					parent_pt = pt->parent;
				}

				/* Case : 3
				pt is left child of its parent
				Right-rotation required */
				rotateRight(root, grand_parent_pt);
				swap(parent_pt->color, grand_parent_pt->color);
				pt = parent_pt;
			}
		}

		/* Case : B
		Parent of pt is right child of Grand-parent of pt */
		else
		{
			TreeNode *uncle_pt = grand_parent_pt->left;

			/*  Case : 1
			The uncle of pt is also red
			Only Recoloring required */
			if ((uncle_pt != NULL) && (uncle_pt->color == RED))
			{
				grand_parent_pt->color = RED;
				parent_pt->color = BLACK;
				uncle_pt->color = BLACK;
				pt = grand_parent_pt;
			}
			else
			{
				/* Case : 2
				pt is left child of its parent
				Right-rotation required */
				if (pt == parent_pt->left)
				{
					rotateRight(root, parent_pt);
					pt = parent_pt;
					parent_pt = pt->parent;
				}

				/* Case : 3
				pt is right child of its parent
				Left-rotation required */
				rotateLeft(root, grand_parent_pt);
				swap(parent_pt->color, grand_parent_pt->color);
				pt = parent_pt;
			}
		}
	}

	root->color = BLACK;
}

// Check whether a time is within a range in the interval tree node 
bool withinRange(Time t, float search_time){
	if (t.start_time <= search_time && search_time <= t.end_time){
		return true;
	}
	else{
		return false;
	}
}

// Utility function for Interval tree search
bool intervalSearch(TreeNode *root, float search_time)
{
	// Base Case, tree is empty
	if (root == NULL){
		return NULL;
	}

	// Return true if the current node overlaps with the given time
	if (withinRange(*(root->span), search_time)){
		return true;
	}

	// If left child of root is present and max of left child is
	// greater than or equal to given interval, then i may
	// overlap with an interval is left subtree
	if (root->left != NULL && root->left->max >= search_time){
		return intervalSearch(root->left, search_time);
	}
	else{
		// Else interval can only overlap with right subtree
		return intervalSearch(root->right, search_time);
	}
}

// Interface function to insert rush hours in the Interval tree
void IntervalTree::AddTimeSpan(float start_time, float end_time){

	// Create a new Interval node
	Time *tmp = new Time;
	tmp->start_time = start_time;
	tmp->end_time = end_time;

	TreeNode *p = new TreeNode(tmp);

	// Check whether root is empty. Then check whether the node to be added already exists and it is the root
	if (root == NULL){
		root = insert(root, p);
	}
	else if (p->span->start_time == root->span->start_time && p->span->end_time == root->span->end_time){
		std::cout << "Time Span " << p->span->start_time << "," << p->span->end_time << " exists" << endl;
		(root->count)++;
		p = root;
	}
	else{
		root = insert(root, p);
	}

	// Balancing step - Balance the tree using Red-Black Tree Algorithm
	if (p->count == 0){
		balanceTree(root, p);
	}
}

// Interface function to check for rush hours
bool IntervalTree::IsRushHour(float time){
	bool rush = intervalSearch(root, time);
	return rush;
}
