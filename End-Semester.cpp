#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>

// Screen dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 20;
const int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

// Colors
sf::Color COLORS[] = {sf::Color::Green, sf::Color::Red, sf::Color::Blue};

struct Snake
{
    std::vector<sf::Vector2i> body;
    sf::Color color;
    sf::Vector2i direction;
    int score = 0;
    int lives = 3;
    bool grow = false;

    void respawn(sf::Vector2i startPosition, sf::Vector2i startDirection)
    {
        body.clear();
        body.push_back(startPosition);
        direction = startDirection;
        grow = false;
    }
};

struct Food
{
    sf::Vector2i position;
    sf::Color color;
};

// Utility function to generate random position
sf::Vector2i getRandomPosition()
{
    return sf::Vector2i(rand() % GRID_WIDTH, rand() % GRID_HEIGHT);
}

// Function prototypes
void drawGrid(sf::RenderWindow &window);
void drawSnake(sf::RenderWindow &window, const Snake &snake);
void drawFood(sf::RenderWindow &window, const Food &food);
void updateSnake(Snake &snake);
bool checkSelfCollision(const Snake &snake);
bool checkSnakeCollision(const Snake &snake1, const Snake &snake2);
void drawText(sf::RenderWindow &window, const std::string &text, int x, int y, int size, sf::Color color);
void drawMenu(sf::RenderWindow &window, const std::string &title, const std::vector<std::string> &options, int selectedOption);
void drawGameOverMenu(sf::RenderWindow &window, const std::vector<std::string> &options, int selectedOption, const Snake &snake1, const Snake &snake2);

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    // SFML window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Multiplayer Snake Game");
    window.setFramerateLimit(10);

    // Game States
    enum GameState
    {
        MAIN_MENU,
        SINGLE_PLAYER,
        MULTIPLAYER,
        GAME_OVER,
        PAUSED
    };
    GameState currentState = MAIN_MENU;
    GameState previousState = MAIN_MENU; // Variable to store the previous state

    // Snakes
    Snake snake1{{sf::Vector2i(5, 5)}, COLORS[0], {1, 0}, 0, 3};
    Snake snake2{{sf::Vector2i(15, 15)}, COLORS[1], {-1, 0}, 0, 3};

    // Food
    Food food{getRandomPosition(), sf::Color::Yellow};

    int selectedOption = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    if (currentState == SINGLE_PLAYER || currentState == MULTIPLAYER)
                    {
                        previousState = currentState; // Store the current state before pausing
                        currentState = PAUSED;
                        selectedOption = 0;
                    }
                    else if (currentState == PAUSED)
                    {
                        currentState = previousState; // Restore the previous state when resuming
                    }
                }

                if (currentState == MAIN_MENU)
                {
                    if (event.key.code == sf::Keyboard::Up)
                    {
                        selectedOption = (selectedOption - 1 + 3) % 3;
                    }
                    else if (event.key.code == sf::Keyboard::Down)
                    {
                        selectedOption = (selectedOption + 1) % 3;
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (selectedOption == 0)
                        {
                            currentState = SINGLE_PLAYER;
                        }
                        else if (selectedOption == 1)
                        {
                            currentState = MULTIPLAYER;
                        }
                        else if (selectedOption == 2)
                        {
                            window.close();
                        }
                    }
                }
                else if (currentState == PAUSED)
                {
                    if (event.key.code == sf::Keyboard::Up)
                    {
                        selectedOption = (selectedOption - 1 + 2) % 2;
                    }
                    else if (event.key.code == sf::Keyboard::Down)
                    {
                        selectedOption = (selectedOption + 1) % 2;
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (selectedOption == 0)
                        {
                            currentState = previousState; // Resume the previous state
                        }
                        else if (selectedOption == 1)
                        {
                            currentState = MAIN_MENU;
                            snake1.respawn({5, 5}, {1, 0});
                            snake2.respawn({15, 15}, {-1, 0});
                            snake1.score = 0;
                            snake2.score = 0;
                            snake1.lives = 3;
                            snake2.lives = 3;
                        }
                    }
                }
                else if (currentState == GAME_OVER)
                {
                    if (event.key.code == sf::Keyboard::Up)
                    {
                        selectedOption = (selectedOption - 1 + 2) % 2;
                    }
                    else if (event.key.code == sf::Keyboard::Down)
                    {
                        selectedOption = (selectedOption + 1) % 2;
                    }
                    else if (event.key.code == sf::Keyboard::Enter)
                    {
                        if (selectedOption == 0)
                        {
                            currentState = MAIN_MENU;
                            snake1.respawn({5, 5}, {1, 0});
                            snake2.respawn({15, 15}, {-1, 0});
                            snake1.score = 0;
                            snake2.score = 0;
                            snake1.lives = 3;
                            snake2.lives = 3;
                        }
                        else if (selectedOption == 1)
                        {
                            window.close();
                        }
                    }
                }

                if (currentState == SINGLE_PLAYER || currentState == MULTIPLAYER)
                {
                    // Snake 1 controls
                    if (event.key.code == sf::Keyboard::Up && snake1.direction != sf::Vector2i(0, 1))
                        snake1.direction = {0, -1};
                    else if (event.key.code == sf::Keyboard::Down && snake1.direction != sf::Vector2i(0, -1))
                        snake1.direction = {0, 1};
                    else if (event.key.code == sf::Keyboard::Left && snake1.direction != sf::Vector2i(1, 0))
                        snake1.direction = {-1, 0};
                    else if (event.key.code == sf::Keyboard::Right && snake1.direction != sf::Vector2i(-1, 0))
                        snake1.direction = {1, 0};

                    // Snake 2 controls
                    if (currentState == MULTIPLAYER)
                    {
                        if (event.key.code == sf::Keyboard::W && snake2.direction != sf::Vector2i(0, 1))
                            snake2.direction = {0, -1};
                        else if (event.key.code == sf::Keyboard::S && snake2.direction != sf::Vector2i(0, -1))
                            snake2.direction = {0, 1};
                        else if (event.key.code == sf::Keyboard::A && snake2.direction != sf::Vector2i(1, 0))
                            snake2.direction = {-1, 0};
                        else if (event.key.code == sf::Keyboard::D && snake2.direction != sf::Vector2i(-1, 0))
                            snake2.direction = {1, 0};
                    }
                }
            }
        }

        // Update game logic
        if (currentState == SINGLE_PLAYER || currentState == MULTIPLAYER)
        {
            updateSnake(snake1);
            if (currentState == MULTIPLAYER)
                updateSnake(snake2);

            // Check self-collision
            if (checkSelfCollision(snake1))
            {
                snake1.lives--;
                if (snake1.lives > 0)
                    snake1.respawn({5, 5}, {1, 0});
                else
                    currentState = GAME_OVER;
            }
            if (currentState == MULTIPLAYER && checkSelfCollision(snake2))
            {
                snake2.lives--;
                if (snake2.lives > 0)
                    snake2.respawn({15, 15}, {-1, 0});
                else
                    currentState = GAME_OVER;
            }

            // Check snake collision
            if (currentState == MULTIPLAYER)
            {
                // Check head-on collision
                if (snake1.body[0] == snake2.body[0])
                {
                    if (snake1.body.size() < snake2.body.size())
                    {
                        snake1.lives--;
                        if (snake1.lives > 0)
                            snake1.respawn({5, 5}, {1, 0});
                        else
                            currentState = GAME_OVER;
                    }
                    else if (snake1.body.size() > snake2.body.size())
                    {
                        snake2.lives--;
                        if (snake2.lives > 0)
                            snake2.respawn({15, 15}, {-1, 0});
                        else
                            currentState = GAME_OVER;
                    }
                    else
                    {
                        // Both snakes lose a life if they are the same size
                        snake1.lives--;
                        snake2.lives--;
                        if (snake1.lives > 0)
                            snake1.respawn({5, 5}, {1, 0});
                        if (snake2.lives > 0)
                            snake2.respawn({15, 15}, {-1, 0});
                        if (snake1.lives <= 0 || snake2.lives <= 0)
                            currentState = GAME_OVER;
                    }
                }
                else
                {
                    // Check if snake1 collides with snake2's body (excluding the head)
                    if (std::find(snake2.body.begin() + 1, snake2.body.end(), snake1.body[0]) != snake2.body.end())
                    {
                        snake1.lives--;
                        if (snake1.lives > 0)
                            snake1.respawn({5, 5}, {1, 0});
                        else
                            currentState = GAME_OVER;
                    }

                    // Check if snake2 collides with snake1's body (excluding the head)
                    if (std::find(snake1.body.begin() + 1, snake1.body.end(), snake2.body[0]) != snake1.body.end())
                    {
                        snake2.lives--;
                        if (snake2.lives > 0)
                            snake2.respawn({15, 15}, {-1, 0});
                        else
                            currentState = GAME_OVER;
                    }
                }
            }

            // Food logic
            if (snake1.body[0] == food.position)
            {
                snake1.grow = true;
                snake1.score += 10;
                food.position = getRandomPosition();
            }
            if (currentState == MULTIPLAYER && snake2.body[0] == food.position)
            {
                snake2.grow = true;
                snake2.score += 10;
                food.position = getRandomPosition();
            }

            // Check if both snakes are dead in multiplayer mode
            if (currentState == MULTIPLAYER && snake1.lives <= 0 && snake2.lives <= 0)
            {
                currentState = GAME_OVER;
            }
        }

        // Render game
        window.clear(sf::Color::Black);

        if (currentState == MAIN_MENU)
        {
            std::vector<std::string> options = {"Single Player", "Multiplayer", "Quit"};
            drawMenu(window, "Snake Game", options, selectedOption);
        }
        else if (currentState == SINGLE_PLAYER || currentState == MULTIPLAYER)
        {
            drawGrid(window);
            drawSnake(window, snake1);
            if (currentState == MULTIPLAYER)
                drawSnake(window, snake2);
            drawFood(window, food);

            // Display scores and lives
            drawText(window, "Snake 1: " + std::to_string(snake1.score) + " Lives: " + std::to_string(snake1.lives), 10, 10, 20, sf::Color::Green);
            if (currentState == MULTIPLAYER)
                drawText(window, "Snake 2: " + std::to_string(snake2.score) + " Lives: " + std::to_string(snake2.lives), 10, 40, 20, sf::Color::Red);
        }
        else if (currentState == GAME_OVER)
        {
            std::vector<std::string> options = {"Main Menu", "Quit"};
            drawGameOverMenu(window, options, selectedOption, snake1, snake2);
        }
        else if (currentState == PAUSED)
        {
            std::vector<std::string> options = {"Resume", "Main Menu"};
            drawMenu(window, "Paused", options, selectedOption);
        }

        window.display();
    }

    return 0;
}

// Utility functions
void drawGrid(sf::RenderWindow &window)
{
    for (int x = 0; x < GRID_WIDTH; ++x)
    {
        for (int y = 0; y < GRID_HEIGHT; ++y)
        {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            cell.setFillColor(sf::Color::Transparent);
            cell.setOutlineColor(sf::Color(50, 50, 50));
            cell.setOutlineThickness(1);
            window.draw(cell);
        }
    }
}

void drawSnake(sf::RenderWindow &window, const Snake &snake)
{
    for (const auto &segment : snake.body)
    {
        sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        rect.setPosition(segment.x * CELL_SIZE, segment.y * CELL_SIZE);
        rect.setFillColor(snake.color);
        window.draw(rect);
    }
}

void drawFood(sf::RenderWindow &window, const Food &food)
{
    sf::CircleShape circle(CELL_SIZE / 2);
    circle.setPosition(food.position.x * CELL_SIZE, food.position.y * CELL_SIZE);
    circle.setFillColor(food.color);
    window.draw(circle);
}

void updateSnake(Snake &snake)
{
    if (snake.body.empty())
    {
        return; // Early exit if the snake body is empty
    }

    sf::Vector2i newHead = snake.body[0] + snake.direction;

    // Wrap around borders
    if (newHead.x < 0)
        newHead.x = GRID_WIDTH - 1;
    if (newHead.y < 0)
        newHead.y = GRID_HEIGHT - 1;
    if (newHead.x >= GRID_WIDTH)
        newHead.x = 0;
    if (newHead.y >= GRID_HEIGHT)
        newHead.y = 0;

    snake.body.insert(snake.body.begin(), newHead);

    if (!snake.grow)
    {
        if (!snake.body.empty())
        {
            snake.body.pop_back();
        }
    }
    else
    {
        snake.grow = false;
    }
}

bool checkSelfCollision(const Snake &snake)
{
    for (size_t i = 1; i < snake.body.size(); ++i)
    {
        if (snake.body[0] == snake.body[i])
            return true;
    }
    return false;
}

bool checkSnakeCollision(const Snake &snake1, const Snake &snake2)
{
    for (size_t i = 1; i < snake2.body.size(); ++i)
    {
        if (snake1.body[0] == snake2.body[i])
            return true;
    }
    return false;
}

void drawText(sf::RenderWindow &window, const std::string &text, int x, int y, int size, sf::Color color)
{
    static sf::Font font;
    if (!font.loadFromFile("LuckiestGuy-Regular.ttf"))
        return;

    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    sfText.setPosition(x, y);
    window.draw(sfText);
}

void drawMenu(sf::RenderWindow &window, const std::string &title, const std::vector<std::string> &options, int selectedOption)
{
    drawText(window, title, 300, 150, 50, sf::Color::White);

    for (size_t i = 0; i < options.size(); ++i)
    {
        sf::Color color = (i == selectedOption) ? sf::Color::Yellow : sf::Color::White;
        drawText(window, options[i], 300, 250 + i * 50, 30, color);
    }
}

void drawGameOverMenu(sf::RenderWindow &window, const std::vector<std::string> &options, int selectedOption, const Snake &snake1, const Snake &snake2)
{
    drawText(window, "Game Over!", 300, 150, 50, sf::Color::Red);

    // Display scores and rank players
    std::vector<std::pair<std::string, int>> scores = {
        {"Snake 1", snake1.score},
        {"Snake 2", snake2.score}};

    // Sort scores in descending order
    std::sort(scores.begin(), scores.end(), [](const auto &a, const auto &b)
              { return a.second > b.second; });

    for (size_t i = 0; i < scores.size(); ++i)
    {
        drawText(window, scores[i].first + ": " + std::to_string(scores[i].second), 300, 250 + i * 50, 30, sf::Color::White);
    }

    for (size_t i = 0; i < options.size(); ++i)
    {
        sf::Color color = (i == selectedOption) ? sf::Color::Yellow : sf::Color::White;
        drawText(window, options[i], 300, 350 + i * 50, 30, color);
    }
}