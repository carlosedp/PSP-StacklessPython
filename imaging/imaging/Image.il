// -*- c -*-

inline void Imaging::Image::putPixel(u32 color, u16 x, u16 y)
{
    *(_data + y * _textureWidth + x) = color;
}

inline u32 Imaging::Image::getPixel(u16 x, u16 y) const
{
    return *(_data + y * _textureWidth + x);
}
