#ifndef __Objetos_H__
#define __Objetos_H__

#include "gl_canvas2d.h"
#include "Bmp.h"

class Objetos {
    float x, y, width, height, raio, distX, distY, vel, angulo;
    float escala = 1.0f;
    int cor, tipo;
    bool arrastar, selecao;
    Bmp *imagem;
    unsigned char* data;
    
public:
    Objetos(float x, float y, float width, float height, int cor){//retangulo tipo 1
        this->tipo = 1;
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
    }
    Objetos(float x, float y, float raio, int cor) {//circulo tipo 2
        this->tipo = 2;
        this->x = x;
        this->y = y;
        this->raio = raio;
        this->cor = cor;
        this->arrastar = false;
        this->vel = 5;
        this->selecao = false;
        this->distX = 0;
        this->distY = 0;
    }
    Objetos(float x, float y, float raio) {//circulo slider tipo 3
        this->tipo = 3;
        this->x = x;
        this->y = y;
        this->raio = raio;
        this->cor = cor;
        this->arrastar = false;
        this->vel = 5;
        this->selecao = false;
        this->distX = 0;
        this->distY = 0;
    }
    Objetos(char* nome) {//imagem tipo 4
        this->tipo = 4;
        imagem = new Bmp(nome);
        imagem->convertBGRtoRGB();
        data = imagem->getImage();
        x = 0;
        y = 0;
        this->vel = 5;
        this->selecao = false;
        width = (float)imagem->getWidth();
        height = (float)imagem->getHeight();
        angulo = 0;
    }

    bool getArrast() {
        return arrastar;
    }

    unsigned char* getData(){
        return data;
    }

    Bmp *getBmp() {
        return imagem;
    }

    float getWidth() {
        return width;
    }

    float getHeight() {
        return height;
    }

    float getX() {
        return x;
    }

    float getY() {
        return y;
    }

    int getTipo(){
        return tipo;
    }

    void soltaArrast() {
        arrastar = false;
    }

    void rotacionar(float graus) {
        angulo += graus * (3.14159f / 180.0f);
    }

    void editImagem(float escalaSlider, int mouseX, int mouseY) {
        if (arrastar) {
            x = mouseX - distX;
            y = mouseY - distY;
        }

        if (selecao) {
            this->escala = escalaSlider;
        }

        float c = cosf(angulo);
        float s = sinf(angulo);

        float meioW = (imagem->getWidth() * escala) / 2.0f;
        float meioH = (imagem->getHeight() * escala) / 2.0f;

        for (int idxY = 0; idxY < imagem->getHeight(); idxY++) {
            for (int idxX = 0; idxX < imagem->getWidth(); idxX++) {
                int idx = idxY * imagem->getBytes() + idxX * 3;

                if (!arrastar) {
                    CV::color(data[idx] / 255.0, data[idx + 1] / 255.0, data[idx + 2] / 255.0);
                }
                    
                else {
                    float y = data[idx] * 0.3 + data[idx + 1] * 0.5 + data[idx + 2] * 0.2;//cinza

                    CV::color(y / 255.0 , y / 255.0, y / 255.0);
                    //CV::color(0, 0 ,y/ 255.0);
                }

                //valor escala entre 0 e 1,
                int dimX = idxX * escala;
                int dimY = idxY * escala;

                //calcula a translacao para o centro da imagem a fim de rotacionar ela em relacao ao proprio centro
                float relX = dimX - meioW;
                float relY = dimY - meioH;
                float rotX = relX * c - relY * s;
                float rotY = relX * s + relY * c;
                CV::point(x + meioW + rotX, y + meioH + rotY);
            }
            
        }
        width = escala * imagem->getWidth();
        height = escala * imagem->getHeight();
    }

    void setSelecao(bool selec) {
        selecao = selec;
    }

    float normaliz(float v, float minV, float max, float min) {
        return (v - minV) / (max - min);
    }

    void desenhaHistograma(float pX, float pY, float largura, float altura, std::vector<Botao*>& botoes) {
        int histR[256] = { 0 };
        int histG[256] = { 0 };
        int histB[256] = { 0 };
        int histL[256] = { 0 };

        for (int idxY = 0; idxY < imagem->getHeight(); idxY++) {
            for (int idxX = 0; idxX < imagem->getWidth(); idxX++) {
                int idx = idxY * imagem->getBytes() + idxX * 3;

                
                int r = data[idx];
                int g = data[idx + 1];
                int b = data[idx + 2];

                histR[r]++;
                histG[g]++;
                histB[b]++;
                
                int luminancia = (r * 0.3 + g * 0.5 + b * 0.2);
                if (luminancia > 255) {
                    luminancia = 255;
                }
                histL[luminancia]++;
            

            }
        }

        
        int maxR = 0;
        int maxG = 0;
        int maxB = 0;
        int maxL = 0;
        for (int i = 0; i < 256; i++) {
            if (histR[i] > maxR)
                maxR = histR[i];
            if (histG[i] > maxG)
                maxG = histG[i];
            if (histB[i] > maxB)
                maxB = histB[i];
            if (histL[i] > maxL)
                maxL = histL[i];
        }


        for (auto* btn : botoes) {
            if (btn->getPress()) {

                if (!strcmp(btn->getLabel(), "Vermelho")) {
                    CV::color(1, 0, 0);
                    for (int i = 0; i < 256; i++) {
                        float xR = pX + ((float)i / 255.0f) * largura;
                        float hR = ((float)histR[i] / maxR) * altura;
                        CV::line(xR, pY, xR, pY + hR);
                    }
                }
                if (!strcmp(btn->getLabel(), "Verde")) {
                    CV::color(0, 1, 0);
                    for (int i = 0; i < 256; i++) {
                        float xG = pX + ((float)i / 255.0f) * largura;
                        float hG = ((float)histG[i] / maxG) * altura;
                        CV::line(xG, pY, xG, pY + hG);
                    }
                }
                if (!strcmp(btn->getLabel(), "Azul")) {
                    CV::color(0, 0, 1);
                    for (int i = 0; i < 256; i++) {
                        float xB = pX + ((float)i / 255.0f) * largura;
                        float hB = ((float)histB[i] / maxB) * altura;
                        CV::line(xB, pY, xB, pY + hB);
                    }
                }
                if (!strcmp(btn->getLabel(), "Lumin")) {
                    CV::color(0.5, 0.5, 0.5);
                    for (int i = 0; i < 256; i++) {
                        float xL = pX + ((float)i / 255.0f) * largura;
                        float hL = ((float)histL[i] / maxL) * altura;
                        CV::line(xL, pY, xL, pY + hL);
                    }
                }
            }
        }
    }

    bool hitClick(int mouseX, int mouseY) {
        if (tipo == 1 || tipo == 4) {
            return rectBorda(mouseX, mouseY);
        }
        else if (tipo == 2 || tipo == 3) {
            return circBorda(mouseX, mouseY);
        }

    }

    static bool checaListaArrasto(std::vector<Objetos*>& lista) {//checase algum ta sendo arrastado
        for (auto* obj : lista) {
            if (obj->arrastar) {
                return true;
            }
        }
        return false;
    }

    static void checaListaColisao(int mouseX, int mouseY, std::vector<Objetos*>& lista) {
        for (int i = lista.size() -1; i >= 0; i--) {
            if (lista[i]->hitClick(mouseX, mouseY) && lista[i]->tipo != 5) {
                lista[i]->setArrast(mouseX, mouseY);
                break;
            }
        }
    }

    void desenhaRect(float escalaSlider) {
        if (selecao) {
            this->escala = escalaSlider;
            cor = 3;
        }
        else {
            cor = 4;

        }
        float wEscala = this->width * escala;
        float hEscala = this->height * escala;

        CV::color(cor);
        CV::rectFill(this->x, this->y, this->x + wEscala, this->y + hEscala);
       

    }


    void desenhaCircle(float escalaSlider) {
        if (selecao) {
            this->escala = escalaSlider;
            cor = 3;
        }
        else {
            cor = 4;
        }
        float rEscala = this->raio * this->escala;
        CV::color(cor);
        CV::circleFill(this->x, this->y, rEscala, 50);
    }

    void desenhaCircle() {
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
        float wAtual = this->width * this->escala;
        float hAtual = this->height * this->escala;
        return (mouseX >= x && mouseX <= x + wAtual
            && mouseY >= y && mouseY <= y + hAtual);
    }

    bool circBorda(int mouseX, int mouseY) {
        float rAtual = this->raio * this->escala;
        float difX = mouseX - x;
        float difY = mouseY - y;
        float quad = (difX * difX) + (difY * difY);
        return (quad <= (rAtual * rAtual));
    }

    bool checaSelecaoRect(int mouseX, int mouseY) {
        if (rectBorda(mouseX, mouseY)) {
            return true;
        }
        else {
            return false;
        }
    }

    bool checaSelecaoCircle(int mouseX, int mouseY) {
        if (circBorda(mouseX, mouseY)) {
            return true;
        }
        else {
            return false;
        }
    }

    bool checaSelec(int mouseX, int mouseY) {
        if (tipo == 1 || tipo == 5 || tipo == 4) {
            return checaSelecaoRect(mouseX, mouseY);
        }
        else if (tipo == 2 || tipo == 3) {
            return checaSelecaoCircle(mouseX, mouseY);
        }
        return false;
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
