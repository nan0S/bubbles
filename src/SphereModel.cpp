#include "SphereModel.hpp"

std::vector<GLfloat> SphereModel::vertices = {};
std::vector<GLuint> SphereModel::indices = {};

void SphereModel::init(int limit)
{
    size_t size = (limit + 1) * (limit + 1);
    vertices.reserve(3 * size);

    float sector_step = 2 * M_PI / limit;
    float stack_step = M_PI / limit;

    for (int i = 0; i <= limit; ++i)
    {
        float stack_angle = M_PI_2 - i * stack_step;
        float xz = cosf(stack_angle);
        float y = sinf(stack_angle);

        for (int j = 0; j <= limit; ++j)
        {
            float sector_angle = j * sector_step;
            float z = xz * cosf(sector_angle);
            float x = xz * sinf(sector_angle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (int i = 0; i < limit; ++i)
    {
        GLuint k1 = i * (limit + 1);
        GLuint k2 = k1 + limit + 1;    

        for (int j = 0; j < limit; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (limit - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

const std::vector<GLfloat>& SphereModel::getVertices()
{
    return vertices;
}

const std::vector<GLuint>& SphereModel::getIndices()
{
    return indices;
}