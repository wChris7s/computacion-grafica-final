#ifndef CUBORUBIK_RUBIK_H
#define CUBORUBIK_RUBIK_H

#include <random>
#include "Operations.h"
#include "CuboRubik.h"
#include "../External/CUBE/random.h"
#include "../External/CUBE/solve.h"

class Rubik {
    GLFWwindow *window;
    Shader *shader;
    std::vector<CuboRubik> cubosRubik;
    float dim;
    std::unordered_map<char, std::vector<int>> parts; // Almacena las partes del cubo por cara
    float lejania = 0.005f;

public:
    float degrees;  // Grados de rotación
    vector<string> shuffle;
    vector<string> solution;

    Rubik(GLFWwindow *_window, Shader *_shader, float _dim = 0.1f) {
      // Inicialización de ventana y shader
      window = _window;
      shader = _shader;
      dim = _dim;
      degrees = -1.0f;

      float pos = dim * 2 + (dim / 4);  //Positivo
      float neg = -1 * (pos);           //Negativo

      // Definición de posiciones de los cubos en el espacio tridimensional
      // Cada posición representa una parte del cubo
      glm::vec3 cubePositions[] = {
       glm::vec3(neg, pos, pos),
       glm::vec3(0.0f, pos, pos),
       glm::vec3(pos, pos, pos),
       glm::vec3(neg, 0.0f, pos),
       glm::vec3(0.0f, 0.0f, pos),
       glm::vec3(pos, 0.0f, pos),
       glm::vec3(neg, neg, pos),
       glm::vec3(0.0f, neg, pos),
       glm::vec3(pos, neg, pos),

       glm::vec3(neg, pos, 0.0f),
       glm::vec3(0.0f, pos, 0.0f),
       glm::vec3(pos, pos, 0.0f),
       glm::vec3(neg, 0.0f, 0.0f),
       glm::vec3(pos, 0.0f, 0.0f),
       glm::vec3(neg, neg, 0.0f),
       glm::vec3(0.0f, neg, 0.0f),
       glm::vec3(pos, neg, 0.0f),

       glm::vec3(neg, pos, neg),
       glm::vec3(0.0f, pos, neg),
       glm::vec3(pos, pos, neg),
       glm::vec3(neg, 0.0f, neg),
       glm::vec3(0.0f, 0.0f, neg),
       glm::vec3(pos, 0.0f, neg),
       glm::vec3(neg, neg, neg),
       glm::vec3(0.0f, neg, neg),
       glm::vec3(pos, neg, neg)
      };

      // Definición de colores para cada parte del cubo
      glm::vec3 colors[] = {
       glm::vec3(0.0f, 0.0f, 0.0f),      // Negro
       glm::vec3(1.0f, 1.0f, 1.0f),      // Blanco
       glm::vec3(1.0f, 0.5f, 0.0f),      // Naranja
       glm::vec3(1.0f, 1.0f, 0.0f),      // Amarillo
       glm::vec3(1.0f, 0.025f, 0.25f),   // Rojo
       glm::vec3(0.224f, 1.0f, 0.078f),  // Verde
       glm::vec3(0.15f, 0.35f, 1.0f)     // Azul
      };

      // Asignación de colores a cada parte del cubo según su posición
      int assignColor[][6] = {
       {4, 1, 5, 0, 0, 0},  //Frontal
       {4, 1, 0, 0, 0, 0},
       {4, 1, 0, 0, 6, 0},
       {4, 0, 5, 0, 0, 0},
       {4, 0, 0, 0, 0, 0},
       {4, 0, 0, 0, 6, 0},
       {4, 0, 5, 3, 0, 0},
       {4, 0, 0, 3, 0, 0},
       {4, 0, 0, 3, 6, 0},
       {0, 1, 5, 0, 0, 0},  //Medio
       {0, 1, 0, 0, 0, 0},
       {0, 1, 0, 0, 6, 0},
       {0, 0, 5, 0, 0, 0},
       {0, 0, 0, 0, 6, 0},
       {0, 0, 5, 3, 0, 0},
       {0, 0, 0, 3, 0, 0},
       {0, 0, 0, 3, 6, 0},
       {0, 1, 5, 0, 0, 2}, //Trasera
       {0, 1, 0, 0, 0, 2},
       {0, 1, 0, 0, 6, 2},
       {0, 0, 5, 0, 0, 2},
       {0, 0, 0, 0, 0, 2},
       {0, 0, 0, 0, 6, 2},
       {0, 0, 5, 3, 0, 2},
       {0, 0, 0, 3, 0, 2},
       {0, 0, 0, 3, 6, 2}
      };

      // Mapeo de las partes del cubo a las caras correspondientes
      parts.insert({'F', std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8}});
      parts.insert({'B', std::vector<int>{19, 18, 17, 22, 21, 20, 25, 24, 23}});
      parts.insert({'L', std::vector<int>{17, 9, 0, 20, 12, 3, 23, 14, 6}});
      parts.insert({'R', std::vector<int>{2, 11, 19, 5, 13, 22, 8, 16, 25}});
      parts.insert({'U', std::vector<int>{17, 18, 19, 9, 10, 11, 0, 1, 2}});
      parts.insert({'D', std::vector<int>{6, 7, 8, 14, 15, 16, 23, 24, 25}});

      parts.insert({'Z', std::vector<int>{0, 1, 2, 11, 19, 18, 17, 9, 10}});
      parts.insert({'X', std::vector<int>{3, 5, 4, 13, 22, 21, 20, 12}});
      parts.insert({'C', std::vector<int>{6, 7, 8, 16, 25, 24, 23, 14, 15}});


      parts.insert({'H', std::vector<int>{0, 1, 2, 11, 19, 18, 17, 9}});
      parts.insert({'J', std::vector<int>{3, 5, 22, 20}});
      parts.insert({'K', std::vector<int>{6, 7, 8, 16, 25, 24, 23, 14}});

      // Creación de los cubos del cubo de Rubik con sus posiciones y colores
      std::vector<glm::vec3> cubeColor;
      for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 6; j++) {
          cubeColor.push_back(colors[assignColor[i][j]]);
        }
        cubosRubik.emplace_back(dim, cubePositions[i], cubeColor);
        cubeColor.clear();
      }

      // Generación de buffers para los cubos
      genBuffers();
    }

    void draw();

    void updateCurrentPart(char);

    void updateParts(char);

    void move(char);

    void deleteBuffers();

    void setSolve();

    void fillShuffle(char sideMove);

    void explosion();

    void implosion();

    void crumble();

    void reset_cube();

    void randomLayerMovement();

    void jumpAnimation(float deltaTime);

    Rubik deepCopy() const;

    void RotarCubo();

private:

    void genBuffers();

    void solve(vector<string> vector1);
};


void Rubik::genBuffers() {
  for (auto &cubito: cubosRubik) {
    cubito.genBuffers();
  }
}

// Dibuja el cubo de Rubik en la ventana utilizando OpenGL
void Rubik::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia el buffer de color y profundidad
  for (auto &cubito: cubosRubik) {
    cubito.draw(shader); // Dibuja cada cubo utilizando el shader proporcionado
  }
  glfwSwapBuffers(window); // Intercambia los buffers de la ventana
  glfwPollEvents(); // Procesa eventos de la ventana
}

void Rubik::fillShuffle(char sideMove) { // guarda los movimientos para el solver
  if (degrees == -1) { // guarda los movimiendos normales
    shuffle.push_back(string(1, sideMove));
    cout << sideMove << " ";
  } else { // guarda movimientos prima
    string s(1, sideMove);
    s.push_back('\'');
    shuffle.push_back(s);
    cout << s << " ";
  }
}


// Actualiza la posición de las partes del cubo actual tras un movimiento específico
void Rubik::updateCurrentPart(char p) {
  std::vector<int> *pv = &(parts.find(p)->second); // Obtiene un puntero a la parte del cubo especificada
  std::vector<int> temp = (*pv); // Almacena temporalmente los valores actuales
  for (int i = 0; i < 3; i++) {
    (*pv)[i] = temp[6 - (i * 3)]; // Actualiza las posiciones de las partes del cubo según el movimiento
    (*pv)[i + 3] = temp[10 - ((i + 1) * 3)];
    (*pv)[i + 6] = temp[i + 12 - ((i + 1) * 4)];
  }
}

// Actualiza las partes afectadas por el movimiento de una cara específica
void Rubik::updateParts(char movedChoosen) {
  std::vector<int> *pMoved = &(parts.find(movedChoosen)->second); // Obtiene las partes afectadas por el movimiento
  std::vector<char> updateSides; // Lista de caras a actualizar
  std::vector<std::vector<int>> updateIndex; // Índices de las partes a actualizar en cada cara
  switch (movedChoosen) {
    case 'F':
      updateSides = std::vector<char>{'U', 'R', 'D', 'L'};
      updateIndex = std::vector<std::vector<int>>{{6, 7, 8},
                                                  {0, 3, 6},
                                                  {0, 1, 2},
                                                  {2, 5, 8}}; // indices de cada cara, hay 9
      break;
    case 'B':
      updateSides = std::vector<char>{'U', 'L', 'D', 'R'};       //Actualizar desde B
      updateIndex = std::vector<std::vector<int>>{{2, 1, 0},
                                                  {0, 3, 6},
                                                  {8, 7, 6},
                                                  {2, 5, 8}};
      break;
    case 'L':
      updateSides = std::vector<char>{'U', 'F', 'D', 'B'};
      updateIndex = std::vector<std::vector<int>>{{0, 3, 6},
                                                  {0, 3, 6},
                                                  {6, 3, 0},
                                                  {2, 5, 8}};
      break;
    case 'R':
      updateSides = std::vector<char>{'U', 'B', 'D', 'F'};
      updateIndex = std::vector<std::vector<int>>{{8, 5, 2},
                                                  {0, 3, 6},
                                                  {2, 5, 8},
                                                  {2, 5, 8}};
      break;
    case 'D':
      updateSides = std::vector<char>{'F', 'R', 'B', 'L'};
      updateIndex = std::vector<std::vector<int>>{{6, 7, 8},
                                                  {6, 7, 8},
                                                  {8, 7, 6},
                                                  {8, 7, 6}};
      break;
    case 'U':
      updateSides = std::vector<char>{'B', 'R', 'F', 'L'};
      updateIndex = std::vector<std::vector<int>>{{2, 1, 0},
                                                  {2, 1, 0},
                                                  {0, 1, 2},
                                                  {0, 1, 2}};
      break;
    default:
      break;
  }
  updateCurrentPart(movedChoosen); // Actualiza la parte del cubo que se ha movido
  std::vector<std::vector<int>> sortedIndex = {{0, 1, 2},
                                               {2, 5, 8},
                                               {6, 7, 8},
                                               {0, 3, 6}}; // Índices ordenados
  std::vector<int> *side_updating; // Cara que se está actualizando
  // Actualiza las caras afectadas por el movimiento
  for (int i = 0; i < 4; i++) {
    side_updating = &(parts.find(updateSides[i])->second); // Obtiene la cara a actualizar
    for (int j = 0; j < 3; j++) {
      // Actualiza las partes de la cara con los valores de las partes movidas
      (*side_updating)[updateIndex[i][j]] = (*pMoved)[sortedIndex[i][j]];
    }
  }
}

// Realiza el movimiento de una cara del cubo de Rubik
void Rubik::move(char sideMove) {
  std::vector<int> *pv = &(parts.find(sideMove)->second); // Obtiene las partes de la cara a mover
  for (int k = 0; k < 90; k++) { // Realiza un giro de 90 grados
    for (int j = 0; j < pv->size(); j++) { // Realiza el movimiento en cada una de las partes de la cara
      cubosRubik[(*pv)[j]].move(cubosRubik[(*pv)[4]].pos, shader, degrees); // Mueve las partes del cubo
    }
    draw(); // Dibuja la animación del movimiento en lugar de hacer un giro instantáneo
  }
}

// Elimina los buffers de los cubos del cubo de Rubik
void Rubik::deleteBuffers() {
  for (auto &i: cubosRubik) {
    i.deleteBuffers(); // Elimina los buffers de cada cubo individualmente
  }
}


void Rubik::setSolve() {
  vector<string> move = get_solution(to_cube_not(shuffle));
  cout << "\nSolucion: ";
  for (int i = 0; i < move.size(); i++) cout << move[i] << " ";
  solve(move);
  shuffle.clear();
}

void Rubik::solve(vector<string> sol) {
  solution = sol;
  char sideMoved;
  degrees = -1.0f;
  for (int i = 0; i < solution.size(); i++) {
    if (i > 1) {
      std::cout << "\nSolucion: ";
      for (int j = i; j < solution.size(); j++) std::cout << solution[j] << " ";
    }
    if (solution[i].size() == 1) {
      sideMoved = solution[i].c_str()[0];
      move(sideMoved);
      updateParts(sideMoved);
    } else if (solution[i].size() == 2) {
      if (solution[i][1] == '\'') {
        degrees = 1.0f;
        sideMoved = solution[i].c_str()[0];
        move(sideMoved);
        updateParts(sideMoved);
        updateParts(sideMoved);
        updateParts(sideMoved);
        degrees = -1.0f;
      } else {
        sideMoved = solution[i].c_str()[0];
        move(sideMoved);
        updateParts(sideMoved);
        move(sideMoved);
        updateParts(sideMoved);
      }
    }
  }
}

void Rubik::explosion() {
  vector<int> pv = {0, 2, 6, 8, 17, 19, 23, 25, 1, 9, 11, 18, 7, 14, 16, 24, 3, 4, 5, 12, 13, 20, 21, 22, 10, 15};
  for (int k = 0; k < 500; k++) {
    for (int j : pv) {
      if (j == 0) cubosRubik[j].trans(glm::vec3(-lejania, lejania, lejania), shader);
      else if (j == 2) cubosRubik[j].trans(glm::vec3(lejania, lejania, lejania), shader);
      else if (j == 6) cubosRubik[j].trans(glm::vec3(-lejania, -lejania, lejania), shader);
      else if (j == 8) cubosRubik[j].trans(glm::vec3(lejania, -lejania, lejania), shader);
      else if (j == 17) cubosRubik[j].trans(glm::vec3(-lejania, lejania, -lejania), shader);
      else if (j == 19) cubosRubik[j].trans(glm::vec3(lejania, lejania, -lejania), shader);
      else if (j == 23) cubosRubik[j].trans(glm::vec3(-lejania, -lejania, -lejania), shader);
      else if (j == 25) cubosRubik[j].trans(glm::vec3(lejania, -lejania, -lejania), shader);
        // CENTROS
      else if (j == 1) cubosRubik[j].trans(glm::vec3(0.0f, lejania, lejania), shader);
      else if (j == 9) cubosRubik[j].trans(glm::vec3(-lejania, lejania, 0.0f), shader);
      else if (j == 11) cubosRubik[j].trans(glm::vec3(lejania, lejania, 0.0f), shader);
      else if (j == 18) cubosRubik[j].trans(glm::vec3(0.0f, lejania, -lejania), shader);
      else if (j == 7) cubosRubik[j].trans(glm::vec3(0.0f, -lejania, lejania), shader);
      else if (j == 14) cubosRubik[j].trans(glm::vec3(-lejania, -lejania, 0.0f), shader);
      else if (j == 16) cubosRubik[j].trans(glm::vec3(lejania, -lejania, 0.0f), shader);
      else if (j == 24) cubosRubik[j].trans(glm::vec3(0.0f, -lejania, -lejania), shader);

        // MEDIO
      else if (j == 3) cubosRubik[j].trans(glm::vec3(-lejania, 0.0f, lejania), shader);
      else if (j == 4) cubosRubik[j].trans(glm::vec3(0.0f, 0.0f, lejania), shader);
      else if (j == 5) cubosRubik[j].trans(glm::vec3(lejania, 0.0f, lejania), shader);
      else if (j == 12) cubosRubik[j].trans(glm::vec3(-lejania, 0.0f, 0.0f), shader);
      else if (j == 13) cubosRubik[j].trans(glm::vec3(lejania, 0.0f, 0.0f), shader);
      else if (j == 20) cubosRubik[j].trans(glm::vec3(-lejania, 0.0f, -lejania), shader);
      else if (j == 21) cubosRubik[j].trans(glm::vec3(0.0f, 0.0f, -lejania), shader);
      else if (j == 22) cubosRubik[j].trans(glm::vec3(lejania, 0.0f, -lejania), shader);

        // CABEZA Y PIE
      else if (j == 10) cubosRubik[j].trans(glm::vec3(0.0f, lejania, 0.0f), shader);
      else if (j == 15) cubosRubik[j].trans(glm::vec3(0.0f, -lejania, 0.0f), shader);
    }
    draw();
  }
}

void Rubik::implosion() {
  vector<int> pv = {0,2,6,8,17,19,23,25,1,9,11,18,7,14,16,24,3,4,5,12,13,20,21,22,10,15};

  for (int k = 0; k < 500; k++) { // 90 grados
    for (int j : pv) { // size 9 roto los 9 cubos 1 grado 90 veces
      if(j==0) cubosRubik[j].trans(glm::vec3(lejania, -lejania ,-lejania), shader);      //ESQUINAS
      else if(j==2) cubosRubik[j].trans(glm::vec3(-lejania, -lejania ,-lejania), shader);
      else if(j==6) cubosRubik[j].trans(glm::vec3(lejania, lejania ,-lejania), shader);
      else if(j==8) cubosRubik[j].trans(glm::vec3(-lejania, lejania ,-lejania), shader);
      else if(j==17) cubosRubik[j].trans(glm::vec3(lejania, -lejania ,lejania), shader);
      else if(j==19) cubosRubik[j].trans(glm::vec3(-lejania, -lejania ,lejania), shader);
      else if(j==23) cubosRubik[j].trans(glm::vec3(lejania, lejania ,lejania), shader);
      else if(j==25) cubosRubik[j].trans(glm::vec3(-lejania, lejania ,lejania), shader);
        // CENTROS
      else if(j==1) cubosRubik[j].trans(glm::vec3(0.0f, -lejania ,-lejania), shader);
      else if(j==9) cubosRubik[j].trans(glm::vec3(lejania, -lejania ,0.0f), shader);
      else if(j==11) cubosRubik[j].trans(glm::vec3(-lejania, -lejania ,0.0f), shader);
      else if(j==18) cubosRubik[j].trans(glm::vec3(0.0f, -lejania ,lejania), shader);
      else if(j==7) cubosRubik[j].trans(glm::vec3(0.0f, lejania ,-lejania), shader);
      else if(j==14) cubosRubik[j].trans(glm::vec3(lejania, lejania ,0.0f), shader);
      else if(j==16) cubosRubik[j].trans(glm::vec3(-lejania, lejania ,0.0f), shader);
      else if(j==24) cubosRubik[j].trans(glm::vec3(0.0f, lejania ,lejania), shader);

        // MEDIO
      else if(j==3) cubosRubik[j].trans(glm::vec3(lejania, 0.0f ,-lejania), shader);
      else if(j==4) cubosRubik[j].trans(glm::vec3(0.0f, 0.0f ,-lejania), shader);
      else if(j==5) cubosRubik[j].trans(glm::vec3(-lejania, 0.0f ,-lejania), shader);
      else if(j==12) cubosRubik[j].trans(glm::vec3(lejania, 0.0f ,0.0f), shader);
      else if(j==13) cubosRubik[j].trans(glm::vec3(-lejania, 0.0f ,0.0f), shader);
      else if(j==20) cubosRubik[j].trans(glm::vec3(lejania, 0.0f ,lejania), shader);
      else if(j==21) cubosRubik[j].trans(glm::vec3(0.0f, 0.0f ,lejania), shader);
      else if(j==22) cubosRubik[j].trans(glm::vec3(-lejania, 0.0f ,lejania), shader);

        // CABEZA Y PIE
      else if(j==10) cubosRubik[j].trans(glm::vec3(0.0f, -lejania ,0.0f), shader);
      else if(j==15) cubosRubik[j].trans(glm::vec3(0.0f, lejania ,0.0f), shader);
    }
    draw(); // dibuja la animacion, en vez de hacer el giro de golpe
  }
}

void Rubik::crumble() {
  for (auto &cubito: cubosRubik) {
    // Generar dirección y distancia aleatorias
    float dx = getRandom(-0.04f, 0.04f);
    float dy = getRandom(-0.04f, 0.04f);
    float dz = getRandom(-0.04f, 0.04f);

    // Crear un vector de dirección
    glm::vec3 direction(dx, dy, dz);

    // Mover el cubo en la dirección generada
    cubito.pos += direction;
  }
}

void Rubik::reset_cube() {
  for (auto &cubito: cubosRubik) {
    cubito.pos = cubito.originalPos;
  }
}

void Rubik::randomLayerMovement() {
  int randomLayer = getRandom(0, 5);
  char layer;
  switch (randomLayer) {
    case 0:
      layer = 'F';
      break;
    case 1:
      layer = 'B';
      break;
    case 2:
      layer = 'L';
      break;
    case 3:
      layer = 'R';
      break;
    case 4:
      layer = 'U';
      break;
    case 5:
      layer = 'D';
      break;
  }
  fillShuffle(layer);
  move(layer); // Mueve la camada 90°
  if (degrees == 1.0f) {
    updateParts(layer);
    updateParts(layer);
    updateParts(layer);
  }
  updateParts(layer);
}


void Rubik::jumpAnimation(float deltaTime) {
  // Definir la velocidad de la animación
  float speed = 0.01f; // Ajusta la velocidad según sea necesario

  // Definir la altura máxima del salto
  float maxHeight = 1.0f;

  // Definir el ángulo inicial
  static float angle = 0.0f;

  // Calcular la nueva posición en y del cubo usando el tiempo transcurrido
  for (int i = 0; i < cubosRubik.size(); i++) {
    // Calcular la nueva posición en y del cubo
    // Introducir un desfase en el ángulo basado en el índice del cubo
    cubosRubik[i].pos.y = maxHeight * sin(angle + i * 0.1f);

    // Incrementar el ángulo para el próximo paso basado en el tiempo
    angle += speed * deltaTime;
  }
  draw();
}

Rubik Rubik::deepCopy() const {
  // Crear una nueva instancia de Rubik con las mismas propiedades que la instancia actual
  Rubik copy(*this);

  // Realizar una copia profunda de los cubosRubik
  for (int i = 0; i < cubosRubik.size(); i++) {
    copy.cubosRubik[i] = cubosRubik[i].deepCopy();
  }

  return copy;
}

void Rubik::RotarCubo()
{
  vector<int> pv = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};

  for (int k = 0; k < 7200; k++) { // 90 grados
    for (int j = 0; j < pv.size(); j++) { // size 9 roto los 9 cubos 1 grado 90 veces
      cubosRubik[pv[j]].move(cubosRubik[pv[10]].pos, shader, degrees);      //[4] es el pivot
    }
    draw(); // dibuja la animacion, en vez de hacer el giro de golpe
  }

  return;
}

#endif //CUBORUBIK_RUBIK_H
