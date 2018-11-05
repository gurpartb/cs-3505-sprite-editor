#ifndef SPRITEFRAME_H
#define SPRITEFRAME_H

#include "pixel.h"


class SpriteFrame
{
public:
    SpriteFrame();
    Pixel getPixel(int coordinateX, int coordinateY);
    void setPixel(int coordinateX, int coordinateY, int redValue, int greenValue, int blueValue, int transparencyValue);
};

#endif // SPRITEFRAME_H
