#include "History.h"
#include "Arena.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	for (int r = 0; r < nRows; r++)
		for (int c = 0; c < nCols; c++)
			m_grid[r][c] = 0;
	m_row = nRows;
	m_col = nCols;
}

bool History::record(int r, int c)
{
	if (r < 0 || r > m_row || c < 0 || c > m_col)
		return false;
	m_grid[r-1][c-1]++;
	return true;
}

void History::display() const
{
	clearScreen();

	char historyGrid[MAXROWS][MAXCOLS];
	int r, c;

	for (r = 0; r < m_row; r++)
	{
		for (c = 0; c < m_col; c++)
		{
			if (m_grid[r][c] == 0)
				historyGrid[r][c] = '.';
			else if (m_grid[r][c] < 26)
				historyGrid[r][c] = m_grid[r][c] + 64;
			else
				historyGrid[r][c] = 'Z';
		}
	}

	for (r = 1; r <= m_row; r++)
    {
        for (c = 1; c <= m_col; c++)
            cout << historyGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
}
