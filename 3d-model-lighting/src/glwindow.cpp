#include "glwindow.h"
#include "geometry.h"
#include "inputmanager.h"
#include "stb_image.h"

GLuint cubemapTexture;

void print(std::string message, bool error) {
  if (error) {
    std::cout << "\033[1;31m" << message << "\033[0m" << std::endl;
  } else {
    std::cout << "\033[1;32m" << message << "\033[0m" << std::endl;
  }
}

OpenGLWindow::OpenGLWindow(std::string modelName, std::string albedoName,
                           std::string normalName) {
  sdlWin = WindowUtils::initGL(modelName);
  SDL_HideWindow(sdlWin);
  // shading
  shadermanager = ShaderManager();
  modelShader = shadermanager.loadShaderProgram("shaders/model.vert",
                                                "shaders/model.frag");
  skyboxShader = shadermanager.loadShaderProgram("shaders/skybox.vert",
                                                 "shaders/skybox.frag");
  // input
  inputmanager = InputManager(&model, &view, modelShader);
  // lighting
  lightmanager = LightManager();
  // texturing for model
  print("Loading model textures...", false);
  TextureManager texturemanager = TextureManager();
  texturemanager.loadTexture("textures/" + albedoName, modelShader, 0);
  texturemanager.loadTexture("textures/" + normalName, modelShader, 1);
  glUniform1i(glGetUniformLocation(modelShader, "albedoTex"), 0);
  glUniform1i(glGetUniformLocation(modelShader, "normalTex"), 1);
  useNormalMap = false;
  glUniform1i(glGetUniformLocation(modelShader, "useNormalMap"), useNormalMap);

  // texturing for skybox
  print("Loading skybox textures...", false);
  glUseProgram(skyboxShader);
  glGenTextures(1, &cubemapTexture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

  // Set texture parameters
  std::string faceTextures[6] = {"px.png", "nx.png", "py.png",
                                 "ny.png", "pz.png", "nz.png"};
  int width, height, numChannels;
  for (int i = 0; i < 6; ++i) {
    stbi_set_flip_vertically_on_load(false);
    unsigned char *imageData =
        stbi_load(("textures/skybox/" + faceTextures[i]).c_str(), &width,
                  &height, &numChannels, 0);
    if (imageData) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
      stbi_image_free(imageData);
    } else {
      // Handle error loading the texture
      std::cout << "Failed to load texture: " << faceTextures[i] << std::endl;
      stbi_image_free(imageData);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);

  // set up model vao and vbo
  print("Loading model...", false);
  glGenVertexArrays(1, &modelVAO);
  glBindVertexArray(modelVAO);
  initModelVBO(modelName);

  print("Loading skybox...", false);
  glGenVertexArrays(1, &skyboxVAO);
  glBindVertexArray(skyboxVAO);
  initSkyboxVBO();

  initMVP();
  lightmanager.updateLightUniforms(modelShader, view);
  WindowUtils::glPrintError("Setup complete", true);
  SDL_ShowWindow(sdlWin);
}

void OpenGLWindow::initModelVBO(std::string modelName) {
  GeometryData g;
  g.loadFromOBJFile("objs/" + modelName + ".obj");
  float *verts = (float *)g.vertexData();
  float *norms = (float *)g.normalData();
  if (g.textureCoordData() == nullptr) {
    print("No texture coordinates found for model", true);
    cleanup();
    exit(1);
  }
  float *texs = (float *)g.textureCoordData();
  float *tans = (float *)g.tangentData();
  float *bitans = (float *)g.bitangentData();
  numVerticesModel = g.vertexCount();
  objectCenter = g.center();

  glm::vec3 maxBounds, minBounds;
  g.boundingBox(minBounds, maxBounds);
  float width = maxBounds.x - minBounds.x;
  float height = maxBounds.y - minBounds.y;
  float depth = maxBounds.z - minBounds.z;
  float maxDim = std::max(std::max(width, height), depth);
  distToObj = maxBounds.z + maxDim;

  std::vector<float> interleaved;
  interleaved.reserve(numVerticesModel * 14);
  for (int i = 0; i < numVerticesModel; i++) {
    interleaved.insert(interleaved.end(), verts + 3 * i, verts + 3 * i + 3);
    interleaved.insert(interleaved.end(), norms + 3 * i, norms + 3 * i + 3);
    interleaved.insert(interleaved.end(), texs + 2 * i, texs + 2 * i + 2);
    interleaved.insert(interleaved.end(), tans + 3 * i, tans + 3 * i + 3);
    interleaved.insert(interleaved.end(), bitans + 3 * i, bitans + 3 * i + 3);
  }

  glGenBuffers(1, &modelVBO);
  glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * interleaved.size(),
               interleaved.data(), GL_STATIC_DRAW);

  // set up vertex attributes
  // packed like this:
  // position (3), normal (3), texcoord (2), tangent (3), bitangent (3)
  glVertexAttribPointer(glGetAttribLocation(modelShader, "vertPosition"), 3,
                        GL_FLOAT, false, 14 * sizeof(float), 0);
  glVertexAttribPointer(glGetAttribLocation(modelShader, "vertNormal"), 3,
                        GL_FLOAT, false, 14 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glVertexAttribPointer(glGetAttribLocation(modelShader, "vertTexCoord"), 3,
                        GL_FLOAT, false, 14 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glVertexAttribPointer(glGetAttribLocation(modelShader, "vertTangent"), 3,
                        GL_FLOAT, false, 14 * sizeof(float),
                        (void *)(8 * sizeof(float)));
  glVertexAttribPointer(glGetAttribLocation(modelShader, "vertBitangent"), 3,
                        GL_FLOAT, false, 14 * sizeof(float),
                        (void *)(11 * sizeof(float)));
  glEnableVertexAttribArray(glGetAttribLocation(modelShader, "vertPosition"));
  glEnableVertexAttribArray(glGetAttribLocation(modelShader, "vertNormal"));
  glEnableVertexAttribArray(glGetAttribLocation(modelShader, "vertTexCoord"));
  glEnableVertexAttribArray(glGetAttribLocation(modelShader, "vertTangent"));
  glEnableVertexAttribArray(glGetAttribLocation(modelShader, "vertBitangent"));
}

void OpenGLWindow::initSkyboxVBO() {
  GeometryData g;
  g.loadFromOBJFile("objs/skybox.obj");
  float *verts = (float *)g.vertexData();
  numVerticesSkybox = g.vertexCount();

  glGenBuffers(1, &skyboxVBO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * g.vertexCount() * 3, verts,
               GL_STATIC_DRAW);

  // set up vertex attributes
  glVertexAttribPointer(glGetAttribLocation(skyboxShader, "vertPosition"), 3,
                        GL_FLOAT, false, 3 * sizeof(float), 0);
  glEnableVertexAttribArray(glGetAttribLocation(skyboxShader, "vertPosition"));
}

void OpenGLWindow::render() {
  lightmanager.updateLightPositions(modelShader, view, distToObj, time);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // render model
  glUseProgram(modelShader);
  glBindVertexArray(modelVAO);
  glDrawArrays(GL_TRIANGLES, 0, numVerticesModel);

  // render skybox
  // we do this last because it is always in the background
  glDepthFunc(GL_LEQUAL);
  glUseProgram(skyboxShader);
  glBindVertexArray(skyboxVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthFunc(GL_LESS);

  SDL_GL_SwapWindow(sdlWin);
  // update time
  time += 0.01f;
}

void OpenGLWindow::initMVP() {
  // create all necessary matrices
  projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  view = glm::lookAt(glm::vec3(0.0f, 0.0f, distToObj), glm::vec3(0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::translate(glm::mat4(1.0f), -objectCenter);
  updateMVP();
}

void OpenGLWindow::updateMVP() {
  // do all model matrix updates
  glUseProgram(modelShader);
  glm::mat3 normalMatrix = glm::transpose(glm::inverse(view * model));
  glUniformMatrix4fv(glGetUniformLocation(modelShader, "projectionMatrix"), 1,
                     GL_FALSE, &projection[0][0]);
  glUniformMatrix3fv(glGetUniformLocation(modelShader, "normalMatrix"), 1,
                     GL_FALSE, &normalMatrix[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(modelShader, "modelViewMatrix"), 1,
                     GL_FALSE, &(view * model)[0][0]);

  // do all skybox matrix updates
  glUseProgram(skyboxShader);
  glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
  glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projectionMatrix"), 1,
                     GL_FALSE, &projection[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "viewMatrix"), 1,
                     GL_FALSE, &skyboxView[0][0]);
}

bool OpenGLWindow::handleEvent(SDL_Event e) {
  if (e.type == SDL_KEYDOWN) {
    if (e.key.keysym.sym == SDLK_ESCAPE) {
      return false;
    }

    switch (e.key.keysym.sym) {
    case SDLK_r:
      inputmanager.setTransformMode(ROTATE);
      break;
    case SDLK_s:
      inputmanager.setTransformMode(SCALE);
      break;
    case SDLK_x:
      inputmanager.setAxis(X);
      break;
    case SDLK_y:
      inputmanager.setAxis(Y);
      break;
    case SDLK_z:
      inputmanager.setAxis(Z);
      break;
    case SDLK_1:
      inputmanager.setLighting(AMBIENT);
      break;
    case SDLK_2:
      inputmanager.setLighting(DIFFUSE);
      break;
    case SDLK_3:
      inputmanager.setLighting(SPECULAR);
      break;
    case SDLK_4:
      inputmanager.setLighting(SHININESS);
      break;
    case SDLK_v: {
      inputmanager.setTarget(VIEW);
      break;
    }
    case SDLK_m: {
      inputmanager.setTarget(MODEL);
      break;
    }
    case SDLK_l: {
      inputmanager.setTarget(LIGHT);
      break;
    }
    case SDLK_SPACE: {
      model = glm::translate(glm::mat4(1.0f), -objectCenter);
      updateMVP();
      break;
    }
    case SDLK_UP: {
      inputmanager.increment();
      updateMVP();
      break;
    }
    case SDLK_DOWN: {
      inputmanager.decrement();
      updateMVP();
      break;
    }
    case SDLK_n: {
      useNormalMap = !useNormalMap;
      glUseProgram(modelShader);
      glUniform1i(glGetUniformLocation(modelShader, "useNormalMap"),
                  useNormalMap);
      break;
    }
    }
    // change window title depending on if target is light or model/view
    if (inputmanager.getTarget() == "Light") {
      std::string title = inputmanager.getTarget() + " Mode (" +
                          inputmanager.getLighting() + ")";
      SDL_SetWindowTitle(sdlWin, title.c_str());
    } else {
      std::string title = inputmanager.getTransformMode() + " Mode (" +
                          inputmanager.getAxis() + ")" + "(" +
                          inputmanager.getTarget() + ")";
      SDL_SetWindowTitle(sdlWin, title.c_str());
    }
  }
  return true;
}

void OpenGLWindow::cleanup() {
  // model cleanup
  glDeleteBuffers(1, &modelVBO);
  glDeleteVertexArrays(1, &modelVAO);
  glDeleteProgram(modelShader);
  // skybox cleanup
  glDeleteBuffers(1, &skyboxVBO);
  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteProgram(skyboxShader);

  SDL_DestroyWindow(sdlWin);
}
