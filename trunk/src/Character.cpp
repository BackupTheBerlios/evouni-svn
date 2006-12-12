#include "character.h"
#include "EUClient.h"

Asteroid::Asteroid(String name, SceneManager* sceneMgr, Vector3 position, Vector3 velocity)
:Character(name, sceneMgr, position, velocity)
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

OgreCharacter::OgreCharacter(String name, SceneManager *sceneMgr, Vector3 position, Vector3 velocity)
:Character(name, sceneMgr, position, velocity)
{
	mSpeed = 0;
	mSceneMgr = sceneMgr;
	// Give this character a shape :)
	mEntity = mSceneMgr->createEntity (mName, "ogrehead.mesh");
	mMainNode->attachObject(mEntity);
	
	
/*	mEntity = mSceneMgr->createEntity (mName, "atlantis.mesh");
	SceneNode* temp = mMainNode->createChildSceneNode(Vector3(0,0,0),Quaternion(0,0,1,0));
	temp->attachObject (mEntity);
	mEntity->setMaterialName("EU/Atlantis"); 
/**/

	//Partikel
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
