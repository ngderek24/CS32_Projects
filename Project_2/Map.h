#ifndef Map_INCLUDED
#define Map_INCLUDED

#include <string>
typedef std::string KeyType;
typedef double ValueType;

class Map
{
  public:
    Map();         // Create an empty map (i.e., one with no key/value pairs)

	~Map();		//destructor

	Map(const Map& src);		//copy constructor

	Map& operator=(const Map& src);		//assignment operator

    bool empty() const;  // Return true if the map is empty, otherwise false.

    int size() const;    // Return the number of key/value pairs in the map.

    bool insert(const KeyType& key, const ValueType& value);
      // If key is not equal to any key currently in the map then add 
	  // the key/value pair to map and return true.
      // Otherwise, make no change to the map and return false

    bool update(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value it currently maps to, but instead map to
      // the value of the second parameter; return true in this case.
      // Otherwise, make no change to the map and return false.

    bool insertOrUpdate(const KeyType& key, const ValueType& value);
      // If key is equal to a key currently in the map, then make that key no
      // longer map to the value it currently maps to, but instead map to
      // the value of the second parameter; return true in this case.
      // If key is not equal to any key currently in the map, then add the
      // key/value pair to the map and return true.
      // Otherwise, make no change to the map and return false.

    bool erase(const KeyType& key);
      // If key is equal to a key currently in the map, remove the key/value
      // pair with that key from the map and return true.  Otherwise, make
      // no change to the map and return false.
     
    bool contains(const KeyType& key) const;
      // Return true if key is equal to a key currently in the map, otherwise
      // false.
     
    bool get(const KeyType& key, ValueType& value) const;
      // If key is equal to a key currently in the map, set value to the
      // value in the map that that key maps to and return true.  Otherwise,
      // make no change to the value parameter of this function and return
      // false.
     
    bool get(int i, KeyType& key, ValueType& value) const;
      // If 0 <= i < size(), copy into the key and value parameters the
      // key and value of one of the key/value pairs in the map and return
      // true.  Otherwise, leave the key and value parameters unchanged and
      // return false. 

    void swap(Map& other);
      // Exchange the contents of this map with the other one.

  private:
	struct MapNode
	{
		KeyType m_key;
		ValueType m_value;
		MapNode* m_nextNode;
		MapNode* m_prevNode;
	};
	MapNode* head;
	MapNode* tail;
	int m_size;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif
