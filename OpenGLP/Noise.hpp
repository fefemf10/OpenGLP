#pragma once
namespace Noise
{
	double getNoise2D(unsigned char countOctaves, double x, double z, double seed, double scale, double persistence = 0.5, double pow = 1.0);
	double getNoise3D(unsigned char countOctaves, double x, double y, double z, double seed, double scale, double persistence = 0.5, double pow = 1.0);
}