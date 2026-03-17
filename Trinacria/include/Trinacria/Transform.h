#pragma once

#include "Macros.h"

#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.hpp"

namespace TRCN_CORE_NAMESPACE
{
    struct Transform
    {
        glm::mat4 GetMatrix() const
        {
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(Position, 0.f));

            // rotate only z axis because we're in 2d. Why would you even rotate in other axis?!
            transform = glm::rotate(transform, glm::radians(YawDegreesRotation), glm::vec3(0.f, 0.f, 1.f));

            transform = glm::scale(transform, glm::vec3(Scale, 1.f));

            // the order is right T * R * S -> first S then R then T

            return transform;
        }

        glm::vec2 Position = glm::vec2(0.f);
        glm::vec2 Scale = glm::vec2(0.5f);

        float YawDegreesRotation = 0.f;
    };
}
