#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <ctime>

#include "gl_canvas2d.h"
#include "Fps.h"
#include "Engine.cpp"
#include "Botao.h"
#include "Checkbox.h"


//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
double angle = 0;
float addy = 0, addx = 0;
float cont=0;
bool frente = false, tras = false, direita = false, esquerda = false;
int screenWidth = 1280, screenHeight = 720; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY, old_mouseX, old_mouseY; //variaveis globais do mouse para poder exibir dentro da render().
clock_t timer;

Fps *fps = nullptr;
Engine *eng = nullptr, *eng2 = nullptr;
Botao *less_tooth = nullptr, *more_tooth = nullptr, *less_radius = nullptr, *more_radius = nullptr, *less_tsize = nullptr, *more_tsize = nullptr;
Botao *less_speed = nullptr, *more_speed = nullptr, *less_thick = nullptr, *more_thick = nullptr;
Checkbox *cb_wide = nullptr;

bool moving = false;

void DrawMouseScreenCoords(){
    char str[100];
    sprintf(str, "X: %d  Y: %d", mouseX, mouseY);
    CV::text(830,210, str);
}

void render(){
    // Set timer to calculate the FPS
    timer = clock();
    glClearColor(0, 0, 0, 0);

    if (moving){
        eng->add(float(mouseX-old_mouseX), float(mouseY-old_mouseY));
        eng2->add(float(mouseX-old_mouseX), float(mouseY-old_mouseY));
    }

    // Checkbox renders
    CV::translate(0, 0);
    cb_wide->Render();

    // Gear renders
    eng->Render(eng2, cb_wide->getStatus());
    eng2->Render(eng, cb_wide->getStatus());

    // Button renders
    CV::translate(0, 0);
    less_tooth->Render();
    more_tooth->Render();
    less_radius->Render();
    more_radius->Render();
    less_tsize->Render();
    more_tsize->Render();
    less_speed->Render();
    more_speed->Render();
    less_thick->Render();
    more_thick->Render(true);

    DrawMouseScreenCoords();

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
            } else if (less_speed->Colidiu(x, y)){
                less_speed->updateStatus();
                eng->set_speed(float(eng->get_speed()-0.001));
                eng2->set_speed(float(eng2->get_speed()+0.001));
            } else if (more_speed->Colidiu(x, y)){
                more_speed->updateStatus();
                eng->set_speed(float(eng->get_speed()+0.001));
                eng2->set_speed(float(eng2->get_speed()-0.001));
            } else if (less_thick->Colidiu(x, y)){
                less_thick->updateStatus();
                eng->set_thick(eng->get_thick()-1);
                eng2->set_thick(eng2->get_thick()-1);
            } else if (more_thick->Colidiu(x, y)){
                more_thick->updateStatus();
                eng->set_thick(eng->get_thick()+1);
                eng2->set_thick(eng2->get_thick()+1);
            } else if(x > 0 && x < 1000 && y > 0 && y < 720){
                moving = true;
                old_mouseX = mouseX;
                old_mouseY = mouseY;
            } else if (cb_wide->Colidiu(x, y))  // Atualiza o status da checkbox
                cb_wide->updateStatus();
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
            else if (less_speed->Colidiu(x, y))
                less_speed->updateStatus();
            else if (more_speed->Colidiu(x, y))
                more_speed->updateStatus();
            else if (less_thick->Colidiu(x, y))
                less_thick->updateStatus();
            else if (more_thick->Colidiu(x, y))
                more_thick->updateStatus();
            else {
                moving = false;
                eng->save_status();
                eng2->save_status();
            }
        }
    }
}

int main(){
   CV::init(&screenWidth, &screenHeight, "Canvas de Aula");

   eng = new Engine(70, 10, 20, 445, 295, 0.005, 25);
   eng2 = new Engine(70, 10, 20, 555, 425, -0.005, 25);
   fps = new Fps(30, 690);

   less_tooth = new Botao(1050, 550, 80, 40, "-Tooth");
   more_tooth = new Botao(1150, 550, 80, 40, "+Tooth");
   less_radius = new Botao(1050, 500, 80, 40, "-Radius");
   more_radius = new Botao(1150, 500, 80, 40, "+Radius");
   less_tsize = new Botao(1050, 450, 80, 40, "-TSize");
   more_tsize = new Botao(1150, 450, 80, 40, "+TSize");
   less_thick = new Botao(1050, 400, 80, 40, "-Thick");
   more_thick = new Botao(1150, 400, 80, 40, "+Thick");
   less_speed = new Botao(1050, 350, 80, 40, "-Speed");
   more_speed = new Botao(1150, 350, 80, 40, "+Speed");
   cb_wide = new Checkbox(1050, 220, 40, 40, "Wideframe", true);

   CV::run();
}
