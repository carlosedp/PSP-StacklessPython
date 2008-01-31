
/**
 * @file menu.cpp
 * @brief Implementation of class menu
 */

/**********************************************************************

  Created: 26 Nov 2005

**********************************************************************/
// $Id: menu.cpp 3 2006-03-12 15:13:39Z fraca7 $

#include <sstream>

#include "menu.h"

using namespace std;
using namespace PSP2D;

#define MAX(x, y) ((x) < (y) ? (y) : (x))

MenuItem::MenuItem(const string& name)
    : _name(name)
{
}

MenuItem::~MenuItem()
{
}

void MenuItem::onLeft()
{
}

void MenuItem::onRight()
{
}

//==========================================================================

Menu::Menu(u16 x, u16 y)
    : _x(x), _y(y), _w(0), _h(0), _bg(NULL), _abg(NULL), _current(0)
{
}

Menu::~Menu()
{
    for (vector<MenuItem*>::iterator k = _items.begin();
         k != _items.end();
         ++k)
    {
       delete *k;
    }

    delete _bg;
    delete _abg;
}

void Menu::addItem(MenuItem *item)
{
    _items.push_back(item);
}

void Menu::realize(Font *font)
{
    _w = _h = 0;

    for (vector<MenuItem*>::iterator k = _items.begin();
         k != _items.end();
         ++k)
    {
       u16 w, h;

       (*k)->getDimension(font, w, h);

       w += 20;
       h += 10;

       _w = MAX(_w, w);
       _h = MAX(_h, h);
    }

    _abg = new Image(_w, _h);
    _abg->clear(0xc8ff0000U);

    _bg = new Image(_w, _h);
    _bg->clear(0x64ff0000U);
}

void Menu::draw(Font *font, Drawable *drw)
{
    u16 cy = _y;
    int idx = 0;

    for (vector<MenuItem*>::iterator k = _items.begin();
         k != _items.end();
         ++k, ++idx)
    {
       Image *bg = (idx == _current) ? _abg : _bg;

       drw->blit(bg, 0, 0, _w, _h, _x, cy, true);
       (*k)->draw(font, drw, _x, cy, _w, _h);
       cy += _h;
    }
}

void Menu::update(Controller *pad)
{
    if (*pad == _old)
       return;

    _old = *pad;

    if (pad->left())
       _items[_current]->onLeft();

    if (pad->right())
       _items[_current]->onRight();

    if (pad->up())
       _current = (_current + _items.size() - 1) % _items.size();

    if (pad->down())
       _current = (_current + 1) % _items.size();
}

//==========================================================================

FloatMenuItem::FloatMenuItem(const string& name, float value, float step)
    : MenuItem(name),
      _value(value),
      _step(step)
{
}

FloatMenuItem::~FloatMenuItem()
{
}

float FloatMenuItem::getValue()
{
    return _value;
}

void FloatMenuItem::getDimension(Font *font, u16& w, u16& h)
{
    stringstream s;

    s << _name << ": " << (int)(_value * 100) * .01;

    w = font->getTextWidth(s.str());
    h = font->getTextHeight(s.str());
}

void FloatMenuItem::draw(Font *font, Drawable *drw, u16 x, u16 y, u16 w, u16 h)
{
    stringstream s;
    u16 mw, mh;

    s << _name << ": " << (int)(_value * 100) * .01;

    mw = font->getTextWidth(s.str());
    mh = font->getTextHeight(s.str());

    font->drawText(drw,
                   s.str(),
                   x + (w - mw)/2,
                   y + (h - mh)/2);
}

void FloatMenuItem::onLeft()
{
    _value -= _step;
}

void FloatMenuItem::onRight()
{
    _value += _step;
}

static const char* _rcsid_menu __attribute__((unused)) = "$Id: menu.cpp 3 2006-03-12 15:13:39Z fraca7 $";
