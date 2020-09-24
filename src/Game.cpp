#include "Game.hpp"

void Game::start()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return;
    }

    if (!Window::create(1080, 1080, "Bubbles"))
    {
        fprintf(stderr, "Failed to create window.\n");
        getchar();
        glfwTerminate();
        return;
    }

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW.\n");
        getchar();
        glfwTerminate();
        return;
    }

    glfwSetInputMode(Window::get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SphereModel::init(40);
    Camera::init();
    player.init();
    board.init();

    std::cout << "\nControls:\n";
    std::cout << "W - move forward\n";
    std::cout << "S - move backward\n";
    std::cout << "A - move left\n";
    std::cout << "D - move right\n";
    std::cout << "SPACE - move up\n";
    std::cout << "LEFT ALT - move down\n";
    std::cout << "MOUSE - move camera\n";
    std::cout << "MOUSE SCROLL - zoom\n";
    std::cout << "TAB - change view\n";

    std::cout << "\nRULES OF THE GAME:\n";
    std::cout << "Get to the other side of aquarium without touching the bubbles\n"
	    		  "before running out of the time.\n"
			  "Collect lightning green bubbles to aquire points and time.\n";
    std::cout << "If you manage to complete the level before running out of the time\n"
	    		  "you get 1 point for every 5 seconds of the left time.\n";
    std::cout << "When you fail you lose 1 point + the points you earned in the current level.\n\n";

    std::cout << "Prepare to beat the 13 stages of the increasing level of difficulty.\n";
    std::cout << "Enjoy and good luck!\n\n";
    // std::cout << "Przedostaj sie na drugi koniec akwarium bez dotykania baniek\n"
                 // "przed uplywem czasu. Zbieraj swiecace zielone banki, zeby\n"
                 // "zdobyc punkty i wydluzyc czas.\n";
    // std::cout << "Kazde 5 sekund z pozostalego czasu przy ukonczeniu poziomu\n"
                 // "to jeden dodatkowy punkt.\n";
    // std::cout << "Kazda porazka to starta jednego punkty i utrata punktow\n"
                 // "uzyskanych na aktualnym poziomie.\n";
    // std::cout << "Przed toba 13 poziomow ze wzrastajacym stopniem trudności.\n";
    // std::cout << "Milej gry!\n\n";

    run();
}

void Game::run()
{
    while (!Window::shouldClose())
    {   
        update();

        if (collided())
            restart();
        if (finished())
            restart(true);

        draw();
        glfwPollEvents();
    }

    board.terminate();
    player.terminate();
    glfwTerminate();
}   

void Game::update()
{
    player.update(board.min_x, board.max_x,
                  board.min_y, board.max_y,
                  board.min_z, board.max_z);
    board.update();

    static double last_time = glfwGetTime();
    double current_time = glfwGetTime();
    double delta_time = current_time - last_time;
    last_time = current_time;
    double new_time = remaining_time - delta_time;

    if (std::floor(remaining_time) != std::floor(new_time))
        std::cout << std::floor(remaining_time) << "s\n";

    remaining_time -= delta_time;

    if (remaining_time < 0.0)
        restart();
}

bool Game::collided()
{
    int score_add;
    bool collision = board.collided(player.getPosition(), player.getRadius(), score_add);

    level_score += score_add;
    while (score_add--)
        ++remaining_time,
        std::cout << "\nSCORED!\n" << "\n";

    return collision;
}

bool Game::finished()
{
    float delta = 1.0f;
    return player.getPosition().z >= board.max_z - delta;
}

void Game::restart(bool finished)
{
    player.restart();

    if (finished)
    {
        ++level;
        score += level_score;

        if (level == 13)
        {
            std::cout << "\nCONGRATULATIONS!\n";
            std::cout << "You have won the game!\n";
            std::cout << "You have finished with " << score << " points!\n";
            int add_points = int(remaining_time) / 5;
            if (add_points)
            {
                std::cout << "Adding " << add_points << " additional points for remaining time.\n";
                score += add_points;
            }
            else
                std::cout << "No additional points for remaining time.\n";
            std::cout << "Starting over\n\n";

            score = 0;
            level = 1;
        }
        else
        {
            std::cout << "\nCONGRATUALATIONS!\n";
            std::cout << "You have finished this level!\n";
            int add_points = int(remaining_time) / 5;
            if (add_points)
            {
                std::cout << "Adding " << add_points << " additional points for remaining time.\n";
                score += add_points;
            }
            else
                std::cout << "No additional points for remaining time.\n";
            std::cout << "Current score: " << score << "\n";
        }
        
        board.loadLevel(level);
    }
    else
    {       
        std::cout << "\nGAME OVER!\n";
        --score;
        std::cout << "Current score: " << score << "\n";
        board.restart();
    }

    level_score = 0;
    remaining_time = 20.0;
}

void Game::draw()
{
    Window::clear();

    board.draw();
    player.draw();

    Window::display();
}
