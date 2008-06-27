// -*- c -*-

inline void Imaging::Screen::putPixel(u32 color, u16 x, u16 y)
{
    u32* vram = getVramDrawBuffer();
    vram[PSP_LINE_SIZE * y + x] = color;
}

inline u32 Imaging::Screen::getPixel(u16 x, u16 y) const
{
    u32* vram = getVramDrawBuffer();
    return vram[PSP_LINE_SIZE * y + x];
}
