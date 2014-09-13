#ifndef History_INCLUDED
#define History_INCLUDED

#include "globals.h"
class Arena;

class History
{
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
  private:
	int m_grid[MAXROWS][MAXCOLS];
	int m_row;
	int m_col;
};

#endif
