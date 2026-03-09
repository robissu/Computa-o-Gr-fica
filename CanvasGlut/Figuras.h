#ifndef __FIGURAS_H__
#define __FIGURAS_H__

#include "gl_canvas2d.h"

class Figuras {
    float x, y, width, height, raio, distX, distY;
    int cor;


public:
    Figuras(float x, float y, float width, float height, int cor){//retangulo
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->cor = cor;
        //desenhaRect(this->x = x, this->y = y, this->width = width, this->height = height, this->cor = cor);
    }
    Figuras(float x, float y, float raio, int cor) {//circulo
        this->x = x;
        this->y = y;
        this->raio = raio;
        this->cor = cor;
        //desenhaCircle(this->x = x, this->y = y, this->raio = raio, this->cor = cor);
    }

    void desenhaRect() {
        CV::color(cor);
        CV::rectFill(this->x,this->y,this->x+this->width,this->y+this->height);
        
    }

    void desenhaRect(float x, float y, float width, float height, int cor) {
        CV::color(cor);
        CV::rectFill(x, y, x+width, y+height);
    }

    void desenhaCircle() {
        CV::color(cor);
        CV::circleFill(this->x, this->y, this->raio, 50);
    }

    void desenhaCircle(float x, float y, float raio, int cor) {
        CV::color(cor);
        CV::circleFill(x, y, raio, 50);
    }

    void setDist(int mouseX, int mouseY) {
        distX = mouseX - x;
        distY = mouseY - y;
    }

    void colisaoRect(int mouseX, int mouseY, bool pressionado) {
        if (mouseX >= x && mouseX <= x + width
            && mouseY >= y && mouseY <= y + height && pressionado) {
            cor = 13;
            x = mouseX - distX;
            y = mouseY - distY;
        }
        else {
            cor = 4;
        }
    }
};

#endif
