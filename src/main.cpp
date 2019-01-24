#include "main.h"
#include "timer.h"
#include "ball.h"
#include "player.h"
#include "platform.h"
#include "wall.h"
#include "coin.h"
#include "firelines.h"
#include "firebeams.h"
#include "boomerang.h"
#include "dragon.h"
#include "waterballoon.h"
#include "magnet.h"
#include "piggybank.h"
#include "powerup.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

Player player;

Platform platform;
Platform ceiling;
Wall wall[50];

vector<Coin> coins;

vector<Firelines> firelines;
vector<Firebeams> firebeams;
Boomerang boomerang;
Dragon dragon;
vector<WaterBalloon> iceballs;

vector<WaterBalloon> waterballoons;
Magnet magnet;
Piggybank piggybank;
Powerup powerup;

float screen_zoom = 0.25, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float speed_horizontal, speed_vertical;
float player_original_x, player_original_y;
float boomerang_speed_x, boomerang_speed_y;
float ceiling_pos = 12;
int water_balloon, num_max_waterballoons;

glm::vec3 camera_pos, camera_center, camera_up;
glm::mat4 VP;

int points = 0, coins_collected = 0;
int num_wb = -1;
time_t time_start, magnet_created, dragon_appear;
long long time_elapsed;

Timer t60(1.0 / 60);

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);
    Matrices.view = glm::lookAt(camera_pos, camera_center, camera_up);
    VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;

    // Scene render
    for (int i = 0; i < 50; i++)
        wall[i].draw(VP);

    int len = coins.size();
    for (int i = 0; i < len; i++)
    {
        if (coins[i].exist == true)
            coins[i].draw(VP);
    }
    len = firelines.size();
    for (int i = 0; i < len; i++)
    {
        firelines[i].draw(VP);
    }

    len = firebeams.size();
    for (int i = 0; i < len; i++)
    {
        firebeams[i].draw(VP);
    }

    if (boomerang.exist)
        boomerang.draw(VP);

    dragon.draw(VP);
    len = iceballs.size();
    for (int i = 0; i < len; i++)
    {
        if (iceballs[i].exist)
            iceballs[i].draw(VP);
    }

    platform.draw(VP);
    ceiling.draw(VP);

    player.draw(VP);

    len = waterballoons.size();
    for (int i = 0; i < len; i++)
    {
        if (waterballoons[i].exist)
            waterballoons[i].draw(VP);
    }

    if (magnet.exist)
        magnet.draw(VP);

    if (piggybank.exist)
        piggybank.draw(VP);

    if (powerup.exist)
        powerup.draw(VP);
}

void tick_input(GLFWwindow *window)
{
    int left = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    water_balloon = glfwGetKey(window, GLFW_KEY_W);

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
        player.magnet_influence_x = 0;
        player.tick(speed_horizontal, speed_vertical);
        speed_vertical = 0;
        if (player.position.y > ceiling_pos)
        {
            player.position.y = ceiling_pos;
            player.box.y = player.position.y;
        }
    }

    if (water_balloon)
    {
        if (num_wb == -1)
        {
            WaterBalloon wb = WaterBalloon(player.position.x, player.position.y, COLOR_BLUE);
            waterballoons.push_back(wb);
            num_wb++;
            num_max_waterballoons--;
        }
        WaterBalloon wb = WaterBalloon(player.position.x, player.position.y, COLOR_BLUE);
        float x1 = wb.position.x, y1 = wb.position.y, x2 = waterballoons[num_wb].position.x, y2 = waterballoons[num_wb].position.y;
        if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) > 6.0 && num_max_waterballoons > 0)
        {
            waterballoons.push_back(wb);
            num_wb++;
            num_max_waterballoons--;
            if (num_max_waterballoons == 5)
            {
                powerup.exist = true;
                powerup.position.x = player.position.x - 10;
                powerup.position.y = player_original_y + 10;
            }
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

    int len = firebeams.size();
    for (int i = 0; i < len; i++)
    {
        firebeams[i].tick();
        if (firebeams[i].position.y >= ceiling_pos)
            firebeams[i].direction = -1;
        if (firebeams[i].position.y <= player_original_y + 3)
            firebeams[i].direction = 1;
    }

    if (boomerang.exist)
    {
        if (boomerang.direction == -1)
        {
            boomerang.tick(-0.2, 0);
            if (boomerang.position.x <= player.position.x - 8)
                boomerang.direction = -2;
        }
        if (boomerang.direction == 1)
        {
            boomerang.tick(0.2, 0);
            if (boomerang.position.x >= player.position.x + 10)
                boomerang.exist = false;
        }
        if (boomerang.direction == -2)
        {
            boomerang.rotate(2);
            if (boomerang.rotation <= 90)
                boomerang.tick(-0.1, -0.1);
            else
                boomerang.tick(0.1, -0.1);
            if (boomerang.rotation >= 180)
                boomerang.direction = 1;
        }
        if (boomerang.direction == 2)
        {
            boomerang.rotate(2);
            if (boomerang.rotation <= 270)
                boomerang.tick(0.1, 0.1);
            else
                boomerang.tick(-0.1, 0.1);
            if (boomerang.rotation <= 0)
                boomerang.direction = -1;
        }
    }

    if (abs(dragon.position.y - player.position.y) >= 5)
    {
        if (dragon.position.y > player.position.y)
            dragon.speed_y = -0.1;
        if (dragon.position.y < player.position.y)
            dragon.speed_y = 0.1;
        dragon.tick();
    }
    if (dragon.position.y <= player_original_y + 3)
        dragon.position.y = player_original_y + 3;
    if (dragon.position.y >= ceiling_pos - 3)
        dragon.position.y = ceiling_pos - 3;

    // if (dragon.position.x <= player.position.x + 20)
    // {
    //     dragon.iceball();
    // }

    if (dragon.position.x <= player.position.x)
    {
        dragon_appear = time(NULL);
    }
    if ((time(NULL) - dragon_appear) % 2 == 0)
    {
        int num_ib = iceballs.size();
        WaterBalloon iceball = WaterBalloon(dragon.position.x, dragon.position.y, COLOR_ICE);
        if (num_ib == 0)
        {
            iceball.speed_x = -0.05;
            iceball.speed_y = 0.01;
            iceballs.push_back(iceball);
        }
        else
        {
            float x1 = iceball.position.x, y1 = iceball.position.y, x2 = iceballs[num_ib - 1].position.x, y2 = iceballs[num_ib - 1].position.y;
            if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) > 6.0)
            {
                iceball.speed_x = -0.05;
                iceball.speed_y = 0.01;
                iceballs.push_back(iceball);
            }
        }
    }
    len = iceballs.size();
    for (int i = 0; i < len; i++)
    {
        iceballs[i].tick();
    }

    len = waterballoons.size();
    for (int i = 0; i < len; i++)
    {
        waterballoons[i].tick();
        waterballoons[i].speed_y -= 0.01;
    }

    if (magnet.exist)
    {
        if (player.position.x >= magnet.position.x - 7 && player.position.x <= magnet.position.x + 7 && player.position.y <= magnet.position.y + 5 && player.position.y >= magnet.position.y - 5)
        {
            if (player.position.x <= magnet.position.x)
                player.magnet_influence_x += 0.05;
            if (player.position.x >= magnet.position.x)
                player.magnet_influence_x -= 0.05;
            if (player.position.y <= magnet.position.y)
                player.magnet_influence_y += 0.01;
            if (player.position.y >= magnet.position.y)
                player.magnet_influence_y -= 0.01;
            player.tick(speed_horizontal, speed_vertical);
            // if (player.position.x > camera_center.x + 7)
            // {
            //     camera_pos += glm::vec3(speed_horizontal + player.magnet_influence_x, 0, 0);
            //     camera_center += glm::vec3(speed_horizontal + player.magnet_influence_x, 0, 0);
            // }
            // if (player.position.x < camera_center.x - 7 && player.position.x > player_original_x)
            // {
            //     camera_pos -= glm::vec3(speed_horizontal + player.magnet_influence_x, 0, 0);
            //     camera_center -= glm::vec3(speed_horizontal + player.magnet_influence_x, 0, 0);
            // }
        }
        else
        {
            player.magnet_influence_x = 0;
            player.magnet_influence_y = 0;
        }
    }
    if (player.position.y <= player_original_y)
    {
        player.magnet_influence_y = 0;
        player.position.y = player_original_y;
    }

    if ((time(NULL) - time_start) % 20 == 0 && magnet.exist == false)
    {
        magnet_created = time(NULL);
        magnet.exist = true;
        int mag_x = 3 + rand() % 5;
        int mag_y = rand() % 10;
        magnet.position.x = player.position.x + (float)mag_x;
        magnet.position.y = player_original_y + (float)mag_y;
    }
    if ((time(NULL) - magnet_created) == 10)
    {
        magnet.exist = false;
        player.magnet_influence_x = 0;
        player.magnet_influence_y = 0;
    }

    if ((time(NULL) - time_start) % 50 == 0 && boomerang.exist == false)
    {
        boomerang.exist = true;
        boomerang.position.x = player.position.x + 20;
        boomerang.position.y = player_original_y + 10;
    }

    if (piggybank.exist)
    {
        if (piggybank.position.y >= ceiling_pos)
            piggybank.speed_y = -0.1;
        if (piggybank.position.y <= player_original_y + 5)
            piggybank.speed_y = 0.1;
        piggybank.tick();
    }

    if (powerup.exist)
    {
        if (powerup.position.y >= ceiling_pos)
            powerup.speed_y = -0.1;
        if (powerup.position.y <= player_original_y + 5)
            powerup.speed_y = 0.1;
        powerup.tick();
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

    float prev_c_x = 0;
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
        prev_c_x = c_x + num_coins;
    }

    float prev_fl_x = 10;
    for (int i = 0; i < 20; i++)
    {
        int f_x = rand() % 20;
        int f_y = rand() % 5;
        int f_l = rand() % 3;
        int f_r = rand() % 180;
        float fl_x = prev_fl_x + 10 + (float)f_x;
        float fl_y = player_original_y + 5 + (float)f_y;
        float fl_len = 3 + (float)f_l;
        float fl_rot = -90 + (float)f_r;
        Firelines fl = Firelines(fl_x, fl_y, fl_len, fl_rot);
        firelines.push_back(fl);
        prev_fl_x = fl_x + 3 * fl_len;
    }

    float prev_fb_x = 10;
    for (int i = 0; i < 10; i++)
    {
        int f_y = rand() % 5;
        int f_x = rand() % 20;
        float fb_x = prev_fb_x + 10 + (float)f_x;
        float fb_y = 5 + (float)f_y;
        float fb_len = 10;
        Firebeams fb = Firebeams(fb_x, fb_y, fb_len);
        firebeams.push_back(fb);
        prev_fb_x = fb_x + 3 * fb_len;
    }

    boomerang = Boomerang(40, 0);

    dragon = Dragon(20, 20);

    magnet = Magnet(7, player_original_y);

    piggybank = Piggybank(0, 0);
    powerup = Powerup(4, 4);

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
    num_max_waterballoons = 10;

    window = initGLFW(width, height);
    initGL(window, width, height);
    time_start = time(NULL);

    camera_pos = glm::vec3(0, 0, 3);
    camera_center = glm::vec3(0, 0, 0);
    camera_up = glm::vec3(0, 1, 0);

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
                    cout << points << endl;
                    cout << points << endl;
                    coins_collected++;
                    coins[i].exist = false;
                }
            }

            // len = firelines.size();
            // for (int i = 0; i < len; i++)
            // {
            //     if (detect_collision(player.box, firelines[i].box))
            //         cout << "DIE" << '\n';
            // }

            len = firebeams.size();
            for (int i = 0; i < len; i++)
            {
                if (detect_collision(player.box, firebeams[i].box))
                {
                    points -= 10;
                    cout << points << endl;
                    player.position.y = player_original_y;
                    player.box.y = player.position.y;
                }
            }

            len = iceballs.size();
            for (int i = 0; i < len; i++)
            {
                if (iceballs[i].exist && detect_collision(player.box, iceballs[i].box))
                {
                    points -= 5;
                    cout << points << endl;
                    iceballs[i].exist = false;
                }
            }

            if (boomerang.exist && detect_collision(player.box, boomerang.box))
            {
                boomerang.exist = false;
                points -= 20;
                cout << points << endl;
            }

            if (piggybank.exist && detect_collision(player.box, piggybank.box))
            {
                points += 50;
                cout << points << endl;
                piggybank.exist = false;
            }

            if (powerup.exist && detect_collision(player.box, powerup.box))
            {
                num_max_waterballoons += 5;
                cout << num_max_waterballoons << endl;
                powerup.exist = false;
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
