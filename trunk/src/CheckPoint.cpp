#include "checkpoint.h"

CheckPoint::CheckPoint(String name, SceneManager *sceneMgr, CPState state, ParticleSystem* psystem, Vector3 position)
{
	mPosition = position;
	mName = name;
	mSceneMgr = sceneMgr;
	pSystem = psystem;
	mState = state;

	pEmitter = pSystem->addEmitter( "Ellipsoid" );
	pEmitter->setAngle( Degree(0) );
    pEmitter->setTimeToLive( 5 );
    pEmitter->setEmissionRate( 100 );

    pEmitter->setParticleVelocity( 0 );

	pEmitter->setDirection( -Vector3::UNIT_Z );

	setState(state);

	pEmitter->setPosition(position);

}
CheckPoint::~CheckPoint(void)
{
}

void CheckPoint::setState(CPState state)
{
	mState = state;
	switch(mState)
	{
	case TODO:
		pEmitter->setColour( ColourValue::Red, ColourValue::Red);
		pEmitter->setParameter("width", "350");
		pEmitter->setParameter("height", "350");
		pEmitter->setParameter("depth", "350");
		break;
	case CURRENT:
		pEmitter->setColour( ColourValue(1,1,0), ColourValue(1,1,0));
		pEmitter->setParameter("width", "350");
		pEmitter->setParameter("height", "350");
		pEmitter->setParameter("depth", "350");
		break;
	case DONE:
		pEmitter->setColour( ColourValue::Green, ColourValue::Green);
		pEmitter->setParameter("width", "250");
		pEmitter->setParameter("height", "250");
		pEmitter->setParameter("depth", "250");
		break;
	case INACTIVE:
		pEmitter->setColour( ColourValue::White, ColourValue::White);
		pEmitter->setParameter("width", "250");
		pEmitter->setParameter("height", "250");
		pEmitter->setParameter("depth", "250");
		break;
	}
}
inline CPState CheckPoint::getState()
{
	return mState;
}

