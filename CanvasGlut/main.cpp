/*********************************************************************
// Canvas
// Autor: Robson Daniel Marchesan
// Botões:
/* Load Imagem = Carrega a imagem configurada na string arquivo
*  Add Circ = Adiciona Circulo preenchido
*  Add Rect = Adiciona Retangulo preenchido
*  Remove Objeto = Remove o ultimo objeto adicionado ou carregado no caso da imagem
*  Rot 90 = Rotaciona a imagem ou retangulo em 90º no sentido horário
*  Lumin = Mostra o no histograma o grafico da luminancia
*  Vermelho, Verde ou Azul = mostra o grafico da sua respectiva cor no histograma
*  CheckBox(cinza) = Coloca a imagem no tom cinza
*  Slider = Manipula a escala da imagem ou figura selecionada, podendo ser usado para redimensionar a mesma
*  Obs: Tudo que for para alterar ou manipular os objetos precisa que voce clique na imagem antes de usar cada vez que for usar senão nao fará nada
*  A seleção funciona com um clique simples do botao esquerdo do mouse em cima da figura/imagem. Quando selecionado o objeto mostrará um contorno
* 
*/
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
int screenWidth = 800, screenHeight = 600;

char arquivo[] = { "tank.bmp" };
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
Botao* loadImagem;
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

void setInicio() {
    slid = new Slider(500, screenHeight - 150, 20, 10, 0, 0);
    imagem = new Objetos(arquivo);
    listaObjetos.push_back(slid->getCirc());
    //------------------------------------------------------
    loadImagem = new Botao(50, screenHeight - 50, 90, 30, "Load Img", 0);
    addCirc = new Botao(50, screenHeight - 100, 90, 30, "ADD Circ", 0);
    addRect = new Botao(50, screenHeight - 150, 90, 30, "ADD Rect", 0);
    removObj = new Botao(200, screenHeight - 50, 135, 30, "Remove Objeto", 0);
    rotaciona = new Botao(200, screenHeight - 100, 70, 30, "Rot 90º", 0);
    lumin = new Botao(200, screenHeight - 150, 60, 30, "Lumin", 0);
    vermelho = new Botao(350, screenHeight - 50, 85, 30, "Vermelho", 2);
    verde = new Botao(350, screenHeight - 100, 60, 30, "Verde", 3);
    azul = new Botao(350, screenHeight - 150, 60, 30, "Azul", 4);
    checkbox = new Botao(450, screenHeight - 100, 10, 10, "Cinza");
    listaBotao.push_back(checkbox);
    listaBotao.push_back(vermelho);
    listaBotao.push_back(verde);
    listaBotao.push_back(azul);
    listaBotao.push_back(lumin);
    listaBotao.push_back(rotaciona);
    listaBotao.push_back(addRect);
    listaBotao.push_back(addCirc);
    listaBotao.push_back(removObj);
    listaBotao.push_back(loadImagem);
}

void qualBotao() {
    if (addRect->getPress()) {
        listaObjetos.push_back(new Objetos(50 + (listaObjetos.size() * 5), 100, 50, 100, 5));
        addRect->alterna();
    }
    else if (addCirc->getPress()) {
        listaObjetos.push_back(new Objetos(250 + (listaObjetos.size() * 5), 100, 30, 5));
        addCirc->alterna();
    }
    else if (loadImagem->getPress()) {
        listaObjetos.push_back(imagem);
        loadImagem->alterna();
    }
    else if (rotaciona->getPress()) {
        for (auto* obj : listaObjetos) {
            if (obj->getSelecao()) {
                obj->rotacionar(-90);//- para sentido horario e + para sentido anti horario
            }
        }
        rotaciona->alterna();
    }
    else if (removObj->getPress()) {
        if (!listaObjetos.empty()) {
            Objetos* ultimo = listaObjetos.back();
            if (ultimo->getTipo() != 3) {
                //delete ultimo;
                listaObjetos.pop_back();
                removObj->alterna();
            }
        }
    }
}

void testArrasto() {
    if (!Objetos::checaListaArrasto(listaObjetos)) {
        Objetos::checaListaColisao(mouseX, mouseY, listaObjetos);
    }
}

void testSelecao() {
    for (auto* btn : listaBotao) {
        if (btn->hitClick(mouseX, mouseY)) {
            btn->alterna();
            qualBotao();
        }
    }
    for (auto* obj : listaObjetos) {
        obj->soltaArrast();
        if (obj->checaSelec(mouseX, mouseY)) {
            obj->setSelecao(true);
        }
        else {
            obj->setSelecao(false);
        }
    }
}

void desenhaBotoes() {
    for (auto* btn : listaBotao) {
        btn->Render();
    }
}

void desenhaObjetos() {
    float vEscala = slid->normaCirc();

    for (auto* obj : listaObjetos) {
        if (obj->getTipo() == 1) {
            obj->desenhaRect(vEscala);
            if (obj->getArrast()) {
                obj->drag(mouseX, mouseY);
            }
            if (pressTeclado) {
                obj->mexer(direcaoTeclado);
            }
        }
        else if (obj->getTipo() == 2) {
            obj->desenhaCircle(vEscala);
            if (obj->getArrast()) {
                obj->drag(mouseX, mouseY);
            }
            if (pressTeclado) {
                obj->mexer(direcaoTeclado);
            }
        }
        else if (obj->getTipo() == 3) {
            slid->barraDeslize();
            slid->circSeleciona(mouseX, mouseY, pressMouse, listaObjetos);
            //slid->retanguloDegrade(slid->normaCirc());
        }
        else if (obj->getTipo() == 4) {
            imagem->editImagem(slid->normaCirc(), mouseX, mouseY, checkbox->getPress());
            imagem->desenhaHistograma(500, 680, 256, 100, listaBotao);
            if (pressTeclado) {
                obj->mexer(direcaoTeclado);
            }
        }
    }
    
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
void render()
{
   CV::clear(1, 1, 1);
   CV::color(11);
   CV::rectFill(0, 400, screenWidth, screenHeight);//fundo dos botoes
   desenhaBotoes();
   desenhaObjetos();
   
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
       testArrasto();
   }
   if (state == 1) {
       pressMouse = false;
       testSelecao();
   }


}


int main(void){
    setInicio();
    CV::init(&screenWidth, &screenHeight, "Demo Robson");
    CV::run();
}
