
/**
 * @file TreeItem.h
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

#ifndef _TREEITEM_H
#define _TREEITEM_H

#include <vector>
#include <string>

#include <imaging/Drawable.h>
#include <imaging/Font.h>

class TreeItem
{
  public:
    TreeItem();
    virtual ~TreeItem();

    virtual void draw(Imaging::Drawable*) = 0;

    void visibleChildren(int, std::vector<std::pair<int, TreeItem*> >&);

    bool collapsed();
    void collapsed(bool);

    void appendChild(TreeItem*);
    TreeItem* childAt(int);

    bool hasChildren()
    {
       return !m_Children.empty();
    }

  protected:
    bool m_bCollapsed;
    std::vector<TreeItem*> m_Children;
};

class TextTreeItem : public TreeItem
{
  public:
    TextTreeItem(const std::string& fontName = "font_small");
    virtual ~TextTreeItem();

    void draw(Imaging::Drawable*);

    virtual std::string getText() = 0;

  protected:
    Imaging::Font* m_pFont;
};

class StaticTextTreeItem : public TextTreeItem
{
  public:
    StaticTextTreeItem(const std::string& text, const std::string& fontName = "font_small");
    virtual ~StaticTextTreeItem();

    std::string getText();

  protected:
    std::string m_Text;
};

#endif /* _TREEITEM_H */
