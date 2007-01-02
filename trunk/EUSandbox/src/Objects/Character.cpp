#include "Objects/character.h"
#include "EUClient.h"

//todo

Asteroid::Asteroid(String name, SceneManager* sceneMgr, Vector3 position, Vector3 velocity, Vector3 acceleration)
:Character(name, sceneMgr, position, velocity, acceleration)
{
	mSceneMgr = sceneMgr;
	// Give this character a shape :)
	mEntity = mSceneMgr->createEntity (mName, "asteroid.mesh");
	mEntity->setMaterialName("asteroid");
	mMainNode->attachObject(mEntity);
	mMainNode->scale(10,10,10);

	mMainNode->setPosition(Vector3(Math::RangeRandom(-MAX_RAND,MAX_RAND),Math::RangeRandom(-MAX_RAND,MAX_RAND),Math::RangeRandom(-MAX_RAND,MAX_RAND)));
	mVelocity = Vector3(Math::RangeRandom(-5000,5000),Math::RangeRandom(-5000,5000),Math::RangeRandom(-5000,5000));
	mMainNode->scale(Math::RangeRandom(1,20),Math::RangeRandom(1,20),Math::RangeRandom(1,20));
	Quaternion q;
	q = q * Quaternion(Degree(Math::RangeRandom(-180,180)), Vector3::UNIT_X); //rotate by mAzimut on Y-Axis
	q = q * Quaternion(Degree(Math::RangeRandom(-180,180)), Vector3::UNIT_Y); //rotate by mAzimut on Y-Axis
	q = q * Quaternion(Degree(Math::RangeRandom(-180,180)), Vector3::UNIT_Z); //rotate by mAzimut on Y-Axis
	mMainNode->rotate(q);
}



OgreCharacter::OgreCharacter(String name, SceneManager *sceneMgr, Vector3 position, Vector3 velocity, Vector3 acceleration)
:Character(name, sceneMgr, position, velocity, acceleration)
{
	mSpeed = 0;
	mAcceleration = 0;
	mSceneMgr = sceneMgr;
	// Give this character a shape :)
	mEntity = mSceneMgr->createEntity (mName, "ogrehead.mesh");
	mMainNode->attachObject(mEntity);
	
	
/*	mEntity = mSceneMgr->createEntity (mName, "atlantis.mesh");
	SceneNode* temp = mMainNode->createChildSceneNode(Vector3(0,0,0),Quaternion(0,0,1,0));
	temp->attachObject (mEntity);
	mEntity->setMaterialName("EU/Atlantis"); 
/**/

	//Particle
	LogManager::getSingletonPtr()->logMessage("3");
    pThrusters = mSceneMgr->createParticleSystem( "ParticleSys1", 200000 );

    pThrusters ->setMaterialName( "Examples/Flare" );
    pThrusters ->setDefaultDimensions( 10, 10 );

    ParticleEmitter *pEmit1 = pThrusters ->addEmitter( "Point" );
    ParticleEmitter *pEmit2 = pThrusters ->addEmitter( "Point" );
    /*ParticleEmitter *pEmit3 = pThrusters ->addEmitter( "HollowEllipsoid" );


	pEmit3->setParameter("width", "1000");
	pEmit3->setParameter("height", "1000");
	pEmit3->setParameter("depth", "1000");
	pEmit3->setEmissionRate( 1000 );
	pEmit3->setTimeToLive(5);/**/
    // Thruster 1
    pEmit1->setAngle( Degree(3) );
    pEmit1->setTimeToLive( 0.3 );
    pEmit1->setEmissionRate( 100 );

    pEmit1->setParticleVelocity( 0 );

    pEmit1->setDirection( -Vector3::UNIT_Z);
    pEmit1->setColour( ColourValue::Red, ColourValue(1,0.5,0) );        
	
	// Thruster 2
    pEmit2->setAngle( Degree(3) );
    pEmit2->setTimeToLive( 0.3 );
    pEmit2->setEmissionRate( 100 );

    pEmit2->setParticleVelocity( 0 );
    pEmit2->setDirection( -Vector3::UNIT_Z );
    pEmit2->setColour( ColourValue::Red, ColourValue(1,0.5,0) );

    // Set the position of the thrusters
    pEmit1->setPosition( Vector3( 5.7f, 0.0f, 0.0f ) );
    pEmit2->setPosition( Vector3( -18.0f, 0.0f, 0.0f ) );

	LogManager::getSingletonPtr()->logMessage("4");

    mMainNode->createChildSceneNode( Vector3( 0.0f, 6.5f, -67.0f ) )->attachObject(pThrusters);

	LogManager::getSingletonPtr()->logMessage("OGREChar done");
}

OgreCharacter::~OgreCharacter () {
	mMainNode->detachAllObjects ();
	delete mEntity;
	mMainNode->removeAndDestroyAllChildren ();
	mSceneMgr->destroySceneNode (mName);
}


void OgreCharacter::update (Real elapsedTime, InputReader *input) {
	// Handle movement
	if (input->isKeyDown (KC_UP)) {
		mAcceleration += 1 * elapsedTime;				
	}
	if (input->isKeyDown (KC_DOWN)) {
		mAcceleration -= 1 * elapsedTime;				
	}
	if (input->isKeyDown (KC_W)) {
		mMainNode->pitch (Radian (elapsedTime));
	}
	if (input->isKeyDown (KC_S)) {
		mMainNode->pitch (Radian (-elapsedTime));
	}
	if (input->isKeyDown (KC_Q)) {
		mMainNode->yaw (Radian (elapsedTime));
	}
	if (input->isKeyDown (KC_E)) {
		mMainNode->yaw (Radian (-elapsedTime));
	}
	if (input->isKeyDown (KC_A)) {
		mMainNode->roll (Radian (-elapsedTime));
	}
	if (input->isKeyDown (KC_D)) {
		mMainNode->roll (Radian (elapsedTime));
	}
	if (input->isKeyDown (KC_SPACE))
	{
		mAcceleration = 0;
	}
	if(mAcceleration<0) {
		mAcceleration=0;
	}
	if(mAcceleration>10) {
		mAcceleration=10;
	}
	pThrusters->setDefaultDimensions(mAcceleration/10, mAcceleration/10);
	mMainNode->yaw( Degree(-50 * input->getMouseRelativeX() * elapsedTime) );
	//mCamNode->getChild( 0 )->pitch( Degree(-mRotate * mInputDevice->getMouseRelativeY()) );
	mMainNode->pitch( Degree(50 * input->getMouseRelativeY() * elapsedTime) );
	
	mSpeed += ( mMainNode->getOrientation() * Vector3(0,0,mAcceleration) ) * elapsedTime;
	mMainNode->translate (mSpeed * elapsedTime);
}

/** Arcade Mode vehicle physics
		void update (Real elapsedTime, InputReader *input) {
			// Handle movement
			if (input->isKeyDown (KC_UP)) {
				mAcceleration += 0.01;				
			}
			if (input->isKeyDown (KC_DOWN)) {
				mAcceleration -= 0.01;				
			}
			if (input->isKeyDown (KC_W)) {
				mMainNode->pitch (Radian (elapsedTime));
			}
			if (input->isKeyDown (KC_S)) {
				mMainNode->pitch (Radian (-elapsedTime));
			}
			if (input->isKeyDown (KC_Q)) {
				mMainNode->yaw (Radian (elapsedTime));
			}
			if (input->isKeyDown (KC_E)) {
				mMainNode->yaw (Radian (-elapsedTime));
			}
			if (input->isKeyDown (KC_A)) {
				mMainNode->roll (Radian (-elapsedTime));
			}
			if (input->isKeyDown (KC_D)) {
				mMainNode->roll (Radian (elapsedTime));
			}
			if (input->isKeyDown (KC_SPACE))
			{
				mAcceleration = 0;
			}
			if(mAcceleration<0) {
				mAcceleration=0;
			}
			if(mAcceleration>0.1) {
				mAcceleration=0.1;
			}
			pThrusters->setDefaultDimensions(mSpeed, mSpeed);
			mMainNode->yaw( Degree(-50 * input->getMouseRelativeX() * elapsedTime) );
			//mCamNode->getChild( 0 )->pitch( Degree(-mRotate * mInputDevice->getMouseRelativeY()) );
			mMainNode->pitch( Degree(50 * input->getMouseRelativeY() * elapsedTime) );
			
			mSpeed+=mAcceleration*elapsedTime;
			mMainNode->translate (mMainNode->getOrientation () * Vector3 (0, 0, mSpeed));
		}
*/