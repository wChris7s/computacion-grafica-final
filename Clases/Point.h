#ifndef CUBORUBIK_POINT_H
#define CUBORUBIK_POINT_H

template<class T>
class Point {
public:
    T x;
    T y;
    T z;

    T c1;
    T c2;
    T c3;

    T t1;
    T t2;

    Point(T x, T y, T z, glm::vec3 color, T t1, T t2) {
      this->x = x;
      this->y = y;
      this->z = z;
      this->c1 = color.x;
      this->c2 = color.y;
      this->c3 = color.z;
      this->t1 = t1;
      this->t2 = t2;
    }
};

#endif //CUBORUBIK_POINT_H
