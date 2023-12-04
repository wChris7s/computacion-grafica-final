#define GLAD_GL_IMPLEMENTATION

#include <glad/gl.h>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "External/STB_IMAGE/stb_image.h"
#include "Clases/Camera.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include <thread>

#include "Clases/Camera.h"
#include "Clases/Shader.h"
#include "Clases/Rubik.h"
#include "Clases/CuboRubik.h"

#include "Clases/Operations.h"

using namespace std;

//----------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void key_callback(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//----------------------------------------


//----------------------------------------
Camera cam;

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

char sideMove;
bool isMoving = false;
bool solving = false;
//----------------------------------------



bool changeDirection = false;

bool animacion = false;

GLFWwindow *createWindow(const char *title) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  gladLoadGL(glfwGetProcAddress);
  glEnable(GL_DEPTH_TEST);

  glfwSetKeyCallback(window, reinterpret_cast<GLFWkeyfun>(key_callback));
  return window;
}


void menu() {
  cout << "-- Cubo de Rubik --" << endl;
  cout << "> Teclas para moverse" << endl;
  cout << "> Arriba (Flecha Arriba)" << endl;
  cout << "> Abajo (Flecha Abajo)" << endl;
  cout << "> Izquierda (Flecha Izquierda)" << endl;
  cout << "> Derecha (Flecha Derecha)" << endl;
  cout << "> Cara frontal (F)" << endl;
  cout << "> Cara trasera (B)" << endl;
  cout << "-- Animación --" << endl;
  cout << "> Solver (S)" << endl;
  cout << "> Explosion " << endl;
  cout << "> Implosion " << endl;
}

int main() {
  GLFWwindow *window = createWindow("Cubo de Rubik");

  //Camera
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);

  namespace fs = std::filesystem;
  fs::path sourceFilePath = fs::absolute(__FILE__);
  const std::string path = sourceFilePath.string().substr(0, sourceFilePath.string().find("main.cpp"));

  string vsShader = path + "\\shader.vs";
  string fsShader = path + "\\shader.fs";

  Shader ourShader(vsShader.c_str(), fsShader.c_str());

  menu();

  Rubik rubik(window, &ourShader, 0.1f);

  Color color = getRandomRgbColor();

  float lastFrame = 0.0f;
  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    cam.updateFrame();
    glClearColor(
     color.getR(),
     color.getG(),
     color.getB(),
     1.0f);

    ourShader.use();

    glm::mat4 projection = glm::perspective(glm::radians(cam.fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
                                            100.0f);
    ourShader.setMat4("projection", projection);

    glm::mat4 view = glm::lookAt(cam.cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), cam.cameraUp);
    ourShader.setMat4("view", view);

    if (solving) {
      Rubik *rubikMoving = &rubik;
      rubikMoving->setSolve();
      solving = false;
    } else if (!isMoving && changeDirection) {
      changeDirection = false;
      rubik.degrees *= -1; // cambia direccion
    } else if (isMoving) {
      Rubik *rubikMoving = &rubik;
      rubikMoving->fillShuffle(sideMove);
      rubikMoving->move(sideMove); // Mueve la camada 90°
      isMoving = false;

      for (int i = 0; rubikMoving->degrees == 1.0f && i < 2; i++) { // Actualiza las camadas.
        rubikMoving->updateParts(sideMove); // si se giro en antihorario, actualizo 2 veces
      } // antihorario: actualizo 2 veces y luego una tercera vez
      rubikMoving->updateParts(sideMove);
    } else if (animacion) {
      Rubik tmp = rubik.deepCopy();
      rubik.RotarCubo();
      for (int i = 0; i < 6; i++) {
        rubik.explosion();
        std::this_thread::sleep_for(std::chrono::milliseconds (400));
        rubik.implosion();
        std::this_thread::sleep_for(std::chrono::milliseconds (400));
        rubik.crumble();
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
      rubik.explosion();
      rubik.implosion();
      rubik = tmp;
      rubik.draw();
      std::this_thread::sleep_for(std::chrono::seconds(1));
      for(int i = 0; i < 30; i++) {
        rubik.randomLayerMovement();
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
      rubik.setSolve();
      std::this_thread::sleep_for(std::chrono::seconds(1));
      Rubik rubik_tmp = rubik;
      rubik.RotarCubo();
      for(long i = 0; i < 30000; i++) {
        rubik.jumpAnimation(deltaTime);
      }
      rubik.explosion();
      rubik.implosion();
      rubik = rubik_tmp;
      rubik.draw();
      animacion = false;
    }
    else {
      rubik.draw();
    }
  }
  rubik.deleteBuffers();
  glfwTerminate();

  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (cam.firstMouse) {
    cam.lastX = xpos;
    cam.lastY = ypos;
    cam.firstMouse = false;
  }

  float xoffset = xpos - cam.lastX;
  float yoffset = cam.lastY - ypos; // reversed since y-coordinates go from bottom to top
  cam.lastX = xpos;
  cam.lastY = ypos;

  float sensitivity = 0.1f; // change this value to your liking
  xoffset *= sensitivity;
  yoffset *= sensitivity;


  if (xoffset > 0 && cam.cameraPos.x < 5) cam.cameraPos.x += xoffset;
  else if (xoffset < 0 && cam.cameraPos.x > -5) cam.cameraPos.x += xoffset;

  if (yoffset > 0 && cam.cameraPos.y < 5) cam.cameraPos.y += yoffset;
  else if (yoffset < 0 && cam.cameraPos.y > -5) cam.cameraPos.y += yoffset;

  cam.yaw += xoffset;
  cam.pitch += yoffset;

  if (cam.pitch > 89.0f)
    cam.pitch = 89.0f;
  if (cam.pitch < -89.0f)
    cam.pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
  front.y = sin(glm::radians(cam.pitch));
  front.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
  cam.cameraFront = glm::normalize(front);
}

void key_callback(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (!isMoving) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      isMoving = true;
      sideMove = 'U';
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      isMoving = true;
      sideMove = 'L';
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      isMoving = true;
      sideMove = 'R';
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      isMoving = true;
      sideMove = 'D';
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
      isMoving = true;
      sideMove = 'F';
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
      isMoving = true;
      sideMove = 'B';
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      solving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
      changeDirection = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
      animacion = true;
      sideMove = 'Z';
    }
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  cam.fov -= (float) yoffset;
  if (cam.fov < 1.0f)
    cam.fov = 1.0f;
  if (cam.fov > 45.0f)
    cam.fov = 45.0f;
}