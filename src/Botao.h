#ifndef __BOTAO_H__
#define __BOTAO_H__

#include "gl_canvas2d.h"

class Botao {
    float altura, largura, x, y;
    char label[100];
    bool pressed = false;

public:
    Botao(float _x, float _y, float _larg, float _alt, char *_label) {
        altura = _alt;
        largura = _larg;
        x = _x;
        y = _y;
        strcpy(label, _label);
    }

    void updateStatus() {
        pressed = not pressed;
    }

    void Render(bool panel = false) {
        CV::color(1, 1, 1);
        if (pressed) {
            CV::rectFill(x, y, x + largura, y + altura);
            CV::color(0, 0, 0);
        } else
            CV::rect(x, y, x + largura, y + altura);
        CV::text(x + 5, y + altura / 2, label);

        if (panel){
            CV::color(1, 1, 1);
            CV::text(1115, 530, "PANEL");
            CV::line(1000, 0, 1000, 720);
        }
    }

    //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
    bool Colidiu(int mx, int my) {
        if (mx >= x && mx <= (x + largura) && my >= y && my <= (y + altura)) {
            return true;
        }
        return false;
    }

};

#endif
