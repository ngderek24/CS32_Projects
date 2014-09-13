#include "Map.h"
using namespace std;

Map::Map()	//constructor of Map that initializes private members of Map
{
	head = nullptr;
	tail = nullptr;
	m_size = 0;
}

Map::~Map()	//destructor of Map that deallocates dynamically allocated nodes
{
	MapNode* k = head;
	while (k != nullptr)	//exit loop if there are no nodes left to be deleted
	{
		MapNode* nextOne;
		nextOne = k->m_nextNode;
		delete k;
		k = nextOne;
	}
}

Map::Map(const Map& src)	//copy constructor of Map that creates an exact duplicate of the map being copied
{
	m_size = 0;
	head = nullptr;
	tail = nullptr;

	KeyType key;
	ValueType value;
	for (int k = 0; k < src.size(); k++)	//loop to copy each node of the map being copied
	{
		MapNode* p = new MapNode;
		src.get(k, key, value);		//get the key/value pair at each node to copy to the new map
		if (head == nullptr)		//check if the new map is empty to correctly assign data members
		{
			p->m_key = key;
			p->m_value = value;
			p->m_nextNode = head;
			p->m_prevNode = nullptr;
			head = p;
			tail = p;
			m_size++;
		}
		else		//the new map is not empty, so assign the data members accordingly
		{
			p->m_key = key;
			p->m_value = value;
			p->m_nextNode = nullptr;
			p->m_prevNode = tail;
			tail->m_nextNode = p;
			tail = p;
			m_size++;
		}
	}
}

Map& Map::operator=(const Map& src)	//assignment operator for Map
{
	if (&src != this)	//check for aliasing (if they're in the same place in memory)
	{
		Map temp(src);	//copy-and-swap method, create a temporary Map that is a copy of the  
		swap(temp);		//map we want to copy and then swap with new map
	}
	return *this;	//return newly assigned map
}

bool Map::empty() const		//check if map is empty 
{
	if (head == nullptr)
		return true;
	else 
		return false;
}

int Map::size() const		//returns the number of key/value pair in map
{
	return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)	//insert key/value pair into map if key 
{																//doesn't already exist in map
	for (MapNode* k = head; k != nullptr; k = k->m_nextNode)
		if (k->m_key == key)		//check if the key already exists in map
			return false;

	MapNode* last = new MapNode;	//allocate new node to be linked into map
	last->m_key = key;
	last->m_value = value;
	last->m_nextNode = head;		//insertion at the top of the map
	last->m_prevNode = nullptr;
	if (head != nullptr)			//check if map is empty, don't want to deference nullptr
		head->m_prevNode = last;
	head = last;					//link the new node into the top of the map
	if (tail == nullptr)	//tail has not been assigned, need to assign to first node added to map
		tail = last;
	m_size++;
	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (MapNode* k = head; k != nullptr; k = k->m_nextNode)	//loop to go through each node in map
		if (k->m_key == key)		//check if key exists in map
		{
			k->m_value = value;
			return true;
		}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	for (MapNode* k = head; k != nullptr; k = k->m_nextNode)
		if (k->m_key == key)		//check if key exists in map, if so then update
		{
			k->m_value = value;
			return true;
		}

	MapNode* last = new MapNode;	//key doesn't exist in map, so allocate new node to insert key/value pair
	last->m_key = key;
	last->m_value = value;
	last->m_nextNode = head;
	last->m_prevNode = nullptr;
	if (head != nullptr)			//check if map is empty, don't want to deference nullptr
		head->m_prevNode = last;
	head = last;					//link the new node into the top of the map
	if (tail == nullptr)		//tail has not been assigned, need to assign to first node added to map
		tail = last;
	m_size++;
	return true;
}

bool Map::erase(const KeyType& key)
{
	if (head == nullptr)	//check if map is empty
		return false;
	if(head->m_key == key)	//check to see if we're erasing first node in map
	{
		MapNode* toDelete = head;
		head = toDelete->m_nextNode;
		if (head != nullptr)				//check if map is empty after deletion, don't want to deference nullptr
			head->m_prevNode = nullptr;
		if (toDelete->m_nextNode == nullptr)	//if we're deleting last node, tail resets to nullptr
			tail = nullptr;
		delete toDelete;
		m_size--;
		return true;
	}

	MapNode* above = head;
	while(above->m_nextNode != nullptr)		//check if above is pointing to the last node
	{
		if (above->m_nextNode->m_key == key)	//find node right before the node we want to erase
			break;
		above = above->m_nextNode;
	}
	if(above->m_nextNode == nullptr)	//end of map, and we didn't find key
		return false;
	MapNode* toDelete = above->m_nextNode;
	above->m_nextNode = toDelete->m_nextNode;	//link node above the node being deleted to the next node
	if (toDelete->m_nextNode != nullptr)			//check if we're at the end of map, don't want to deference nullptr
		toDelete->m_nextNode->m_prevNode = above;
	if (toDelete->m_nextNode == nullptr)	//if we're at the end of map, set tail to previous node
		tail = above;
	delete toDelete;
	m_size--;
	return true;
}

bool Map::contains(const KeyType& key) const
{
	for (MapNode* k = head; k != nullptr; k = k->m_nextNode)
		if(k->m_key == key)		//check if key matches key in map
			return true;
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	for (MapNode* k = head; k != nullptr; k = k->m_nextNode)
		if(k->m_key == key)		//if key matches key in map, then set value to value of the key in map
		{
			value = k->m_value;
			return true;
		}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i >= 0 && i < m_size)	//check if i is within appropriate boundaries
	{
		MapNode* k = head;
		for (int n = 0; n < i; n++)	//get the ith node in the map
			k = k->m_nextNode;
		key = k->m_key;				//set key and value to key/value of ith node found
		value = k->m_value;
		return true;
	}
	return false;
}

void Map::swap(Map& other)
{
	int temp;		//swap sizes of the maps
	temp = m_size;
	m_size = other.size();
	other.m_size = temp;
	
	MapNode* tempHead;		//swap the heads of the maps
	tempHead = head;
	head = other.head;
	other.head = tempHead;

	MapNode* tempTail;		//swap the tails of the map
	tempTail = tail;
	tail = other.tail;
	other.tail = tempTail;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
	Map temp;
	int count = 0;
	for (int k = 0; k < m1.size(); k++)	//insert the nodes of m1 into a temporary map
	{
		KeyType key;
		ValueType value;
		m1.get(k, key, value);
		temp.insert(key, value);	
	}

	for (int n = 0; n < m2.size(); n++)	//go through m2
	{
		KeyType key2;
		ValueType value2;
		m2.get(n, key2, value2);
		if (temp.contains(key2))	//check if temporary map already contain key from m2
		{
			ValueType number;
			if (temp.get(key2, number) && number == value2) //if values of the same key are different, continue loop
				continue;
			else
			{
				temp.erase(key2);	//erase key from temp map if values of same key are different
				count++;		//increment count to return false later
				continue;
			}
		}
		temp.insert(key2, value2);	//if key is not in temp map, then insert key/value pair
	}
	result = temp;		//copy temp map into result 
	if (count > 0)
		return false;
	else
		return true;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	Map temp;
	for (int k = 0; k < m1.size(); k++)		//insert key/value pairs from m1 into temp map
	{
		KeyType key;
		ValueType value;
		m1.get(k, key, value);
		temp.insert(key, value);	
	}

	for (int n = 0; n < m2.size(); n++)
	{
		KeyType key2;
		ValueType value2;
		m2.get(n, key2, value2);
		if (temp.contains(key2))
			temp.erase(key2);		//if key from m2 already exists in temp map, then erase key/value pair
	}
	result = temp;	//copy temp map into result
	return;
}
