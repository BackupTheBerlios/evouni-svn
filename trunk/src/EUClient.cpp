#include "EUClient.h"



EUClient::EUClient()
{
	mGuiFrameListener = 0;
	mRoot = 0;
}

EUClient::~EUClient()
{
	if(mEditorGuiSheet)
	{
		CEGUI::WindowManager::getSingleton().destroyWindow(mEditorGuiSheet);
	}
	if(mGUISystem)
	{
		delete mGUISystem;
		mGUISystem = 0;
	}
	if(mGUIRenderer)
	{
		delete mGUIRenderer;
		mGUIRenderer = 0;
	}
	if (mGuiFrameListener)
		delete mGuiFrameListener;
	if (mRoot)
		delete mRoot;
}


bool EUClient::setup(void)
{
	mRoot = new Root();
	setupResources();

	bool carryOn = configure();
	if (!carryOn) return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();

	// Create the scene
	createGUI();
	createScene();

	createFrameListener();

	return true;
}
    /** Configures the application - returns false if the user chooses to abandon configuration. */
bool EUClient::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = mRoot->initialise(true);
		return true;
	}
	else
	{
		return false;
	}
}

void EUClient::chooseSceneManager(void)
{
	// Create the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance");
	//mSceneMgr = mRoot->createSceneManager(ST_EXTERIOR_CLOSE);
}

void EUClient::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	// Position it at 500 in Z direction
	mCamera->setPosition(Vector3(0,0,500));
	// Look back along -Z
	mCamera->lookAt(Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);
	mCamera->setFarClipDistance(500000);
}

void EUClient::createFrameListener(void)
{
//	mFrameListener= new ExampleFrameListener(mWindow, mCamera);
//	mFrameListener->showDebugOverlay(true);
//	mRoot->addFrameListener(mFrameListener);
	mEUFrameListener = new SampleListener (mWindow, mCamera);
	static_cast<SampleListener *>(mEUFrameListener)->setCharacter (ogre);
	static_cast<SampleListener *>(mEUFrameListener)->setExtendedCamera (cam);
	mRoot->addFrameListener(mEUFrameListener);

	LogManager::getSingletonPtr()->logMessage("adding PhysikListener()");
    mGuiFrameListener = new GuiFrameListener(mWindow, mCamera, mGUIRenderer);
    mRoot->addFrameListener(mGuiFrameListener);
	ARPhysikListener* mBubu = new ARPhysikListener(mWindow, mCamera);
	mRoot->addFrameListener(mBubu);
	mBubu->addObject(ogre);
	Asteroid* astrid_die_asteroidin;
	for(int i = 0; i < 100; i++)
	{
		astrid_die_asteroidin = new Asteroid("Asteroid" + StringConverter::toString(i), mSceneMgr);
		mBubu->addObject(astrid_die_asteroidin);
	}
}

void EUClient::destroyScene(void){}    // Optional to override this

void EUClient::createViewports(void)
{
	// Create one viewport, entire window
	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
	Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

/// Method which will define the source of resources (other than current folder)
void EUClient::setupResources(void)
{
	// Load resource paths from config file
	ConfigFile cf;
	cf.load("resources.cfg");

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
			archName, typeName, secName);
		}
	}
}

/// Optional override method where you can create resource listeners (e.g. for loading screens)
void EUClient::createResourceListener(void)
{

}

/// Optional override method where you can perform resource group loading
/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
void EUClient::loadResources(void)
{
	// Initialise, parse scripts etc
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void EUClient::createGUI()
{
		mGUIRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
		mGUISystem = new CEGUI::System(mGUIRenderer);
		CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

		CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
		mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
		CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseMoveCursor");
		mGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");

		mEditorGuiSheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"Tutorial Gui.xml");
		mGUISystem->setGUISheet(mEditorGuiSheet);
		CEGUI::PushButton* quitButton = (CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Quit");

		setupEventHandlers();
}

void EUClient::createScene(void)
{

	mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));
    //mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
    mSceneMgr->setSkyBox( true, "Examples/SpaceSkyBox" );
	mCamera->setPosition (0, 0, 0);	// Required or else the camera will have an offset


	LogManager::getSingletonPtr()->logMessage("adding Character...");
	ogre = new OgreCharacter ("Ogre 1", mSceneMgr,Vector3(1000,1000,-1000));
	cam = new EUCamera("EUCamera/woot/woot", mSceneMgr, mCamera, ogre->getMainNode());
	CPM = new CheckPointManager("TestCPM", mSceneMgr, Vector3(10000,0,0));


	SceneNode *mNode;
	Entity *mEntity;
	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Sun");
	mEntity = mSceneMgr->createEntity("sun", "iss.mesh");
	mNode->attachObject (mEntity);
	mNode->scale(1000,1000,1000);
	mEntity->setMaterialName("wurmloch/BlackHole");

	
	//DEBUG
	//GRIDWÜRFEL
	ManualObject* manual = mSceneMgr->createManualObject("manual");
	manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);

	int Max = MAX_RAND;
	int Step = MAX_STEP;
	int j = Max;
    for( int i = -Max; i <= Max ; i += Step )
    {         
		//XY
		manual->position(i,-j,j);	manual->colour(ColourValue(0,0,1));
		manual->position(i,j,j);	manual->colour(ColourValue(0,0,1));
		manual->position(-j,i,j);	manual->colour(ColourValue(0,0,1));
		manual->position(j,i,j);	manual->colour(ColourValue(0,0,1));
			
		//XZ
		manual->position(i,j,-j);	manual->colour(ColourValue(0,1,0));
		manual->position(i,j,j);	manual->colour(ColourValue(0,1,0));
		manual->position(-j,j,i);	manual->colour(ColourValue(0,1,0));
		manual->position(j,j,i);	manual->colour(ColourValue(0,1,0));
        //YZ           
        manual->position(j,-j,i);	manual->colour(ColourValue(1,0,0));
		manual->position(j,j,i);	manual->colour(ColourValue(1,0,0));
        manual->position(j,i,-j);	manual->colour(ColourValue(1,0,0));
		manual->position(j,i,j);	manual->colour(ColourValue(1,0,0));

		//XY
		manual->position(i,-j,-j);	manual->colour(ColourValue(0,0,1));
		manual->position(i,j,-j);	manual->colour(ColourValue(0,0,1));
		manual->position(-j,i,-j);	manual->colour(ColourValue(0,0,1));
		manual->position(j,i,-j);	manual->colour(ColourValue(0,0,1));
			
		//XZ
		manual->position(i,-j,-j);	manual->colour(ColourValue(0,1,0));
		manual->position(i,-j,j);	manual->colour(ColourValue(0,1,0));
		manual->position(-j,-j,i);	manual->colour(ColourValue(0,1,0));
		manual->position(j,-j,i);	manual->colour(ColourValue(0,1,0));

		//YZ           
        manual->position(-j,-j,i);	manual->colour(ColourValue(1,0,0));
		manual->position(-j,j,i);	manual->colour(ColourValue(1,0,0));
        manual->position(-j,i,-j);	manual->colour(ColourValue(1,0,0));
		manual->position(-j,i,j);	manual->colour(ColourValue(1,0,0));
	}

	manual->position(0,0,-j);	manual->colour(ColourValue(0,0,1));
	manual->position(0,0,j);	manual->colour(ColourValue(0,0,1));
	manual->position(0,-j,0);	manual->colour(ColourValue(0,1,0));
	manual->position(0,j,0);	manual->colour(ColourValue(0,1,0));
	manual->position(-j,0,0);	manual->colour(ColourValue(1,0,0));
	manual->position(j,0,0);	manual->colour(ColourValue(1,0,0));

	manual->end();
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(manual);

}
	/*

	//EUClient Playground
		mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));
        //mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
        mSceneMgr->setSkyBox( true, "Examples/SpaceSkyBox" );

		mCamera->setPosition (0, 0, 0);	// Required or else the camera will have an offset
		
		
		//Random Stuff
		SceneNode *starfieldNode;
		SceneNode *sunNode;
		SceneNode *mNode;
		Entity *mEntity;

		starfieldNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("starfield", Vector3(0,0,30000));
		sunNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("sun");

	    float const radius = 20000;
		float const accuracy = 1000;

		int point_index = 0;

	    for(float theta = 0; theta <= 2 * Math::PI; theta += Math::PI / (accuracy / 2)) {
			mNode = sunNode->createChildSceneNode ("asteroid" + StringConverter::toString (point_index), Vector3 (radius * cos(theta) + Math::RangeRandom (-1000, 1000), Math::RangeRandom (-1000, 1000), radius * sin(theta) + Math::RangeRandom (-1000, 1000)));
			mEntity = mSceneMgr->createEntity ("asteroid" + StringConverter::toString (point_index), "sphere.mesh");
			mNode->attachObject (mEntity);
			mEntity->setMaterialName("EU/Earth");
			point_index++;
		}


		for (unsigned int i = 0; i < 100; ++i) {
			mNode = starfieldNode->createChildSceneNode (StringConverter::toString (i), Vector3 (Math::RangeRandom (-10000, 10000), Math::RangeRandom (-10000, 10000), Math::RangeRandom (-10000, 10000)));
			mEntity = mSceneMgr->createEntity (StringConverter::toString (i), "razor.mesh");
			mNode->attachObject (mEntity);
		}
		for (unsigned int i = 0; i < 100; ++i) {
			mNode = starfieldNode->createChildSceneNode (StringConverter::toString (i+100), Vector3 (Math::RangeRandom (-10000, 10000), Math::RangeRandom (-10000, 10000), Math::RangeRandom (-10000, 10000)));
			mEntity = mSceneMgr->createEntity (StringConverter::toString (i+100), "sphere.mesh");
			mNode->attachObject (mEntity);
		}
		mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Sun");
		mEntity = mSceneMgr->createEntity("sun", "iss.mesh");
		mNode->attachObject (mEntity);
		mNode->scale(1000,1000,1000);
		mEntity->setMaterialName("EU/Earth");

		CPM = new CheckPointManager("TestCPM", mSceneMgr, Vector3(10000,0,0));
		ogre = new OgreCharacter ("Ogre 1", mSceneMgr,Vector3(1000,3000,-3000));
		cam = new EUCamera("EUCamera/woot/woot", mSceneMgr, mCamera, ogre->getMainNode());

				//DEBUG
		//GRIDWÜRFEL
		ManualObject* manual = mSceneMgr->createManualObject("manual");
		manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);

		int Max = 5000;
		int Step = 1000;
		int j = Max;
        for( int i = -Max; i <= Max ; i += Step )
        {         
			//XY
			manual->position(i,-j,j);	manual->colour(ColourValue(0,0,1));
			manual->position(i,j,j);	manual->colour(ColourValue(0,0,1));
			manual->position(-j,i,j);	manual->colour(ColourValue(0,0,1));
			manual->position(j,i,j);	manual->colour(ColourValue(0,0,1));
			
			//XZ
			manual->position(i,j,-j);	manual->colour(ColourValue(0,1,0));
			manual->position(i,j,j);	manual->colour(ColourValue(0,1,0));
			manual->position(-j,j,i);	manual->colour(ColourValue(0,1,0));
			manual->position(j,j,i);	manual->colour(ColourValue(0,1,0));

            //YZ           
            manual->position(j,-j,i);	manual->colour(ColourValue(1,0,0));
			manual->position(j,j,i);	manual->colour(ColourValue(1,0,0));
            manual->position(j,i,-j);	manual->colour(ColourValue(1,0,0));
			manual->position(j,i,j);	manual->colour(ColourValue(1,0,0));

			//XY
			manual->position(i,-j,-j);	manual->colour(ColourValue(0,0,1));
			manual->position(i,j,-j);	manual->colour(ColourValue(0,0,1));
			manual->position(-j,i,-j);	manual->colour(ColourValue(0,0,1));
			manual->position(j,i,-j);	manual->colour(ColourValue(0,0,1));
			
			//XZ
			manual->position(i,-j,-j);	manual->colour(ColourValue(0,1,0));
			manual->position(i,-j,j);	manual->colour(ColourValue(0,1,0));
			manual->position(-j,-j,i);	manual->colour(ColourValue(0,1,0));
			manual->position(j,-j,i);	manual->colour(ColourValue(0,1,0));

            //YZ           
            manual->position(-j,-j,i);	manual->colour(ColourValue(1,0,0));
			manual->position(-j,j,i);	manual->colour(ColourValue(1,0,0));
            manual->position(-j,i,-j);	manual->colour(ColourValue(1,0,0));
			manual->position(-j,i,j);	manual->colour(ColourValue(1,0,0));
} 
			manual->position(0,0,-j);	manual->colour(ColourValue(0,0,1));
			manual->position(0,0,j);	manual->colour(ColourValue(0,0,1));
			manual->position(0,-j,0);	manual->colour(ColourValue(0,1,0));
			manual->position(0,j,0);	manual->colour(ColourValue(0,1,0));
			manual->position(-j,0,0);	manual->colour(ColourValue(1,0,0));
			manual->position(j,0,0);	manual->colour(ColourValue(1,0,0));


		manual->end();
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(manual);

}
		*/

/*
	    // Assuming scene_mgr is your SceneManager.
    ManualObject * circle = scene_mgr->createManualObject("circle_name");
    
    
    circle->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_STRIP);
    
    unsigned point_index = 0;
    for(float theta = 0; theta <= 2 * Math::PI; theta += Math::PI / (radius * accuracy)) {
        circle->position(radius * cos(theta), 0, radius * sin(theta));
        circle->index(point_index++);
    }
    circle->index(0); // Rejoins the last point to the first.
    
    circle->end();
    
    scene_mgr->getRootSceneNode()->createChildSceneNode()->attachObject(circle);


				//XY
			manual->position(i,-j,0);	manual->colour(ColourValue(0,0,1));
			manual->position(i,j,0);	manual->colour(ColourValue(0,0,1));
			manual->position(-j,i,0);	manual->colour(ColourValue(0,0,1));
			manual->position(j,i,0);	manual->colour(ColourValue(0,0,1));
			
			//XZ
			manual->position(i,0,-j);	manual->colour(ColourValue(0,1,0));
			manual->position(i,0,j);	manual->colour(ColourValue(0,1,0));
			manual->position(-j,0,i);	manual->colour(ColourValue(0,1,0));
			manual->position(j,0,i);	manual->colour(ColourValue(0,1,0));

            //YZ           
            manual->position(0,-j,i);	manual->colour(ColourValue(1,0,0));
			manual->position(0,j,i);	manual->colour(ColourValue(1,0,0));
            manual->position(0,i,-j);	manual->colour(ColourValue(1,0,0));
			manual->position(0,i,j);	manual->colour(ColourValue(1,0,0));

*/
