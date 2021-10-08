#include "Noise.hpp"
#include <cmath>
#include <GLM/gtc/noise.hpp>
namespace Noise
{
	double getNoise2D(unsigned char countOctaves, double x, double z, double seed, double scale, double persistence, double pow)
	{
		double maxAmp = 0.0;
		double amp = 1.0;
		double noise{};
		for (unsigned char i = 0; i < countOctaves; ++i)
		{
			noise += glm::simplex(glm::dvec3(x * scale, seed, z * scale)) * amp;
			maxAmp += amp;
			amp *= persistence;
			scale *= 2.0;
		}
		noise /= maxAmp;
		return std::pow(noise, pow);
	}
	double getNoise3D(unsigned char countOctaves, double x, double y, double z, double seed, double scale, double persistence, double pow)
	{
		double maxAmp = 0.0;
		double amp = 1.0;
		double noise{};
		for (unsigned char i = 0; i < countOctaves; ++i)
		{
			noise += glm::simplex(glm::dvec4(x * scale, y * scale, z * scale, seed)) * amp;
			maxAmp += amp;
			amp *= persistence;
			scale *= 2.0;
		}
		noise /= maxAmp;
		return std::pow(noise, pow);
	}
}