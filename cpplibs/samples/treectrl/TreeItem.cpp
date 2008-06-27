
/**
 * @file TreeItem.cpp
 * @brief Implementation of class TreeItem
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

#include "TreeItem.h"
#include "FontFactory.h"

using namespace std;
using namespace Imaging;

TreeItem::TreeItem()
    : m_bCollapsed(true)
{
}

TreeItem::~TreeItem()
{
    for (vector<TreeItem*>::iterator k = m_Children.begin();
         k != m_Children.end();
         ++k)
       delete *k;
}

void TreeItem::visibleChildren(int level, vector<pair<int, TreeItem*> >& result)
{
    if (!m_bCollapsed)
    {
       for (vector<TreeItem*>::iterator k = m_Children.begin();
            k != m_Children.end();
            ++k)
       {
          result.push_back(pair<int, TreeItem*>(level + 1, *k));
          (*k)->visibleChildren(level + 1, result);
       }
    }
}

bool TreeItem::collapsed()
{
    return m_bCollapsed;
}

void TreeItem::collapsed(bool c)
{
    m_bCollapsed = c;
}

void TreeItem::appendChild(TreeItem *child)
{
    m_Children.push_back(child);
}

TreeItem* TreeItem::childAt(int idx)
{
    return m_Children[idx];
}

//==========================================================================
//

TextTreeItem::TextTreeItem(const string& fontName)
    : m_pFont(FontFactory::getInstance()->getFont(fontName))
{
}

TextTreeItem::~TextTreeItem()
{
}

void TextTreeItem::draw(Drawable *drw)
{
    string txt = getText();
    int h = m_pFont->getTextHeight(txt);

    m_pFont->drawText(drw, txt, 2, (16 - h) / 2);
}

//==========================================================================
//

StaticTextTreeItem::StaticTextTreeItem(const string& text, const string& fontName)
    : TextTreeItem(fontName),
      m_Text(text)
{
}

StaticTextTreeItem::~StaticTextTreeItem()
{
}

string StaticTextTreeItem::getText()
{
    return m_Text;
}

static const char* _rcsid_TreeItem __attribute__((unused)) = "$Id$";
