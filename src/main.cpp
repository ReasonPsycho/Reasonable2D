
#pragma region Includes

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
#include "Cube.h"
#include "Camera.h"
#include "Crircle.h"
#include <stdio.h>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/random.inl"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

//#include <glad/glad.h>  // Initialize with gladLoadGL()

#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

using namespace gl;

//Instancing
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <spdlog/spdlog.h>

#pragma endregion Includes

#pragma region Function definitions

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool init();

void init_textures_vertices();

void init_imgui();

void init_camera();

void before_frame();

void input();

void update();

void render();

void imgui_begin();

void imgui_render();

void imgui_end();

void end_frame();

void cleanup();



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

#pragma endregion Function definitions

#pragma region Orginal set up

constexpr int32_t WINDOW_WIDTH = 1920;
constexpr int32_t WINDOW_HEIGHT = 1080;

GLFWwindow *window = nullptr;

// Change these to lower GL version like 4.5 if GL 4.6 can't be initialized on your machine
const char *glsl_version = "#version 460";
constexpr int32_t GL_VERSION_MAJOR = 4;
constexpr int32_t GL_VERSION_MINOR = 6;

//Not my things but I could probably change them

bool show_demo_window = false;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

#pragma endregion Orginal set up

#pragma region My set up

//Camera set up
int display_w, display_h;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 0;
float lastY = 0;

bool ballsBounce = true;
bool ballsSeperate = true;
bool wallsCollide = true;
int amountOfCircles = 60;

Shader ourShader("res/shaders/basic.vert", "res/shaders/basic.frag");
Texture ourTexture("res/textures/stone.jpg");
Cube cube;
std::vector<Crircle> myCircles;
Crircle circle(glm::vec2(1.0f,1.0f),glm::vec2(0.0f,0.01f),.5f,1.5f);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

#pragma endregion My set up


int main(int, char **) {

    #pragma region Init

    if (!init()) {
        spdlog::error("Failed to initialize project!");
        return EXIT_FAILURE;
    }
    spdlog::info("Initialized project.");

    init_textures_vertices();
    spdlog::info("Initialized textures and vertices.");

    init_imgui();
    spdlog::info("Initialized ImGui.");

    init_camera();
    spdlog::info("Initialized camera and viewport.");
    
    // configure global opengl state
    glEnable(static_cast<gl::GLenum>(GL_DEPTH_TEST));
   glDepthFunc(static_cast<gl::GLenum>(GL_LESS));

    #pragma endregion Init

    auto shader = glCreateShader(static_cast<gl::GLenum>(GL_COMPUTE_SHADER));


    // Main loop
    while (!glfwWindowShouldClose(window)) {
        //Setting up things for the rest of functionalities (ex. delta time)
        before_frame();
        
        // Process I/O operations here
        input();

        // Update game objects' state here
        update();

        // OpenGL rendering code here
        render();

        // Draw ImGui
        imgui_begin();
        imgui_render(); // edit this function to add your own ImGui controls
        imgui_end(); // this call effectively renders ImGui

        // End frame and swap buffers (double buffering)
        end_frame();
    }

    cleanup();
    // Cleanup
    return 0;
}


#pragma region Functions

void cleanup() {
    //Orginal clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

bool init() {
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        spdlog::error("Failed to initalize GLFW!");
        return false;
    }

    // GL 4.6 + GLSL 460
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dear ImGui GLFW+OpenGL4 example", NULL, NULL);
    if (window == NULL) {
        spdlog::error("Failed to create GLFW Window!");
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync - fixes FPS at the refresh rate of your screen
    glbinding::initialize(glfwGetProcAddress);
    /* was used for glads
    bool err = !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    if (err) {
        spdlog::error("Failed to initialize OpenGL loader!");
        return false;
    }
    */
    return true;
}


void init_textures_vertices() {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

   // cube.init();
    circle.init();
    for(int i = 0;i < amountOfCircles;i++){
        Crircle newCircle(glm::linearRand(glm::vec2(-5.0f),glm::vec2(5.0f)),   glm::linearRand(glm::vec2(0.5f),glm::vec2(1.0f)), glm::linearRand(0.5f,1.0f),1.5f);
      //  while (newCircle.collide(myCircles, -1, true, true, true)){
      //      newCircle = Crircle(glm::linearRand(glm::vec2(-5.0f),glm::vec2(5.0f)),   glm::linearRand(glm::vec2(0.5f),glm::vec2(1.0f)), glm::linearRand(0.4f,0.6f),1.5f);
      //  }
        myCircles.push_back(newCircle);
        myCircles[i].init(circle.VBO,circle.VAO,circle.EBO);  
    }
    ourTexture.init();
    ourShader.init();
    ourTexture.use();
    ourShader.use();
    ourShader.setInt("ourTexture", 0);
}

void init_imgui() {
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
}

void before_frame()
{
    // Setting up delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
};

void input() {
    processInput(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);
}

void update() {
    for(int i = 0;i < amountOfCircles;i++){
        myCircles[i].collide(myCircles, i, ballsBounce ,  ballsSeperate, wallsCollide);
    }
    for(int i = 0;i < amountOfCircles;i++){
        myCircles[i].changeMovment(ballsSeperate, ballsBounce);
        myCircles[i].move(deltaTime);
    }
}

void render() {
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(static_cast<ClearBufferMask>(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    camera.UpdateShader(&ourShader,display_w,display_h);
    
    //cube.render(&ourShader,&ourTexture);
    //circle.render(&ourShader,&ourTexture);
    
    for(int i = 0;i < amountOfCircles;i++){
        myCircles[i].render(&ourShader,&ourTexture);
   }
}

void imgui_begin() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imgui_render() {
    /// Add new ImGui controls here
    // Show the big demo window
    ImGui::Begin("Switch:");
    ImGui::Checkbox("Does balls separate?", &ballsSeperate);
    ImGui::Checkbox("Does balls collide?", &ballsBounce);
  //  ImGui::Checkbox("Does balls and walls collide?", &wallsCollide);
    ImGui::End();
    
}

void imgui_end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UPWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWNWARD, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    display_h = height;
    display_w = width;
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

   // camera.ProcessMouseMovement(xoffset, yoffset, true,deltaTime);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
   // camera.ProcessMouseScroll(static_cast<float>(yoffset),deltaTime);
}

void end_frame() {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();
    glfwSwapBuffers(window);
}

void init_camera() {
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    lastX = display_w / 2.0f;
    lastY = display_h / 2.0f;
    
    // Capture and lock the mouse to the window
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

#pragma endregion Functions
