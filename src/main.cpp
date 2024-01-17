
#pragma region Includes

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "Utilities/Shader.h"
#include "Utilities/Texture.h"
#include "Squere.h"
#include "Circlee.h"
#include "Camera.h"
#include <stdio.h>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/random.inl"
#include "MapSystem.h"
#include "glad/glad.h"
#include "Arrow.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

//#include <glad/glad.h>  // Initialize with gladLoadGL()


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


void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

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
ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

#pragma endregion Orginal set up

#pragma region My set up

//Camera set up
int display_w, display_h;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 0;
float lastY = 0;
glm::vec2 keyMove = glm::vec2(0);
glm::vec2 keyMove2 = glm::vec2(0);

Shader ourShader("res/shaders/basic.vert", "res/shaders/basic.frag");
Texture* arrowTexture;
const int mapSystemCount = 3; // number of MapSystem objects
MapSystem mapSystems[mapSystemCount] = {
        MapSystem(&ourShader,"res/levels/level1.json"),
        MapSystem(&ourShader,"res/levels/level2.json"),
        MapSystem(&ourShader,"res/levels/level3.json")
};

int playerMap = 0;
int player2Map = 0;
bool playerWon = false;
bool player2Won = false;

Squere *squere;
Circlee *circle;
Arrow *firstPlayerArrow;
Arrow *secondPlayerArrow;
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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
#pragma endregion Init

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
    bool err = !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    if (err) {
        spdlog::error("Failed to initialize OpenGL loader!");
        return false;
    }
    return true;
}


void init_textures_vertices() {
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    ourShader.init();
    ourShader.use();
    ourShader.setInt("ourTexture", 0);
    arrowTexture = new Texture( "Arrow.png","C:\\Users\\redkc\\CLionProjects\\assignment-x-the-project-ReasonPsycho\\res\\textures"," ");
    mapSystems[0].init();
    mapSystems[1].init();
    mapSystems[2].init();

    squere = new Squere(&ourShader,   mapSystems[0].textureMap["black.jpg"].get(), Square, false,
                        mapSystems[0].randomPositionAtEgedeOfTheMap(), 0, glm::vec2(0), glm::vec2(2));
    circle = new Circlee(&ourShader,   mapSystems[0].textureMap["black.jpg"].get(), Circle, false,
                         mapSystems[0].randomPositionAtEgedeOfTheMap(), 0, glm::vec2(0), glm::vec2(2));
    firstPlayerArrow = new Arrow(&ourShader,arrowTexture,mapSystems[0].VBO, Square, false,glm::vec2(0),0, glm::vec2(0), glm::vec2(2,2));
    secondPlayerArrow = new Arrow(&ourShader,arrowTexture,mapSystems[0].VBO, Square, false,glm::vec2(0),0, glm::vec2(0), glm::vec2(2,2));
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

void before_frame() {
    // Setting up delta time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
};

void input() {
    keyMove = glm::vec2(0);
    keyMove2 = glm::vec2(0);
    processInput(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);
}

void update() {
    squere->moveByInputVector(keyMove,deltaTime);
    circle->moveByInputVector(keyMove2,deltaTime);
    firstPlayerArrow->transform.position = squere->transform.position;
    secondPlayerArrow->transform.position = circle->transform.position;
    firstPlayerArrow->transform.rotation = firstPlayerArrow->rotateTowardsPosition2D(mapSystems[playerMap].closestGoal(firstPlayerArrow->transform.position));
    secondPlayerArrow->transform.rotation = secondPlayerArrow->rotateTowardsPosition2D(mapSystems[player2Map].closestGoal(secondPlayerArrow->transform.position));
    
    
    squere->detectCollisions(  mapSystems[playerMap].collisions);
    squere->seperateObject();
    circle->detectCollisions(  mapSystems[player2Map].collisions);
    circle->seperateObject();

    if(squere->detectCollisions(  mapSystems[playerMap].goals)){
        spdlog::info("ReachedGoal!");
        if(playerMap < 2){
            playerMap++;
            squere->transform.position = mapSystems[playerMap].randomPositionAtEgedeOfTheMap();
            
        }else{
            playerWon = true;
        }

    }
    if(circle->detectCollisions(  mapSystems[player2Map].goals)){
        spdlog::info("ReachedGoal!");
        if(player2Map < 2){
            player2Map++;
            circle->transform.position = mapSystems[player2Map].randomPositionAtEgedeOfTheMap();
        }else{
            player2Won = true;
        }

    }
}

void render() {
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    camera.SetUpSplitGlViewport(1,display_w,display_h);

    camera.Position = glm::vec3 (squere->transform.position,0);
    camera.UpdateShader(&ourShader);

    firstPlayerArrow->render();
   squere->render();
    if(playerMap == player2Map)
        circle->render();
    mapSystems[playerMap].render();
    
    camera.SetUpSplitGlViewport(0,display_w,display_h);

    camera.Position = glm::vec3 (circle->transform.position,0);
    camera.UpdateShader(&ourShader);

    secondPlayerArrow->render();
    if(playerMap == player2Map)
        squere->render();
    circle->render();
    mapSystems[player2Map].render();
    
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
    ImGui::Begin("Score:");
    std::ostringstream playerFirstStr;
    playerFirstStr << "First player is on level nr: " << playerMap + 1;
    std::string playerFirstStrg = playerFirstStr.str();
    std::ostringstream playerSecondStr;
    playerSecondStr <<"Second player is on level nr: " << player2Map + 1;
    std::string playerSecondStrg = playerSecondStr.str();
    ImGui::Text(playerFirstStrg.c_str());
    ImGui::Text(playerSecondStrg.c_str());
    //  ImGui::Checkbox("Does balls and walls collide?", &wallsCollide);
    ImGui::End();

    if(playerWon || player2Won){
        int window_w = 800; // Width of the window
        int window_h = 600; // Height of the window
        ImGui::SetNextWindowSize(ImVec2(window_w, window_h)); // Set the window size
        ImGui::SetNextWindowPos(ImVec2((display_w - window_w)*0.5f, (display_h - window_h)*0.5f)); // Center the window
        ImGui::Begin("Vitory!!!");
        ImGui::GetFont()->Scale = 5.0f;
        ImGui::Text(playerWon ? "First player won!" : "Second player won!");
        ImGui::End();
    }
}

void imgui_end() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /*
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
*/
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        keyMove.y += 1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        keyMove.y -= 1;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        keyMove.x -= 1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        keyMove.x += 1;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        keyMove2.y += 1;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        keyMove2.y -= 1;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        keyMove2.x -= 1;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        keyMove2.x += 1;
 
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    display_h = height;
    display_w = width;
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
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
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
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
    camera.SetUpSingleGlViewport(display_w,display_h);
    lastX = display_w / 2.0f;
    lastY = display_h / 2.0f;

    // Capture and lock the mouse to the window
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

#pragma endregion Functions
