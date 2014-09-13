#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;

class Actor : public GraphObject
{
  public:
	Actor(int imageID, int x, int y, StudentWorld* obj);
	virtual ~Actor() {}
	virtual void doSomething() = 0;
	virtual void killed(int zumiID) {}
	
	//Modifiers
	void setDead();
	void setTicksNum(int num);
	void revealExit();
	
	//Accessors
	bool isAlive() const;
	int getID() const;
	int actorAt(int x, int y) const;
	int getTicksLeft() const;
	StudentWorld* world() const;
	Actor* getActor(int x, int y) const;
	
  private:
	bool m_alive;
	StudentWorld* m_world;
	int m_ID;
	int m_ticksLeft;
};

class Player : public Actor
{
  public:
	Player(int x, int y, StudentWorld* obj);
	virtual ~Player() {}
	virtual void doSomething();
	
	//Modifiers
	void setThruBrickTicks(int num);
	void setBoostedSprayerTicks(int num);
	void setMaxSprayers(int num);
	void decNumSprayers();
	
	//Accessors
	bool canMove(int x, int y, bool thru) const;
	bool hasDied(int x, int y) const;

  private:
	int m_maxNumSprayers;
	int m_numSprayer;
	int m_thruBrickTicks;
	int m_boostedSprayerTicks;
};

class Zumi : public Actor
{
  public:
	Zumi(int gameID, int x, int y, StudentWorld* obj);
	virtual ~Zumi() {}
	int randInt(int lowest, int highest);
	virtual void move(int dir, int ID);
	virtual void doSomething();
	
	//Modifiers
	void setCurDirection();
	virtual void killed(int zumiID);
	
	//Accessors
	int getCurDirection() const;
	int xDistanceFromPlayer();
	int yDistanceFromPlayer();
	int searchPath(int x, int y, int px, int py);
	
  private:
	int m_curDirection;
	int m_ticksPerMove;
};

class SimpleZumi : public Zumi
{
  public:
	SimpleZumi(int x, int y, StudentWorld* obj) : Zumi(IID_SIMPLE_ZUMI, x, y, obj) {}
	virtual ~SimpleZumi() {}
  private:
};

class ComplexZumi : public Zumi
{
  public:
	ComplexZumi(int x, int y, StudentWorld* obj) : Zumi(IID_COMPLEX_ZUMI, x, y, obj) {}
	virtual ~ComplexZumi() {}
  private:
};

class Brick : public Actor
{
  public:
	Brick(int imageID, int x, int y, StudentWorld* obj) : Actor(imageID, x, y, obj) { setVisible(true); }
	virtual ~Brick() {}
	virtual void doSomething() {}
  private:
};

class PermanentBrick : public Brick
{
  public:
	PermanentBrick(int x, int y, StudentWorld* obj) : Brick(IID_PERMA_BRICK, x, y, obj) {}
	virtual ~PermanentBrick() {}
  private:
};

class DestroyableBrick : public Brick
{
  public:
	DestroyableBrick(int x, int y, StudentWorld* obj) : Brick(IID_DESTROYABLE_BRICK, x, y, obj) {}
	virtual ~DestroyableBrick() {}
  private:
};

class Goodies : public Actor
{
  public:
	Goodies(int imageID, int x, int y, StudentWorld* obj);
	virtual ~Goodies() {}
	virtual void doSomething();
  private:
};

class ExtraLifeGoodies : public Goodies
{
  public:
	ExtraLifeGoodies(int x, int y, StudentWorld* obj) : Goodies(IID_EXTRA_LIFE_GOODIE, x, y, obj) {}
	virtual ~ExtraLifeGoodies() {}
  private:
};

class WalkThroughGoodies : public Goodies
{
  public:
	WalkThroughGoodies(int x, int y, StudentWorld* obj) : Goodies(IID_WALK_THRU_GOODIE, x, y, obj) {}
	virtual ~WalkThroughGoodies() {}
  private:
};

class SimultaneousSprayerGoodies : public Goodies
{
  public:
	SimultaneousSprayerGoodies(int x, int y, StudentWorld* obj) : Goodies(IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, x, y, obj) {}
	virtual ~SimultaneousSprayerGoodies() {}
  private:
};

class Exit : public Actor
{
  public:
	Exit(int x, int y, StudentWorld* obj) : Actor(IID_EXIT, x, y, obj) {}
	virtual ~Exit() {}
	virtual void doSomething();
  private:
};

class BugSprayer : public Actor
{
  public:
	BugSprayer(int x, int y, StudentWorld* obj);
	virtual ~BugSprayer() {}
	virtual void doSomething();
	bool canDropSpray(int x, int y) const;
  private:
};

class BugSpray : public Actor
{
  public:
	BugSpray(int x, int y, StudentWorld* obj);
	virtual ~BugSpray() {}
	virtual void doSomething();
  private:
};

class Coord
{
  public:
	Coord(int rr, int cc, int dir) : m_r(rr), m_c(cc), m_dir(dir) {}
	~Coord() {}
	int r() const { return m_r; }
	int c() const { return m_c; }
	int dir() const { return m_dir; }
  private:
	int m_r;
	int m_c;
	int m_dir;
};

#endif // ACTOR_H_
