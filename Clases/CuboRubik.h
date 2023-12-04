#ifndef CUBORUBIK_CUBORUBIK_H
#define CUBORUBIK_CUBORUBIK_H

#include "Point.h"

class CuboRubik {
public:
    std::vector<Point<float>> vertices;
    float dim;
    glm::vec3 pos{};
    glm::mat4 prevRot = glm::mat4(1.0f);
    unsigned int VAO = 0, VBO = 0;
    unsigned int texture = 0;
    float acc = 1.0f;
    glm::vec3 originalPos;


    CuboRubik(float dim, glm::vec3 pos, std::vector<glm::vec3> color) {
      this->dim = dim;
      this->pos = pos;
      this->originalPos = pos;

      //FRONT
      vertices.emplace_back(-dim, -dim, dim, color[0], 0.0f, 0.0f);
      vertices.emplace_back(dim, -dim, dim, color[0], 1.0f, 0.0f);
      vertices.emplace_back(dim, dim, dim, color[0], 1.0f, 1.0f);
      vertices.emplace_back(dim, dim, dim, color[0], 1.0f, 1.0f);
      vertices.emplace_back(-dim, dim, dim, color[0], 0.0f, 1.0f);
      vertices.emplace_back(-dim, -dim, dim, color[0], 0.0f, 0.0f);
      //UP
      vertices.emplace_back(-dim, dim, -dim, color[1], 0.0f, 1.0f);
      vertices.emplace_back(dim, dim, -dim, color[1], 1.0f, 1.0f);
      vertices.emplace_back(dim, dim, dim, color[1], 1.0f, 0.0f);
      vertices.emplace_back(dim, dim, dim, color[1], 1.0f, 0.0f);
      vertices.emplace_back(-dim, dim, dim, color[1], 0.0f, 0.0f);
      vertices.emplace_back(-dim, dim, -dim, color[1], 0.0f, 1.0f);
      //LEFT
      vertices.emplace_back(-dim, dim, dim, color[2], 1.0f, 0.0f);
      vertices.emplace_back(-dim, dim, -dim, color[2], 1.0f, 1.0f);
      vertices.emplace_back(-dim, -dim, -dim, color[2], 0.0f, 1.0f);
      vertices.emplace_back(-dim, -dim, -dim, color[2], 0.0f, 1.0f);
      vertices.emplace_back(-dim, -dim, dim, color[2], 0.0f, 0.0f);
      vertices.emplace_back(-dim, dim, dim, color[2], 1.0f, 0.0f);
      //DOWN
      vertices.emplace_back(-dim, -dim, -dim, color[3], 0.0f, 1.0f);
      vertices.emplace_back(dim, -dim, -dim, color[3], 1.0f, 1.0f);
      vertices.emplace_back(dim, -dim, dim, color[3], 1.0f, 0.0f);
      vertices.emplace_back(dim, -dim, dim, color[3], 1.0f, 0.0f);
      vertices.emplace_back(-dim, -dim, dim, color[3], 0.0f, 0.0f);
      vertices.emplace_back(-dim, -dim, -dim, color[3], 0.0f, 1.0f);
      //RIGHT
      vertices.emplace_back(dim, dim, dim, color[4], 1.0f, 0.0f);
      vertices.emplace_back(dim, dim, -dim, color[4], 1.0f, 1.0f);
      vertices.emplace_back(dim, -dim, -dim, color[4], 0.0f, 1.0f);
      vertices.emplace_back(dim, -dim, -dim, color[4], 0.0f, 1.0f);
      vertices.emplace_back(dim, -dim, dim, color[4], 0.0f, 0.0f);
      vertices.emplace_back(dim, dim, dim, color[4], 1.0f, 0.0f);
      //BACK
      vertices.emplace_back(-dim, -dim, -dim, color[5], 0.0f, 0.0f);
      vertices.emplace_back(dim, -dim, -dim, color[5], 1.0f, 0.0f);
      vertices.emplace_back(dim, dim, -dim, color[5], 1.0f, 1.0f);
      vertices.emplace_back(dim, dim, -dim, color[5], 1.0f, 1.0f);
      vertices.emplace_back(-dim, dim, -dim, color[5], 0.0f, 1.0f);
      vertices.emplace_back(-dim, -dim, -dim, color[5], 0.0f, 0.0f);
    }

    void genBuffers();

    void load_create_texture(Shader *);

    void draw(Shader *, glm::vec3);

    void move(glm::vec3, Shader *, float);

    void trans(glm::vec3, Shader *);

    void deleteBuffers();

    CuboRubik deepCopy() const;
private:

    void updateBuffers();

    void move_vertices(glm::vec3, float);

    void trans_vertices(glm::vec3);
};


void CuboRubik::genBuffers() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
  //vertex position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

void CuboRubik::load_create_texture(Shader *ourShader) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT);  // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  stbi_set_flip_vertically_on_load(false);

  int width, height, nrChannels;
  // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load("C://texture//allspark.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  ourShader->use();
  ourShader->setInt("texture1", 0);
}

void CuboRubik::updateBuffers() {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
}

void CuboRubik::draw(Shader *ourShader, glm::vec3 pivot = glm::vec3(0.0f, 0.0f, 0.0f)) {

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, pos);
  ourShader->setMat4("model", model);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  return;
}

void CuboRubik::move_vertices(glm::vec3 pivot, float degrees) {
  glm::vec4 vertex;
  glm::mat4 rotMatrix = glm::mat4(1.0f);
  for (int i = 0; i < vertices.size(); i++) // aplico el cambio para la rotacion en los vertices
  {
    vertex = glm::vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1.0f);

    vertex = glm::rotate(rotMatrix, glm::radians(degrees), pivot) * vertex;

    vertices[i].x = vertex.x;
    vertices[i].y = vertex.y;
    vertices[i].z = vertex.z;
  }
  updateBuffers();
}


void CuboRubik::trans_vertices(glm::vec3 pivot) {
  glm::vec4 vertex;
  glm::mat4 rotMatrix = glm::mat4(1.0f);
  for (int i = 0; i < vertices.size(); i++) // aplico el cambio para la rotacion en los vertices
  {
    vertex = glm::vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1.0f);

    vertex = glm::translate(rotMatrix, pivot) * vertex;

    vertices[i].x = vertex.x;
    vertices[i].y = vertex.y;
    vertices[i].z = vertex.z;
  }
  updateBuffers();
}

void CuboRubik::move(glm::vec3 pivot, Shader *ourShader, float degrees) {

  glm::mat4 model = glm::mat4(1.0f);

  //Rotamos los centros
  glm::mat4 rot = glm::mat4(1.0f);
  rot = glm::rotate(rot, glm::radians(degrees), pivot);
  glm::vec4 r = rot * glm::vec4(pos, 1.0f);
  pos = r;

  model = glm::translate(model, pos);
  move_vertices(pivot, degrees);
  ourShader->setMat4("model", model);


  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void CuboRubik::trans(glm::vec3 pivot, Shader *ourShader) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, pos);
  trans_vertices(pivot);
  ourShader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}


void CuboRubik::deleteBuffers() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

CuboRubik CuboRubik::deepCopy() const {
    // Create a new instance of CuboRubik with the same properties as the current instance
    CuboRubik copy(dim, pos, std::vector<glm::vec3>(vertices.size()));

    // Copy over all the properties from the current instance to the new one
    copy.dim = dim;
    copy.pos = pos;
    copy.prevRot = prevRot;
    copy.VAO = VAO;
    copy.VBO = VBO;
    copy.texture = texture;
    copy.acc = acc;
    copy.originalPos = originalPos;

    // Deep copy the vertices vector
    for (int i = 0; i < vertices.size(); i++) {
        copy.vertices[i] = vertices[i];
    }

    return copy;
}
#endif //CUBORUBIK_CUBORUBIK_H
