// AVLSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    struct Node
    {
        ElementType key;
        Node* left = nullptr;
        Node* right = nullptr;
        int height = 1;
    };
    Node * head;
    int avl_size;
    void avl_copy(Node *& first, const Node * second);
    void avl_delete(Node* n);
    int tree_height(Node* n) const;
  void rotateLeftOnce(Node*& current);
  void rotateLeftTwice(Node*& current);
  void rotateRightOnce(Node*& current);
  void rotateRightTwice(Node*& current);
  void adding(Node*& current,const ElementType& element);
  bool isIn(const ElementType& element, Node* current) const;
  void preorderHelper( Node* n, VisitFunction visit )const;
  void inorderHelper( Node* n, VisitFunction visit)const;
  void postorderHelper(Node* n, VisitFunction visit)const;
  void destroy( Node* current );
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
  avl_size = 0;
  head = nullptr;
}

template <typename ElementType>
void AVLSet<ElementType>::destroy(Node* current)
{
  if(current == nullptr)
    return;
  destroy( current->left );
  destroy(current->right);
  delete current;
}

template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    destroy(head);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
  avl_copy( head, s.head);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
  std::swap(head, s.head);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
  if (this != &s)
  {
      destroy(head);
      head = nullptr;
      avl_copy(head, s.head);
      avl_size = s.size();
  }
  return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
  std::swap(head, s.head);
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

template <typename ElementType>
void AVLSet<ElementType>::rotateLeftOnce(Node*& current){
     Node* temp;

     temp = current->left;
     current->left = temp->right;
     temp->right = current;
     current = temp;
}

template <typename ElementType>
void AVLSet<ElementType>::rotateLeftTwice(Node*& current){
     rotateRightOnce(current->left);
     rotateLeftOnce(current);
}

template <typename ElementType>
void AVLSet<ElementType>::rotateRightOnce(Node*& current){
     Node* temp;

     temp = current->right;
     current->right = temp->left;
     temp->left = current;
     current = temp;
}

template <typename ElementType>
void AVLSet<ElementType>::rotateRightTwice(Node*& current){
     rotateLeftOnce(current->right);
     rotateRightOnce(current);
}

template <typename ElementType>
void AVLSet<ElementType>::adding(Node*& current,const ElementType& element)
{
    if(current == nullptr)
        current = new Node{element};
    else if(element > current->key)
    {
        adding(current->right,element);
        if(tree_height(current->right)-tree_height(current->left) == 2)
        {
            if(element > current->right->key)
                rotateRightOnce(current);
            else
                rotateRightTwice(current);
        }
    }
    else if(element < current->key)
    {
        adding(current->left,element);
        if(tree_height(current->left)-tree_height(current->right) == 2)
        {
            if(element < current->left->key)
                rotateLeftOnce(current);
            else
                rotateLeftTwice(current);
        }
    }
    
}

template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
    adding(head,element);
    avl_size++;
}

template<typename ElementType>
bool AVLSet<ElementType>::isIn(const ElementType& element, Node* current) const
{
    if(current !=nullptr && element == current->key)
    {
        return true;
    }
    else if(current == nullptr)
    {
        return false;
    }
    else if(current != nullptr && element > current->key)
    {
        isIn(element,current->right);
    }
    else if(current != nullptr && element < current->key)
    {
        isIn(element,current->left);
    }
    return false;
   
}

template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return isIn(element,head);
}
     

template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return avl_size;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const
{
  return tree_height(this->head);
}
template <typename ElementType>
int AVLSet<ElementType>::tree_height(Node* n) const
{
    int l_height = 0;
    int r_height = 0;
    if( avl_size == 0 )
      return -1;
    if (n == nullptr)
        return 0;
    l_height = tree_height(n -> left);
    l_height = tree_height(n -> right);
    if (l_height > r_height)
        return l_height + 1;
    else
        return r_height + 1;
}


template <typename ElementType>
void AVLSet<ElementType>::avl_copy(Node *& first, const Node* second)
{
  if(second == nullptr)
    first = nullptr;
  else
    {
      first = new Node{second->key};
      if(first->right == nullptr && first->left == nullptr)
        return;
      avl_copy( first->left, second->left );
      avl_copy( first->right, second->right );
    }
}


template <typename ElementType>
void AVLSet<ElementType>::preorderHelper( Node* n, VisitFunction visit )const
{
  if (n == nullptr)
        return;
    /* first print data of head */
    //std::cout << head->key << " ";
  visit( n->key );
    /* then recur on left sutree */
  preorderHelper(n->left, visit);

    /* now recur on right subtree */
  preorderHelper(n->right, visit);
}
template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
  
  preorderHelper(head, visit);
  
}

template <typename ElementType>
void AVLSet<ElementType>::inorderHelper( Node* n, VisitFunction visit)const
{
  
  if (n == nullptr)
        return;
  /* first recur on left child */
  inorderHelper(n->left, visit);

    /* then print the key of head */
    //std::cout << head->key << " ";
    visit(n->key);
    /* now recur on right child */
    inorderHelper(n->right, visit);
}
template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
  inorderHelper(head, visit);
}
template <typename ElementType>
void AVLSet<ElementType>::postorderHelper(Node* n, VisitFunction visit) const
{
  
  if (n == nullptr)
        return;
  // first recur on left subtree
  postorderHelper(n->left, visit);

    // then recur on right subtree
  postorderHelper(n->right, visit);

    // now deal with the head
    //std::cout << head->key << " ";
    visit(n->key);
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
  postorderHelper( head, visit); 
}



#endif // AVLSET_HPP
