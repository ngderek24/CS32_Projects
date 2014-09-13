#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <vector>
#include <string>

class StudentWorld : public GameWorld
{
public:
	StudentWorld() {}
	virtual ~StudentWorld() {}
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	
	//Modifiers
	int loadMaze(int level);
	std::string formatLevel(int level);
	void setDisplayText(int level);
	std::string formatString(unsigned int score, unsigned int level, unsigned int lives, unsigned int bonus);
	void reduceBonusByOne();
	void removeDead();
	void setLvlCompleted();
	void setExitRevealed();
	
	//Accessors
	Player* person();
	Level curLevel();
	std::vector<Actor*>& actor();
	bool killedAllZumis();

private:
	std::vector<Actor*> m_actors;
	Player* m_player;
	bool m_lvlCompleted;
	int m_level;
	int m_levelBonus;
	bool m_exitRevealed;
};

#endif // STUDENTWORLD_H_
