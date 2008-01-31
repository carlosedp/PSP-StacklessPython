// -*- c -*-

inline void PSP2D::Screen::putPixel(u32 color, u16 x, u16 y)
{
    u32* vram = getVramDrawBuffer();
    vram[PSP_LINE_SIZE * y + x] = color;
}

inline u32 PSP2D::Screen::getPixel(u16 x, u16 y)
{
    u32* vram = getVramDrawBuffer();
    return vram[PSP_LINE_SIZE * y + x];
}
