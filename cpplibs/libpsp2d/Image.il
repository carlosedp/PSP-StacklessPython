// -*- c -*-

inline void PSP2D::Image::putPixel(u32 color, u16 x, u16 y)
{
    *(_data + y * _textureWidth + x) = color;
}

inline u32 PSP2D::Image::getPixel(u16 x, u16 y)
{
    return *(_data + y * _textureWidth + x);
}
