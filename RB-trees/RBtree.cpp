#include "stdafx.h"
#include "RBtree.h"

void
RBtree::clear(Node* tree)
{
	if (tree->llink_ == 0) return;

	clear(tree->llink_);
	clear(tree->rlink_);
	delete tree;
}


void
RBtree::print(const Node* tree, int indent)const
{
	if (tree->llink_ == 0) return;

	print(tree->rlink_, indent + 5);

	for (int i = 0; i<indent; i++) cout << ' ';
	cout << tree->key_;
	if (tree->color_ == BLACK)
		cout << 'B' << endl;
	else
		cout << 'R' << endl;
	print(tree->llink_, indent + 5);
}

////////////////////////////////////////////////////////////////////////
//          *****BALANCING METHODS: Implementation*****               //
////////////////////////////////////////////////////////////////////////
void
RBtree::leftRotate(Node*& p)
{
	Node* p1 = p->rlink_;
	p->rlink_ = p1->llink_;
		p1->llink_->parent_ = p;
	if (p->parent_ == &NIL_)
		tree_ = p1;
	else
		(p == p->parent_->llink_) ? p->parent_->llink_ = p1 : p->parent_->rlink_ = p1;
	p1->llink_ = p;
	p1->parent_ = p->parent_;
	p->parent_ = p1;
	p = p1;
}

void
RBtree::rightRotate(Node*& p)
{
	Node* p1 = p->llink_;
	p->llink_ = p1->rlink_;
	p1->rlink_->parent_ = p;
	if (p->parent_ == &NIL_)
		tree_ = p1;
	else
		(p == p->parent_->llink_) ? p->parent_->llink_ = p1 : p->parent_->rlink_ = p1;
	p1->rlink_ = p;
	p1->parent_ = p->parent_;
	p->parent_ = p1;
	p = p1;
}

void
RBtree::balancingAfterInsertion(Node* p)
{
	
	Node * parent = &NIL_;
	Node * gparent = &NIL_;

	while ((p != tree_) && (p->color_ != BLACK) && (p->parent_->color_ == RED))
	{
		parent = p->parent_;
		gparent = p->parent_->parent_;

		// Parent of inserted node left child of gparent
		if (parent == gparent->llink_)
		{
			Node * uncle = gparent->rlink_;
			//Case 1
			if (uncle != &NIL_ && uncle->color_ == RED)
			{
				gparent->color_ = RED;
				parent->color_ = BLACK;
				uncle->color_ = BLACK;
				p = gparent;
			}
			else
			{
				// Case 2 : Not on same branch new_node-parent-gparent
				if (p == parent->rlink_)
				{
					leftRotate(parent);
					p = parent;
					parent = p->parent_;
				}
				// Case 3 : On the same branch
				rightRotate(gparent);
				auto c = parent->color_;
				parent->color_ = parent->rlink_->color_;
				parent->rlink_->color_ = c;
				p = parent;
			}
		}
		// parent RIGHT of gparent
		else
		{
			Node * uncle = gparent->llink_;
			//Case 1
			if (uncle != &NIL_ && uncle->color_ == RED)
			{
				gparent->color_ = RED;
				parent->color_ = BLACK;
				uncle->color_ = BLACK;
				p = gparent;
			}
			else
			{
				// Case 2 : Not on same branch new_node-parent-gparent
				if (p == parent->llink_)
				{
					rightRotate(parent);
					p = parent;
					parent = p->parent_;
				}
				// Case 3 : On the same branch
				leftRotate(gparent);
				auto c = parent->color_;
				parent->color_ = parent->llink_->color_;
				parent->llink_->color_ = c;
				p = parent;
			}
		}
	}

	/*
	
	while ((p != tree_) && (p->color_ != BLACK) && (p->parent_->color_ == RED))
	{
		// Parent of inserted node LEFT child of grandparent
		if (p->parent_ == p->parent_->parent_->llink_)
		{
			Node* y = p->parent_->parent_->rlink_;
			if (y->color_ == RED)				// Case 1			
			{
				p->parent_->color_ = BLACK;
				y->color_ = BLACK;
				p->parent_->parent_->color_ = RED;
				p = p->parent_->parent_;
			}
			else
			{
				if (p == p->parent_->rlink_)
			{
				p = p->parent_;				//Case 2
				leftRotate(p->parent_);		//Case 2
			}
			p->parent_->color_ = BLACK;
			p->parent_->parent_->color_ = RED;
			rightRotate(p->parent_->parent_);				//Case 3
		}
		}
		else
		{
			Node* y = p->parent_->parent_->llink_;
			if (y->color_ == RED)				// Case 1			
			{
				p->parent_->color_ = BLACK;
				y->color_ = BLACK;
				p->parent_->parent_->color_ = RED;
				p = p->parent_->parent_;
			}	
			else
			{
				if (p == p->parent_->llink_)
				{
					p = p->parent_;
					rightRotate(p->parent_);		// Case 2
				}
				p->parent_->color_ = BLACK;
				p->parent_->parent_->color_ = RED;
				leftRotate(p->parent_->parent_);				//Case 3
			}
		}
	}
	*/
	tree_->color_ = BLACK;
}

void
RBtree::balancingAfterRemoval(Node* p)
{
	//...should be implemented...
}

//PUBLIC METHODS
Node*
RBtree::search(int elem)const
{
	Node * current = tree_;
	while (current != &NIL_)
	{
		if (current->key_ == elem)
			return current;
		if (elem >= current->key_)
			current = current->rlink_;
		else
			current = current->llink_;
	}
	return 0;
}

void
RBtree::insert(Node* node)
{
	Node* current = tree_, *prev = &NIL_;
	int key = node->key_;
	while (current != &NIL_)
	{
		if (current->key_ == key) return; //insertion is not done
		prev = current;
		(key<current->key_) ? current = current->llink_ : current = current->rlink_;
	}

	node->color_ = RED;
	node->parent_ = prev;
	node->llink_ = &NIL_;
	node->rlink_ = &NIL_;

	if (tree_ == &NIL_)
	{
		tree_ = node;
		node->color_ = BLACK;
		return;
	}

	(key<prev->key_) ? prev->llink_ = node : prev->rlink_ = node;

	balancingAfterInsertion(node);
}

void
RBtree::remove(Node* node)
{
	Color color = node->color_;
	Node* next;
	if (node->llink_ != &NIL_&& node->rlink_ != &NIL_) {
		for (next = node->rlink_; next->llink_ != &NIL_; next = next->llink_)
			;

		node->key_ = next->key_;//rewriting info
		color = next->color_;
		node = next;
	}

	if (node->llink_ == &NIL_) {
		next = node->rlink_;
		if (node->parent_ != &NIL_)
			if (node == node->parent_->llink_)
				node->parent_->llink_ = next;
			else
				node->parent_->rlink_ = next;
		else
		{
			tree_ = next; tree_->parent_ = &NIL_;
		}

		next->parent_ = node->parent_;
		delete node;
		if (color == BLACK)
			balancingAfterRemoval(next);
		return;
	}

	//node->rlink_==&NIL_	
	next = node->llink_;
	if (node->parent_ != &NIL_)
		if (node == node->parent_->llink_)
			node->parent_->llink_ = next;
		else
			node->parent_->rlink_ = next;
	else
	{
		tree_ = next; tree_->parent_ = &NIL_;
	}

	next->parent_ = node->parent_;
	delete node;
	if (color == BLACK)
		balancingAfterRemoval(next);
}
