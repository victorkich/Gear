#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>

#include "Vector2.h"
#include "Vector3.h"
#include "gl_canvas2d.h"
#include "Fps.h"
#include "Engine.cpp"
#include "Botao.h"


//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
double angle = 0;
float addy = 0, addx = 0;
float cont=0;
bool frente = false, tras = false, direita = false, esquerda = false;
int screenWidth = 1280, screenHeight = 720; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
clock_t timer;

Fps *fps = nullptr;
Engine *eng = nullptr, *eng2 = nullptr;
Botao *less_tooth = nullptr, *more_tooth = nullptr, *less_radius = nullptr, *more_radius = nullptr, *less_tsize = nullptr, *more_tsize = nullptr;
Botao *less_hspin = nullptr, *more_hspin = nullptr, *less_vspin = nullptr, *more_vspin = nullptr, *less_thick = nullptr, *more_thick = nullptr;


void DrawMouseScreenCoords(){
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render(){
    // Set timer to calculate the FPS
    timer = clock();
    glClearColor(0, 0, 0, 0);

    // Gear renders
    eng->Render(eng2);
    eng2->Render(eng);

    // Button renders
    CV::translate(0, 0);
    less_tooth->Render();
    more_tooth->Render();
    less_radius->Render();
    more_radius->Render();
    less_tsize->Render();
    more_tsize->Render();
    less_hspin->Render();
    more_hspin->Render();
    less_vspin->Render();
    more_vspin->Render();
    less_thick->Render();
    more_thick->Render(true);

    // FPS render
    fps->Render(timer);
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key){
   printf("\nTecla: %d" , key);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key){
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y){
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    if (button == 0){
        if (state == 0){
            if (less_tooth->Colidiu(x, y)){
                less_tooth->updateStatus();
                eng->set_n(eng->get_n()-1);
                eng2->set_n(eng2->get_n()-1);
            } else if (more_tooth->Colidiu(x, y)){
                more_tooth->updateStatus();
                eng->set_n(eng->get_n()+1);
                eng2->set_n(eng2->get_n()+1);
            } else if (more_radius->Colidiu(x, y)){
                more_radius->updateStatus();
                eng->set_r(eng->get_r()+1);
                eng2->set_r(eng2->get_r()+1);
            } else if (less_radius->Colidiu(x, y)){
                less_radius->updateStatus();
                eng->set_r(eng->get_r()-1);
                eng2->set_r(eng2->get_r()-1);
            } else if (more_tsize->Colidiu(x, y)){
                more_tsize->updateStatus();
                eng->set_r2(eng->get_r2()+1);
                eng2->set_r2(eng2->get_r2()+1);
            } else if (less_tsize->Colidiu(x, y)){
                less_tsize->updateStatus();
                eng->set_r2(eng->get_r2()-1);
                eng2->set_r2(eng2->get_r2()-1);
            } else if (less_hspin->Colidiu(x, y)){
                less_hspin->updateStatus();
                eng->set_hangle(eng->get_hangle()-0.1);
                eng2->set_hangle(eng2->get_hangle()-0.1);
            } else if (more_hspin->Colidiu(x, y)){
                more_hspin->updateStatus();
                eng->set_hangle(eng->get_hangle()+0.1);
                eng2->set_hangle(eng2->get_hangle()+0.1);
            } else if (less_vspin->Colidiu(x, y)){
                less_vspin->updateStatus();
                eng->set_vangle(eng->get_vangle()-0.1);
                eng2->set_vangle(eng2->get_vangle()-0.1);
            } else if (more_vspin->Colidiu(x, y)){
                more_vspin->updateStatus();
                eng->set_vangle(eng->get_vangle()+0.1);
                eng2->set_vangle(eng2->get_vangle()+0.1);
            } else if (less_thick->Colidiu(x, y)){
                less_thick->updateStatus();
                eng->set_thick(eng->get_thick()-1);
                eng2->set_thick(eng2->get_thick()-1);
            } else if (more_thick->Colidiu(x, y)){
                more_thick->updateStatus();
                eng->set_thick(eng->get_thick()+1);
                eng2->set_thick(eng2->get_thick()+1);
            }
        } else if (state == 1){
            if (less_tooth->Colidiu(x, y))
                less_tooth->updateStatus();
            else if (more_tooth->Colidiu(x, y))
                more_tooth->updateStatus();
            else if (more_radius->Colidiu(x, y))
                more_radius->updateStatus();
            else if (less_radius->Colidiu(x, y))
                less_radius->updateStatus();
            else if (more_tsize->Colidiu(x, y))
                more_tsize->updateStatus();
            else if (less_tsize->Colidiu(x, y))
                less_tsize->updateStatus();
            else if (less_hspin->Colidiu(x, y))
                less_hspin->updateStatus();
            else if (more_hspin->Colidiu(x, y))
                more_hspin->updateStatus();
            else if (less_vspin->Colidiu(x, y))
                less_vspin->updateStatus();
            else if (more_vspin->Colidiu(x, y))
                more_vspin->updateStatus();
            else if (less_thick->Colidiu(x, y))
                less_thick->updateStatus();
            else if (more_thick->Colidiu(x, y))
                more_thick->updateStatus();
        }
    }
}

int main(){
   CV::init(&screenWidth, &screenHeight, "Canvas de Aula");

   eng = new Engine(70, 10, 20, 445, 295, 0.01, 25);
   eng2 = new Engine(70, 10, 20, 555, 425, -0.01, 25);
   fps = new Fps(30, 690);

   less_tooth = new Botao(1050, 450, 80, 40, "-Tooth");
   more_tooth = new Botao(1150, 450, 80, 40, "+Tooth");
   less_radius = new Botao(1050, 400, 80, 40, "-Radius");
   more_radius = new Botao(1150, 400, 80, 40, "+Radius");
   less_tsize = new Botao(1050, 350, 80, 40, "-T Size");
   more_tsize = new Botao(1150, 350, 80, 40, "+T Size");
   less_thick = new Botao(1050, 300, 80, 40, "-Thick");
   more_thick = new Botao(1150, 300, 80, 40, "+Thick");

   less_hspin = new Botao(1050, 250, 80, 40, "-H Spin");
   more_hspin = new Botao(1150, 250, 80, 40, "+H Spin");
   less_vspin = new Botao(1050, 200, 80, 40, "-V Spin");
   more_vspin = new Botao(1150, 200, 80, 40, "+V Spin");


   CV::run();
}
