#ifndef __FIGURAS_H__
#define __FIGURAS_H__

#include "gl_canvas2d.h"

class Figuras {
    float x, y, width, height, raio;
    int cor;


public:
    Figuras(float x, float y, float width, float height, int cor){//retangulo
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->cor = cor;
    }
    Figuras(float x, float y, float raio, int cor) {//circulo
        this->x = x;
        this->y = y;
        this->raio = raio;
        this->cor = cor;
    }
};

#endif
