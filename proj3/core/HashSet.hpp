// HashSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;
    struct Nodes
        {
            ElementType key;
            Nodes* next = nullptr;
        };

        Nodes** hash;
        unsigned int hash_capacity;
        unsigned int hash_size;
        void inputNode(const ElementType& element);
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
  hash_capacity = DEFAULT_CAPACITY;
  hash = new Nodes*[hash_capacity];
  hash_size = 0;
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
  for (unsigned int i = 0; i < hash_capacity; ++i)
      delete hash[i];
  delete hash;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
  hash = s.hash;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
  std::swap( hash, s.hash );
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
  if (this != &s)
  {
      hashFunction = s.hashFunction;
      hash = s.hash;
  }

  return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
  std::swap(hash, s.hash);
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
  inputNode(element);
    if ((hash_size / hash_capacity) > 0.8)
    {
    	Nodes** old_hash = hash;
    	unsigned int prev_amount = hash_capacity;
        hash_capacity = (hash_capacity * 2);
        hash = new Nodes*[hash_capacity];
        for (unsigned int i = 0; i < prev_amount; i++)
        {
        	Nodes* second_temp = old_hash[i];
        	while(second_temp != nullptr)
        	{
        		inputNode(element);
        		second_temp = second_temp -> next;
        	}
        }
        for (unsigned int x = 0; x < prev_amount; x++)
        {
        	if (old_hash[x] != nullptr)
        		delete old_hash[x];
        }
        delete[] old_hash;
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
  unsigned int index = static_cast<unsigned int>(hashFunction(element)) % hash_capacity;
  Nodes* test = hash[index];
  while(test != nullptr)
  {
      if(test->key == element)
        return true;
      test = test -> next;
  }
  return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return hash_size;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
  unsigned int count = 0;
  if (hash[index] == nullptr)
    return 0;
  else
    for ( Nodes* temp = hash[index]; temp != nullptr; temp++ )
      count++;
  return count;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
  if (elementsAtIndex(index) != 0)
    {
      for ( Nodes* temp = hash[index]; temp != nullptr; temp++ )
      {
        if (element == temp->key)
          {
            return 1;
          }
      }
    }
  return 0;
}

template <typename ElementType>
void HashSet<ElementType>::inputNode(const ElementType& element)
{
    unsigned int index = static_cast<unsigned int>(hashFunction(element)) % hash_capacity;
	Nodes* alpha = hash[index];
	if (alpha == nullptr)
		hash[index] = new Nodes{element};
	else
	{
		while(alpha != nullptr)
			alpha = alpha -> next;
		alpha = new Nodes{element};
	}
}



#endif // HASHSET_HPP
