#ifndef OPENGL_COLOR_H
#define OPENGL_COLOR_H

class Color {
private:
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
public:
    Color() = default;

    Color(float r, float g, float b);

    Color getColor() const;

    float getR() const;

    void setR(float r);

    float getG() const;

    void setG(float g);

    float getB() const;

    void setB(float b);

    int size() {
       return sizeof (Color) / sizeof(float);
    }
};

float Color::getR() const {
  return r;
}

void Color::setR(float r) {
  Color::r = r;
}

float Color::getG() const {
  return g;
}

void Color::setG(float g) {
  Color::g = g;
}

float Color::getB() const {
  return b;
}

void Color::setB(float b) {
  Color::b = b;
}

Color::Color(float r, float g, float b) {
  Color::r = r;
  Color::g = g;
  Color::b = b;
}

Color Color::getColor() const {
  return Color();
}

#endif //OPENGL_COLOR_H
