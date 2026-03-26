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
int screenWidth = 500, screenHeight = 500;

char arquivo[] = { "tank.bmp" };
Slider* slid;
Objetos* retangulo;
Objetos* sliderChoice;
Objetos* circulo;
Objetos* imagem;
Objetos* checkbox;
std::vector<Objetos*> listaObjetos;
Botao* vermelho;
Botao* verde;
Botao* azul;
Botao* lumin;
std::vector<Botao*> listaBotao;

//se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
int opcao  = 50;//variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
bool pressMouse = false;
bool pressTeclado = false;
int direcaoTeclado = -1;


void retanguloConfig() {
    retangulo->desenhaRect();
    if (retangulo->getArrast()) {
        retangulo->drag(mouseX, mouseY);
    }
    if (pressTeclado) {
        retangulo->mexer(direcaoTeclado);
    }
}

void circleConfig() {
    circulo->desenhaCircle();
    if (circulo->getArrast()) {
        circulo->drag(mouseX, mouseY);
    }
    if (pressTeclado) {
        circulo->mexer(direcaoTeclado);
    }
}

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
    imagem->escalaImagem(escala, mouseX, mouseY);
    imagem->desenhaHistograma(50,10, 256, 100);
}

void configBotao() {
    vermelho->Render();
    verde->Render();
    azul->Render();
    lumin->Render();
}


int Objetos::graficoR = 0;
int Objetos::graficoG = 0;
int Objetos::graficoB = 0;
int Objetos::graficoL = 0;

void checaBotao() {
    if (vermelho->hitClick(mouseX,mouseY)) {
        Objetos::graficoR = 1;
    }
    else if (verde->hitClick(mouseX, mouseY)) {
        Objetos::graficoG = 1;
    }
    else if (azul->hitClick(mouseX, mouseY)) {
        Objetos::graficoB = 1;
    }
    else if (lumin->hitClick(mouseX, mouseY)) {
        Objetos::graficoL = 1;
    }
    else {
        Objetos::graficoR = Objetos::graficoG = Objetos::graficoB = Objetos::graficoL = 0;
    }
}

void configCheckBox() {
    checkbox->desenhaBox();
}
//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
   CV::clear(1, 1, 1);

   //polinomio();
   retanguloConfig();
   circleConfig();
   sliderConfig();
   configImagem();
   configCheckBox();
   configBotao();
   


   Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}


//funcao chamada toda vez que uma tecla for pressMouseionada.
void keyboard(int key)
{
   //printf("\nTecla: %d" , key);
   pressTeclado = true;
   if( key < 200 )
   {
      opcao = key;
   }
   if (pressTeclado) {
       switch (key)
       {
       case 27://esq
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
       checaBotao();

       for (auto* btn : listaBotao) {
           if (btn->hitClick(mouseX,mouseY)) {
               btn->setPress(true);
           }
           else {
               btn->setPress(false);
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
   retangulo = new Objetos(50, 100, 50, 100, 4);
   circulo = new Objetos( 250, 100, 30, 4);
   imagem = new Objetos(arquivo);
   checkbox = new Objetos(30, 30);
   listaObjetos.push_back(checkbox);
   listaObjetos.push_back(slid->getCirc());
   listaObjetos.push_back(retangulo);
   listaObjetos.push_back(circulo);
   listaObjetos.push_back(imagem);

   //------------------------------------------------------
   vermelho = new Botao(100, 400, 80, 30, "Vermelho", 2);
   verde = new Botao(200, 400, 60, 30, "Verde", 3);
   azul = new Botao(300, 400, 60, 30, "Azul", 4);
   lumin = new Botao(400, 400, 60, 30, "Lumin", 0);
   listaBotao.push_back(vermelho);
   listaBotao.push_back(verde);
   listaBotao.push_back(azul);
   listaBotao.push_back(lumin);

   CV::init(&screenWidth, &screenHeight, "Demo Robson");
   CV::run();
}
