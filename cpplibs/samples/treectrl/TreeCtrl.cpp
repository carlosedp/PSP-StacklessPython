
/**
 * @file TreeCtrl.cpp
 * @brief Implementation of class TreeCtrl
 */

/**********************************************************************

  Created: 13 May 2008

    Copyright (C) 2008 Frank Buss, Jérôme Laheurte

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The names of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**********************************************************************/
// $Id$

#include "TreeCtrl.h"
#include "TreeItem.h"

#include <imaging/Screen.h>

using namespace std;
using namespace Imaging;

class RootItem : public TreeItem
{
   public:
      virtual ~RootItem() {};

      void draw(Drawable*) {};
};

TreeCtrl::TreeCtrl()
    : m_pRoot(new RootItem()),
      m_bFrozen(false),
      m_iFirst(0),
      m_pArrow1(new Image("arrow1.png")),
      m_pArrow2(new Image("arrow2.png")),
      m_pSelected(0),
      m_pSelect(new Image(480, 16))
{
    m_pRoot->collapsed(false);

    m_pSelect->clear(0x80FF0000U);
}

TreeCtrl::~TreeCtrl()
{
    delete m_pRoot;
    delete m_pArrow1;
    delete m_pArrow2;
    delete m_pSelect;
}

void TreeCtrl::freeze()
{
    m_bFrozen = true;
}

void TreeCtrl::thaw()
{
    m_bFrozen = false;
    redraw();
}

TreeItem* TreeCtrl::append(TreeItem *parent, TreeItem *item)
{
    if (parent)
    {
       parent->appendChild(item);
    }
    else
    {
       m_pRoot->appendChild(item);
    }

    if (!m_pSelected)
       m_pSelected = item;

    if (!m_bFrozen)
       redraw();

    return item;
}

void TreeCtrl::selectNext()
{
    vector<pair<int, TreeItem*> > children;
    m_pRoot->visibleChildren(0, children);
    unsigned int idx = 0;

    for (vector<pair<int, TreeItem*> >::iterator k = children.begin() + m_iFirst;
         k != children.end();
         ++k)
    {
       if (k->second == m_pSelected)
       {
          if (idx + 1 >= children.size())
          {
             m_pSelected = m_pRoot->childAt(0);
             m_iFirst = 0;
          }
          else
          {
             m_pSelected = children[idx + 1].second;
             if (idx + 1 >= 17)
                ++m_iFirst;
          }

          break;
       }

       ++idx;
    }

    if (!m_bFrozen)
       redraw();
}

void TreeCtrl::selectPrev()
{
    vector<pair<int, TreeItem*> > children;
    m_pRoot->visibleChildren(0, children);
    unsigned int idx = 0;

    for (vector<pair<int, TreeItem*> >::iterator k = children.begin() + m_iFirst;
         k != children.end();
         ++k)
    {
       if (k->second == m_pSelected)
       {
          if (idx == 0)
          {
             vector<pair<int, TreeItem*> > allChildren;
             m_pRoot->visibleChildren(0, allChildren);

             if (m_iFirst == 0)
             {
                m_pSelected = allChildren[allChildren.size() - 1].second;
                if (allChildren.size() >= 17)
                   m_iFirst = allChildren.size() - 17;
                else
                   m_iFirst = 0;
             }
             else
             {
                --m_iFirst;
                m_pSelected = allChildren[m_iFirst].second;
             }
          }
          else
          {
             m_pSelected = children[idx - 1].second;
          }

          break;
       }

       ++idx;
    }

    if (!m_bFrozen)
       redraw();
}

void TreeCtrl::redraw()
{
    Screen *screen = Screen::getInstance();

    screen->clear(0x00FFFFFFU);

    vector<pair<int, TreeItem*> > children;
    m_pRoot->visibleChildren(0, children);

    int idx = 0;
    for (vector<pair<int, TreeItem*> >::iterator k = children.begin() + m_iFirst;
         k != children.end();
         ++k)
    {
       Image *drw = new Image(480 - (16 * k->first), 16);

       drw->clear(0x00FFFFFFU);

       k->second->draw(drw);
       screen->blit(drw, 0, 0, (480 - (16 * k->first)), 16, 16 * k->first, 16 * idx, true);

       if (k->second->hasChildren())
       {
          if (k->second->collapsed())
          {
             screen->blit(m_pArrow1, 0, 0, 16, 16, 16 * (k->first - 1), idx * 16, true);
          }
          else
          {
             screen->blit(m_pArrow2, 0, 0, 16, 16, 16 * (k->first - 1), idx * 16, true);
          }
       }

       if (k->second == m_pSelected)
       {
          screen->blit(m_pSelect, 0, 0, 480, 16, 0, idx * 16, true);
       }

       delete drw;
       ++idx;
    }

    screen->flip();
}

void TreeCtrl::collapse(bool c)
{
    m_pSelected->collapsed(c);

    if (!m_bFrozen)
       redraw();
}

TreeItem* TreeCtrl::selection()
{
    return m_pSelected;
}

static const char* _rcsid_TreeCtrl __attribute__((unused)) = "$Id$";
