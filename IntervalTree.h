#pragma once
#include <stddef.h>


// Class for the rush hour interval
struct Time
{
	float start_time;
	float end_time;
};

enum Color { RED, BLACK };

// Class for the Interval tree node
struct TreeNode
{
	Time *span;
	float max;
	bool color;
	int count;
	TreeNode *left;
	TreeNode *right;	
	TreeNode *parent;


	TreeNode(Time *span) {
		this->span = span;
		this->max = span->end_time;
		this->color = 0;
		this->count = 0;
		this->left = NULL;
		this->right = NULL;
		this->parent = NULL;		
	}
};

class IntervalTree
{
private:
	TreeNode *root;

protected:
	void rotateLeft(TreeNode *&, TreeNode *&);
	void rotateRight(TreeNode *&, TreeNode *&);
	void balanceTree(TreeNode *&, TreeNode *&);

public:
	IntervalTree() { root = NULL; }
	void AddTimeSpan(float, float);
	bool IsRushHour(float);
};

