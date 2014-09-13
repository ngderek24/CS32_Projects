#include "test.h"
#include "http.h"
#include "Database.h"
#include <iostream>
#include <string>
using namespace std;
 
//int main(int argc, char *argv[])
//{
//    string filename;
//
//    switch (argc)
//    {
//      case 1:
//        cout << "Enter test script file name: ";
//        getline(cin, filename);
//        break;
//      case 2:
//        filename = argv[1];
//        break;
//      default:
//        cout << "Usage: " << argv[0] << " scriptname" << endl;
//        return 1;
//    }
//    
//    Test t;
//    string problem;
//
//    if (!t.loadAndRunTestScript(filename, problem))
//    {
//        cout << "Problem running script: " << problem << endl;
//        return 1;
//    }
//}

int main()
{
	Database base;
	//base.loadFromFile("C:/Users/Derek/Documents/test.txt");
	base.loadFromURL("http://cs.ucla.edu/classes/winter14/cs32/Projects/4/Data/census.csv");
	Database::SearchCriterion cri; 
	cri.fieldName = "name";
	cri.minValue = "A";
	cri.maxValue = "B";
	//Database::SearchCriterion cri2;
	//cri2.fieldName = "age";
	//cri2.minValue = "20";
	//cri2.maxValue = "60";
	vector<Database::SearchCriterion> crit;
	crit.push_back(cri);
	//crit.push_back(cri2);

	Database::SortCriterion sort;
	sort.fieldName = "name";
	sort.ordering = Database::ot_ascending;
	vector<Database::SortCriterion> sor;
	sor.push_back(sort);

	vector<int> results;
	base.search(crit, sor, results);
	vector<string> row;
	for (int k = 0; k < results.size(); k++)
	{
		base.getRow(results[k], row);
		for (int i = 0; i < row.size(); i++)
			cout << row[i] << ",";
		cout << endl;
	}
	/*MultiMap map;
	map.insert("20", 3);
	map.insert("40", 3);
	map.insert("30", 3);
	map.insert("35", 3);
	map.insert("45", 3);
	map.insert("50", 3);
	map.insert("15", 3);
	map.insert("10", 3);
	map.insert("18", 3);
	map.insert("16", 4);
	map.insert("38", 4);
	MultiMap::Iterator it = map.findEqualOrSuccessor("16");
	cerr<<it.getKey()<<endl;*/
}
