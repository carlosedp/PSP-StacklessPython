
/**
 * @file menu.h
 */

/**********************************************************************

  Created: 26 Nov 2005

**********************************************************************/
// $Id$

#ifndef _MENU_H
#define _MENU_H

#include <string>
#include <vector>

#include <libpsp2d/Font.h>
#include <libpsp2d/Drawable.h>
#include <libpsp2d/Image.h>
#include <libpsp2d/Controller.h>

class MenuItem
{
  public:
    MenuItem(const std::string& name);
    virtual ~MenuItem();

    virtual void getDimension(PSP2D::Font *font, u16& w, u16& h) = 0;
    virtual void draw(PSP2D::Font *font, PSP2D::Drawable *drw, u16 x, u16 y, u16 w, u16 h) = 0;
    virtual void onLeft(void);
    virtual void onRight(void);

  protected:
    std::string _name;
};

class Menu
{
  public:
    Menu(u16 x, u16 y);
    virtual ~Menu();

    void addItem(MenuItem *item); // takes ownership

    void realize(PSP2D::Font *font);

    void draw(PSP2D::Font *font, PSP2D::Drawable *drw);

    void update(PSP2D::Controller *ctrl);

  protected:
    u16 _x, _y, _w, _h;
    std::vector<MenuItem*> _items;
    PSP2D::Image *_bg, *_abg;
    int _current;
    PSP2D::Controller _old;
};

class FloatMenuItem : public MenuItem
{
  public:
    FloatMenuItem(const std::string& name, float value, float step);
    ~FloatMenuItem();

    float getValue(void);

    void getDimension(PSP2D::Font *font, u16& w, u16& h);
    void draw(PSP2D::Font *font, PSP2D::Drawable *drw, u16 x, u16 y, u16 w, u16 h);

    void onLeft(void);
    void onRight(void);

  protected:
    float _value, _step;
};

#endif /* _MENU_H */
