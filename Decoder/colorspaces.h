#ifndef color_spaces_h
#define color_spaces_h

#include <math.h>

struct xybP_t{
    double X;
    double Y;
    double B;
};
typedef struct xybP_t xybP;


struct pixel_cs_t{
    uint8_t c1;
    uint8_t c2;
    uint8_t c3;
};
typedef struct pixel_cs_t pixel_cs;


double notLinear(double color) {
    double x;
    if (color <= 0.798355) {
        x = 12.92 * (color / 255);
    }
    else {
        x = (1.055 * pow(color / 255, 0.41666666) - 0.055);
    }
    x = x * 255;
    if (x > 255)
        x = 255.0;
    else if (x < 0.0)
        x = 0.0;

    return x;
}


xybP xybtransform (double x, double y, double b) {  //estos numeros tienen que estar entre 0  1
    double bias = 0.00379307325527544933;
    double bias_crt = 0.15595420054924863;
    double  Lgamma = y + x;
    double Mgamma = y - x;
    double Sgamma = y - x + b;
    double Lmix = pow((Lgamma + bias_crt), 3) - bias;
    double Mmix = pow((Mgamma + bias_crt), 3) - bias;
    double Smix = pow((Sgamma + bias_crt), 3) - bias;
    double r = 11.031566901960783 * Lmix - 9.866943921568629 * Mmix - 0.16462299647058826 * Smix;
    double g = -3.254147380392157 * Lmix + 4.418770392156863 * Mmix - 0.16462299647058826 * Smix;
    double B = -3.6588512862745097 * Lmix + 2.7129230470588235 * Mmix + 1.9459282392156863 * Smix;
    
    return {r, g, B};
}


double clamp_double(double num, double superior, double inferior) {
    if (num > superior)
        return superior;
    if (num < inferior)
        return inferior;
    return num;
}


pixel_cs ycbcr2rgb(double y, double cb, double cr, int quality, xybP (*sistema) (double, double, double)) { //quality = 1, 2, 3, 4
    int multiplier = (quality) << 7;
    double r = (y + multiplier)                      +   1.402f       * cr;
    double g = (y + multiplier) - 0.344136f  * cb    -   0.714136f   * cr;
    double b = (y + multiplier) + 1.772f     * cb;
    
    r = clamp_double(r / quality, 255.0, 0.0);
    g = clamp_double(g / quality, 255.0, 0.0);
    b = clamp_double(b / quality, 255.0, 0.0);
    
    uint8_t c1 = uint8_t(r + (r >= 0 ? +0.5 : -0.5));
    uint8_t c2 = uint8_t(g + (g >= 0 ? +0.5 : -0.5));
    uint8_t c3 = uint8_t(b + (b >= 0 ? +0.5 : -0.5));
    
    return {c1, c2, c3};
}


xybP sistema256(double X, double Y, double B) {
    double xaux = (X - 89.8910660369987) /  917.7928292048051;
    double yaux = Y / 41.22609346538956;
    double baux = (B - 109.36763436215823) / 58.446562111877796;
    
    return {xaux, yaux, baux};
}


xybP sistema512(double X, double Y, double B) {
    double xaux = (X - 180.4871600429) /  1842.7840335406;
    double yaux = Y / 82.7755288403;
    double baux = (B - 219.5930540919) / 117.3515286325;

    return {xaux, yaux, baux};
}


xybP sistema1024(double X, double Y, double B) {
    double xaux = (X - 360.9743200858) /  3685.5680670813;
    double yaux = Y / 165.5510576806;
    double baux = (B - 439.1861081837) / 234.703057265;
    
    return {xaux, yaux, baux};
}


pixel_cs xyb2rgb(double x, double y, double b, int quality, xybP (*sistema) (double, double, double)) {
    xybP xyb = sistema(x, y, b);
    
    xyb = xybtransform(xyb.X, xyb.Y, xyb.B);
    xyb = {notLinear(xyb.X), notLinear(xyb.Y), notLinear(xyb.B)};
    //xyb = {clamp_double(xyb.X, 255.0, 0), clamp_double(xyb.Y, 255.0, 0), clamp_double(xyb.B, 255.0, 0)};
    int x1 = xyb.X + (xyb.X >= 0 ? +0.5f : -0.5f);
    int x2 = (xyb.Y + (xyb.Y >= 0 ? +0.5f : -0.5f));
    int x3 = (xyb.B + (xyb.B >= 0 ? +0.5f : -0.5f));
    
    if (x1 > 255)
        x1 = 255;
    if (x1 < 0)
        x1 = 0;
    if (x2 > 255)
        x2 = 255;
    if (x2 < 0)
        x2 = 0;
    if (x3 > 255)
        x3 = 255;
    if (x3 < 0)
        x3 = 0;
    
    return {(uint8_t)x1, (uint8_t)x2, (uint8_t)x3};
}

#endif

//Sistema 2048
//X = (X - 721.9486401717) /  7371.1361341625;
//Y = Y / 331.1021153612;
//B = (B - 878.3722163675) / 469.4061145299;
