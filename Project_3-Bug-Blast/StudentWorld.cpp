#include "StudentWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

int StudentWorld::init()
{
	m_lvlCompleted = false; 
	m_exitRevealed = false;
	m_level = getLevel();
	m_levelBonus = curLevel().getOptionValue(optionLevelBonus);
	return loadMaze(m_level);
}

Level StudentWorld::curLevel()
{
	Level lev;
	string level = formatLevel(m_level);

	lev.loadLevel(level);
	return lev;
}

int StudentWorld::loadMaze(int level)
{
	string curLevel;
	curLevel = formatLevel(level);
			
	Level lev;	
	Level::LoadResult result = lev.loadLevel(curLevel);	
	 	 	
	if(curLevel == "level00.dat" && result == Level::load_fail_file_not_found)
		return GWSTATUS_NO_FIRST_LEVEL;
	if(curLevel != "level00.dat" && result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
 	if(result == Level::load_fail_bad_format)	
		return GWSTATUS_LEVEL_ERROR;
	
	for(int x = 0; x < VIEW_WIDTH; x++) 
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
			Level::MazeEntry item = lev.getContentsOf(x, y);	
		
			if (item == Level::player)
				m_player = new Player(x, y, this);

			if (item == Level::perma_brick) 
			{
				Actor* pBrick = new PermanentBrick(x, y, this);
				m_actors.push_back(pBrick);
			}

			if (item == Level::destroyable_brick) 
			{
				Actor* dBrick = new DestroyableBrick(x, y, this);
				m_actors.push_back(dBrick);
			}

			if (item == Level::complex_zumi)
			{
				Actor* cZumi = new ComplexZumi(x, y , this);
				m_actors.push_back(cZumi);
			}

			if (item == Level::simple_zumi)
			{
				Actor* sZumi = new SimpleZumi(x, y, this);
				m_actors.push_back(sZumi);
			}

			if (item == Level::exit)
			{
				Actor* exit = new Exit(x, y, this);
				m_actors.push_back(exit);
			}
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

std::vector<Actor*>& StudentWorld::actor()
{
	return m_actors;
}

Player* StudentWorld::person()
{
	return m_player;
}

string StudentWorld::formatLevel(int level)
{
	ostringstream lvl;
	lvl << "level";
	lvl.fill('0');
	lvl << setw(2) << level << ".dat";
	return lvl.str();
}

int StudentWorld::move()
{
	setDisplayText(m_level);
	
	if (m_player->isAlive())
		m_player->doSomething();
	else
		return GWSTATUS_PLAYER_DIED;

	for (size_t k = 0; k < m_actors.size(); k++)
	{
		if (m_actors[k]->isAlive())
		{
			m_actors[k]->doSomething();
			if (!m_player->isAlive())
				return GWSTATUS_PLAYER_DIED;
			if (m_lvlCompleted)
			{
				increaseScore(curLevel().getOptionValue(optionLevelBonus));
				return GWSTATUS_FINISHED_LEVEL;
			}
		}
	}
	
	removeDead();

	reduceBonusByOne();

	if (!m_exitRevealed && killedAllZumis())
	{
		vector<Actor*>::iterator it;
		for (it = m_actors.begin(); it != m_actors.end(); it++)
		{
			if ((*it)->getID() == IID_EXIT)
				(*it)->revealExit();
		}
	}
	
	if (!m_player->isAlive())
		return GWSTATUS_PLAYER_DIED;

	if (m_lvlCompleted)
	{
		increaseScore(curLevel().getOptionValue(optionLevelBonus));
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setDisplayText(int level)
{
	int score = getScore();
	int clevel = getLevel();
	unsigned int bonus = m_levelBonus;
	int livesLeft = getLives();

	string s = formatString(score, clevel, livesLeft, bonus);

	setGameStatText(s);
}

string StudentWorld::formatString(unsigned int score, unsigned int level, unsigned int lives, unsigned int bonus)
{
	string t;
	ostringstream txt;
	txt << "Score: "; 
	txt.fill('0');
	txt << setw(8) << score << "  Level: "; 
	txt.fill('0');
	txt << setw(2) << level << "  Lives: "; 
	txt.fill('0');
	txt << setw(3) << lives << "  Bonus: ";
	txt.fill(' ');
	txt << setw(6) << bonus;
	t = txt.str();
	return t;
}

void StudentWorld::setLvlCompleted()
{
	m_lvlCompleted = true;
}

void StudentWorld::setExitRevealed()
{
	m_exitRevealed = true;
}

bool StudentWorld::killedAllZumis()
{
	vector<Actor*>::iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		if (((*it)->getID() == IID_SIMPLE_ZUMI || (*it)->getID() == IID_COMPLEX_ZUMI) && (*it)->isAlive())
			return false;
	}
	return true;
}

void StudentWorld::removeDead()
{
	for (size_t k = 0; k < m_actors.size(); k++)
	{
		if (!m_actors[k]->isAlive())
		{
			delete m_actors[k];
			vector<Actor*>::iterator it, a, b;
			a = m_actors.begin();
			b = m_actors.end();
			it = find(a, b, m_actors[k]);
			if (it != b)
				m_actors.erase(it);
		}
	}
}

void StudentWorld::reduceBonusByOne()
{
	if (m_levelBonus > 0)
		m_levelBonus--;
}

void StudentWorld::cleanUp()
{
	delete m_player;

	for (size_t k = 0; k < m_actors.size(); k++)
		delete m_actors[k];

	vector<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end())
	{
		m_actors.erase(it);
		it = m_actors.begin();
	}
}
