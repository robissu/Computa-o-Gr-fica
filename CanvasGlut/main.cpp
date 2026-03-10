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
Bola    *b = NULL;
Relogio *r = NULL;
Botao   *bt = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
int opcao  = 50;//variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
bool pressMouse = false;
bool pressTeclado = false;
int direcaoTeclado = -1;

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



//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
   
   CV::clear(0, 0, 0);
   retangulo->desenhaRect();
   retangulo->colisaoRect(mouseX,mouseY, pressMouse);
   retangulo->drag(mouseX, mouseY);
   if (pressTeclado) {
       retangulo->mexer(direcaoTeclado);
   }

   
   //arrastaRect();
   //colisaoRect();
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
   printf("\nTecla: %d" , key);
   pressTeclado = true;
   if( key < 200 )
   {
      opcao = key;
   }

   if (pressTeclado) {
       switch (key)
       {
       case 27:
           exit(0);
           break;

           //seta para a esquerda
       case 200:
           direcaoTeclado = 0;
           break;

           //cima
       case 201:
           direcaoTeclado = 1;
           break;

           //seta para a direita
       case 202:
           direcaoTeclado = 2;
           break;

           //baixo
       case 203:
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
   printf("\nLiberou: %d" , key);
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

   if (state == 0) {
       pressMouse = true;
       retangulo->setDist(mouseX,mouseY);
   }
   if (state == 1) {
       pressMouse = false;
   }


}


int main(void)
{
   //b = new Bola();
   //r = new Relogio();
   //bt = new Botao(200, 400, 140, 50, "Sou um botao");
   retangulo = new Figuras(50, 100, 50, 100, 4);
   CV::init(&screenWidth, &screenHeight, "Demo Robson");
   CV::run();
}
