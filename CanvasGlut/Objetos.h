#ifndef __Objetos_H__
#define __Objetos_H__

#include "gl_canvas2d.h"
#include "Bmp.h"

class Objetos {
    float x, y, width, height, raio, distX, distY, vel;
    int cor, tipo;
    bool arrastar, selecao;
    Bmp *imagem;
    unsigned char* data;
public:
    static int graficoR, graficoG, graficoB, graficoL;
    Objetos(int tipo, float x, float y, float width, float height, int cor){//retangulo tipo 1
        this->tipo = tipo;
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
    Objetos(int tipo, float x, float y, float raio, int cor) {//circulo tipo 2
        this->tipo = tipo;
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
    Objetos(int tipo, float x, float y, float raio) {//circulo sem cor tipo 3
        this->tipo = tipo;
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
    Objetos(int tipo, char* nome) {//imagem tipo 4
        this->tipo = tipo;
        imagem = new Bmp(nome);
        imagem->convertBGRtoRGB();
        data = imagem->getImage();
        x = 0;
        y = 0;
        width = (float)imagem->getWidth();
        height = (float)imagem->getHeight();
    }
    Objetos(int tipo, float x, float y) {//checkbox tipo 5
        this->tipo = tipo;
        this->x = x;
        this->y = y;
        this->width = 20;
        this->height = 20;
        this->cor = 0;
        this->vel = 5;
        this->arrastar = false;
        this->selecao = false;
        this->distX = 0;
        this->distY = 0;
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

    void soltaArrast() {
        arrastar = false;
    }

    void escalaImagem(float escala, int mouseX, int mouseY) {
        if (arrastar) {
            x = mouseX - distX;
            y = mouseY - distY;
        }

        float anguloRad = -90 * (3.14159f / 180.0f);
        float c = cosf(anguloRad);
        float s = sinf(anguloRad);

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

                    CV::color(y / 255.0 * (0.5), y / 255.0 * (0.5), y / 255.0* (0.5));
                }
               


                //valor entre 0 e 1,
                int dimX = idxX * escala;
                int dimY = idxY * escala;
                

                float relX = dimX - meioW;
                float relY = dimY - meioH;
                if (!arrastar) {
                    CV::point(x + dimX, y + dimY);
                }
                else {
                    float rotX = relX * c - relY * s;
                    float rotY = relX * s + relY * c;
                    CV::point(x + meioW + rotX, y + meioH + rotY);
                }
            }
            
        }
        width = escala * imagem->getWidth();
        height = escala * imagem->getHeight();
    }


    void setSelecao(bool selec) {
        this->selecao = selec;
    }

    float normaliz(float v, float minV, float max, float min) {
        return (v - minV) / (max - min);
    }

    void desenhaHistograma(float pX, float pY, float largura, float altura) {
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

        
        int maxR = 0, maxG = 0, maxB = 0, maxL =0;
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


        //vermelho
        if (graficoR) {
            CV::color(1, 0, 0);
            for (int i = 0; i < 256; i++) {
                float xR = pX + normaliz(i, 0, 255.0f, 0) * largura;
                float hR = normaliz(histR[i], 0, maxR, 0) * altura;

                CV::line(xR, pY, xR, pY + hR);
            }
        }
        

        //verde
        if (graficoG) {
            CV::color(0, 1, 0);
            for (int i = 0; i < 256; i++) {
                float xG = pX + normaliz(i, 0, 255.0f, 0) * largura;
                float hG = normaliz(histG[i], 0, maxG, 0) * altura;

                CV::line(xG, pY, xG, pY + hG);
            }
        }
        

        //AZUL
        if (graficoB) {
            CV::color(0, 0, 1);
            for (int i = 0; i < 256; i++) {
                float xB = pX + normaliz(i, 0, 255.0f, 0) * largura;
                float hB = normaliz(histB[i], 0, maxB, 0) * altura;

                CV::line(xB, pY, xB, pY + hB);
            }
        }
        
        //luminancia
        if (graficoL) {
            CV::color(0.5, 0.5, 0.5);
            for (int i = 0; i < 256; i++) {
                float xL = pX + normaliz(i, 0, 255.0f, 0) * largura;
                float hL = normaliz(histL[i], 0, maxL, 0) * altura;

                CV::line(xL, pY, xL, pY + hL);
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

    void desenhaRect() {
        if (selecao)
            cor = 3;
        else {
            cor = 4;
        }
        CV::color(cor);
        CV::rectFill(this->x, this->y, this->x + this->width, this->y + this->height);

    }

    void desenhaBox() {
        if (selecao) {
            CV::color(cor);
            CV::line(x, y, x + width, y + height);
            CV::line(x+width, y, x, y + height);
        }
        CV::color(cor);
        CV::rect(this->x, this->y, this->x + this->width, this->y + this->height);

    }

    void desenhaRect(float x, float y, float width, float height, int cor) {
        if (selecao)
            cor = 3;
        else {
            cor = 4;
        }
        CV::color(cor);
        CV::rectFill(x, y, x+width, y+height);
    }

    void desenhaCircle() {
        if (selecao)
            cor = 3;
        else {
            cor = 4;
        }
        CV::color(cor);
        CV::circleFill(this->x, this->y, this->raio, 50);
    }
    void desenhaCircle(float x, float y, float raio, int cor) {
        if (selecao)
            cor = 3;
        else {
            cor = 4;
        }
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

    bool circBorda(int mouseX, int mouseY) {
        float difX = mouseX - x;
        float difY = mouseY - y;
        float quad = (difX * difX) + (difY * difY);
        return (quad <= (raio * raio));
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
        if (tipo == 1 || tipo == 5) {
            return checaSelecaoRect(mouseX, mouseY);
        }
        else if (tipo == 2 || tipo == 3) {
            return checaSelecaoCircle(mouseX, mouseY);
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
