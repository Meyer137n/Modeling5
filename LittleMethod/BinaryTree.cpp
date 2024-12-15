#include "BinaryTree.h"

using namespace std;

BinaryTree::BinaryTree()
{
	this->size = 0;
	root = nullptr;
}

BinaryTree::~BinaryTree()
{
	destroyBranch(root);
}

void BinaryTree::destroyBranch(Branch* src)
{
	if (src->leftBr) destroyBranch(src->leftBr);
	if (src->rightBr) destroyBranch(src->rightBr);
	delete src;
}

void BinaryTree::print(Branch* src, string const& rpref, string const& cpref, string const& lpref)
{
	if (!src) return;
	if (src->rightBr)
		print(src->rightBr, rpref + "  ", rpref + ch_ddia_hor, rpref + ch_ver_spa);
	string color = src->includeEdge ? "\033[32m" : "\033[31m";
	if (src->parent) cout << cpref << color << "(" << src->edge.first + 1<< "-" << src->edge.second + 1 << ") " << src->border << "\033[0m" << endl;
	else cout << cpref << "\033[33m" << "S(0) "<< src->border << "\n\033[0m";

	if (src->leftBr)
		print(src->leftBr, lpref + ch_ver_spa, lpref + ch_udia_hor, lpref + "  ");
}