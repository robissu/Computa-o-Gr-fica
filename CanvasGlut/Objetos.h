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
        
        for (int idxY = 0; idxY < imagem->getHeight(); idxY++) {
            for (int idxX = 0; idxX < imagem->getWidth(); idxX++) {
                int idx = idxY * imagem->getBytes() + idxX * 3;

                if (!arrastar) {
                    CV::color(data[idx] / 255.0, data[idx + 1] / 255.0, data[idx + 2] / 255.0);
                }
                    
                else {
                    CV::color(data[idx] / 255.0 * (0.5), data[idx + 1] / 255.0 * (0.5), data[idx + 2] / 255.0* (0.5));
                }
                
                //valor entre 0 e 1,
                int dimX = idxX * escala;
                int dimY = idxY * escala;
                
                CV::point(x + dimX, y + dimY);
            }
            
        }
        width = escala * imagem->getWidth();
        height = escala * imagem->getHeight();

        //for (int idx = 0; idx < imagem->getWidth() * imagem->getHeight() * 3; idx += 3) {
        //    
        //    CV::color(data[idx] / 255.0, data[idx + 1] / 255.0, data[idx + 2] / 255.0);
        //    
        //        
        //    
        //    int pixel = idx / 3;
        //    //valor entre 0 e 1,
        //    float pX = pixel % imagem->getWidth();//colunas
        //    float pY= pixel / imagem->getWidth();//linhas
        //    int dimX = pX * escala;
        //    int dimY = pY * escala;
        //    
        //    CV::point(x + dimX, y + dimY);
        //}
        //width = escala * imagem->getWidth();
        //height = escala * imagem->getHeight();
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
            if (lista[i]->hitClick(mouseX, mouseY)) {
                lista[i]->setArrast(mouseX, mouseY);
                break;
            }
        }
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
