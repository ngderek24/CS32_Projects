#include "Database.h"
#include "MultiMap.h"
#include "Tokenizer.h"
#include "http.h"
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <iostream>
using namespace std;

Database::Database() {}

Database::~Database() 
{
	for (size_t k = 0; k < m_fieldIndex.size(); k++)	//destructor: delete all dynamically allocated MultiMaps
		delete m_fieldIndex[k];
}

bool Database::specifySchema(const vector<FieldDescriptor>& schema)
{
	m_schema.clear();						//clear previous schema,fieldIndex, and rows when specifying new schema
	m_fieldIndex.clear();
	for (size_t n = 0; n < m_rows.size(); n++)
		m_rows[n].clear();

	for (size_t i = 0; i < schema.size(); i++)
	{
		m_schema.push_back(schema[i]);		//create new schema and allocate new MultiMap for each field
		MultiMap* map = new MultiMap;
		m_fieldIndex.push_back(map);
	}

	if (m_fieldIndex.empty())	//if there's no indexed field, leave schema empty and return false
	{
		m_schema.clear();
		return false;
	}
	else
		return true;
}

bool Database::addRow(const vector<string>& rowOfData)
{
	if (rowOfData.size() != m_schema.size())	//exit function if the row of data being added doesn't match schema
		return false;

	m_rows.push_back(rowOfData);						//add new row of data into rows
	for (size_t k = 0; k < m_schema.size(); k++)		//check which field is indexed and insert into MultiMap
		if (m_schema[k].index == it_indexed)
			m_fieldIndex[k]->insert(rowOfData[k], m_rows.size()-1);
			
	return true;
}

bool Database::loadFromURL(string url)
{
	string page;
	if (HTTP().get(url, page))		//get content of the url and load it into page variable
	{
		string delimiters = "\n";			//set delimiter as newline character to separate each line of text
		Tokenizer t(page, delimiters);		//create a Tokenizer
		string token;
		t.getNextToken(token);				//get the first line to get schema
		
		string limits = ",";				//set another delimiter as comma to cut up text fields within each line
		Tokenizer z(token, limits);
		vector<FieldDescriptor> schema;
		FieldDescriptor toAdd;
		while (z.getNextToken(token))		//go through each field in first line to specify schema
		{
			if (token[token.size()-1] == '*')		//field has * so it is supposed to be indexed
			{
				token = token.substr(0, token.size()-1);
				toAdd.name = token;
				toAdd.index = it_indexed;
				schema.push_back(toAdd);		//add to schema
			}
			else
			{
				toAdd.name = token;
				toAdd.index = it_none;
				schema.push_back(toAdd);
			}
		}
		specifySchema(schema);			//specify schema with schema vector

		vector<string> row;				//row represent new row to added to database
		while (t.getNextToken(token))	//keep getting new lines of text until end of file
		{
			Tokenizer s(token, limits);
			while (s.getNextToken(token))	//keep getting each field of information within one line to add to row
				row.push_back(token);
			addRow(row);					//add to database
			row.clear();					//reset row to specify new row
		}
		return true;
	}
	else
		return false;			//return false for whatever reason url couldn't be loaded
}

bool Database::loadFromFile(string filename)
{
	ifstream infile(filename);		//read in file
	if (!infile)					//if file couldn't not be loaded, exit function
		return false;
	
	string line;
	getline(infile, line);			//get each line of text from file
		
	string limits = ",";
	Tokenizer z(line, limits);
	string token;
	vector<FieldDescriptor> schema;
	FieldDescriptor toAdd;

	while (z.getNextToken(token))				//go through each field in first line to specify schema
	{
		if (token[token.size()-1] == '*')				//field has * so it is supposed to be indexed
		{
			token = token.substr(0, token.size()-1);
			toAdd.name = token;
			toAdd.index = it_indexed;
			schema.push_back(toAdd);			//add to schema
		}
		else
		{
			toAdd.name = token;
			toAdd.index = it_none;
			schema.push_back(toAdd);
		}
	}
	specifySchema(schema);						//specify schema with schema vector

	vector<string> row;							//row represent new row to added to database
	while (getline(infile, line))				//keep getting new lines of text until end of file
	{
		Tokenizer s(line, limits);
		while (s.getNextToken(token))			//keep getting each field of information within one line to add to row
			row.push_back(token);
		addRow(row);							//add to database
		row.clear();							//reset row to specify new row
	}

	return true;
}

int Database::getNumRows() const
{
	return m_rows.size();			//get number of rows in database
}

bool Database::getRow(int rowNum, vector<string>& row) const
{
	if (rowNum >= m_rows.size())		//rowNum is out of range so return false
		return false;

	row.clear();
	for (size_t n = 0; n < m_schema.size(); n++)	//add each field of information into row
		row.push_back(m_rows[rowNum][n]);
	
	return true;
}

int	Database::search(const vector<SearchCriterion>& searchCriteria, const vector<SortCriterion>& sortCriteria, vector<int>& results)
{
	results.clear();
	if (searchCriteria.empty())				//error if there's no search criteria
		return ERROR_RESULT;

	for (size_t m = 0; m < searchCriteria.size(); m++)		//go through each search criteria
	{
		string field = searchCriteria[m].fieldName;		//get the field, min, max of criteria
		string min = searchCriteria[m].minValue;
		string max = searchCriteria[m].maxValue;

		if (isError(field, min, max))			//if criteria has invalid input then return error
			return ERROR_RESULT;

		if (results.empty())						//searching based on first criteria so add directly to results
			searchHelper(field, min, max, results);		//use helper function to search database
		else
		{
			vector<int> temp;	
			unordered_set<int> holder;		//searching based on next criteria, so create unordered set as temporary holder 
			searchHelper(field, min, max, temp);
			
			for (size_t i = 0; i < temp.size(); i++)	//transfer search results from vector into unordered set
				holder.insert(temp[i]);

			for (size_t r = 0; r < results.size(); r++)	
			{
				if (holder.find(results[r]) == holder.end())	//if value in results is not in set, then value doesn't satisfy
				{												//multiple criteria so erase from results
					results.erase(results.begin()+r);
					r--;
				}
			}
		}
	}

	if (!sortCriteria.empty())					//sort results if there's a sort criteria
		sortHelper(results, 0, results.size()-1, sortCriteria);

	return results.size();
}

void Database::sortHelper(vector<int>& toBeSorted, int first, int last, const vector<SortCriterion>& sortCriteria)
{
	if (last-first >= 1)	//sort the results based on sort criteria using quicksort
	{
		int pivotIndex;
		pivotIndex = partition(toBeSorted, first, last, sortCriteria);		//find spot in results to divide and recursively sort
		sortHelper(toBeSorted, first, pivotIndex-1, sortCriteria);			//sort left sub-vector
		sortHelper(toBeSorted, pivotIndex+1, last, sortCriteria);			//sort right sub-vector
	}
}

int Database::partition(vector<int>& toBeSorted, int low, int high, const vector<SortCriterion>& sortCriteria)
{
	int pos = low;
	int pivot = toBeSorted[low];
	do
	{
		while (low <= high && isInOrder(toBeSorted[low], pivot, sortCriteria))	//keep going until find spot greater than pivot
			low++;
		while (isInOrder(pivot, toBeSorted[high], sortCriteria))		//keep going until find spot less than pivot
		{
			if (high-1 < pos)			//make sure high doesn't go out of range
				break;
			high--;
		}
		if (low < high)					//if has not traverse through whole vector, swap positions and keep looping
			swap(toBeSorted[low], toBeSorted[high]);
	}
	while (low < high);
	swap(toBeSorted[pos], toBeSorted[high]);		//has finished traversing, so swap pivot into position to divide vector
	pos = high;
	return pos;									//return position that divides vector into two for recursion
}

bool Database::isInOrder(int a, int b, const std::vector<SortCriterion>& sortCriteria)
{
	for (size_t k = 0; k < sortCriteria.size(); k++)		//go through sort criteria
	{
		int field = 0;
		for (size_t n = 0; n < m_schema.size(); n++)		//get field that we're comparing
		{
			if (m_schema[n].name == sortCriteria[k].fieldName)
			{
				field = n;
				break;
			}
		}

		if (sortCriteria[k].ordering == ot_ascending)		//if sorting in ascending order
		{
			if (m_rows[a][field] < m_rows[b][field])	//true if field of first variable is less than field ofsecond variable
				return true;
			if (m_rows[a][field] > m_rows[b][field])	//false otherwise
				return false;
		}

		if (sortCriteria[k].ordering == ot_descending)		//if sorting in descending order
		{
			if (m_rows[a][field] > m_rows[b][field])	//true if field of first variable is greater than field ofsecond variable
				return true;
			if (m_rows[a][field] < m_rows[b][field])	//false otherwise
				return false;
		}
	}
	return true;
}

bool Database::isError(string fieldName, string minValue, string maxValue)
{
	if (minValue == "" && maxValue == "")		//error if both min and max not specified
		return true;
	
	for (size_t n = 0; n < m_schema.size(); n++)
		if (fieldName == m_schema[n].name)			//if specified field name is valid then not error
			return false;

	return true;				//can't find field name in schema so error
}

void Database::searchHelper(string fieldName, string minValue, string maxValue, vector<int>& result)
{
	int mapNum = findMap(fieldName);			//find index of MultiMap of field we're trying to search
	if (minValue != "" && maxValue != "")		//if there's a specified range
	{
		MultiMap::Iterator it = m_fieldIndex[mapNum]->findEqualOrSuccessor(minValue);	//find equal or successor of min
		for(;;)
		{
			if (!it.valid() || it.getKey() > maxValue)		//add value of iterator into results until iterator is invalid
				break;										//or greater than max by using next()
			result.push_back(it.getValue());
			it.next();
		}
	}
	
	if (minValue == "")					//if no specified min
	{
		MultiMap::Iterator it = m_fieldIndex[mapNum]->findEqualOrPredecessor(maxValue);		//find equal or predecessor of max
		for(;;)
		{
			if (!it.valid())			//add value of iterator into results until iterator is invalid, meaning all valid
				break;					//data has been added by using prev()
			result.push_back(it.getValue());
			it.prev();
		}
	}

	if (maxValue == "")				//if no specified max
	{
		MultiMap::Iterator it = m_fieldIndex[mapNum]->findEqualOrSuccessor(minValue);		//find equal or successor of min
		for(;;)
		{
			if (!it.valid())					//add value of iterator into results until iterator is invalid, meaning all valid
				break;							//data has been added by using next()
			result.push_back(it.getValue());
			it.next();
		}
	}
}

int Database::findMap(string fieldName)
{
	for (size_t j = 0; j < m_schema.size(); j++)		//go through schema to see if field name is valid in schema
		if (fieldName == m_schema[j].name)
			return j;									//return position of that field if found

	return -1;
}
