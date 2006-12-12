#include "EUClient.h"

CheckPointManager::CheckPointManager()
{
	mName = "";
	mSceneMgr = 0;
	mPosition = 0;
	num_of_cp = 0;
	counter = 0;
	done = true;
	pTargets = 0;
}

CheckPointManager::CheckPointManager(String name, SceneManager *sceneMgr, Vector3 position)
{
	init(name,sceneMgr,position);
}

void CheckPointManager::init(String name, SceneManager *sceneMgr, Vector3 position)
{
	Log* log = LogManager::getSingletonPtr()->getDefaultLog();
	log->logMessage("Name: " + name);
	mName = name;
	log->logMessage("Name: " + mName);
	mSceneMgr = sceneMgr;
	mPosition = mSceneMgr->getRootSceneNode()->createChildSceneNode ( mName + "_node" );
	mPosition->setPosition(position);

	num_of_cp = 0;
	counter = 0;
	done = true;
	
	pTargets = mSceneMgr->createParticleSystem( mName + "_particlesystem", num_of_cp * MAX_PARTICLES_PER_CHECKPOINT);
    pTargets->setMaterialName( "Examples/Flare" );
    pTargets->setDefaultDimensions( 20, 20 );
	mPosition->attachObject(pTargets);

	mSolid = new ManualObject("Test");
	mPosition->attachObject(mSolid);
	loadLevel("level 1");
}
CheckPointManager::~CheckPointManager(void)
{
	resetLevel();
	delete pTargets;
	mPosition->detachAllObjects ();
	mPosition->removeAndDestroyAllChildren ();
	mSceneMgr->destroySceneNode (mName + "_node");
	mSceneMgr->destroyParticleSystem(mName + "_particlesystem");
}

void CheckPointManager::resetLevel()
{
	for(int i = 0; i < num_of_cp; i++)
	{
		delete cp[i];
	}
	pTargets->removeAllEmitters();
	done = true;
	num_of_cp = 0;
	counter = 0;
	mSolid->clear();
}

void CheckPointManager::CheckColission(Character *object)
{
	if(done) return;
	Vector3 vObject = object->getWorldPosition();
	Vector3 position = mPosition->getPosition();
	Vector3 temp;
	float distance;

	temp = vObject - cp[counter]->getPosition() - position;
	distance = temp.length();
	if(distance <= 200)
	{
		CPState state = DONE;
		cp[counter++]->setState(state);
		if(counter >= num_of_cp)
		{
			done = true;
			for(int i = 0; i < num_of_cp; i++)
			{
				state = INACTIVE;
				cp[i]->setState(state);
			}
			return;
		}
		state = CURRENT;
		cp[counter]->setState(state);
	}
}

void CheckPointManager::AddCheckPoint(Vector3 position, CPState state)
{
	cp[num_of_cp++] = new CheckPoint( mName + "_Emitter_" + StringConverter::toString(num_of_cp), mSceneMgr, state, pTargets, position);
	pTargets->setParticleQuota(num_of_cp * MAX_PARTICLES_PER_CHECKPOINT);
}

void CheckPointManager::loadLevel(String level)
{
	resetLevel();
	if( level == "random" )
	{
		CPState status = CURRENT;
		AddCheckPoint(Vector3(0,0,1000), status);
		status = TODO;
		for( int i = 1; i < 15; i++ ) // generate 3 CPs
			AddCheckPoint(Vector3(Math::RangeRandom (-600, 600),Math::RangeRandom(-600, 600),Math::RangeRandom (0000, 15000)), status);
		done = false;

	}
	else if( level == "level 1" )
	{
		CPState status = CURRENT;
		AddCheckPoint(Vector3(0,0,3000), status);
		status = TODO;
		AddCheckPoint(Vector3(3000,0,3000), status);
		AddCheckPoint(Vector3(3000,0,0), status);
		AddCheckPoint(Vector3(0,0,0), status);
		done = false;

		mSolid->begin("BaseWhiteNoLighting",RenderOperation::OT_LINE_STRIP); //BaseWhiteNoLighting"); //Ogre/Compositor/OldTV");
		mSolid->position(0,0,3000);
		mSolid->colour(0,0,1);
		mSolid->position(3000,0,3000);
		mSolid->colour(1,0,1);
		mSolid->position(3000,0,0);
		mSolid->colour(1,0,0);
		mSolid->position(0,0,0);
		mSolid->colour(0,0,0);
		mSolid->position(0,0,3000);
		mSolid->colour(0,0,1);
		mSolid->end();
	}
	else if( level == "level 2" )
	{
		CPState status = CURRENT;
		AddCheckPoint(Vector3(0,0,3000), status);
		status = TODO;
		AddCheckPoint(Vector3(0,3000,3000), status);
		AddCheckPoint(Vector3(3000,3000,3000), status);
		AddCheckPoint(Vector3(3000,0000,3000), status);
		AddCheckPoint(Vector3(3000,0,0), status);
		AddCheckPoint(Vector3(3000,3000,0), status);
		AddCheckPoint(Vector3(0,3000,0), status);
		AddCheckPoint(Vector3(0,0,0), status);
		done = false;

		mSolid->begin("BaseWhiteNoLighting",RenderOperation::OT_LINE_LIST); //BaseWhiteNoLighting"); //Ogre/Compositor/OldTV");
		mSolid->position(0,0,0);
		mSolid->colour(0,0,0);
		mSolid->position(0,0,3000);
		mSolid->colour(0,0,1);
		mSolid->position(3000,0,3000);
		mSolid->colour(1,0,1);
		mSolid->position(3000,0,0);
		mSolid->colour(1,0,0);
		mSolid->position(0,3000,0);
		mSolid->colour(0,1,0);
		mSolid->position(0,3000,3000);
		mSolid->colour(0,1,1);
		mSolid->position(3000,3000,3000);
		mSolid->colour(1,1,1);
		mSolid->position(3000,3000,0);
		mSolid->colour(1,1,0);

		mSolid->index(0);
		mSolid->index(1);
		mSolid->index(0);
		mSolid->index(3);
		mSolid->index(0);
		mSolid->index(4);
		mSolid->index(1);
		mSolid->index(2);
		mSolid->index(1);
		mSolid->index(5);
		mSolid->index(2);
		mSolid->index(3);
		mSolid->index(2);
		mSolid->index(6);
		mSolid->index(3);
		mSolid->index(7);
		mSolid->index(4);
		mSolid->index(5);
		mSolid->index(4);
		mSolid->index(7);
		mSolid->index(5);
		mSolid->index(6);
		mSolid->index(6);
		mSolid->index(7);
		

		mSolid->end();
	}
	else if( level == "level 3" )
	{
		CPState status = CURRENT;
		AddCheckPoint(Vector3(0,0,3000), status);
		status = TODO;
		AddCheckPoint(Vector3(0,3000,0), status);
		AddCheckPoint(Vector3(3000,0,0), status);
		AddCheckPoint(Vector3(-3000,0,0), status);
		AddCheckPoint(Vector3(0,-3000,0), status);
		AddCheckPoint(Vector3(0,0,-3000), status);
		done = false;

		mSolid->begin("BaseWhiteNoLighting",RenderOperation::OT_LINE_LIST); //BaseWhiteNoLighting"); //Ogre/Compositor/OldTV");
		mSolid->position(0,0,3000);
		mSolid->colour(0,0,1);
		mSolid->position(0,3000,0);
		mSolid->colour(0,1,0);
		mSolid->position(3000,0,0);
		mSolid->colour(1,0,0);
		mSolid->position(-3000,0,0);
		mSolid->colour(0,1,1);
		mSolid->position(0,-3000,0);
		mSolid->colour(1,0,1);
		mSolid->position(0,0,-3000);
		mSolid->colour(1,1,0);

		mSolid->index(0);
		mSolid->index(1);
		mSolid->index(0);
		mSolid->index(2);
		mSolid->index(0);
		mSolid->index(3);
		mSolid->index(0);
		mSolid->index(4);
		mSolid->index(1);
		mSolid->index(2);
		mSolid->index(1);
		mSolid->index(3);
		mSolid->index(1);
		mSolid->index(5);
		mSolid->index(2);
		mSolid->index(4);
		mSolid->index(2);
		mSolid->index(5);
		mSolid->index(3);
		mSolid->index(4);
		mSolid->index(3);
		mSolid->index(5);
		mSolid->index(4);
		mSolid->index(5);
		

		mSolid->end();

	}
	else
	{
		CPState status = CURRENT;
		AddCheckPoint(Vector3(0,0,0), status);
		status = TODO;
		for( int i = 1; i < 15; i++ ) // generate 3 CPs
			AddCheckPoint(Vector3(Math::RangeRandom (-600, 600),Math::RangeRandom(-600, 600),Math::RangeRandom (0, 15000)), status);
		done = false;
	}
}
/**/