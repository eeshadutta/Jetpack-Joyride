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
#include "score.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

Player player;
vector<WaterBalloon> smoke;

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

vector<Score> scores;

float screen_zoom = 0.25, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float speed_horizontal, speed_vertical;
float player_original_x, player_original_y;
float dragon_x;
float boomerang_speed_x, boomerang_speed_y;
float ceiling_pos = 11;
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

    platform.draw(VP);
    ceiling.draw(VP);

    player.draw(VP);
    len = smoke.size();
    for (int i = 0; i < len; i++)
    {
        if (smoke[i].exist)
            smoke[i].draw(VP);
    }

    len = scores.size();
    for (int i = 0; i < len; i++)
    {
        scores[i].draw(VP);
    }
}

void score_decide(int digit, int i)
{
    if (digit == 0)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist3 = scores[i].exist4 = scores[i].exist5 = scores[i].exist6 = true;
        scores[i].exist7 = false;
    }
    if (digit == 1)
    {
        scores[i].exist2 = scores[i].exist3 = true;
        scores[i].exist1 = scores[i].exist4 = scores[i].exist5 = scores[i].exist6 = scores[i].exist7 = false;
    }
    if (digit == 2)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist4 = scores[i].exist5 = scores[i].exist7 = true;
        scores[i].exist3 = scores[i].exist6 = false;
    }
    if (digit == 3)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist3 = scores[i].exist4 = scores[i].exist7 = true;
        scores[i].exist5 = scores[i].exist6 = false;
    }
    if (digit == 4)
    {
        scores[i].exist2 = scores[i].exist3 = scores[i].exist6 = scores[i].exist7 = true;
        scores[i].exist1 = scores[i].exist4 = scores[i].exist5 = false;
    }
    if (digit == 5)
    {
        scores[i].exist1 = scores[i].exist3 = scores[i].exist4 = scores[i].exist6 = scores[i].exist7 = true;
        scores[i].exist2 = scores[i].exist5 = false;
    }
    if (digit == 6)
    {
        scores[i].exist1 = scores[i].exist3 = scores[i].exist4 = scores[i].exist5 = scores[i].exist6 = scores[i].exist7 = true;
        scores[i].exist2 = false;
    }
    if (digit == 7)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist3 = true;
        scores[i].exist4 = scores[i].exist5 = scores[i].exist6 = scores[i].exist7 = true;
    }
    if (digit == 8)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist3 = scores[i].exist4 = scores[i].exist5 = scores[i].exist6 = scores[i].exist7 = true;
    }
    if (digit == 9)
    {
        scores[i].exist1 = scores[i].exist2 = scores[i].exist3 = scores[i].exist4 = scores[i].exist6 = scores[i].exist7 = true;
        scores[i].exist5 = false;
    }
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
            int len = scores.size();
            for (int i = 0; i < len; i++)
                scores[i].position.x -= 0.4;
        }
    }

    if (right)
    {
        if (player.position.x < dragon_x - 8)
            speed_horizontal = 0.4;
        player.tick(speed_horizontal, 0);
        speed_horizontal = 0;
        if (player.position.x > camera_center.x)
        {
            camera_pos += glm::vec3(0.4, 0, 0);
            camera_center += glm::vec3(0.4, 0, 0);
            int len = scores.size();
            for (int i = 0; i < len; i++)
                scores[i].position.x += 0.4;
        }
        else
        {
            speed_horizontal = 0;
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
        int type = rand() % 3;
        color_t color_out;
        if (type == 0)
            color_out = COLOR_MAROON;
        else if (type == 1)
            color_out = COLOR_ORANGE;
        else
            color_out = COLOR_YELLOW;
        WaterBalloon sb = WaterBalloon(player.position.x - 0.5, player.position.y - 0.2, 0.2, color_out, COLOR_BACKGROUND);
        sb.speed_x = 0;
        sb.speed_y = -0.01;
        int num_sb = smoke.size();
        if (num_sb == 0)
            smoke.push_back(sb);
        else
        {
            float x1 = sb.position.x, y1 = sb.position.y, x2 = smoke[num_sb - 1].position.x, y2 = smoke[num_sb - 1].position.y;
            if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) > 0.4)
            {
                smoke.push_back(sb);
            }
        }
    }

    if (down)
    {
        player.tick(0, -0.05);
    }

    if (water_balloon)
    {
        if (num_wb == -1)
        {
            WaterBalloon wb = WaterBalloon(player.position.x, player.position.y, 0.3, COLOR_BLUE, COLOR_BLUE);
            wb.speed_x = 0.1;
            wb.speed_y = 0.4;
            waterballoons.push_back(wb);
            num_wb++;
            num_max_waterballoons--;
        }
        WaterBalloon wb = WaterBalloon(player.position.x, player.position.y, 0.3, COLOR_BLUE, COLOR_BLUE);
        wb.speed_x = 0.1;
        wb.speed_y = 0.4;
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

    if (abs(dragon.position.y - player.position.y) >= 0)
    {
        if (dragon.position.y > player.position.y)
            dragon.speed_y = -0.3;
        if (dragon.position.y < player.position.y)
            dragon.speed_y = 0.3;
        dragon.tick();
    }
    if (dragon.position.y <= player_original_y + 2)
        dragon.position.y = player_original_y + 2;
    if (dragon.position.y >= ceiling_pos - 2)
        dragon.position.y = ceiling_pos - 2;

    if (dragon.position.x <= player.position.x + 10)
    {
        dragon_appear = time(NULL);
    }
    if ((time(NULL) - dragon_appear) % 2 == 0)
    {
        int num_ib = iceballs.size();
        WaterBalloon iceball = WaterBalloon(dragon.position.x, dragon.position.y, 0.3, COLOR_ICE, COLOR_ICE);
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

    len = smoke.size();
    for (int i = 0; i < len; i++)
    {
        if (smoke[i].exist)
            smoke[i].tick();

        if (player.position.y - smoke[i].position.y >= 4 || player.position.x - smoke[i].position.x >= 4 || smoke[i].position.x - player.position.x >= 4)
            smoke[i].exist = false;
    }

    int buf = points;
    int h = buf / 100;
    buf %= 100;
    int t = buf / 10;
    int u = buf % 10;
    score_decide(h, 2);
    score_decide(t, 3);
    score_decide(u, 4);
    t = coins_collected / 10;
    u = coins_collected % 10;
    score_decide(t, 7);
    score_decide(u, 8);
    t = num_max_waterballoons / 10;
    u = num_max_waterballoons % 10;
    score_decide(t, 11);
    score_decide(u, 12);
}

void initGL(GLFWwindow *window, int width, int height)
{
    // Create the models
    player = Player(player_original_x, player_original_y);
    platform = Platform(-30, -28, COLOR_BLACK);
    ceiling = Platform(-30, 13, COLOR_LIGHT_GREY);

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
        prev_c_x = c_x + 2 * num_coins;
    }

    float prev_fl_x = 0;
    for (int i = 0; i < 20; i++)
    {
        int f_x = rand() % 20;
        int f_y = rand() % 5;
        int f_l = rand() % 3;
        int f_r = rand() % 180;
        float fl_x = prev_fl_x + 10 + (float)f_x;
        float fl_y = player_original_y + 5 + (float)f_y;
        float fl_len = 4 + (float)f_l;
        float fl_rot = -90 + (float)f_r;
        Firelines fl = Firelines(fl_x, fl_y, fl_len, fl_rot);
        firelines.push_back(fl);
        prev_fl_x = fl_x + fl_len;
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
        prev_fb_x = fb_x + 5 * fb_len;
    }

    boomerang = Boomerang(40, 0);

    dragon = Dragon(dragon_x, 20);

    magnet = Magnet(7, player_original_y);

    piggybank = Piggybank(0, 0);
    powerup = Powerup(4, 4);

    float pos_org_x = camera_pos.x - 15;
    float pos_org_y = camera_pos.y + 15;
    Score s;
    Score p;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist5 = s.exist6 = s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 1.0;
    s = Score(pos_org_x, pos_org_y);
    s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 4.0;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 0.8;
    s = Score(pos_org_x, pos_org_y);
    s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 1.1;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 4.0;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 1.0;
    s = Score(pos_org_x, pos_org_y);
    s.exist7 = true;
    scores.push_back(s);

    pos_org_x += 0.6;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

    pos_org_x += 1.2;
    s = Score(pos_org_x, pos_org_y);
    s.exist1 = s.exist2 = s.exist3 = s.exist4 = s.exist5 = s.exist6 = true;
    scores.push_back(s);

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
    player_original_y = -6;
    boomerang_speed_x = -0.2;
    num_max_waterballoons = 30;
    dragon_x = 150;

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
                    coins[i].exist = false;
                    points += coins[i].type * 5;
                    coins_collected++;
                }
            }

            len = firelines.size();
            for (int i = 0; i < len; i++)
            {
                float x1, y1, x2, y2;
                x1 = firelines[i].position.x + (firelines[i].length / 2) * cos(firelines[i].rotation * M_PI / 180.0f);
                y1 = firelines[i].position.y + (firelines[i].length / 2) * sin(firelines[i].rotation * M_PI / 180.0f);
                x2 = firelines[i].position.x - (firelines[i].length / 2) * cos(firelines[i].rotation * M_PI / 180.0f);
                y2 = firelines[i].position.y - (firelines[i].length / 2) * sin(firelines[i].rotation * M_PI / 180.0f);

                if (firelines[i].exist && detect_collision_line(x1, y1, x2, y2, player.box))
                {
                    points -= 5;
                    player.position.y = player_original_y;
                    player.box.y = player.position.y;
                }

                int l = waterballoons.size();
                for (int j = 0; j < l; j++)
                {
                    if (waterballoons[j].exist && firelines[i].exist && detect_collision_line(x1, y1, x2, y2, waterballoons[j].box))
                    {
                        firelines[i].exist = false;
                        waterballoons[j].exist = false;
                    }
                }
            }

            len = firebeams.size();
            for (int i = 0; i < len; i++)
            {
                if (detect_collision(player.box, firebeams[i].box))
                {
                    points -= 10;
                    player.position.y = player_original_y;
                    player.box.y = player.position.y;
                }
            }

            len = iceballs.size();
            for (int i = 0; i < len; i++)
            {
                if (iceballs[i].exist && detect_collision(player.box, iceballs[i].box))
                {
                    iceballs[i].exist = false;
                    points -= 5;
                }
            }

            if (boomerang.exist && detect_collision(player.box, boomerang.box))
            {
                boomerang.exist = false;
                points -= 20;
            }

            if (piggybank.exist && detect_collision(player.box, piggybank.box))
            {
                piggybank.exist = false;
                points += 50;
            }

            if (powerup.exist && detect_collision(player.box, powerup.box))
            {
                powerup.exist = false;
                num_max_waterballoons += 5;
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

bool detect_collision_line(float x1, float y1, float x2, float y2, bounding_box_t box)
{
    float a1, b1, c1, px, py;

    a1 = y2 - y1;
    b1 = x1 - x2;
    c1 = a1 * x1 + b1 * y1;

    float y_upper = box.y + box.height / 2;
    float y_lower = box.y - box.height / 2;
    float x_left = box.x - box.width / 2;
    float x_right = box.x + box.width / 2;

    px = (c1 - b1 * y_upper) / a1;
    if (px >= min(x1, x2) && px <= max(x1, x2) && px >= x_left && px <= x_right)
    {
        return true;
    }

    px = (c1 - b1 * y_lower) / a1;
    if (px >= min(x1, x2) && px <= max(x1, x2) && px >= x_left && px <= x_right)
    {
        return true;
    }

    py = (c1 - a1 * x_left) / b1;
    if (py >= min(y1, y2) && py <= max(y1, y2) && py >= y_lower && py <= y_upper)
    {
        return true;
    }

    py = (c1 - a1 * x_right) / b1;
    if (py >= min(y1, y2) && py <= max(y1, y2) && py >= y_lower && py <= y_upper)
    {
        return true;
    }
    return false;
}

void reset_screen()
{
    float top = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left = screen_center_x - 4 / screen_zoom;
    float right = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
