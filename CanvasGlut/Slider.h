#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "gl_canvas2d.h"
#include "Objetos.h"

#define MAX 14

class Slider {
    float sliderX, sliderY, sliderW, sliderH;
    int sent, tipo;
    Objetos* circ;
public:
    Slider(float initX, float initY, float width, float height, int sentido, int _tipo) {
        float meioHorizontal = initX + (width * MAX) / 2.0f;
        float meioVertical = initY + (height * MAX) / 2.0f;

        if (sentido) {
            circ = new Objetos(initX + width / 2, meioVertical, height);
        }
        else {
            circ = new Objetos(meioHorizontal, initY + height / 2, height);
        }
        
        sliderX = initX;
        sliderY = initY;
        sliderW = width;
        sliderH = height;
        sent = sentido;
        tipo = _tipo;
    }

    Objetos* getCirc() {
        return circ;
    }

    void setPos(float novoX, float novoY) {
        this->sliderX = novoX;
        this->sliderY = novoY;
        float meioHorizontal = sliderX + (sliderW * MAX) / 2.0f;
        float meioVertical = sliderY + (sliderH * MAX) / 2.0f;
        if (sent) {
            circ->setPos(sliderX + sliderW / 2, meioVertical);
   
        }
        else {
            circ->setPos(meioHorizontal, sliderY + sliderH/2);
        }
    }

    void barraDeslize() {
        //desenha barra de apoio
        int idx;
        if (sent) {
            for (idx = 0; idx < MAX; idx++)
            {
                if (tipo)
                    CV::color(idx);
                else
                    CV::color(1);
                CV::rectFill(sliderX, sliderY + (sliderH * idx), sliderX + sliderW, sliderY + sliderH + (sliderH * idx));
            }
        }
        else {
            for (idx = 0; idx < MAX; idx++)
            {
                if (tipo)
                    CV::color(idx);
                else
                    CV::color(1);
                CV::rectFill(sliderX + (sliderW * idx), sliderY, sliderX + sliderW + (sliderW * idx), sliderY + sliderH);
            }
        }
    }
    void circSeleciona(int mouseX, int mouseY, bool pressMouse, std::vector<Objetos*>& lista) {
        circ->desenhaCircle();
        if (circ->getArrast() || !Objetos::checaListaArrasto(lista)) {
            if (sent) {
                circ->dragY(mouseY, sliderY, sliderY + sliderH + (sliderH * (MAX - 1)));
            }
            else {
                circ->dragX(mouseX, sliderX, sliderX + sliderW + (sliderW * (MAX - 1)));
            }
        }
    }

    float normaCirc() {
        //calcula posicao do circulo na barra
        if (sent) {
            float alturaTotalBarra = sliderH * MAX;
            float deltaY = circ->getY() - sliderY;
            float t = deltaY / alturaTotalBarra;
            if (t < 0)t = 0;
            if (t > 0.99f)t = 0.99f;
            return (t);
        }
        else {
            float larguraTotalBarra = sliderW * MAX;
            float deltaX = circ->getX() - sliderX;
            float t = deltaX / larguraTotalBarra;
            if (t < 0)t = 0;
            if (t > 0.99f)t = 0.99f;
            return (t);
        }
        
    }
};

#endif
#pragma once
