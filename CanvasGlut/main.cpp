/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         08/2025
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.1
//
//  Instruções:
//	  Para alterar a animacao, digite numeros entre 1 e 3
//    Programa cheio de numeros magicos. Nao use isso nunca.
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Bola.h"
#include "Relogio.h"
#include "Botao.h"
#include "Figuras.h"


#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;

Figuras* retangulo;
Figuras* sliderChoice;
Figuras* circulo;
Figuras* lista[3];
Bola    *b = NULL;
Relogio *r = NULL;
Botao   *bt = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
int opcao  = 50;//variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
bool pressMouse = false;
bool pressTeclado = false;
int direcaoTeclado = -1;


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


void DesenhaSenoide()
{
   float x=0, y;
   CV::color(1);
   CV::translate(20, 200); //desenha o objeto a partir da coordenada (200, 200)
   for(float i=0; i < 68; i+=0.001f)
   {
      y = sinf(i)*50;
      CV::point(x, y);
      x += 0.01f;
   }
   CV::translate(0, 0);
}

void DesenhaLinhaDegrade()
{
   Vector2 p;
   for(float i=0; i<350; i++)
   {
	  CV::color(i/200, i/200, i/200);
	  p.set(i+100, 240);
	  CV::point(p);
   }

   //desenha paleta de cores predefinidas na Canvas2D.
   for(int idx = 0; idx < 14; idx++)
   {
	  CV::color(idx);
      CV::translate(20 + idx*30, 100);
	  CV::rectFill(Vector2(0,0), Vector2(30, 30));
   }
   CV::translate(0, 0);
}

void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}

float sliderX = 30, sliderY = 220, sliderW = 30, sliderH = 20;
int coresMax = 14;
void slider() {
    int idx;
    //desenha barra de cores
    for (idx = 0; idx < coresMax; idx++)
    {
        CV::color(idx);
        CV::rectFill(sliderX, sliderY + (sliderH * idx), sliderX + sliderW, sliderY + sliderH + (sliderH * idx));
    }
    sliderChoice->desenhaCircle(idx);
    if (!retangulo->getArrast() && !circulo->getArrast()) {
        sliderChoice->colisaoCirc(mouseX, mouseY, pressMouse);
        sliderChoice->drag(mouseY, sliderY, sliderY + sliderH + (sliderH * (coresMax - 1)));
    }

    float alturaTotalBarra = sliderH * coresMax;
    float deltaY = sliderChoice->getY() - sliderY;
    //printf("\ndelta: %f", deltaY);
    float t = deltaY / alturaTotalBarra;
    if (t < 0)t = 0;
    if (t > 0.99f)t = 0.99f;
    int indexSelecionado = (int)(t * coresMax);
    

    
    //retangulo degrade
    Vector2 v1, v2;
    float largura = 350;

    int idxFim = indexSelecionado;
    int idxInicio = indexSelecionado - 1;
    for (float i = 0; i < largura; i++)
    {
        float t = i / largura;

        // 3. O cálculo busca na matriz usando os dois índices calculados
        float r = Colors1[idxInicio][0] + t * (Colors1[idxFim][0] - Colors1[idxInicio][0]);
        float g = Colors1[idxInicio][1] + t * (Colors1[idxFim][1] - Colors1[idxInicio][1]);
        float b = Colors1[idxInicio][2] + t * (Colors1[idxFim][2] - Colors1[idxInicio][2]);

        CV::color(r, g, b);
        v1.set(i + 100, 240);
        v2.set(i + 101, 300);
        CV::rectFill(v1, v2);
    }

}

void retanguloConfig() {
    retangulo->desenhaRect();
    if (!circulo->getArrast() && !sliderChoice->getArrast()) {
        retangulo->colisaoRect(mouseX, mouseY, pressMouse);
        retangulo->drag(mouseX, mouseY);
    }
    if (pressTeclado) {
        retangulo->mexer(direcaoTeclado);
    }
}

void circleConfig() {
    circulo->desenhaCircle();
    if (!retangulo->getArrast() && !sliderChoice->getArrast()) {
        circulo->colisaoCirc(mouseX, mouseY, pressMouse);
        circulo->drag(mouseX, mouseY);
    }
    if (pressTeclado) {
        circulo->mexer(direcaoTeclado);
    }
}

/*
(v-min)/(max-min)
*/
void polinomio()
{
    float limitX = 500;
    float escalaX = 0.03f, escalaY = 30.0f;
    CV::color(1);
    CV::translate(100, 200); //desenha o objeto a partir da coordenada (20, 200)
    for (float xF = -limitX; xF < limitX; xF+=0.001) {
        float x = xF * escalaX;
        float y = (x * x * x) - (3 * x) + 2;
        float yF = y * escalaY;
        CV::point(xF, yF);
    }
    CV::translate(0, 0);
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
   CV::clear(1, 1, 1);

   //retanguloConfig();
   //circleConfig();
   //slider();
   polinomio();




   // --------------------------------------------------------------------
   //Codigo do professor
   /*//CV::rectFill(10, 10, 10, 10);
   float x[3] = {10, 130, 20};
   float y[3] = {10, 50, 100};
   //CV::triangleFill(x, y);
   DrawMouseScreenCoords();
   //bt->Render();
   //DesenhaLinhaDegrade();
   if( opcao == 49 ) //'1' -> relogio
   {
      r->anima();
   }
   if( opcao == '2' ) //50 -> bola
   {
      b->anima();
   }
   if( opcao == 51 ) //'3' -> senoide
   {
       DesenhaSenoide();
   }*/
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
       if (retangulo->rectBorda(mouseX, mouseY)) {
           retangulo->setArrast(mouseX, mouseY);
       }
       if (circulo->circBorda(mouseX, mouseY)) {
           circulo->setArrast(mouseX, mouseY);
       }
       if (sliderChoice->circBorda(mouseX,mouseY)) {
           sliderChoice->setArrast(mouseX, mouseY);
       }
       
   }
   if (state == 1) {
       pressMouse = false;
       retangulo->checaSelecaoRect(mouseX,mouseY);
       circulo->checaSelecaoCircle(mouseX, mouseY);
   }


}


int main(void)
{
   //b = new Bola();
   //r = new Relogio();
   //bt = new Botao(200, 400, 140, 50, "Sou um botao");
   retangulo = new Figuras(50, 100, 50, 100, 4);
   circulo = new Figuras(250, 100, 30, 4);
   sliderChoice = new Figuras(45, 220, 20);
   lista[0] = retangulo;
   lista[1] = circulo;
   lista[2] = sliderChoice;
   CV::init(&screenWidth, &screenHeight, "Demo Robson");
   CV::run();
}
