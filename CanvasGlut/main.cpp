/*********************************************************************
// Canvas
// Autor: Robson Daniel Marchesan
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


#include "gl_canvas2d.h"
#include "Bola.h"
#include "Relogio.h"
#include "Botao.h"
#include "Objetos.h"
#include "Slider.h"
#include "Bmp.h"


#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 800, screenHeight = 800;

char arquivo[] = { "gremio.bmp" };
Slider* slid;
Objetos* sliderChoice;
Objetos* imagem;
std::vector<Objetos*> listaObjetos;
//--------------------------------------------
Botao* vermelho;
Botao* verde;
Botao* azul;
Botao* lumin;
Botao* rotaciona;
Botao* addRect;
Botao* addCirc;
Botao* removObj;
Botao* checkbox;
std::vector<Botao*> listaBotao;

int mouseX, mouseY; 
bool pressMouse = false;
bool pressTeclado = false;
int direcaoTeclado = -1;

void polinomio()
{
    CV::color(0);
    CV::translate(screenWidth/2, screenHeight/2);
    CV::line(-500, 0, 500, 0);
    CV::line(0, -500, 0, 500);
    float escalaX = 100, escalaY = 50;
    for (float x = -3; x <= 3; x += 0.001)
    {
        float y = x * x * x - 3 * x + 2;
        CV::point(x * escalaX, y * escalaY);
    }
    CV::translate(0,0);
}

void sliderConfig() {
    slid->barraDeslize();
    slid->circSeleciona(mouseX,mouseY, pressMouse, listaObjetos);
    //slid->retanguloDegrade(slid->normaCirc());
}

void configImagem() {
    float escala = slid->normaCirc();
    imagem->editImagem(escala, mouseX, mouseY, rotaciona);
    imagem->desenhaHistograma(50,10, 256, 100, listaBotao);
}

void qualBotao() {
    if (addRect->getPress()) {
        listaObjetos.push_back(new Objetos(50 + (listaObjetos.size() * 5), 100, 50, 100, 4));
        addRect->alterna();
    }
    else if (addCirc->getPress()) {
        listaObjetos.push_back(new Objetos(250 + (listaObjetos.size() * 5), 100, 30, 4));
        addCirc->alterna();
    }
    else if (removObj->getPress()) {
        if (!listaObjetos.empty()) {
            Objetos* ultimo = listaObjetos.back();
            if (ultimo->getTipo() != 4 && ultimo->getTipo() != 3) {
                delete ultimo;
                listaObjetos.pop_back();
                removObj->alterna();
            }
        }
    }
}

void configBotao() {
    for (auto* btn : listaBotao) {
        btn->Render();
    }
}

void desenhaObjetos() {
    for (auto* obj : listaObjetos) {
        if (obj->getTipo() == 1) {
            obj->desenhaRect();
            if (obj->getArrast()) {
                obj->drag(mouseX, mouseY);
            }
            if (pressTeclado) {
                obj->mexer(direcaoTeclado);
            }
        }
        else if (obj->getTipo() == 2) {
            obj->desenhaCircle();
            if (obj->getArrast()) {
                obj->drag(mouseX, mouseY);
            }
            if (pressTeclado) {
                obj->mexer(direcaoTeclado);
            }
        }
        else if (obj->getTipo() == 3) {
            imagem->editImagem(slid->normaCirc(), mouseX, mouseY, rotaciona);
            imagem->desenhaHistograma(50, 10, 256, 100, listaBotao);
        }
    }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
   CV::clear(1, 1, 1);

   //polinomio();
   desenhaObjetos();
   sliderConfig();
   //configImagem();
   configBotao();
   
   Sleep(10); //limitador FPS
}


//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   //printf("\nTecla: %d" , key);
   pressTeclado = true;
   switch (key){
        case 27://Esq
           exit(0);
           break;  
        case 200://esquerda
           direcaoTeclado = 0;
           break;
        case 201://cima
           direcaoTeclado = 1;
           break;
        case 202://direita
           direcaoTeclado = 2;
           break;
        case 203://baixo
           direcaoTeclado = 3;
           break;
        default:
           direcaoTeclado = -1;
           break;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   //printf("\nLiberou: %d" , key);
   pressTeclado = false;
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
/*button 0 -> esquerdo || 2 -> direito
state = 0 -> botao pressMouseionado || 1 -> solto
-2 -> nada acontecendo*/
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
   //printf("\n COORDENADAS MOUSE-> X: %d Y: %d", mouseX, mouseY);
   if (state == 0) {
       pressMouse = true;
       if (!Objetos::checaListaArrasto(listaObjetos)) {
           Objetos::checaListaColisao(mouseX, mouseY, listaObjetos);
       }

   }
   if (state == 1) {
       pressMouse = false;
       for (auto* btn : listaBotao) {
           if (btn->hitClick(mouseX,mouseY)) {
               btn->alterna();
               qualBotao();
           }
       }
       for (auto* obj : listaObjetos) {
           obj->soltaArrast();
           if (obj->checaSelec(mouseX,mouseY)) {
               obj->setSelecao(true);
           }
           else {
               obj->setSelecao(false);
           }
       }
   }


}


int main(void)
{
   
   slid = new Slider(30, 220, 30, 10, 0, 0);
   imagem = new Objetos(arquivo);
   listaObjetos.push_back(slid->getCirc());
   listaObjetos.push_back(imagem);
   //------------------------------------------------------
   vermelho = new Botao(100, 400, 85, 30, "Vermelho", 2);
   verde = new Botao(200, 400, 60, 30, "Verde", 3);
   azul = new Botao(300, 400, 60, 30, "Azul", 4);
   lumin = new Botao(400, 400, 60, 30, "Lumin", 0);
   rotaciona = new Botao(10, 400, 70, 30, "Rot 90º", 0);
   addRect = new Botao(100, 600, 90, 30, "ADD rect", 0);
   addCirc = new Botao(100, 500, 90, 30, "ADD circ", 0);
   removObj = new Botao(300, 600, 135, 30, "Remove Objeto", 0);
   checkbox = new Botao(30,30, 10, 10, "box");
   listaBotao.push_back(checkbox);
   listaBotao.push_back(vermelho);
   listaBotao.push_back(verde);
   listaBotao.push_back(azul);
   listaBotao.push_back(lumin);
   listaBotao.push_back(rotaciona);
   listaBotao.push_back(addRect);
   listaBotao.push_back(addCirc);
   listaBotao.push_back(removObj);
   CV::init(&screenWidth, &screenHeight, "Demo Robson");
   CV::run();
}
