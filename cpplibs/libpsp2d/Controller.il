// -*- c -*-

inline int PSP2D::Controller::analogX()
{
    return (int)_data.Lx - 127;
}

inline int PSP2D::Controller::analogY()
{
    return (int)_data.Ly - 127;
}

inline bool PSP2D::Controller::square()
{
    return (_data.Buttons & PSP_CTRL_SQUARE) != 0;
}

inline bool PSP2D::Controller::cross()
{
    return (_data.Buttons & PSP_CTRL_CROSS) != 0;
}

inline bool PSP2D::Controller::triangle()
{
    return (_data.Buttons & PSP_CTRL_TRIANGLE) != 0;
}

inline bool PSP2D::Controller::circle()
{
    return (_data.Buttons & PSP_CTRL_CIRCLE) != 0;
}

inline bool PSP2D::Controller::up()
{
    return (_data.Buttons & PSP_CTRL_UP) != 0;
}

inline bool PSP2D::Controller::down()
{
    return (_data.Buttons & PSP_CTRL_DOWN) != 0;
}

inline bool PSP2D::Controller::left()
{
    return (_data.Buttons & PSP_CTRL_LEFT) != 0;
}

inline bool PSP2D::Controller::right()
{
    return (_data.Buttons & PSP_CTRL_RIGHT) != 0;
}

inline bool PSP2D::Controller::l()
{
    return (_data.Buttons & PSP_CTRL_LTRIGGER) != 0;
}

inline bool PSP2D::Controller::r()
{
    return (_data.Buttons & PSP_CTRL_RTRIGGER) != 0;
}

inline bool PSP2D::Controller::select()
{
    return (_data.Buttons & PSP_CTRL_SELECT) != 0;
}

inline bool PSP2D::Controller::start()
{
    return (_data.Buttons & PSP_CTRL_START) != 0;
}

inline bool PSP2D::Controller::operator==(const PSP2D::Controller& ctrl)
{
    return (_data.Buttons == ctrl._data.Buttons);
}

inline PSP2D::Controller& PSP2D::Controller::operator=(const PSP2D::Controller& ctrl)
{
    memcpy(&_data, &ctrl._data, sizeof(_data));
    return *this;
}
