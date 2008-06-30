/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * Copyright (c) 2005 Jesper Svennevid
 */

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <pspgu.h>
#include <pspgum.h>

#include "TreeCtrl.h"
#include "TreeItem.h"
#include "FontFactory.h"

#include "Task.h"
#include "TaskFile.h"

#include <libpsp2d/Controller.h>

using namespace PSP2D;
using namespace Imaging;

using namespace std;

PSP_MODULE_INFO("TreeCtrl Sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

//==========================================================================
// Task tree item

class TaskTreeItem : public TreeItem
{
   public:
      TaskTreeItem(Task*);
      virtual ~TaskTreeItem();

      void draw(Drawable*);

      static void loadImages();

      void markCompleted();

   protected:
      Task *m_pTask;
      Image* m_Led;

      void _updateLed();

      static Image* m_pGreenLed;
      static Image* m_pRedLed;
      static Image* m_pBlueLed;
      static Image* m_pOrangeLed;
      static Image* m_pGreyLed;
};

TaskTreeItem::TaskTreeItem(Task *pTask)
    : m_pTask(pTask),
      m_Led(0)
{
    _updateLed();
}

TaskTreeItem::~TaskTreeItem()
{
}

void TaskTreeItem::draw(Drawable *drw)
{
    Font *pFont = FontFactory::getInstance()->getFont("font_small");

    string txt = m_pTask->subject();
    int h = pFont->getTextHeight(txt);

    drw->blit(m_Led, 0, 0, 16, 16, 2, 0, true);
    pFont->drawText(drw, txt, 20, (16 - h) / 2);
}

void TaskTreeItem::_updateLed()
{
    if (m_pTask->completed())
       m_Led = m_pGreenLed;
    else if (m_pTask->overdue())
       m_Led = m_pRedLed;
    else if (m_pTask->dueToday())
       m_Led = m_pOrangeLed;
    else if (m_pTask->inactive())
       m_Led = m_pGreyLed;
    else
       m_Led = m_pBlueLed;
}

void TaskTreeItem::markCompleted()
{
    m_pTask->markCompleted();
    _updateLed();

    for (vector<TreeItem*>::iterator k = m_Children.begin();
         k!= m_Children.end();
         ++k)
    {
       ((TaskTreeItem*)(*k))->_updateLed();
    }
}

void TaskTreeItem::loadImages()
{
    m_pGreenLed = new Image("ledgreen.png");
    m_pRedLed = new Image("ledred.png");
    m_pBlueLed = new Image("ledblue.png");
    m_pOrangeLed = new Image("ledorange.png");
    m_pGreyLed = new Image("ledgrey.png");
}

Image* TaskTreeItem::m_pGreenLed = 0;
Image* TaskTreeItem::m_pRedLed = 0;
Image* TaskTreeItem::m_pBlueLed = 0;
Image* TaskTreeItem::m_pOrangeLed = 0;
Image* TaskTreeItem::m_pGreyLed = 0;

//==========================================================================

static int exitRequest = 0;

int running()
{
	return !exitRequest;
}

int exitCallback(int arg1, int arg2, void *common)
{
	exitRequest = 1;
	return 0;
}

int callbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}

int setupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

TreeItem* ItemFromTask(Task *pTask)
{
    TreeItem *item = new TaskTreeItem(pTask);

    for (list<Task*>::iterator k = pTask->children().begin();
         k != pTask->children().end();
         ++k)
    {
       item->appendChild(ItemFromTask(*k));
    }

    return item;
}

int main(int argc, char* argv[])
{
	setupCallbacks();

	sceGuInit();

        TaskTreeItem::loadImages();

        vector<Task*> tasks = TaskFile("tasks").read();

        TreeCtrl *tree = new TreeCtrl();

        for (vector<Task*>::iterator k = tasks.begin();
             k != tasks.end();
             ++k)
        {
           tree->append(0, ItemFromTask(*k));
        }

        while (true)
        {
           Controller ctrl;

           if (ctrl.circle())
              break;

           if (ctrl.right())
              tree->collapse(false);

           if (ctrl.left())
              tree->collapse(true);

           if (ctrl.down())
           {
              tree->selectNext();

              sceKernelDelayThreadCB(100000);
           }

           if (ctrl.up())
           {
              tree->selectPrev();

              sceKernelDelayThreadCB(100000);
           }

           if (ctrl.cross())
           {
              ((TaskTreeItem*)tree->selection())->markCompleted();
              tree->redraw();
           }
        }

        delete tree;

        FILE *fp = fopen("tasks", "w");

        for (vector<Task*>::iterator k = tasks.begin();
             k != tasks.end();
             ++k)
           (*k)->write(fp, 0);

        fclose(fp);

	sceGuTerm();

	sceKernelExitGame();
	return 0;
}
