#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#include "Window.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "SphereModel.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "FastNoise.h"

class Game
{
private:
    Player player;
    Board board;

    GLuint level = 1;

    int score = 0;
    int level_score = 0;

    double remaining_time = 20.0;

    void run();
    void update();  
    bool collided();
    bool finished();
    void restart(bool finished = false);
    void draw();

public:
    void start();
};