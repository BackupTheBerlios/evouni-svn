#pragma once
#include "OGREinclude.h"

enum CPState
{
DONE,
TODO,
INACTIVE,
CURRENT
};

class CheckPoint
{
protected:
	String mName;
	SceneManager *mSceneMgr;

	Vector3 mPosition;

	ParticleEmitter *pEmitter;
	ParticleSystem *pSystem;
	CPState mState;

public:
	CheckPoint(String name, SceneManager *sceneMgr, CPState state, ParticleSystem* psystem, Vector3 position);
	~CheckPoint(void);
	inline Vector3 CheckPoint::getPosition()
	{
		return pEmitter->getPosition();
	}
	void setState(CPState);
	inline CPState getState();
};


