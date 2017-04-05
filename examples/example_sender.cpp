#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <VideoSenderReceiverApi.h>

/* ------------------------------------------------ */

struct Color {
  Color():r(1.0f),g(0.0f),b(0.0f){}
  Color(float r, float g, float b):r(r),g(g),b(b){}
  float r;
  float g;
  float b;
};

static Color palette(float t, Color& a, Color& b, Color& c, Color& d);

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

  win = glfwCreateWindow(win_w, win_h, "Video Sender", NULL, NULL);
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

  glfwSetWindowPos(win, 40, 40);

  /* Moving your mouse will change the background color. */
  double mouse_x = 0.0f;
  double mouse_y = 0.0f;
  Color color_a(0.5f, 0.5f, 0.5f);
  Color color_b(0.5f, 0.5f, 0.5f);
  Color color_c(1.0f, 1.0f, 1.0f);
  Color color_d(0.0f, 0.33f, 0.67f);
  Color background_color;
  float t = 0.0f;

  VideoSender* ctx = NULL;
  if (0 != video_sender_alloc(&ctx)) {
    printf("Failed to alloc. (exiting).\n");
    exit(EXIT_FAILURE);
  }

  if (0 != video_sender_init(ctx, win_w, win_h, "127.0.0.1", 1234)) {
    printf("Failed to init. (exiting).\n");
    exit(EXIT_FAILURE);
  }
  
  while (!glfwWindowShouldClose(win)) {

    /* Calculate the background color. */
    glfwGetCursorPos(win, &mouse_x, &mouse_y);
    t = mouse_x / win_w;
    background_color = palette(t, color_a, color_b, color_c, color_d);

    /* Set the clear color. */
    glViewport(0, 0, win_w, win_h);
    glClearColor(background_color.r, background_color.g, background_color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* We just fill the FBO into which we render with the current clear color. */
    video_sender_begin_capture(ctx);
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    }
    video_sender_end_capture(ctx);

    /* And send the last frame to the receiver. */
    video_sender_update(ctx);

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  if (0 != video_sender_dealloc(ctx)) {
    printf("Failed to cleanly dealloc the video sender.\n");
  }
  
  glfwTerminate();

  return EXIT_SUCCESS;
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  if (GLFW_KEY_ESCAPE == key) {
    glfwSetWindowShouldClose(win, GL_TRUE);
  }
}

/* ------------------------------------------------ */

void char_callback(GLFWwindow* win, unsigned int key) {}
void resize_callback(GLFWwindow* window, int width, int height) {}
void cursor_callback(GLFWwindow* win, double x, double y) {}
void button_callback(GLFWwindow* win, int bt, int action, int mods) {}
void error_callback(int err, const char* desc) {  printf("GLFW error: %s (%d)\n", desc, err); }

/* ------------------------------------------------ */

static Color palette(float t, Color& a, Color& b, Color& c, Color& d) {
  Color result;
  result.r = a.r + b.r * cosf(6.28318 * (c.r * t + d.r));
  result.g = a.g + b.g * cosf(6.28318 * (c.g * t + d.g));
  result.b = a.b + b.b * cosf(6.28318 * (c.b * t + d.b));
  return result;
}

/* ------------------------------------------------ */
