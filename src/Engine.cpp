#include <math.h>
#include <GL/glut.h>
#include "gl_canvas2d.h"
#include "Vector3.h"

class Engine{
private:
  float r, r2, x, y, n, zangle = 0, xangle = PI/2, yangle = PI/2, add_angle, thick;
  struct { GLubyte red, green, blue; } pixel;

public:
  Engine(float _r, float _n, float _r2, float _x, float _y, float _add_angle, float _thick)
  {
     r = _r;
     n = _n;
     r2 = _r2;
     x = _x;
     y = _y;
     add_angle = _add_angle;
     thick = _thick;
  }

  void move(float _x, float _y){
      x = _x;
      y = _y;
  }

  float get_x(){
      return x;
  }

  float get_y(){
      return y;
  }

  float get_r(){
      return r;
  }

  void set_r(float _r){
      r = _r;
  }

  float get_n(){
      return n;
  }

  void set_n(float _n){
      n = _n;
  }

  float get_r2(){
    return r2;
  }

  void set_r2(float _r2){
      r2 = _r2;
  }

  float get_hangle(){
      return xangle;
  }

  void set_hangle(float _angle){
      xangle = _angle;
  }

  float get_vangle(){
        return yangle;
  }

  void set_vangle(float _angle){
        yangle = _angle;
  }

  float get_thick(){
      return thick;
  }

  void set_thick(float _thick){
      thick = _thick;
  }

  void Render(Engine *eng)
  {
      float xt, yt;
      xt = (x+eng->get_x())/2 + (x-(x+eng->get_x())/2)*sin(xangle);
      yt = (y+eng->get_y())/2 + (y-(y+eng->get_y())/2)*sin(yangle);;
      CV::translate(xt, yt);
      CV::color(1, 1, 1);

      //float max_x = 0, min_x = 10000, max_y = 0, min_y = 10000;
      float old_x, old_y, px, py, ra, aux = r / 5, px2, py2, po;
      bool first = true;
      for(float theta = 0; theta < PI_2; theta += 0.001){
          po = pow(sin(n * theta), 2);
          ra = r + round(po)*po*r2;
          px = ra * cos(theta);
          py = ra * sin(theta);
          px2 = (px*cos(zangle)-py*sin(zangle)) * sin(xangle);
          py2 = (px*sin(zangle)+py*cos(zangle)) * sin(yangle);
          if (first)
              first = false;
          else {
              CV::line(old_x, old_y, px2, py2);
              px = thick*cos(xangle);
              py = thick*cos(yangle);
              CV::line(old_x+px, old_y+py, px2+px, py2+py);
              if((po > 0.49 && po < 0.50) || po > 0.999)
                  CV::line(px2, py2, px2+px, py2+py);
          }
          old_x = px2;
          old_y = py2;
      }

      /*first = true;
      for(float theta = 0; theta < PI_2; theta += 0.001){
          po = pow(sin(n * theta), 2);
          ra = r + round(po)*po*r2;
          px = ra * cos(theta);
          py = ra * sin(theta);
          px2 = px*cos(angle) - py*sin(angle);
          py2 = px*sin(angle) + py*cos(angle);
          py2 = py2*cos(0.00001) - pz*sin(0.00001);
          pz = py2*sin(0.00001) + pz*cos(0.00001);
          px2 = (px2*d)/pz;
          py2 = (py2*d)/pz;
          if (first)
              first = false;
          else
              CV::line(old_x, old_y, px2, py2);
          old_x = px2;
          old_y = py2;
      }*/
      for(float theta = 0; theta < PI_2; theta += 0.01){
          px = aux * cos(theta);
          py = aux * sin(theta);
          px2 = (px*cos(zangle)-py*sin(zangle)) * sin(xangle);
          py2 = (px*sin(zangle)+py*cos(zangle)) * sin(yangle);
          CV::point(px2, py2);
          CV::point(px2+thick*cos(xangle), py2+thick*cos(yangle));
      }
      zangle += add_angle;

      /*
      if(max_x < px2)
          max_x = px;
      if(min_x > px2)
          min_x = px;
      if(max_y < py2)
          max_y = py;
      if(min_y > py2)
          min_y = py;

      CV::translate(0, 0);
      unsigned ma_x, mi_x, ma_y, mi_y;
      ma_x = unsigned(max_x + x);
      mi_x = unsigned(min_x + x);
      ma_y = unsigned(max_y + y);
      mi_y = unsigned(min_y + y);
      printf("\n%d, %d, %d, %d", ma_x, mi_x, ma_y, mi_y);

      bool painting, seq;
      for(unsigned gy = mi_y; gy < ma_y; gy++){
          painting = false;
          seq = false;
          for(unsigned gx = mi_x; gx < ma_x; gx++) {
              glReadPixels(gx, gy, 1, 2, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
              if (pixel.red && pixel.green && pixel.red){
                  if (!seq) {
                      seq = true;
                      painting = not painting;
                  }
              }else{
                  seq = false;
                  if(painting)
                      CV::point(float(gx), float(gy));
              }
          }
      }*/
  }
};