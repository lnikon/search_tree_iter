#pragma once

#include <stack>
#include <iostream>
#include <cstdint>

using std::size_t;
using std::cout;
using std::endl;

template <class T>
class SearchTree {
    struct Node {
        T		info_;
        Node*	llink_;
        Node*	rlink_;
        Node(T info = T(), Node* llink = 0, Node* rlink = 0)
            : info_(info), llink_(llink), rlink_(rlink)
        {}
    };

    //Static methods
    static Node*  copy(const Node* tree);
    static void   clear(Node*& tree);
    static void   print(const Node* tree, int indent);
    static void   visit(const Node* node);

    static bool   find(const Node* tree, const T& elem);
    static bool   insert(Node*& tree, const T& elem);
    static bool	  remove(Node*& tree, const T& elem);

    static Node* min(Node* tree);

    //Service methods
    void	PrintInorder(Node* ptr) const;
    void	PrintPreorder(Node* ptr) const;
    void  	PrintPostorder(Node* ptr) const;

    static void swap(SearchTree<T>& tree1, SearchTree<T>& tree2);

    //Member variables
    private:
    Node* 	tree_{0};
    size_t	size_;

    //Public methods
    public:
    SearchTree();
    SearchTree(const SearchTree& other);

    ~SearchTree();

    SearchTree& operator=(const SearchTree& other);

    //Some move related staff
    SearchTree(SearchTree&& other);
    SearchTree& operator=(const SearchTree&& other);

    //Accessors
    bool   	empty()const;
    size_t	size()const;
    bool  	find(const T& elem)const; 
    void   	print()const;
    void	PrintInorder()const;
    void	PrintPreorder()const;
    void	PrintPostorder()const;

    //Iterators
    struct iterator {
        const T& operator*()
        {
            return current_->info_;
        }

        private:
            Node* current_{nullptr};
            std::stack<Node *> prevNodes_{};
    };

    iterator begin();
    iterator end();

    //Modifiers
    void	emplace(T&& elem);
    void	insert(const T& elem);
    void	remove(const T& elem);
    void	clear();
};

////////////////////////////////////////////////////////////////////////
//	   		      _____ IMPLEMENTATION _____                          //
////////////////////////////////////////////////////////////////////////

//_____ STATIC METHODS _____
template<class T>
    typename SearchTree<T>::Node*
SearchTree<T>::copy(const Node* tree)
{
    // if tree is empty
    if (tree == 0){
        // we are done
        return 0;
    }

    // construct copy of the root with
    return new Node(
            tree->info_, 
            copy(tree->llink_), // left subtree copied
            copy(tree->rlink_)  // and right one
            );
}

template<class T>
    void  
SearchTree<T>::clear(Node*& tree)
{
    // if tree is empty
    if (tree == 0) {
        // we are done
        return;
    }

    // clear left
    clear(tree->llink_);

    // clear left
    clear(tree->rlink_);

    // delete root
    delete tree;

    // mark tree empty
    tree = 0;
}

template<class T>
    void  
SearchTree<T>::print(const Node* tree, int indent)
{
    // if tree is empty
    if (tree == 0){
        // we are done
        return;
    }

    // some magic margin
    const int margin = 5;

	// print left with some indent plus margin
	print(tree->rlink_, indent + margin);

	// emulate indentation	
	for (int i = 0; i < indent; i++) cout << ' ';

	// print root
	cout<<tree->info_<<endl;
	
	// print left subtree
	print(tree->llink_, indent + margin);
}

template<class T>
bool  
SearchTree<T>::find(const Node* tree, const T& elem)
{
	// if tree is empty
	if (tree == 0){
		// tree doesn't contain elem for sure 
		return false;
	}
    
	// if elem is equal to root element 
	if(tree->info_==elem) {
		// say we are lucky
		return true;
	}

	// if elem compares less than root element
	if(elem<tree->info_) {
		// it must be in left subtree
		return find(tree->llink_, elem);
	}
	
	// last hope is right subtree
	return find(tree->rlink_, elem);
}

template<class T>
bool   
SearchTree<T>::insert(Node*& tree, const T& elem)
{
    if(tree == 0)
    {
        tree = new Node(elem);
        return true;
    }
    else if(elem < tree->info_)
    {
        return SearchTree::insert(tree->llink_, elem);
    }
    else if(elem > tree->info_)
    {
        SearchTree::insert(tree->rlink_, elem);
    }

    return false;
}

template<class T>
    bool   
SearchTree<T>::remove(Node*& tree, const T& elem)
{
    if(tree == 0) 
    {
        return false;
    }

    if(elem < tree->info_)
    {
        SearchTree<T>::remove(tree->llink_, elem);
    }
    else if(elem > tree->info_)
    {
        SearchTree<T>::remove(tree->rlink_, elem);
    }
    else 
    {
        if(tree->llink_ == 0)
        {
            Node* tmp = tree->rlink_;
            delete tree;
            tree = tmp;
        } 
        else if(tree->rlink_ == 0)
        {
            Node* tmp = tree->llink_;
            delete tree;
            tree = tmp;
        }
        else
        {
            Node* tmp = SearchTree<T>::min(tree->rlink_);
            tree->info_ = tmp->info_;
            delete tmp;

            return true;
        }

        return false;
    }
}

template<class T>
typename SearchTree<T>::Node* 
SearchTree<T>::min(Node* tree)
{
    Node* tmp = tree;
    while(tmp != 0 && tmp->llink_ != 0)
    {
        tmp = tmp->llink_;
    }

    return tmp;
}

//_______SERVICE METHODS_______

template<class T>
void
SearchTree<T>::PrintInorder(Node *ptr) const
{
    if(ptr == 0) 
    {
        return;
    }

    SearchTree<T>::PrintInorder(ptr->llink_);
    visit(ptr);
    SearchTree<T>::PrintInorder(ptr->rlink_);
}

template<class T>
void
SearchTree<T>::PrintPreorder(Node* ptr)const
{
    if(ptr == 0) 
    {
        return;
    }

    visit(ptr);
    SearchTree<T>::PrintPreorder(ptr->llink_);
    SearchTree<T>::PrintPreorder(ptr->rlink_);
}

template<class T>
void
SearchTree<T>::PrintPostorder(Node *ptr) const
{
    if(ptr == 0) 
    {
        return;
    }

    SearchTree<T>::PrintPostorder(ptr->llink_);
    SearchTree<T>::PrintPostorder(ptr->rlink_);
    visit(ptr);
}

//______ PUBLIC METHODS ______
    template<class T>
SearchTree<T>::SearchTree()
{
}

    template<class T>
SearchTree<T>::SearchTree(const SearchTree& other)
{
    Node* tmp = SearchTree<T>::copy(other.tree_);

    tree_ = tmp;
    size_ = other.size_;
}

    template<class T>
SearchTree<T>::~SearchTree()	
{
    SearchTree::clear(this->tree_);
}

template<class T>
    SearchTree<T>&
SearchTree<T>::operator=(const SearchTree& other)
{	
    if(this == &other)
    {
        return *this;
    }

    SearchTree<T> tmp(other);
    SearchTree<T>::swap(tmp, *this);

    return *this;
}

template<class T>
bool	
SearchTree<T>::empty() const
{
    return (this->tree_ == 0);
}

template<class T>
std::size_t		
SearchTree<T>::size() const
{ 
    return size_;
}

template<class T>	
bool 
SearchTree<T>::find(const T& elem)const	
{
    return SearchTree::find(this->tree_, elem);
}

template<class T>
void
SearchTree<T>::PrintInorder()const
{
    SearchTree<T>::PrintInorder(this->tree_);
}

template<class T>
void
SearchTree<T>::PrintPreorder() const
{
    SearchTree<T>::PrintPreorder(this->tree_);
}

template<class T>
void
SearchTree<T>::PrintPostorder()const
{
    SearchTree<T>::PrintPostorder(this->tree_);
}

template<class T>
void 
SearchTree<T>::swap(SearchTree<T>& tree1, SearchTree<T>& tree2)
{
    using std::swap;
    swap(tree1.tree_, tree2.tree_);
    swap(tree1.size_, tree2.size_);
}

// template<class T>
// void
// SearchTree<T>::Sort(const T* Array,int size)
// {
// }

template<class T>	
    void 
SearchTree<T>::insert(const T& elem)		
{
    const bool insertOk = SearchTree::insert(this->tree_, elem);
    if(insertOk)
    {
        size_++;
    }
}

template<class T>
    void 
SearchTree<T>::remove(const T& elem)		
{
    SearchTree<T>::remove(tree_, elem);
}

template<class T>
void 
SearchTree<T>::print() const				
{
    SearchTree<T>::print(this->tree_, 5);
}

template<class T>
void 
SearchTree<T>::visit(const Node* const node)
{
    std::cout << node->info_ << std::endl;
}

template<class T>
    void 
SearchTree<T>::clear()					
{
    SearchTree<T>::clear(this->tree_);
}
