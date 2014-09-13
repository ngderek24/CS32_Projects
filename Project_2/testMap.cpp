#include <cassert>
#include "Map.h"
using namespace std;

int main()
{
	Map a;		//default constructor
	assert(a.empty() && a.size() == 0 && !a.erase("derek")); //test if new Map is empty, if size is zero
	a.insert("Derek", 10);									 //and nothing to erase
	assert(a.size() == 1 && !a.empty()); //test size of Map after one insertion and that map is not empty
	assert(!a.insert("Derek", 5));		//insertion fails because key already exists in map

	KeyType key;
	ValueType value;
	assert(a.get(0, key, value) && key == "Derek" && value == 10); //test "get" function with 3 paramenters and  
																   //whether if insertion was done correctly
	assert(!a.get(-1, key, value) && !a.get(3, key, value));	//get returns false if i is out of boundary
	assert(a.contains("Derek") && !a.contains("Daniel"));	//test contains function
	a.insert("Daniel", 5);
	assert(a.insert("", 3) && a.erase(""));		//test that insert and erase works with empty string
	
	Map b = a;	//debug to test copy constructor

	Map c;
	Map d = c;	//copy constructor on empty map
	c = a;		//debug to test assignment operator

	assert(!a.update("Jacob", 10) && a.update("Derek", 1) && !d.update("hi", 23));	//test update function,
																					//d has nothing to be updated
	assert(a.insertOrUpdate("Derek", 6) && d.insertOrUpdate("Jacob", 7));	//test insertorUpdate function
	assert(b.erase("Daniel") && !b.erase("Jacob") && !d.erase(""));		//debug and test erase function, b doesn't have 
																		//"jacob" to be erased, d is empty so no key to erase
	assert(a.get("Daniel",value) && value == 5 && !a.get("Matt", value));	//test "get" function with 2 parameters
	a.swap(d);	//test swap by debugging

	Map x;
	Map y;
	Map z;
	x.insert("derek", 5);
	x.insert("daniel", 2);
	x.insert("jacob", 4);
	y.insert("jason", 3);
	y.insert("andre", 1);
	assert(combine(x, y, z) && z.size() == 5);	//test most basic form of combine function
	z.insert("bryan", 6);
	assert(combine(x, y, z));	//test combine for when "result" is not passed as an empty Map
	y.insert("jacob", 4);
	assert(combine(x, y, z) && z.size() == 5);	//test combine when m1 and m2 contain same key and corresponding values
	assert(combine(x, x, z) && z.size() == 3);	//test combine when aliasing is present
	assert(combine(x, y, x) && x.size() == 5);	//test combine when aliasing is present
	assert(combine(x, y, y) && y.size() == 5);	//test combine when aliasing is present
	x.update("jacob", 8);
	assert(!combine(x, y, z) && z.size() == 4);	//test combine when m1 and m2 contain same key with different values

	Map o;
	Map p;
	Map q;
	subtract(o, p, q);
	assert(q.size() == 0);	//test subtract function when map is empty
	o.insert("derek", 1);
	o.insert("daniel", 2);
	p.insert("jacob", 3);
	subtract(o, p, q);
	assert(q.size() == 2);	//test subtract function when there's nothing to be subtracted
	p.insert("daniel", 2);
	subtract(o, p, q);
	assert(q.size() == 1);	//test subtract function where 1 key/value pair is being subtracted
	subtract(o, o, q);
	assert(q.size() == 0);	//test aliasing for subtract function
	subtract(o, p, o);
	assert(o.size() == 1);	//test aliasing for subtract function
	subtract(o, p, p);
	assert(p.size() == 1);	//test aliasing for subtract function
}
