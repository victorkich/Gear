#ifndef __PANEL_H__
#define __PANEL_H__

#include "../Canvas/gl_canvas2d.h"

class Panel {
public:
    Panel() {}

    void Render(int mouseX, int mouseY) {
        CV::color(1, 1, 1);
        CV::text(1090, 630, "EDIT PANEL");
        CV::text(1090, 300, "VIEW PANEL");
        CV::text(450, 690, "PERSPECTIVE");
        CV::text(875, 165, "FRONT");
        CV::line(1000, 0, 1000, 720);
        CV::line(805, 0, 805, 200);
        CV::line(805, 200, 1000, 200);
        CV::line(0, 0, 1280, 0);
        CV::line(1280, 0, 1280, 720);
        CV::line(0, 720, 1280, 720);
        CV::line(0, 0, 0, 720);
        char str[100];
        if (mouseX < 1000) {
            sprintf(str, "X: %d  Y: %d", mouseX, mouseY);
            CV::text(830, 210, str);
        }
    }
};

#endif
