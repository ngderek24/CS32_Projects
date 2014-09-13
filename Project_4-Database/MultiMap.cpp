#include "MultiMap.h"
using namespace std;

MultiMap::MultiMap()
{
	m_root = nullptr;
}

MultiMap::~MultiMap()
{
	clear();			//calls clear() function to clean up tree
}

void MultiMap::clear()
{
	clear(m_root);
	m_root = nullptr;
}

void MultiMap::clear(Node* cur)
{
	if (cur == nullptr)
		return;

	clear(cur->m_left);		//using post-order traversal to recursively delete each node in multimap
	clear(cur->m_right);

	delete cur;
}

void MultiMap::insert(string key, unsigned int value)
{
	if (m_root == nullptr)	//if map is empty, link root to first node in tree
	{
		m_root = new Node(key, value, nullptr, nullptr, nullptr);
		return;
	}

	Node* temp = m_root;
	for(;;)					//infinite loop to traverse through tree to find right spot to insert new node
	{
		if (key < temp->m_key)		//look on left subtree if key is smaller than key of node
		{
			if (temp->m_left != nullptr)	//move down tree until we find correct spot to insert
				temp = temp->m_left;
			else							//found spot to insert new node into tree
			{
				temp->m_left = new Node(key, value, nullptr, nullptr, temp);
				return;
			}
		}
		else				//look on right subtree if key is bigger than key of node
		{
			if (temp->m_right != nullptr)		//move down tree until we find correct spot to insert
				temp = temp->m_right;
			else						//found spot to insert new node into tree
			{
				temp->m_right = new Node(key, value, nullptr, nullptr, temp);
				return;
			}
		}
	}
}

MultiMap::Iterator MultiMap::findEqual(string key) const
{
	return findEqual(key, m_root);
}

MultiMap::Iterator MultiMap::findEqual(string key, Node* cur) const
{
	if (cur == nullptr)				//if tree is empty, return invalid iterator
	{
		Iterator it;
		return it;
	}

	if (key < cur->m_key)						//in-order traversal through tree to find matching key
		return findEqual(key, cur->m_left);		//recursively traverse tree 
	if (key == cur->m_key)						//found matching key so return iterator to node
	{
		Iterator it(true, cur);
		return it;
	}
	else
		return findEqual(key, cur->m_right);
}

MultiMap::Iterator MultiMap::findEqualOrSuccessor(string key) const
{
	return findEqualOrSuccessor(key, m_root);
}

MultiMap::Iterator MultiMap::findEqualOrSuccessor(string key, Node* cur) const
{
	if (cur == nullptr)			//return invalid iterator if tree is empty
	{
		Iterator it;
		return it;
	}
	if (cur->m_left == nullptr && cur->m_right == nullptr )			//leaf node
	{
		if (key <= cur->m_key)								//equal or successor of key so return iterator to cur
		{
			Iterator it(true, cur);
			return it;
		}
		if (cur->m_parent != nullptr)				//if there's a parent, then next is successor
		{
			Iterator it(true, cur);
			it.next();
			return it;
		}
		else								//no parent, so invalid iterator
		{
			Iterator it;
			return it;
		}
	}
	
	if (key < cur->m_key)				//key is in left subtree
	{
		if (cur->m_left != nullptr)								//if there's a left child, recursively check left subtree
			return findEqualOrSuccessor(key, cur->m_left);	
		else													//no left child, so cur is successor
		{
			Iterator it(true, cur);
			return it;
		}
	}
	else									//key is in right subtree
	{
		if (cur->m_right != nullptr)						//if there's right child, check right subtree
			return findEqualOrSuccessor(key, cur->m_right);
		else												//no right child, so next node is successor
		{
			Iterator it(true, cur);
			it.next();
			return it;
		}
	}
}

MultiMap::Iterator MultiMap::findEqualOrPredecessor(string key) const
{
	return findEqualOrPredecessor(key, m_root);
}

MultiMap::Iterator MultiMap::findEqualOrPredecessor(string key, Node* cur) const
{
	if (cur == nullptr)						//return invalid iterator if tree is empty
	{
		Iterator it;
		return it;
	}
	if (cur->m_left == nullptr && cur->m_right == nullptr )		//leaf node
	{
		if (key >= cur->m_key)					//equal or predecessor of key so return iterator to cur
		{
			Iterator it(true, cur);
			return it;
		}
		if (cur->m_parent != nullptr)			//if there's a parent, then prev is predecessor
		{
			Iterator it(true, cur);
			it.prev();
			return it;
		}
		else								//no parent, so invalid iterator
		{
			Iterator it;
			return it;
		}
	}
	
	if (key < cur->m_key)					//key is in left subtree
	{
		if (cur->m_left != nullptr)								//if there's a left child, recursively check left subtree
			return findEqualOrPredecessor(key, cur->m_left);
		else													//no left child, so prev of cur is predecessor
		{
			Iterator it(true, cur);
			it.prev();
			return it;
		}
	}
	else										//key is in right subtree
	{
		if (cur->m_right != nullptr)								//if there's right child, check right subtree
			return findEqualOrPredecessor(key, cur->m_right);
		else														//no right child, so cur is predecessor
		{
			Iterator it(true, cur);
			return it;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Iterator's implementation//

MultiMap::Iterator::Iterator()	
{
	m_validState = false;
	m_node = nullptr;
}

MultiMap::Iterator::Iterator(bool state, Node* node)	//overloaded constructor
{
	m_validState = state;
	m_node = node;
}

bool MultiMap::Iterator::valid() const
{
	return m_validState;
}

string MultiMap::Iterator::getKey() const
{
	if (m_validState)			//if iterator is valid, return key
		return m_node->m_key;
	else						//return empty string if invalid iterator
		return "";
}

unsigned int MultiMap::Iterator::getValue() const
{
	if (m_validState)			//if iterator is valid, return value
		return m_node->m_value;
	else						//return 0 if invalid iterator
		return 0;
}

bool MultiMap::Iterator::next()
{
	if (!m_validState)	
		return false;
	
	if (m_node->m_right != nullptr)		//if there's a right child, traverse right subtree to find correct location
	{									//to increment iterator to next node
		Node* temp = m_node->m_right;
		while (temp->m_left != nullptr)
			temp = temp->m_left;
		m_node = temp;
		return true;
	}
	else								//no right child so look at ancestry
	{
		if (m_node->m_parent == nullptr)	//only one node in tree so can't increment iterator
		{
			m_node = nullptr;
			m_validState = false;
			return false;
		}
		if (m_node->m_parent->m_left == m_node)		//immediate parent is next so increment iterator to parent
		{
			m_node = m_node->m_parent;
			return true;
		}
		else
		{
			Node* ancestor = m_node->m_parent;		//traverse up tree to find ancestry
			while (ancestor->m_parent != nullptr && ancestor->m_parent->m_left != ancestor)
				ancestor = ancestor->m_parent;
			if (ancestor->m_parent == nullptr)		//can't find next node in tree, so invalidate iterator
			{
				m_node = nullptr;
				m_validState = false;
				return false;
			}
			m_node = ancestor->m_parent;		//found next node in ancestry so set iterator
			return true;
		}
	}
}

bool MultiMap::Iterator::prev()
{
	if (!m_validState)
		return false;

	if (m_node->m_left != nullptr)			//if there's a left child, traverse left subtree to find correct location
	{										//to increment iterator to next node
		Node* temp = m_node->m_left;
		while (temp->m_right != nullptr)
			temp = temp->m_right;
		m_node = temp;
		return true;
	}
	else									//no left child so look at ancestry
	{
		if (m_node->m_parent == nullptr)	//first node in tree so can't decrement iterator
		{
			m_node = nullptr;
			m_validState = false;
			return false;
		}
		if (m_node->m_parent->m_right == m_node)		//immediate parent is prev so decrement iterator to parent
		{
			m_node = m_node->m_parent;
			return true;
		}
		else
		{
			Node* ancestor = m_node->m_parent;		//traverse up tree to find ancestry
			while (ancestor->m_parent != nullptr && ancestor->m_parent->m_right != ancestor)
				ancestor = ancestor->m_parent;		
			if (ancestor->m_parent == nullptr)		//can't find prev node in tree, so invalidate iterator
			{
				m_node = nullptr;
				m_validState = false;
				return false;
			}
			m_node = ancestor->m_parent;		//found prev node in ancestry so set iterator
			return true;
		}
	}
}
