#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"

#define _CRT_SECURE_NO_WARNINGS

#pragma warning(disable:4996)

class Botao{
  float altura, largura, x, y;
  char label[100];
  int cor;
  bool press;
public:
  Botao(float _x, float _y, float _larg, float _alt, const char *_label, int _cor)
  {
     altura  = _alt;
     largura = _larg;
     x = _x;
     y = _y;
     cor = _cor;
     strcpy(label, _label);
     press = false;
  }

  const char* getLabel() {
      return label; 
  }

  bool getPress() {
      return press;
  }

  void setPress(bool set) {
      press = set;
  }

  void Render()
  {
      CV::color(cor);
      CV::rectFill(x, y, x + largura, y + altura);
      CV::color(1, 1, 1);
      CV::text(x+5, y+altura/2, label); //escreve o label do botao mais ou menos ao centro.
  }

  //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
  bool hitClick(int mx, int my)
  {
      if( mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura) )
      {
          press = true;
          return true;
      }
      return false;
  }
};

#endif
