#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"


//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
double angle = 0;
float addy = 0, addx = 0;
float cont=0;
bool frente = false, tras = false, direita = false, esquerda = false;
int screenWidth = 500, screenHeight = 500; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}

Vector2 avaliaBezier(float t, Vector2 p1, Vector2 p2, Vector2 p3)
{
    float x_t = p1.x*pow(1-t, 2) + p2.x*(2*t*(1-t)) + p3.x*pow(t, 3);
    float y_t = p1.y*pow(1-t, 2) + p2.y*(2*t*(1-t)) + p3.y*pow(t, 3);
    Vector2 ret (x_t, y_t);
    return ret;
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
   //float x = mouseX - screenWidth/2;
   //float y = mouseY - screenHeight/2;

   //Vector2 v(x, y);
   //v.normalize();

   //Vector2 v2(1, 0);
   //v2.normalize();

   //float ang = v2.angulo(v) * 57;
   //char str[10];

   //sprintf(str, "%f", ang);
   //CV::text(50, 50, str);

   //v*100;

   //CV::translate(screenWidth/2, screenHeight/2);
   //CV::color(11);
   //CV::line(0,0, v.x*100, v.y*100);
   //CV::circle(v.x*100, v.y*100, 10, 30);

   if(frente)
   {
    addy+=2*cos(angle);
    addx+=2*sin(angle);
   }
   if(tras)
   {
    addy-=2*cos(angle);
    addx-=2*sin(angle);
   }
   if(direita)
   {
    angle-=0.02;
   }
   if(esquerda)
   {
    angle+=0.02;
   }

   int new_centre_height= round(((100)/2)-1);
   int new_centre_width= round(((50)/2)-1);

   CV::color(0.5, 0.5, 0);
   //CV::rectFill(0, 0, 100, 200);
   for(float y=0; y<100; y+=0.5)
   for(float x=0; x<50; x+=0.5)
   {
    double old_x = x-new_centre_width-1;
    double old_y = y-new_centre_height-1;

    int new_y = round(-old_x*sin(angle)+old_y*cos(angle));
    int new_x = round(old_x*cos(angle)+old_y*sin(angle));

    new_y = new_centre_height-new_y;
    new_x = new_centre_width-new_x;

    CV::point(new_x+addx, new_y+addy);
   }


   CV::translate(screenWidth/2, screenHeight/2);
   for(float t=0; t<1; t+=0.001)
   {
    CV::color(0, 0, 0);
    float y = pow((1-t), 3);
    CV::point(t*50, y*50);

    CV::color(1, 0, 0);
    y = 3*t*pow((1-t), 2);
    CV::point(t*50, y*50);

    CV::color(0, 1, 0);
    y = 3*pow(t,2)*(1-t);
    CV::point(t*50, y*50);

    CV::color(0, 0, 1);
    y = pow(t, 3);
    CV::point(t*50, y*50);
   }


   Vector2 p1 (100,100);
   Vector2 p2 (300,300);
   Vector2 p3 (500,50);
   Vector2 p4 (400,310);


   Vector2 vetor, vetor2;
   CV::translate(screenWidth/2, screenHeight/2-100);
   for(float t=0; t<1; t+=0.001)
   {
    CV::color(0.5, 0.5, 0.5);
    vetor = avaliaBezier(t, p1, p2, p3);
    CV::point(vetor.x, vetor.y);
    vetor = avaliaBezier(cont, p1, p2, p3);
    CV::circle(vetor.x, vetor.y, 10, 10);
    vetor = avaliaBezier(cont, p1, p2, p3);
    vetor2 = avaliaBezier(cont+0.001, p1, p2, p3);
    Vector2 top (vetor2.x - vetor.x, vetor2.y - vetor.y);
    top.normalize();
    CV::line(vetor.x, vetor.y, top.x*100+vetor.x, top.y*100+vetor.y);
   }

   cont += 0.001;


}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);

   if(key==201)
   {
    frente = true;
   }
   if(key==203)
   {
    tras = true;
   }
   if(key==200)
   {
    direita = true;
   }
   if(key==202)
   {
    esquerda = true;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);

   if(key==201)
   {
    frente = false;
   }
   if(key==203)
   {
    tras = false;
   }
   if(key==200)
   {
    direita = false;
   }
   if(key==202)
   {
    esquerda = false;
   }
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Canvas de Aula");
   CV::run();
}
