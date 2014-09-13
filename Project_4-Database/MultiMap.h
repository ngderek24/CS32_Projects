#ifndef MULTIMAP_H 
#define MULTIMAP_H

#include <string>

class MultiMap	
{	
private:
	struct Node
	{
		Node(std::string key, unsigned int value, Node* left, Node* right, Node* parent) 
			: m_key(key), m_value(value), m_left(left), m_right(right), m_parent(parent) {}
		std::string m_key;
		unsigned int m_value;
		Node* m_left;
		Node* m_right;
		Node* m_parent;
	};

	//	To	prevent	Multimaps	from	being	copied	or	assigned,	declare	these	members	
	//	private	and	do	not	implement	them.	
	MultiMap(const	MultiMap& other);	
	MultiMap& operator=(const MultiMap& rhs);	

	//	Private helper functions
	void clear(Node* cur);
	
	//	Private member variables
	Node* m_root;

public:		
	class Iterator	
	{
	public:	
		Iterator();	
		Iterator(bool state, Node* node);	
		bool valid() const;	
		std::string	getKey() const;	
		unsigned int getValue() const;	
		bool next();	
		bool prev();	

	private:
		bool m_validState;
		Node* m_node;
	};	
	
	MultiMap();	
	~MultiMap();	
	void clear();	
	void insert(std::string	key, unsigned int value);	
	Iterator findEqual(std::string key) const;	
	Iterator findEqualOrSuccessor(std::string key) const;	
	Iterator findEqualOrPredecessor(std::string key) const;		
	
private:
	//private helper function
	Iterator findEqual(std::string key, Node* cur) const;
	Iterator findEqualOrSuccessor(std::string key, Node* cur) const;
	Iterator findEqualOrPredecessor(std::string key, Node* cur) const;
};	

#endif //MULTIMAP_H
