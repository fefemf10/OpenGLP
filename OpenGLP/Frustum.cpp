#include "Frustum.hpp"

void Frustum::update()
{
    //m_Frustum[RIGHT][A] = clip[3] - clip[0];
    //m_Frustum[RIGHT][B] = clip[7] - clip[4];
    //m_Frustum[RIGHT][C] = clip[11] - clip[8];
    //m_Frustum[RIGHT][D] = clip[15] - clip[12];

    //// This will extract the LEFT side of the frustum
    //m_Frustum[LEFT][A] = clip[3] + clip[0];
    //m_Frustum[LEFT][B] = clip[7] + clip[4];
    //m_Frustum[LEFT][C] = clip[11] + clip[8];
    //m_Frustum[LEFT][D] = clip[15] + clip[12];

    //// This will extract the BOTTOM side of the frustum
    //m_Frustum[BOTTOM][A] = clip[3] + clip[1];
    //m_Frustum[BOTTOM][B] = clip[7] + clip[5];
    //m_Frustum[BOTTOM][C] = clip[11] + clip[9];
    //m_Frustum[BOTTOM][D] = clip[15] + clip[13];

    //// This will extract the TOP side of the frustum
    //m_Frustum[TOP][A] = clip[3] - clip[1];
    //m_Frustum[TOP][B] = clip[7] - clip[5];
    //m_Frustum[TOP][C] = clip[11] - clip[9];
    //m_Frustum[TOP][D] = clip[15] - clip[13];

    //// This will extract the BACK side of the frustum
    //m_Frustum[BACK][A] = clip[3] - clip[2];
    //m_Frustum[BACK][B] = clip[7] - clip[6];
    //m_Frustum[BACK][C] = clip[11] - clip[10];
    //m_Frustum[BACK][D] = clip[15] - clip[14];

    //// This will extract the FRONT side of the frustum
    //m_Frustum[FRONT][A] = clip[3] + clip[2];
    //m_Frustum[FRONT][B] = clip[7] + clip[6];
    //m_Frustum[FRONT][C] = clip[11] + clip[10];
    //m_Frustum[FRONT][D] = clip[15] + clip[14];

    planes[0] = clip[3] - clip[0];
    glm::normalize(planes[0]);
    planes[1] = clip[3] + clip[0];
    glm::normalize(planes[1]);
    planes[2] = clip[3] + clip[1];
    glm::normalize(planes[2]);
    planes[3] = clip[3] - clip[1];
    glm::normalize(planes[3]);
    planes[4] = clip[3] - clip[2];
    glm::normalize(planes[4]);
    planes[5] = clip[3] + clip[2];
    glm::normalize(planes[5]);
}

int Frustum::PointInFrustum(const glm::vec3& point) const
{
    int result = FRUSTUM_INSIDE;

    for (int i = 0; i < 6; i++)
    {
        if (getDistance(planes[i], point) < 0)
            return FRUSTUM_OUTSIDE;
    }

    return result;
}

int Frustum::SphereInFrustum(const glm::vec3& point, float radius) const
{
    int result = FRUSTUM_INSIDE;
    float distance;

    for (int i = 0; i < 6; i++) {
        distance = getDistance(planes[i], point);

        if (distance < -radius) {
            return FRUSTUM_OUTSIDE;
        }
        else if (distance < radius) {
            result = FRUSTUM_INTERSECT;
        }
    }

    return result;
}

int Frustum::CubeInFrustum(const glm::vec3& center, float x, float y, float z) const
{
    // NOTE : This code can be optimized, it is just easier to read and understand as is

    int result = FRUSTUM_INSIDE;

    for (int i = 0; i < 6; i++)
    {
        // Reset counters for corners in and out
        int out = 0;
        int in = 0;

        out += getDistance(planes[i], center + glm::vec3(-x, -y, -z)) < 0;
        out += getDistance(planes[i], center + glm::vec3(x, -y, -z)) < 0;
        out += getDistance(planes[i], center + glm::vec3(-x, -y, z)) < 0;
        out += getDistance(planes[i], center + glm::vec3(x, -y, z)) < 0;
        out += getDistance(planes[i], center + glm::vec3(-x, y, -z)) < 0;
        out += getDistance(planes[i], center + glm::vec3(x, y, -z)) < 0;
        out += getDistance(planes[i], center + glm::vec3(-x, y, z)) < 0;
        out += getDistance(planes[i], center + glm::vec3(x, y, z)) < 0;
        in = 8 - out;
        // If all corners are out
        if (!in) {
            return FRUSTUM_OUTSIDE;
        }
        // If some corners are out and others are in	
        else if (out) {
            result = FRUSTUM_INTERSECT;
        }
    }

    return result;
}

float Frustum::getDistance(const glm::vec4& plane, const glm::vec3& point) const
{
    return plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w;
}