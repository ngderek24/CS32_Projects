#include "Actor.h"
#include "StudentWorld.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <queue>
using namespace std;

Actor::Actor(int imageID, int x, int y, StudentWorld* obj) : GraphObject(imageID, x, y)
{
	m_alive = true;
	m_world = obj;
	m_ID = imageID;
	m_ticksLeft = 0;
}

void Actor::setTicksNum(int num)
{
	m_ticksLeft = num;
}

void Actor::setDead()
{
	m_alive = false;
}

bool Actor::isAlive() const
{
	return m_alive;
}

int Actor::getID() const
{
	return m_ID;
}

int Actor::actorAt(int x, int y) const
{
	for (size_t k = 0; k < world()->actor().size(); k++)
	{
		if (world()->actor()[k]->getX() == x && world()->actor()[k]->getY() == y)
			return world()->actor()[k]->getID();
	}
	return -1;
}

StudentWorld* Actor::world() const
{
	return m_world;
}

Actor* Actor::getActor(int x, int y) const
{
	for (size_t k = 0; k < world()->actor().size(); k++)
	{
		if (world()->actor()[k]->getX() == x && world()->actor()[k]->getY() == y)
			return world()->actor()[k];
	}
	return nullptr;
}

int Actor::getTicksLeft() const
{
	return m_ticksLeft;
}

void Actor::revealExit()
{
	setVisible(true);
	world()->playSound(SOUND_REVEAL_EXIT);
	world()->setExitRevealed();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Player::Player(int x, int y, StudentWorld* obj) : Actor(IID_PLAYER, x, y, obj)
{
	setVisible(true);
	m_maxNumSprayers = 2;
	m_numSprayer = 0;
	m_thruBrickTicks = 0;
	m_boostedSprayerTicks = 0;
}

void Player::doSomething()
{
	if (!isAlive())
		return;
	if (hasDied(getX(), getY()))
	{
		setDead();
		world()->decLives();
		world()->playSound(SOUND_PLAYER_DIE);
		return;
	}
	if (m_thruBrickTicks > 0)
		m_thruBrickTicks--;
	if (m_boostedSprayerTicks > 0)
		m_boostedSprayerTicks--;

	int key;
	if (world()->getKey(key))
	{
		switch(key)
		{
			case KEY_PRESS_LEFT:
				if (m_thruBrickTicks > 0)
				{
					if (canMove(getX()-1, getY(), true))
						moveTo(getX()-1, getY());
				}
				else
				{
					if (canMove(getX()-1, getY(), false))
						moveTo(getX()-1, getY());
				}
				break;
			case KEY_PRESS_RIGHT:
				if (m_thruBrickTicks > 0)
				{
					if (canMove(getX()+1, getY(), true))
						moveTo(getX()+1, getY());
				}
				else
				{
					if (canMove(getX()+1, getY(), false))
						moveTo(getX()+1, getY());
				}
				break;
			case KEY_PRESS_UP:
				if (m_thruBrickTicks > 0)
				{
					if (canMove(getX(), getY()+1, true))
						moveTo(getX(), getY()+1);
				}
				else
				{
					if (canMove(getX(), getY()+1, false))
						moveTo(getX(), getY()+1);
				}
				break;
			case KEY_PRESS_DOWN:
				if (m_thruBrickTicks > 0)
				{
					if (canMove(getX(), getY()-1, true))
						moveTo(getX(), getY()-1);
				}
				else
				{
					if (canMove(getX(), getY()-1, false))
						moveTo(getX(), getY()-1);
				}
				break;
			case KEY_PRESS_SPACE:
				if (m_numSprayer < m_maxNumSprayers && actorAt(getX(), getY()) != IID_DESTROYABLE_BRICK &&
					actorAt(getX(), getY()) != IID_PERMA_BRICK && actorAt(getX(), getY()) != IID_BUGSPRAYER)
				{
					Actor* sprayer = new BugSprayer(getX(), getY(), world());
					world()->actor().push_back(sprayer);
					m_numSprayer++;
				}
				break;
		}
	}
}

bool Player::hasDied(int x, int y) const
{
	vector<Actor*>::const_iterator it;
	for (it = world()->actor().begin(); it != world()->actor().end(); it++)
	{
		if (((*it)->getID() == IID_COMPLEX_ZUMI || (*it)->getID() == IID_SIMPLE_ZUMI) && x == (*it)->getX() && y == (*it)->getY())
			return true;
		if (m_thruBrickTicks == 0 && (*it)->getID() == IID_DESTROYABLE_BRICK && x == (*it)->getX() && y == (*it)->getY())
			return true;
	}
	return false;
}

bool Player::canMove(int x, int y, bool thru) const
{
	vector<Actor*>::const_iterator it;
	for (it = world()->actor().begin(); it != world()->actor().end(); it++)
	{
		if (thru)
		{
			if ((*it)->getID() == IID_PERMA_BRICK && x == (*it)->getX() && y == (*it)->getY())
				return false; 
		}
		else
		{
			if (((*it)->getID() == IID_PERMA_BRICK || (*it)->getID() == IID_DESTROYABLE_BRICK) && x == (*it)->getX() && y == (*it)->getY()) 
				return false;
		}
	}
	return true;
}

void Player::setThruBrickTicks(int num)
{
	m_thruBrickTicks = num;
}

void Player::setBoostedSprayerTicks(int num)
{
	m_boostedSprayerTicks = num;
}

void Player::setMaxSprayers(int num)
{
	m_maxNumSprayers = num;
}

void Player::decNumSprayers()
{
	if(m_numSprayer > 0)
		m_numSprayer--;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Zumi::Zumi(int imageID, int x, int y, StudentWorld* obj) : Actor(imageID, x, y, obj) 
{
	setVisible(true);
	setCurDirection();
	if (imageID == IID_COMPLEX_ZUMI)
		m_ticksPerMove = world()->curLevel().getOptionValue(optionTicksPerComplexZumiMove);
	if (imageID == IID_SIMPLE_ZUMI)
		m_ticksPerMove = world()->curLevel().getOptionValue(optionTicksPerSimpleZumiMove);
}

int Zumi::randInt(int lowest, int highest)
{
    if (highest < lowest)
        std::swap(highest, lowest);
    return lowest + (std::rand() % (highest - lowest + 1));
}

int Zumi::getCurDirection() const
{
	return m_curDirection;
}

void Zumi::setCurDirection()
{
	srand(time(0));
	m_curDirection = randInt(1000, 1003);
}

void Zumi::move(int dir, int ID)
{
	switch(dir)
	{
	case KEY_PRESS_LEFT:
		if (actorAt(getX()-1, getY()) == IID_BUGSPRAYER || actorAt(getX()-1, getY()) == IID_DESTROYABLE_BRICK ||
			actorAt(getX()-1, getY()) == IID_PERMA_BRICK)
		{
			setCurDirection();
			break;
		}
		else
		{
			moveTo(getX()-1, getY());
			setCurDirection();
			break;
		}
	case KEY_PRESS_RIGHT:
		if (actorAt(getX()+1, getY()) == IID_BUGSPRAYER || actorAt(getX()+1, getY()) == IID_DESTROYABLE_BRICK ||
			actorAt(getX()+1, getY()) == IID_PERMA_BRICK)
		{
			setCurDirection();
			break;
		}
		else
		{
			moveTo(getX()+1, getY());
			setCurDirection();
			break;
		}
	case KEY_PRESS_UP:
		if (actorAt(getX(), getY()+1) == IID_BUGSPRAYER || actorAt(getX(), getY()+1) == IID_DESTROYABLE_BRICK ||
			actorAt(getX(), getY()+1) == IID_PERMA_BRICK)
		{
			setCurDirection();
			break;
		}
		else
		{
			moveTo(getX(), getY()+1);
			setCurDirection();
			break;
		}
	case KEY_PRESS_DOWN:
		if (actorAt(getX(), getY()-1) == IID_BUGSPRAYER || actorAt(getX(), getY()-1) == IID_DESTROYABLE_BRICK ||
			actorAt(getX(), getY()-1) == IID_PERMA_BRICK)
		{
			setCurDirection();
			break;
		}
		else
		{
			moveTo(getX(), getY()-1);
			setCurDirection();
			break;
		}
	default:
		if (ID == IID_COMPLEX_ZUMI)
		{
			move(getCurDirection(), IID_COMPLEX_ZUMI);
			setCurDirection();
		}
		else
			setCurDirection();
		break;
	}
}

void Zumi::killed(int zumiID)
{
	setDead();
	world()->playSound(SOUND_ENEMY_DIE);
	if (zumiID == IID_COMPLEX_ZUMI)
		world()->increaseScore(500);
	else
		world()->increaseScore(100);
	
	int goodieChance = world()->curLevel().getOptionValue(optionProbOfGoodieOverall);
	int randChance = randInt(0, 99);
	if (randChance < goodieChance)
	{
		int extraLifeChance = world()->curLevel().getOptionValue(optionProbOfExtraLifeGoodie);
		int walkThruChance = world()->curLevel().getOptionValue(optionProbOfWalkThruGoodie);
		int boostedSprayerChance = world()->curLevel().getOptionValue(optionProbOfMoreSprayersGoodie);

		randChance = randInt(0, 99);
		if (randChance < extraLifeChance)
		{
			Actor* extraLife = new ExtraLifeGoodies(getX(), getY(), world());
			world()->actor().push_back(extraLife);
			return;
		}

		if (randChance < walkThruChance)
		{
			Actor* walkThru = new WalkThroughGoodies(getX(), getY(), world());
			world()->actor().push_back(walkThru);
			return;
		}

		if (randChance < boostedSprayerChance)
		{
			Actor* boosted = new SimultaneousSprayerGoodies(getX(), getY(), world());
			world()->actor().push_back(boosted);
			return;
		}
	}
}

void Zumi::doSomething()
{
	if (!isAlive())
		return;
	if (getX() == world()->person()->getX() && getY() == world()->person()->getY())
	{
		world()->person()->setDead();
		world()->decLives();
	}
	if (m_ticksPerMove > 1)
	{
		m_ticksPerMove--;
		return;
	}
	if (getID() == IID_SIMPLE_ZUMI)
	{
		move(getCurDirection(), IID_SIMPLE_ZUMI);
		m_ticksPerMove = world()->curLevel().getOptionValue(optionTicksPerSimpleZumiMove);
	}
	if (getID() == IID_COMPLEX_ZUMI)
	{
		int smellDistance = world()->curLevel().getOptionValue(optionComplexZumiSearchDistance);
		if (xDistanceFromPlayer() <= smellDistance && yDistanceFromPlayer() <= smellDistance)
			move(searchPath(15-getY()-1, getX(), 15-world()->person()->getY()-1, world()->person()->getX()), IID_COMPLEX_ZUMI);
		else
			move(getCurDirection(), IID_SIMPLE_ZUMI);
		m_ticksPerMove = world()->curLevel().getOptionValue(optionTicksPerComplexZumiMove);
	}
}

int Zumi::xDistanceFromPlayer()
{
	int xDist;

	if (getX()-world()->person()->getX() > 0)
		xDist = getX()-world()->person()->getX();
	else
		xDist = world()->person()->getX()-getX();

	return xDist;
}

int Zumi::yDistanceFromPlayer()
{
	int yDist;

	if (getY()-world()->person()->getY() > 0)
		yDist = getY()-world()->person()->getY();
	else
		yDist = world()->person()->getY()-getY();

	return yDist;
}

int Zumi::searchPath(int x, int y, int px, int py)
{
	char game[VIEW_HEIGHT][VIEW_WIDTH];
	for (int k = 0; k < VIEW_HEIGHT; k++)
	{
		for (int n = 0; n < VIEW_WIDTH; n++)
		{
			if (actorAt(k, n) == IID_BUGSPRAYER || actorAt(k, n) == IID_DESTROYABLE_BRICK || actorAt(k, n) == IID_PERMA_BRICK)
				game[VIEW_WIDTH-n-1][k] = 'X';
			else
				game[VIEW_WIDTH-n-1][k] = '.';
		}
	}

	int firstTurn = 0;
	
	queue<Coord> search;
	int dir = 0;
	Coord current(x, y, dir);
	search.push(current);
	game[x][y] = 'X';

	while(!search.empty())
	{
		current = search.front();
		search.pop();

		//SPOT TO THE LEFT
		if (current.c()-1 == py && current.r() == px)
		{
			if (firstTurn == 0)
				dir = KEY_PRESS_LEFT;
			else
				dir = current.dir();
			break;
		}
		if (game[current.r()][current.c()-1] == '.')
		{
			if (firstTurn == 0)
				search.push(Coord(current.r(), current.c()-1, KEY_PRESS_LEFT));
			else
				search.push(Coord(current.r(), current.c()-1, current.dir()));
			game[current.r()][current.c()-1] = 'X';
		}

		//SPOT TO THE RIGHT
		if (current.r() == px && current.c()+1 == py)
		{
			if (firstTurn == 0)
				dir = KEY_PRESS_RIGHT;
			else
				dir = current.dir();
			break;
		}
		if (game[current.r()][current.c()+1] == '.')
		{
			if (firstTurn == 0)
				search.push(Coord(current.r(), current.c()+1, KEY_PRESS_RIGHT));
			else
				search.push(Coord(current.r(), current.c()+1, current.dir()));
			game[current.r()][current.c()+1] = 'X';
		}

		//SPOT ABOVE
		if (current.r()-1 == px && current.c() == py)
		{
			if (firstTurn == 0)
				dir = KEY_PRESS_UP;
			else
				dir = current.dir();
			break;
		}
		if (game[current.r()-1][current.c()] == '.')
		{
			if (firstTurn == 0)
				search.push(Coord(current.r()-1, current.c(), KEY_PRESS_UP));
			else
				search.push(Coord(current.r()-1, current.c(), current.dir()));
			game[current.r()-1][current.c()] = 'X';
		}

		//SPOT BELOW
		if (current.r()+1 == px && current.c() == py)
		{
			if (firstTurn == 0)
				dir = KEY_PRESS_DOWN;
			else
				dir = current.dir();
			break;
		}
		if (game[current.r()+1][current.c()] == '.')
		{
			if (firstTurn == 0)
				search.push(Coord(current.r()+1, current.c(), KEY_PRESS_DOWN));
			else
				search.push(Coord(current.r()+1, current.c(), current.dir()));
			game[current.r()+1][current.c()] = 'X';
		}
		firstTurn++;
	}
	return dir;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Exit::doSomething()
{
	if (isAlive() && world()->person()->getX() == getX() && world()->person()->getY() == getY())
	{
		world()->playSound(SOUND_FINISHED_LEVEL);
		world()->setLvlCompleted();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

BugSprayer::BugSprayer(int x, int y, StudentWorld* obj) : Actor(IID_BUGSPRAYER, x, y, obj)
{
	setVisible(true);
	setTicksNum(40);
}

void BugSprayer::doSomething()
{
	if (!isAlive())
		return;
	setTicksNum(getTicksLeft()-1);
	
	if (getTicksLeft() == 0)
	{
		Actor* spray = new BugSpray(getX(), getY(), world());
		world()->actor().push_back(spray);
		
		//drop sprays to the left
		for (int k = 1; k <= 2; k++)
		{
			if (canDropSpray(getX()-k, getY()))
			{
				Actor* spray = new BugSpray(getX()-k, getY(), world());
				world()->actor().push_back(spray);
				if (actorAt(getX()-k, getY()) == IID_DESTROYABLE_BRICK)
					break;
			}
			else
				break;
		}

		//drop sprays to the right
		for (int k = 1; k <= 2; k++)
		{
			if (canDropSpray(getX()+k, getY()))
			{
				Actor* spray = new BugSpray(getX()+k, getY(), world());
				world()->actor().push_back(spray);
				if (actorAt(getX()+k, getY()) == IID_DESTROYABLE_BRICK)
					break;
			}
			else
				break;
		}

		//drop sprays above
		for (int k = 1; k <= 2; k++)
		{
			if (canDropSpray(getX(), getY()-k))
			{
				Actor* spray = new BugSpray(getX(), getY()-k, world());
				world()->actor().push_back(spray);
				if (actorAt(getX(), getY()-k) == IID_DESTROYABLE_BRICK)
					break;
			}
			else 
				break;
		}
		
		//drop sprays below
		for (int k = 1; k <= 2; k++)
		{
			if (canDropSpray(getX(), getY()+k))
			{
				Actor* spray = new BugSpray(getX(), getY()+k, world());
				world()->actor().push_back(spray);
				if (actorAt(getX(), getY()+k) == IID_DESTROYABLE_BRICK)
					break;
			}
			else 
				break;
		}

		world()->playSound(SOUND_SPRAY);
		setDead();
		world()->person()->decNumSprayers();
	}
}

bool BugSprayer::canDropSpray(int x, int y) const
{
	if (actorAt(x, y) == IID_PERMA_BRICK)
		return false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

BugSpray::BugSpray(int x, int y, StudentWorld* obj) : Actor(IID_BUGSPRAY, x, y, obj)
{
	setVisible(true);
	setTicksNum(3);
}

void BugSpray::doSomething()
{
	if (!isAlive())
		return;
	setTicksNum(getTicksLeft()-1);

	if (getTicksLeft() == 0)
		setDead();

	if (actorAt(getX(), getY()) == IID_DESTROYABLE_BRICK)
		getActor(getX(), getY())->setDead();
	
	if (actorAt(getX(), getY()) == IID_COMPLEX_ZUMI)
		getActor(getX(), getY())->killed(IID_COMPLEX_ZUMI);

	if (actorAt(getX(), getY()) == IID_SIMPLE_ZUMI)
		getActor(getX(), getY())->killed(IID_SIMPLE_ZUMI);
	
	if (world()->person()->getX() == getX() && world()->person()->getY() == getY())
	{
		world()->person()->setDead();
		world()->decLives();
	}

	if (actorAt(getX(), getY()) == IID_BUGSPRAYER)
	{
		setDead();
		getActor(getX(), getY())->setTicksNum(1);
		world()->person()->decNumSprayers();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Goodies::Goodies(int imageID, int x, int y, StudentWorld* obj) : Actor(imageID, x, y, obj)
{
	setVisible(true);
	setTicksNum(world()->curLevel().getOptionValue(optionGoodieLifetimeInTicks));
}

void Goodies::doSomething()
{
	if (!isAlive())
		return;
	setTicksNum(getTicksLeft()-1);

	if (getTicksLeft() == 0)
		setDead();

	if (getX() == world()->person()->getX() && getY() == world()->person()->getY())
	{
		if (getID() == IID_EXTRA_LIFE_GOODIE)
			world()->incLives();
		if (getID() == IID_WALK_THRU_GOODIE)
			world()->person()->setThruBrickTicks(world()->curLevel().getOptionValue(optionWalkThruLifetimeTicks));
		if (getID() == IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE)
		{
			world()->person()->setBoostedSprayerTicks(world()->curLevel().getOptionValue(optionBoostedSprayerLifetimeTicks));
			world()->person()->setMaxSprayers(world()->curLevel().getOptionValue(optionMaxBoostedSprayers));
		}
		world()->increaseScore(1000);
		setDead();
		world()->playSound(SOUND_GOT_GOODIE);
	}
}
