#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <random>
#include <iostream>
#include <vector>

struct Color
{
    GLfloat r, g, b, a = 1.0f;

    static Color RED;
    static Color BLUE;
    static Color GREEN;
    static Color YELLOW;
    static Color PURPLE;
    static Color ORANGE;
    static Color BLACK;
    static Color NONE;
    static Color RAINBOW;
    static Color DARK_PURPLE;
    static Color DARK_YELLOW;
    static Color WHITE;
    static Color PINK;
    static Color LIGHT_BLUE;

    static std::vector<Color> colors;

    static Color getRandom();

    bool operator== (Color color)
    {
        return this->r == color.r && this->g == color.g && this->b == color.b;
    }
};

class Random
{
private:
    static std::mt19937 gen;

public:
    template<typename T>
    static T random(T min = 0, T max = 1)
    {
        using dist = std::conditional_t<
            std::is_integral<T>::value,
            std::uniform_int_distribution<T>,
            std::uniform_real_distribution<T>
        >;
        return dist{min, max}(gen);
    }

    static GLfloat randnorm()
    {
        return random(-1.0f, 1.0f);
    }
};

class Math
{
public:

    template<typename T>
    static T constrain(T value, T value_min, T value_max, T target_min, T target_max)
    {
        T value_length = value_max - value_min;
        T ratio = (value - value_min) / value_length;
        return target_min + (target_max - target_min) * ratio;
    }

    template<typename T>
    static T clip(T value, T min, T max)
    {
        return std::min(std::max(min, value), max);
    }
};

struct Bubble
{
    glm::vec3 position;
    GLfloat scale;
    Color color;
    GLfloat zbuffer;
    bool special;

    void calcZBuffer(const glm::mat4& view);
};
