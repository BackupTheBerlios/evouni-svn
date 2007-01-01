#pragma once
#include "CheckPoint.h"

//Workaround
class Character;

class CheckPointManager
{
protected:
	static const int MAX_CHECKPOINTS = 20;
	static const int MAX_PARTICLES_PER_CHECKPOINT = 500;

	SceneManager *mSceneMgr;
	SceneNode* mPosition;
	ManualObject* mSolid;
	String mName;

	ParticleSystem *pTargets;

	//CheckPoint *cp;
	CheckPoint *cp[MAX_CHECKPOINTS];
	int num_of_cp;
	int counter;
	bool done;


	void resetLevel();
public:
	CheckPointManager();
	CheckPointManager(String name, SceneManager *sceneMgr, Vector3 position = Vector3(0,0,0));
	~CheckPointManager(void);
	void init(String name, SceneManager *sceneMgr, Vector3 position);
	void CheckColission(Character *object);
	void AddCheckPoint(Vector3 position, CPState state);
	void loadLevel(String level);

};
