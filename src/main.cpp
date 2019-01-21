#include "main.h"
#include "timer.h"
#include "ball.h"
#include "player.h"
#include "platform.h"
#include "wall.h"
#include "coin.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

Player player;
Platform platform;
Platform ceiling;
Wall wall[50];
vector<Coin> coins;

float screen_zoom = 0.25, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float speed_horizontal, speed_vertical;
float player_original_x, player_original_y;
glm::vec3 camera_pos, camera_center, camera_up;

Timer t60(1.0 / 60);

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);
    Matrices.view = glm::lookAt(camera_pos, camera_center, camera_up); // Fixed camera for 2D (ortho) in XY plane
    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;

    // Scene render
    platform.draw(VP);
    ceiling.draw(VP);
    for (int i = 0; i < 50; i++)
        wall[i].draw(VP);
    if (coins[0].exist == true)
        coins[0].draw(VP);
    player.draw(VP);
}

void tick_input(GLFWwindow *window)
{
    int left = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);

    if (left)
    {
        if (player.position.x > player_original_x)
        {
            speed_horizontal = -0.1;
            player.tick(speed_horizontal, 0);
            speed_horizontal = 0;
        }
        if (player.position.x < camera_center.x - 7 && player.position.x > player_original_x)
        {
            camera_pos -= glm::vec3(0.1, 0, 0);
            camera_center -= glm::vec3(0.1, 0, 0);
        }
    }
    if (right)
    {
        speed_horizontal = 0.1;
        player.tick(speed_horizontal, 0);
        speed_horizontal = 0;
        if (player.position.x > camera_center.x + 7)
        {
            camera_pos += glm::vec3(0.1, 0, 0);
            camera_center += glm::vec3(0.1, 0, 0);
        }
    }
    if (space)
    {
        speed_horizontal = 0;
        speed_vertical = 0.2;
        player.tick(speed_horizontal, speed_vertical);
        speed_vertical = 0;
        if (player.position.y > 12)
        {
            player.position.y = 12;
            player.box.y = player.position.y;
        }
    }
}

void tick_elements()
{
    if (player.position.y > player_original_y && !glfwGetKey(window, GLFW_KEY_SPACE))
    {
        // int loop = speed_vertical * 100 * -1;
        // float initial_speed_vertical = -0.01;
        // for (int i = 1; i <= loop; i++)
        // {
        //     player.tick(speed_horizontal, initial_speed_vertical);
        //     if (coin[0].exist == true && detect_collision(player.box, coin[0].box))
        //     {
        //         coin[0].exist = false;
        //     }
        // }
        speed_vertical -= 0.02;
        player.tick(speed_horizontal, speed_vertical);
        if (player.position.y < player_original_y)
        {
            player.position.y = player_original_y;
            player.box.y = player.position.y;
            player.box.x = player.position.x;
            speed_vertical = 0;
        }
    }
}

void initGL(GLFWwindow *window, int width, int height)
{
    // Create the models
    player = Player(player_original_x, player_original_y);
    platform = Platform(-30, -28);
    ceiling = Platform(-30, 13);
    int wall_x = -15;
    for (int i = 0; i < 50; i++)
    {
        wall[i] = Wall(wall_x, -5);
        wall_x += 25;
    }
    for (int i = 0; i < 1; i++)
    {
        Coin coin = Coin(-10, 0);
        coins.push_back(coin);
    }

    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow(window, width, height);

    glClearColor(COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv)
{
    srand(time(0));
    int width = 1200;
    int height = 1200;
    player_original_x = -12;
    player_original_y = -7;

    window = initGLFW(width, height);
    initGL(window, width, height);

    camera_pos = glm::vec3(0, 0, 3);
    camera_center = glm::vec3(0, 0, 0);
    camera_up = glm::vec3(0, 1, 0);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        if (t60.processTick())
        {
            draw();
            glfwSwapBuffers(window);

            if (coins[0].exist == true && detect_collision(player.box, coins[0].box))
            {
                coins[0].exist = false;
            }

            tick_elements();
            tick_input(window);
        }

        glfwPollEvents();
    }
    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b)
{
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen()
{
    float top = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left = screen_center_x - 4 / screen_zoom;
    float right = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
