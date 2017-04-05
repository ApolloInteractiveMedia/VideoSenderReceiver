#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <VideoSenderReceiverApi.h>

/* ------------------------------------------------ */

void button_callback(GLFWwindow* win, int bt, int action, int mods);
void cursor_callback(GLFWwindow* win, double x, double y);
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow* win, unsigned int key);
void error_callback(int err, const char* desc);
void resize_callback(GLFWwindow* window, int width, int height);

/* ------------------------------------------------ */

int win_w = 640;
int win_h = 480;

/* ------------------------------------------------ */
  
int main() {

  glfwSetErrorCallback(error_callback);

  if(!glfwInit()) {
    printf("Error: cannot setup glfw.\n");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_SAMPLES, 0);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
  
  GLFWwindow* win = NULL;

  win = glfwCreateWindow(win_w, win_h, "Video Receiver", NULL, NULL);
  if (!win) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(win, resize_callback);
  glfwSetKeyCallback(win, key_callback);
  glfwSetCharCallback(win, char_callback);
  glfwSetCursorPosCallback(win, cursor_callback);
  glfwSetMouseButtonCallback(win, button_callback);
  glfwMakeContextCurrent(win);
  glfwSwapInterval(1);

  if (!gladLoadGL()) {
    printf("Cannot load GL.\n");
    exit(1);
  }

  // ----------------------------------------------------------------
  // THIS IS WHERE YOU START CALLING OPENGL FUNCTIONS, NOT EARLIER!!
  // ----------------------------------------------------------------

  glfwSetWindowPos(win, 0, win_h + 100);

  VideoReceiver* ctx = NULL;
  if (0 != video_receiver_alloc(&ctx)) {
    printf("Failed to alloc. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  if (0 != video_receiver_init(ctx, 1234)) {
    printf("Failed to init. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  while(!glfwWindowShouldClose(win)) {
    
    glViewport(0, 0, win_w, win_h);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    video_receiver_update(ctx);
    video_receiver_draw(ctx, 0, 0, win_w, win_h);
       
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  if (0 != video_receiver_dealloc(ctx)) {
    printf("Failed to dealloc cleanly.");
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  if (GLFW_KEY_ESCAPE == action) {
    glfwSetWindowShouldClose(win, GL_TRUE);
  }
}

/* ------------------------------------------------ */

void char_callback(GLFWwindow* win, unsigned int key) {}
void resize_callback(GLFWwindow* window, int width, int height) {}
void cursor_callback(GLFWwindow* win, double x, double y) {}
void button_callback(GLFWwindow* win, int bt, int action, int mods) {}
void error_callback(int err, const char* desc) { printf("GLFW error: %s (%d)\n", desc, err); }

/* ------------------------------------------------ */

  
