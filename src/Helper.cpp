#include "Helper.h"
#include "global.h"

glm::vec3 pos(float d)
{
    std::uniform_real_distribution<float> x(LEFTWALL + d, RIGHTWALL - d);
    std::uniform_real_distribution<float> y(DOWNWALL + d, UPWALL - d);

    return glm::vec3(x(gen), y(gen), Z);
}
