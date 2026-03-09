#include "Relogio.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define POS_X 230
#define POS_Y 300

Relogio::Relogio()
{
   hora = 0;
}

void Relogio::anima()
{
   CV::clear(0,0,0);
   CV::color(1,0,0);

   float x = (cosf(hora) * 100);
   float y = (sinf(hora) * 100);

   CV::translate( Vector2(POS_X, POS_Y) );

   CV::circle(0, 0, 110, 15);
   CV::line(0, 0, x, y);
   hora += 0.001f;

   CV::translate(0, 0);
}

