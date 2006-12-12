
/*
-----------------------------------------------------------------------------
Filename:    EUApplication.h
Description: Base class for the EU Application
-----------------------------------------------------------------------------
*/
#ifndef __EUApplication_H__
#define __EUApplication_H__

#include "OGREinclude.h"
//#include "ExampleApplication.h"

#include "Character.h"
#include "Camera.h"
#include "FrameListener.h"
#include "GUI.h"

//#include "CheckPoint.h"
#include "CheckPointManager.h"

#include "globals.h"

extern CheckPointManager *CPM;
extern OgreCharacter *ogre;
extern EUCamera *cam;


extern const int MAX_X_RAND;
extern const int MAX_Y_RAND;
extern const int MAX_Z_RAND;
extern const int MAX_RAND;
extern const int MAX_STEP;

class EUClient
{
private:
	//GUI
	CEGUI::OgreCEGUIRenderer* mGUIRenderer;
	CEGUI::System* mGUISystem;
	CEGUI::Window* mEditorGuiSheet;
protected:
	//Core-Variables
    Root *mRoot;
    Camera* mCamera;
    SceneManager* mSceneMgr;
    GuiFrameListener* mGuiFrameListener;
	SampleListener* mEUFrameListener;
    RenderWindow* mWindow;


public:
    /// Standard constructor
    EUClient();
    /// Standard destructor
    virtual ~EUClient();
	//Custom
	void setupEventHandlers(void)
	{
		CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
		wmgr.getWindow((CEGUI::utf8*)"Quit")->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EUClient::handleQuit, this));
	}
	//Custom
	bool handleQuit(const CEGUI::EventArgs& e)
	{
		static_cast<GuiFrameListener*>(mGuiFrameListener)->requestShutdown();
		return true;
	}

	

    /// Start the example
    virtual void go(void)
    {
        if (!setup())
            return;

        mRoot->startRendering();
		LogManager::getSingletonPtr()->logMessage("destroying Scene");
        // clean up
        destroyScene();
    }

protected:
    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
    virtual bool setup(void);
	virtual bool configure(void);
	virtual void chooseSceneManager(void);
	virtual void createCamera(void);
	virtual void createFrameListener(void);
	virtual void createGUI(void);
	virtual void createScene(void);    // pure virtual - this has to be overridden
    virtual void destroyScene(void);    // Optional to override this
    virtual void createViewports(void);
    virtual void setupResources(void);
	virtual void createResourceListener(void);
	virtual void loadResources(void);
};



/*
class SampleApplication : public ExampleApplication
{

protected:
public:
protected:
	// Just override the mandatory create scene method
	void createScene(void)
	{
		CPM = new CheckPointManager("TestCPM", mSceneMgr);
		ogre = new OgreCharacter ("Ogre 1", mSceneMgr);
		EUCamera *cam = new EUCamera("EUCamera/woot/woot", mSceneMgr, mCamera, ogre->getMainNode());
		mFrameListener = new SampleListener (mWindow, mCamera);
		static_cast<SampleListener *>(mFrameListener)->setCharacter (ogre);
		static_cast<SampleListener *>(mFrameListener)->setExtendedCamera (cam);
	}
	void destroyScene(void)
	{
	}
	void createFrameListener(void)
	{
		// This is where we instantiate our own frame listener
//		mFrameListener= new SampleListener(mWindow, mCamera);
		mRoot->addFrameListener(mFrameListener);
        mFrameListener = new GuiFrameListener(mWindow, mCamera, mGUIRenderer);
        mRoot->addFrameListener(mFrameListener);
	}
};
*/
#endif