#ifndef GUI_H
#define GUI_H

//mem probs without this next one
#include <OgreNoMemoryMacros.h>
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"
//regular mem handler
#include <OgreMemoryMacros.h>
#include <CEGUI/elements/CEGUIPushButton.h>

#include "ExampleApplication.h"

CEGUI::MouseButton convertOgreButtonToCegui(int);

class GuiFrameListener : public ExampleFrameListener, public MouseMotionListener, public MouseListener
{
private:
   CEGUI::Renderer* mGUIRenderer;
   bool mShutdownRequested;

public:
   // NB using buffered input
   GuiFrameListener(RenderWindow* win, Camera* cam, CEGUI::Renderer* renderer)
       : ExampleFrameListener(win, cam, true, true), 
         mGUIRenderer(renderer),
         mShutdownRequested(false)
   {
       mEventProcessor->addMouseMotionListener(this);
       mEventProcessor->addMouseListener(this);
       mEventProcessor->addKeyListener(this);
   }

   // Tell the frame listener to exit at the end of the next frame
   void requestShutdown(void)
   {
       mShutdownRequested = true;
   }

   bool frameEnded(const FrameEvent& evt)
   {
       if (mShutdownRequested)
           return false;
       else
           return ExampleFrameListener::frameEnded(evt);
   }

   void mouseMoved (MouseEvent *e)
   {
       CEGUI::System::getSingleton().injectMouseMove(
               e->getRelX() * mGUIRenderer->getWidth(), 
               e->getRelY() * mGUIRenderer->getHeight());
       e->consume();
   }

   void mouseDragged (MouseEvent *e) 
   { 
       mouseMoved(e);
   }

   void mousePressed (MouseEvent *e)
   {
       CEGUI::System::getSingleton().injectMouseButtonDown(
         convertOgreButtonToCegui(e->getButtonID()));
       e->consume();
   }

   void mouseReleased (MouseEvent *e)
   {
       CEGUI::System::getSingleton().injectMouseButtonUp(
         convertOgreButtonToCegui(e->getButtonID()));
       e->consume();
   }

   void mouseClicked(MouseEvent* e) {}
   void mouseEntered(MouseEvent* e) {}
   void mouseExited(MouseEvent* e) {}

   void keyPressed(KeyEvent* e)
   {
       if(e->getKey() == KC_ESCAPE)
       {
           mShutdownRequested = true;
           e->consume();
           return;
       }

       CEGUI::System::getSingleton().injectKeyDown(e->getKey());
       CEGUI::System::getSingleton().injectChar(e->getKeyChar());
       e->consume();
   }

   void keyReleased(KeyEvent* e)
   {
       CEGUI::System::getSingleton().injectKeyUp(e->getKey());
       e->consume();
   }

   void keyClicked(KeyEvent* e) 
   {
       // Do nothing
       e->consume();
   }
};

#endif //GUI_H