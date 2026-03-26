#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "gl_canvas2d.h"
#include "Objetos.h"

#define MAX 14

float Colors1[14][3] =
{
    {0, 0, 0}, //Black 0
    {0.5, 0.5, 0.5}, //Gray 1
    {1, 0, 0}, //Red 2
    {0, 1, 0}, //Green 3
    {0, 0, 1}, //Blue 4
    {0, 1, 1}, //Cyan 5
    {1, 0, 1}, //Magenta 6
    {1, 1, 0}, //Yellow 7
    {1, 0.5, 0}, //Orange 8
    {0.5, 0, 0}, //Brown 9 
    {0.5, 0.5, 0}, //Olive 10
    {0, 0.5, 0.5}, // 11 
    {0.5, 0, 0.5}, // 12
    {1, 1, 1}, //white 13
};

class Slider {
    float sliderX, sliderY, sliderW, sliderH;
    int sent, tipo;
    Objetos* circ;
public:
    Slider(float initX, float initY, float width, float height, int sentido, int _tipo) {
        circ = new Objetos(initX+width/2, initY+height/2, height);
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
        circ->desenhaCircle(0);
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

    void retanguloDegrade(float indexSelecionado) {
        //retangulo degrade
        Vector2 v1, v2;
        float largura = 350;

        indexSelecionado = indexSelecionado * MAX;
        int idxFim = indexSelecionado;
        int idxInicio = indexSelecionado - 1;
        for (float i = 0; i < largura; i++)
        {
            float t = i / largura;

            // O cálculo busca na matriz usando os dois índices calculados
            float r = Colors1[idxInicio][0] + t * (Colors1[idxFim][0] - Colors1[idxInicio][0]);
            float g = Colors1[idxInicio][1] + t * (Colors1[idxFim][1] - Colors1[idxInicio][1]);
            float b = Colors1[idxInicio][2] + t * (Colors1[idxFim][2] - Colors1[idxInicio][2]);

            CV::color(r, g, b);
            v1.set(i + 100, 240);
            v2.set(i + 101, 300);
            CV::rectFill(v1, v2);
        }
    }
};

#endif
#pragma once
