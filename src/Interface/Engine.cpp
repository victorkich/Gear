#include <math.h>
#include "../Canvas/gl_canvas2d.cpp"

class Engine {
private:
    float r, r2, x, y, n, zangle = 0, xangle = PI / 2, yangle = PI / 2, add_zangle, add_xangle, add_yangle, thick;

public:
    Engine(float _r, float _n, float _r2, float _x, float _y, float _add_zangle, float _thick) {
        r = _r;
        n = _n;
        r2 = _r2;
        x = _x;
        y = _y;
        add_zangle = _add_zangle;
        thick = _thick;
    }

    void reset_angle(){
        xangle = PI / 2;
        yangle = PI / 2;
    }

    void add(float add_x, float add_y) {
        add_xangle = float(add_x * 0.01);
        add_yangle = float(add_y * 0.01);
    }

    void save_status() {
        xangle = xangle + add_xangle;
        yangle = yangle + add_yangle;
        add_xangle = 0;
        add_yangle = 0;
    }

    float get_x() {
        return x;
    }

    float get_y() {
        return y;
    }

    float get_r() {
        return r;
    }

    void set_r(float _r, Engine *eng) {
        float aux_x = 0.60, aux_y = 0.70;
        if (r > _r) {
            if (x > eng->get_x()) {
                x -= aux_x;
                y -= aux_y;
            } else {
                x += aux_x;
                y += aux_y;
            }
        } else {
            if (x > eng->get_x()) {
                x += aux_x;
                y += aux_y;
            } else {
                x -= aux_x;
                y -= aux_y;
            }
        }
        r = _r;
    }

    float get_n() {
        return n;
    }

    void set_n(float _n, Engine *eng) {
        float aux_x = 0.45 * _n, aux_y = -0.35 * _n;
        if (int(_n) % 2) {
            if (x > eng->get_x()) {
                x -= aux_x;
                y -= aux_y;
            } else {
                x += aux_x;
                y += aux_y;
            }
        } else {
            if (x > eng->get_x()) {
                x += aux_x;
                y += aux_y;
            } else {
                x -= aux_x;
                y -= aux_y;
            }
        }
        n = _n;
    }

    float get_r2() {
        return r2;
    }

    void set_r2(float _r2, Engine *eng) {
        float aux_x = 0.45, aux_y = 0.5;
        if (r2 > _r2) {
            if (x > eng->get_x()) {
                x -= aux_x;
                y -= aux_y;
            } else {
                x += aux_x;
                y += aux_y;
            }
        } else {
            if (x > eng->get_x()) {
                x += aux_x;
                y += aux_y;
            } else {
                x -= aux_x;
                y -= aux_y;
            }
        }
        r2 = _r2;
    }

    float get_speed() {
        return add_zangle;
    }

    void set_speed(float new_speed, Engine *eng) {
        add_zangle = new_speed;
    }

    float get_thick() {
        return thick;
    }

    void set_thick(float _thick, Engine *eng) {
        thick = _thick;
    }

    void update_zangle() {
        zangle += add_zangle;
        if (zangle > PI_2 || zangle < -PI_2)
            zangle = 0;
    }

    void render_miniview() {
        CV::translate(float(x * 0.3 + 750), float(y * 0.3 - 20));
        float old_x, old_y, px, py, px2, py2, po, ra, aux = r / 5;
        bool first = true;

        for (float theta = 0; theta < PI_2; theta += 0.02) {
            po = pow(sin(n * theta), 2);
            if (po > 0.9)
                po = 0.9;
            ra = r + round(po) * po * r2;
            px = ra * cos(theta);
            py = ra * sin(theta);
            px2 = float((px * cos(zangle) - py * sin(zangle)) * 0.3);
            py2 = float((px * sin(zangle) + py * cos(zangle)) * 0.3);
            if (first)
                first = false;
            else
                CV::line(old_x, old_y, px2, py2);
            old_x = px2;
            old_y = py2;
        }

        for (float theta = 0; theta < PI_2; theta += 0.3) {
            px = aux * cos(theta);
            py = aux * sin(theta);
            px2 = float((px * cos(zangle) - py * sin(zangle)) * 0.3);
            py2 = float((px * sin(zangle) + py * cos(zangle)) * 0.3);
            CV::point(px2, py2);
        }
    }

    void Render(Engine *eng, bool axe) {
        CV::color(1, 1, 1);
        float alt_xangle, alt_yangle, zx, zy, xt, yt, old_x, old_y, px, py, px2, py2, po, ra, aux = r / 5;
        bool first = true;

        alt_xangle = xangle + add_xangle;
        alt_yangle = yangle + add_yangle;

        if (axe)
            alt_xangle = PI/2;
        else
            alt_yangle = PI/2;

        zx = thick * cos(alt_xangle);
        zy = thick * cos(alt_yangle);
        xt = (x + eng->get_x()) / 2 + (x - (x + eng->get_x()) / 2) * sin(alt_xangle);
        yt = (y + eng->get_y()) / 2 + (y - (y + eng->get_y()) / 2) * sin(alt_yangle);
        CV::translate(xt, yt);

        for (float theta = 0; theta < PI_2; theta += 0.001) {
            po = pow(sin(n * theta), 2);
            if (po > 0.9)
                po = 0.9;
            ra = r + round(po) * po * r2;
            px = ra * cos(theta);
            py = ra * sin(theta);
            px2 = (px * cos(zangle) - py * sin(zangle)) * sin(alt_xangle);
            py2 = (px * sin(zangle) + py * cos(zangle)) * sin(alt_yangle);
            if (first)
                first = false;
            else {
                CV::line(old_x, old_y, px2, py2);
                CV::line(old_x + zx, old_y + zy, px2 + zx, py2 + zy);
                if ((po > 0.49 && po < 0.50) || (po < 0.89 && po > 0.88))
                    CV::line(px2, py2, px2 + zx, py2 + zy);
            }
            old_x = px2;
            old_y = py2;
        }

        for (float theta = 0; theta < PI_2; theta += 0.02) {
            px = aux * cos(theta);
            py = aux * sin(theta);
            px2 = (px * cos(zangle) - py * sin(zangle)) * sin(alt_xangle);
            py2 = (px * sin(zangle) + py * cos(zangle)) * sin(alt_yangle);
            CV::point(px2, py2);
            CV::point(px2 + zx, py2 + zy);
        }

        render_miniview();
        update_zangle();
    }
};