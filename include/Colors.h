#ifndef Color_h_
#define Color_h_

/***************** Hue to RGB Class ****************/

class hue2rgb {
 public:
  double R, G, B;
  /* h:0~hmax */
  hue2rgb(double h, double hmax);
};

inline hue2rgb::hue2rgb(double h, double hmax) {
  R = 1.0;
  G = 1.0;
  B = 1.0;
  double hue = h / hmax * 360.0;
  if (hue < 0)
    while (hue < 0) hue += 360;
  else if (360 < hue)
    while (360 < hue) hue -= 360;
  if (0 <= hue && hue < 60) {
    R = 1.0;
    G = hue / 60.0;
    B = 0.0;
  } else if (60 <= hue && hue < 120) {
    R = (120 - hue) / 60;
    G = 1.0;
    B = 0.0;
  } else if (120 <= hue && hue < 180) {
    R = 0.0;
    G = 1.0;
    B = (hue - 120) / 60;
  } else if (180 <= hue && hue < 240) {
    R = 0.0;
    G = (240 - hue) / 60;
    B = 1.0;
  } else if (240 <= hue && hue < 300) {
    R = (hue - 240) / 60;
    G = 0.0;
    B = 1.0;
  } else if (300 <= hue && hue <= 360) {
    R = 1.0;
    G = 0.0;
    B = (360 - hue) / 60;
  }
}

/**************************************************/
#endif  //"Color_h_"
