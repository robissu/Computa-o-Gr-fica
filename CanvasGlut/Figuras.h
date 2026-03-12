#ifndef __FIGURAS_H__
#define __FIGURAS_H__

#include "gl_canvas2d.h"

class Figuras {
    float x, y, width, height, raio, distX, distY, vel;
    int cor;
    bool arrastar, selecao;
   
public:
    Figuras(float x, float y, float width, float height, int cor){//retangulo
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->cor = cor;
        this->arrastar = false;
        this->vel = 5;
        this->selecao = false;
        this->distX = 0;
        this->distY = 0;
        //desenhaRect(this->x = x, this->y = y, this->width = width, this->height = height, this->cor = cor);
    }
    Figuras(float x, float y, float raio, int cor) {//circulo
        this->x = x;
        this->y = y;
        this->raio = raio;
        this->cor = cor;
        this->arrastar = false;
        this->vel = 5;
        this->selecao = false;
        this->distX = 0;
        this->distY = 0;
        //desenhaCircle(this->x = x, this->y = y, this->raio = raio, this->cor = cor);
    }
    Figuras(float x, float y, float raio) {//circulo sem cor
        this->x = x;
        this->y = y;
        this->raio = raio;
        this->cor = cor;
        this->arrastar = false;
        this->vel = 5;
        this->selecao = false;
        this->distX = 0;
        this->distY = 0;
        //desenhaCircle(this->x = x, this->y = y, this->raio = raio, this->cor = cor);
    }

    bool getArrast() {
        return arrastar;
    }

    float getX() {
        return x;
    }

    float getY() {
        return y;
    }
    
    bool checaLista(Figuras* lista[]) {
        for (int i = 0; i < 3; i++) {
            if (!(lista[i] == this)) {
                if (lista[i]->getArrast()) {
                    return true;;
                }
            }
        }
        return false;
    }



    void desenhaRect() {
        if (selecao)
            cor = 3;
        CV::color(cor);
        CV::rectFill(this->x,this->y,this->x+this->width,this->y+this->height);
        
    }

    void desenhaRect(float x, float y, float width, float height, int cor) {
        if (selecao)
            cor = 3;
        CV::color(cor);
        CV::rectFill(x, y, x+width, y+height);
    }

    void desenhaCircle() {
        if (selecao)
            cor = 3;
        CV::color(cor);
        CV::circleFill(this->x, this->y, this->raio, 50);
    }
  
    void desenhaCircle(float x, float y, float raio, int cor) {
        if (selecao)
            cor = 3;
        CV::color(cor);
        CV::circleFill(x, y, raio, 50);
    }

    void desenhaCircle(int cor) {
        CV::color(0);
        CV::circle(this->x, this->y, this->raio, 50);
    }

    void setArrast(int mouseX, int mouseY) {
        distX = mouseX - x;
        distY = mouseY - y;
        arrastar = true;
    }

    void drag(int mouseX, int mouseY) {
        if (arrastar) {
            x = mouseX - distX;
            y = mouseY - distY;
        }
    }

    void dragY(int mouseY, int sliderMin, int sliderMax) {
        if (arrastar) {
            if (mouseY >= sliderMin && mouseY < sliderMax) {
                y = mouseY - distY;
                
            }     
        }
    }
    void dragX(int mouseX, int sliderMin, int sliderMax) {
        if (arrastar) {
            if (mouseX >= sliderMin && mouseX < sliderMax) {
                x = mouseX - distX;

            }
        }
    }
    
    bool rectBorda(int mouseX, int mouseY) {
        return (mouseX >= x && mouseX <= x + width
            && mouseY >= y && mouseY <= y + height);
    }

    void colisaoRect(int mouseX, int mouseY, bool pressionado) {
        if (rectBorda(mouseX, mouseY)) {
            cor = 6;  
        }
        else {
            cor = 4;
            
        }
        if (!pressionado) {
            arrastar = false;
            
        }
        //printf("\n TESTE SELECAO RETANGULO: %d", selecao);
        //printf("\nTESTE POSICAO RECT= X: %f Y: %f", x, y);
    }

    bool circBorda(int mouseX, int mouseY) {
        float difX = mouseX - x;
        float difY = mouseY - y;
        float quad = (difX * difX) + (difY * difY);
        return (quad <= (raio * raio));
    }

    void colisaoCirc(int mouseX, int mouseY, bool pressionado) {
        if (circBorda(mouseX, mouseY)) {
            cor = 6;
        }
        else {
            cor = 4;
        }
        if (!pressionado) {
            arrastar = false;
            
        }
        //printf("\n TESTE SELECAO CIRCLE: %d", selecao);
        //printf("\nTESTE POSICAO CIRCLE= X: %f Y: %f", this->x, this->y);
    }


    void checaSelecaoRect(int mouseX, int mouseY) {
        if (rectBorda(mouseX, mouseY)) {
            selecao = true;
        }
        else {
            selecao = false;
        }
    }

    void checaSelecaoCircle(int mouseX, int mouseY) {
        if (circBorda(mouseX, mouseY)) {
            selecao = true;
        }
        else {
            selecao = false;
        }
    }


    void mexer(int direcao) {
        if (selecao) {
            if (direcao == 0) {//esquerda
                x -= vel;
            }
            else if (direcao == 1) {//cima
                y += vel;
            }
            else if (direcao == 2) {//direita
                x += vel;
            }
            else if (direcao == 3) {//baixo
                y -= vel;
            }
        }
        
    }
};

#endif
