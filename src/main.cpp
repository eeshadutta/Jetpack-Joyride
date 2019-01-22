#include "main.h"
#include "timer.h"
#include "ball.h"
#include "player.h"
#include "platform.h"
#include "wall.h"
#include "coin.h"
#include "enemy1.h"
#include "boomerang.h"
#include "waterballoon.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

Player player;
Platform platform;
Platform ceiling;
Wall wall[50];
vector<Coin> coins;
vector<Enemy1> enemies1;
Boomerang boomerang;
WaterBalloon waterballoon;

float screen_zoom = 0.25, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float speed_horizontal, speed_vertical;
float player_original_x, player_original_y;
float boomerang_speed_x, boomerang_speed_y;

glm::vec3 camera_pos, camera_center, camera_up;
glm::mat4 VP;

int points = 0, coins_collected = 0;

Timer t60(1.0 / 60);

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);
    Matrices.view = glm::lookAt(camera_pos, camera_center, camera_up); // Fixed camera for 2D (ortho) in XY plane
    // glm::mat4 VP = Matrices.projection * Matrices.view;
    VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;

    // Scene render
    platform.draw(VP);
    ceiling.draw(VP);
    for (int i = 0; i < 50; i++)
        wall[i].draw(VP);

    int len = coins.size();
    for (int i = 0; i < len; i++)
    {
        if (coins[i].exist == true)
            coins[i].draw(VP);
    }
    len = enemies1.size();
    for (int i = 0; i < len; i++)
    {
        enemies1[i].draw(VP);
    }
    if (waterballoon.move)
        waterballoon.draw(VP);
    player.draw(VP);
    boomerang.draw(VP);
}

void tick_input(GLFWwindow *window)
{
    int left = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int water_balloon = glfwGetKey(window, GLFW_KEY_W);

    if (left)
    {
        if (player.position.x > player_original_x)
        {
            speed_horizontal = -0.4;
            player.tick(speed_horizontal, 0);
            speed_horizontal = 0;
        }
        if (player.position.x < camera_center.x - 7 && player.position.x > player_original_x)
        {
            camera_pos -= glm::vec3(0.4, 0, 0);
            camera_center -= glm::vec3(0.4, 0, 0);
        }
    }
    if (right)
    {
        speed_horizontal = 0.4;
        player.tick(speed_horizontal, 0);
        speed_horizontal = 0;
        if (player.position.x > camera_center.x + 7)
        {
            camera_pos += glm::vec3(0.4, 0, 0);
            camera_center += glm::vec3(0.4, 0, 0);
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
    if (water_balloon)
    {
        waterballoon.position.x = player.position.x;
        waterballoon.position.y = player.position.y;
        waterballoon.move = true;
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

    if (boomerang.direction == -1)
    {
        boomerang.tick(-0.2, 0);
        if (boomerang.position.x <= player.position.x - 8)
            boomerang.direction = -2;
    }
    if (boomerang.direction == 1)
    {
        boomerang.tick(0.2, 0);
        if (boomerang.position.x >= player.position.x + 50)
            boomerang.direction = 2;
    }

    if (boomerang.direction == -2)
    {
        boomerang.rotate(2);
        if (boomerang.rotation <= 90)
            boomerang.tick(-0.1, -0.05);
        else
            boomerang.tick(0.1, -0.05);
        if (boomerang.rotation >= 180)
            boomerang.direction = 1;
    }
    if (boomerang.direction == 2)
    {
        boomerang.rotate(2);
        if (boomerang.rotation <= 270)
            boomerang.tick(0.1, 0.05);
        else
            boomerang.tick(-0.1, 0.05);
        if (boomerang.rotation <= 0)
            boomerang.direction = -1;
    }

    if (waterballoon.move)
    {
        waterballoon.tick(0.1, 0.04);
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

    float prev_c_x = -5;
    for (int i = 0; i < 30; i++)
    {
        float c_y = rand() % 12;
        float c_x = prev_c_x + rand() % 10;
        int num_coins = 2 + rand() % 5;
        for (int j = 0; j < num_coins; j++)
        {
            int type = rand() % 4 + 1;
            Coin coin = Coin(c_x, c_y, type);
            coins.push_back(coin);
            c_x += 1.0;
        }
        prev_c_x = c_x;
    }

    float prev_e_x = 5;
    for (int i = 0; i < 20; i++)
    {
        float e_x = prev_e_x + rand() % 20;
        float e_y = rand() % 12;
        float e_len = 3 + rand() % 3;
        float e_rot = -90 + rand() % 180;
        Enemy1 en = Enemy1(e_x, e_y, e_len, e_rot);
        enemies1.push_back(en);
        prev_e_x = e_x + 3 * e_len;
    }

    boomerang = Boomerang(40, 0);
    waterballoon = WaterBalloon(player_original_x, player_original_y);

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
    boomerang_speed_x = -0.2;

    window = initGLFW(width, height);
    initGL(window, width, height);

    camera_pos = glm::vec3(0, 0, 3);
    camera_center = glm::vec3(0, 0, 0);
    camera_up = glm::vec3(0, 1, 0);

    // Matrices.model = glm::mat4(1.0f);
    // glm::mat4 translate = glm::translate(enemies1[0].position);
    // glm::mat4 rotate = glm::rotate((float)(enemies1[0].rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // Matrices.model *= (translate * rotate);
    // glm::mat4 MVP = VP * Matrices.model;
    // glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // glm::vec4 originalvec = glm::vec4(0, 0, 0, 1);
    // glm::vec4 finalvec = MVP * originalvec;
    // cout << finalvec.x << finalvec.y << finalvec.z << finalvec.w << '\n';

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        if (t60.processTick())
        {
            draw();
            glfwSwapBuffers(window);

            int len = coins.size();
            for (int i = 0; i < len; i++)
            {
                if (coins[i].exist == true && detect_collision(player.box, coins[i].box))
                {
                    points += coins[i].type * 5;
                    coins_collected++;
                    coins[i].exist = false;
                }
            }

            len = enemies1.size();
            for (int i = 0; i < len; i++)
            {
                if (detect_collision(player.box, enemies1[i].box))
                    cout << "DIE" << '\n';
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
