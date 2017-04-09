#include<iostream>
#include "../EWAHBoolArray/headers/ewah.h"
#include "../EWAHBoolArray/headers/boolarray.h"

using namespace std;

struct node
{
	int key_value; //This is the keyValue
	int cardinality;
	EWAHBoolArray<int> bitmap; 
	node *left;
	node *right;
};

class btree
{
	public:
		btree();
		~btree();

		void insert(int key,int cardinality,EWAHBoolArray<int> bitmap);
		EWAHBoolArray<int> rangeQuery(int min,int max);
		node *search(int key);
		void destroy_tree();
		void dfs();
		void inOrder();
	private:
		void destroy_tree(node *leaf);
		void insert(int key,int cardinality,EWAHBoolArray<int> bitmap, node *leaf);
		void rangeQuery(int min,int max,node* leaf,EWAHBoolArray<int>& result);
		node *search(int key, node *leaf);
		void dfs(node *leaf);      
		void inOrder(node *leaf);      
		node *root;
};

btree::btree()
{
	root=NULL;
}
btree::~btree()
{
	destroy_tree();
}
void btree::destroy_tree(node *leaf)
{
	if(leaf!=NULL)
	{
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		delete leaf;
	}
}
void btree::insert(int key,int cardinality,EWAHBoolArray<int> bitmap, node *leaf)
{
	if(key< leaf->key_value)
	{
		if(leaf->left!=NULL)
			insert(key,cardinality,bitmap, leaf->left);
		else
		{
			leaf->left=new node;
			leaf->left->key_value=key;
			leaf->left->cardinality=cardinality;
			leaf->left->bitmap=bitmap;
			leaf->left->left=NULL;    //Sets the left child of the child node to null
			leaf->left->right=NULL;   //Sets the right child of the child node to null
		}  
	}
	else if(key>=leaf->key_value)
	{
		if(leaf->right!=NULL)
			insert(key,cardinality,bitmap, leaf->right);
		else
		{
			leaf->right=new node;
			leaf->right->key_value=key;
			leaf->right->cardinality=cardinality;
			leaf->right->bitmap=bitmap;
			leaf->right->left=NULL;  //Sets the left child of the child node to null
			leaf->right->right=NULL; //Sets the right child of the child node to null
		}
	}
}
void btree::rangeQuery(int min, int max, node* leaf,EWAHBoolArray<int>& result) {
	if(leaf != NULL) {
		if(min <= leaf->key_value && max >=leaf->key_value) {
			//add it and look in both subtree
			EWAHBoolArray<int > resultOld =result;
			resultOld.logicalor(leaf->bitmap,result);
			rangeQuery(min,max,leaf->left,result);
			rangeQuery(min,max,leaf->right,result);
		} else if (max < leaf->key_value) {
			//donot add, look in left subtree
			rangeQuery(min,max,leaf->left,result);
		} else if (min > leaf->key_value){
			//donot add, look in right subtree
			rangeQuery(min,max,leaf->right,result);
		}
	}
}
node *btree::search(int key, node *leaf)
{
	if(leaf!=NULL)
	{
		if(key==leaf->key_value)
			return leaf;
		if(key<leaf->key_value)
			return search(key, leaf->left);
		else
			return search(key, leaf->right);
	}
	else return NULL;
}
void btree::dfs(node *leaf){
	if(leaf != NULL) {
		cout << leaf->key_value << " "; 
		dfs(leaf->left);
		dfs(leaf->right);
	}
}
void btree::inOrder(node *leaf){
	if(leaf != NULL) {
		inOrder(leaf->left);
		cout << leaf->key_value << " "; 
		inOrder(leaf->right);
	}
}
void btree::insert(int key,int cardinality,EWAHBoolArray<int> bitmap)
{
	if(root!=NULL)
		insert(key,cardinality,bitmap, root);
	else
	{
		root=new node;
		root->key_value=key;
		root->bitmap=bitmap;
		root->left=NULL;
		root->right=NULL;
	}
}
EWAHBoolArray<int> btree::rangeQuery(int min,int max) {
	EWAHBoolArray<int> result;
	rangeQuery(min,max,root,result);
	return result;
}

node *btree::search(int key)
{
	return search(key, root);
}
void btree::destroy_tree()
{
	destroy_tree(root);
}
void btree::dfs() {
	dfs(root);
	cout << endl;
}
void btree::inOrder() {
	inOrder(root);
	cout << endl;
}
