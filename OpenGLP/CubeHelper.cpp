#include "CubeHelper.hpp"
#include "World.hpp"


CubeHelper::CubeHelper(Section& section, const PaletteItem& pi, const glm::ivec3& position, const glm::ivec3& localPos, const glm::vec2& rotation, const Blocks::Model::Element& item, uint32_t countVertex, const TextureAtlas& atlas, const std::unordered_map<std::string, std::string>& textures) : section(section), pi(pi), position(position), localPos(localPos), countVertex(countVertex), item(item), atlas(atlas), textures(textures)
{
	/*auto addQuad = [&](Enums::Direction d, const std::array<std::array<glm::vec3, 4>, 6>& cube, const std::map<Enums::Direction, Blocks::Model::Element::Face>& ifaces,
		const std::unordered_map<std::string, std::string>& textures, const glm::vec3& colorFloat, float lightFactor,
		std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& UV, std::vector<glm::vec3>& color, std::vector<float>& AO, std::vector<uint32_t>& indicies, uint32_t& countVertex)
	{

	};*/
	cube = {
		glm::vec3{ item.from.x, item.to.y, item.to.z },
		{ item.from.x, item.from.y, item.to.z },
		{ item.to.x, item.from.y, item.to.z },
		{ item.to.x, item.to.y, item.to.z },
		{ item.from.x, item.to.y, item.from.z },
		{ item.from.x, item.from.y, item.from.z },
		{ item.to.x, item.from.y, item.from.z },
		{ item.to.x, item.to.y, item.from.z }
	};
	if (item.rotation.rescale)
	{
		if (glm::abs(item.rotation.angle) == 45.f)
		{
			const float rescale = 1.41421358f;
			const glm::vec3 origin(0.5f);
			cube[0] -= origin;
			cube[1] -= origin;
			cube[2] -= origin;
			cube[3] -= origin;
			cube[4] -= origin;
			cube[5] -= origin;
			cube[6] -= origin;
			cube[7] -= origin;
			cube[0] *= rescale;
			cube[1] *= rescale;
			cube[2] *= rescale;
			cube[3] *= rescale;
			cube[4] *= rescale;
			cube[5] *= rescale;
			cube[6] *= rescale;
			cube[7] *= rescale;
			cube[0] += origin;
			cube[1] += origin;
			cube[2] += origin;
			cube[3] += origin;
			cube[4] += origin;
			cube[5] += origin;
			cube[6] += origin;
			cube[7] += origin;
		}
		else
		{
			const float rescale = 1.08239223f;
			const glm::vec3 origin(0.5f);
			cube[0] -= origin;
			cube[1] -= origin;
			cube[2] -= origin;
			cube[3] -= origin;
			cube[4] -= origin;
			cube[5] -= origin;
			cube[6] -= origin;
			cube[7] -= origin;
			cube[0] *= rescale;
			cube[1] *= rescale;
			cube[2] *= rescale;
			cube[3] *= rescale;
			cube[4] *= rescale;
			cube[5] *= rescale;
			cube[6] *= rescale;
			cube[7] *= rescale;
			cube[0] += origin;
			cube[1] += origin;
			cube[2] += origin;
			cube[3] += origin;
			cube[4] += origin;
			cube[5] += origin;
			cube[6] += origin;
			cube[7] += origin;
		}
	}
	if (item.rotation.angle != 0.f)
		rotateCube(item.rotation.axis, -item.rotation.angle, item.rotation.origin);
	if (rotation.x != 0.f)
		rotateCube(Enums::Axis::X, rotation.x, glm::vec3(0.5f));
	if (rotation.y != 0.f)
		rotateCube(Enums::Axis::Y, rotation.y, glm::vec3(0.5f));
	cube[0] += position;
	cube[1] += position;
	cube[2] += position;
	cube[3] += position;
	cube[4] += position;
	cube[5] += position;
	cube[6] += position;
	cube[7] += position;

	cubefaces =
	{
		std::array<glm::vec3, 4>{ cube[3], cube[2], cube[6], cube[7] },
		{ cube[4], cube[5], cube[1], cube[0] },
		{ cube[4], cube[0], cube[3], cube[7] },
		{ cube[1], cube[5], cube[6], cube[2] },
		{ cube[0], cube[1], cube[2], cube[3] },
		{ cube[7], cube[6], cube[5], cube[4] }
	};
}
void CubeHelper::addQuad(Enums::Direction d, const glm::vec3& colorBlock)
{
	PaletteItem pi{ section.getBlock(localPos + Enums::getVecFacing(d)) };
	if (item.faces.contains(d))
	{
		const Blocks::Model::Element::Face& f = item.faces.at(d);
		const unsigned int texture = atlas.at(textures.at(f.texture.substr(1)));
		if (f.cullface == d)
		{
			const Material& pim = Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(pi.id)].material)];
			if (this->pi.id != pi.id && !pim.solidBlocking)
			{
				std::array<std::array<std::array<bool, 3>, 3>, 3> nBlocks
				{
					std::array<std::array<bool, 3>, 3>
					{
						std::array<bool, 3>
						{
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(-1, -1, -1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(0, -1, -1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(1, -1, -1)).id)].material)].solid
						},
						{
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(-1, -1, 0)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(0, -1, 0)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(1, -1, 0)).id)].material)].solid
						},
						{
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(-1, -1, 1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(0, -1, 1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(1, -1, 1)).id)].material)].solid
						}
					},
					{
						std::array<bool, 3>
						{
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(-1, 0, -1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(0, 0, -1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(1, 0, -1)).id)].material)].solid
						},
						{
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(-1, 0, 0)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(0, 0, 0)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(1, 0, 0)).id)].material)].solid
						},
						{
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(-1, 0, 1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(0, 0, 1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(1, 0, 1)).id)].material)].solid
						}
					},
					{
						std::array<bool, 3>
						{
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(-1, 1, -1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(0, 1, -1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(1, 1, -1)).id)].material)].solid
						},
						{
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(-1, 1, 0)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(0, 1, 0)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(1, 1, 0)).id)].material)].solid
						},
						{
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(-1, 1, 1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(0, 1, 1)).id)].material)].solid,
							Blocks::blockMaterials[static_cast<size_t>(Blocks::blocks[static_cast<size_t>(section.getBlock(localPos + glm::ivec3(1, 1, 1)).id)].material)].solid
						}
					}
				};
				vertexPush(cubefaces[static_cast<size_t>(std::log2f(static_cast<float>(d)))]);
				if (f.tintindex)
				{
					color.push_back(colorBlock);
					color.push_back(colorBlock);
					color.push_back(colorBlock);
					color.push_back(colorBlock);
				}
				else
				{
					color.push_back(glm::vec3(1.f));
					color.push_back(glm::vec3(1.f));
					color.push_back(glm::vec3(1.f));
					color.push_back(glm::vec3(1.f));
				}
				rotateFunc(f.uv, f.rotation, texture);
				{
					auto getAO = [](int l, int r, int c) -> int
					{
						return 3 - l - r - c;
					};
					/*glm::ivec3 nindex(Enums::getVecFacing(d));
					const std::array<std::array<int, 4>, 6> vertexCubeIndex
					{
						std::array<int, 4>{ 3, 2, 6, 7 },
						{ 4, 5, 1, 0 },
						{ 4, 0, 3, 7 },
						{ 1, 5, 6, 2 },
						{ 0, 1, 2, 3 },
						{ 7, 6, 5, 4 }
					};
					const std::array<glm::ivec3, 8> coordVertexCube
					{
						glm::ivec3{-1, 1, 1},
						{-1, -1, 1},
						{1, -1, 1},
						{1, 1, 1},
						{-1, 1, -1},
						{-1, -1, -1},
						{1, -1, -1},
						{1, 1, -1},
					};*/
					//glm::ivec3 c(1);
					switch (d)
					{
					case Enums::Direction::EAST:
						AO.push_back(getAO(nBlocks[1][2][2], nBlocks[2][1][2], nBlocks[2][2][2]));
						AO.push_back(getAO(nBlocks[1][2][2], nBlocks[0][1][2], nBlocks[0][2][2]));
						AO.push_back(getAO(nBlocks[0][1][2], nBlocks[1][0][2], nBlocks[0][0][2]));
						AO.push_back(getAO(nBlocks[2][1][2], nBlocks[1][0][2], nBlocks[2][0][2]));
						break;
					case Enums::Direction::WEST:
						AO.push_back(getAO(nBlocks[2][1][0], nBlocks[1][0][0], nBlocks[2][0][0]));
						AO.push_back(getAO(nBlocks[0][1][0], nBlocks[1][0][0], nBlocks[0][0][0]));
						AO.push_back(getAO(nBlocks[1][2][0], nBlocks[0][1][0], nBlocks[0][2][0]));
						AO.push_back(getAO(nBlocks[1][2][0], nBlocks[2][1][0], nBlocks[2][2][0]));
						break;
					case Enums::Direction::UP:
						AO.push_back(getAO(nBlocks[2][1][0], nBlocks[2][0][1], nBlocks[2][0][0]));
						AO.push_back(getAO(nBlocks[2][1][0], nBlocks[2][2][1], nBlocks[2][2][0]));
						AO.push_back(getAO(nBlocks[2][2][1], nBlocks[2][1][2], nBlocks[2][2][2]));
						AO.push_back(getAO(nBlocks[2][0][1], nBlocks[2][1][2], nBlocks[2][0][2]));
						break;
					case Enums::Direction::DOWN:
						AO.push_back(getAO(nBlocks[0][1][0], nBlocks[0][2][1], nBlocks[0][2][0]));
						AO.push_back(getAO(nBlocks[0][1][0], nBlocks[0][0][1], nBlocks[0][0][0]));
						AO.push_back(getAO(nBlocks[0][0][1], nBlocks[0][1][2], nBlocks[0][0][2]));
						AO.push_back(getAO(nBlocks[0][2][1], nBlocks[0][1][2], nBlocks[0][2][2]));
						break;
					case Enums::Direction::SOUTH:
						AO.push_back(getAO(nBlocks[1][2][0], nBlocks[2][2][1], nBlocks[2][2][0]));
						AO.push_back(getAO(nBlocks[1][2][0], nBlocks[0][2][1], nBlocks[0][2][0]));
						AO.push_back(getAO(nBlocks[0][2][1], nBlocks[1][2][2], nBlocks[0][2][2]));
						AO.push_back(getAO(nBlocks[2][2][1], nBlocks[1][2][2], nBlocks[2][2][2]));
						break;
					case Enums::Direction::NORTH:
						AO.push_back(getAO(nBlocks[2][0][1], nBlocks[1][0][2], nBlocks[2][0][2]));
						AO.push_back(getAO(nBlocks[0][0][1], nBlocks[1][0][2], nBlocks[0][0][2]));
						AO.push_back(getAO(nBlocks[1][0][0], nBlocks[0][0][1], nBlocks[0][0][0]));
						AO.push_back(getAO(nBlocks[1][0][0], nBlocks[2][0][1], nBlocks[2][0][0]));
						break;
					}
					
				}
				indiciesPush();
			}
		}
		else
		{
			vertexPush(cubefaces[static_cast<size_t>(std::log2f(static_cast<float>(d)))]);
			if (f.tintindex)
			{
				color.push_back(colorBlock);
				color.push_back(colorBlock);
				color.push_back(colorBlock);
				color.push_back(colorBlock);
			}
			else
			{
				color.push_back(glm::vec3(1.f));
				color.push_back(glm::vec3(1.f));
				color.push_back(glm::vec3(1.f));
				color.push_back(glm::vec3(1.f));
			}
			rotateFunc(f.uv, f.rotation, texture);
			AO.push_back(3);
			AO.push_back(3);
			AO.push_back(3);
			AO.push_back(3);
			indiciesPush();
		}
	}
}
void CubeHelper::rotateFunc(const glm::vec4& uv, int angle, GLuint texture)
{
	switch (angle)
	{
	case 90:
		UV.push_back({ uv.z, uv.y, texture });
		UV.push_back({ uv.x, uv.y, texture });
		UV.push_back({ uv.x, uv.w, texture });
		UV.push_back({ uv.z, uv.w, texture });
		break;
	case 180:
		UV.push_back({ uv.z, uv.w, texture });
		UV.push_back({ uv.z, uv.y, texture });
		UV.push_back({ uv.x, uv.y, texture });
		UV.push_back({ uv.x, uv.w, texture });
		break;
	case 270:
		UV.push_back({ uv.x, uv.w, texture });
		UV.push_back({ uv.z, uv.w, texture });
		UV.push_back({ uv.z, uv.y, texture });
		UV.push_back({ uv.x, uv.y, texture });
		break;
	default:
		UV.push_back({ uv.x, uv.y, texture });
		UV.push_back({ uv.x, uv.w, texture });
		UV.push_back({ uv.z, uv.w, texture });
		UV.push_back({ uv.z, uv.y, texture });
		break;
	}
}

void CubeHelper::rotateCube(const Enums::Axis& axis, const float& angle, const glm::vec3& origin)
{
	glm::mat4 rot(1.f);
	cube[0] -= origin;
	cube[1] -= origin;
	cube[2] -= origin;
	cube[3] -= origin;
	cube[4] -= origin;
	cube[5] -= origin;
	cube[6] -= origin;
	cube[7] -= origin;
	if (axis == Enums::Axis::X)
		rot = glm::rotate(rot, glm::radians(angle), glm::vec3(1.f, 0.f, 0.f));
	else if (axis == Enums::Axis::Y)
		rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.f, 1.f, 0.f));
	else if (axis == Enums::Axis::Z)
		rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.f, 0.f, 1.f));
	cube[0] = static_cast<glm::vec3>(glm::vec4(cube[0], 1.f) * rot);
	cube[1] = static_cast<glm::vec3>(glm::vec4(cube[1], 1.f) * rot);
	cube[2] = static_cast<glm::vec3>(glm::vec4(cube[2], 1.f) * rot);
	cube[3] = static_cast<glm::vec3>(glm::vec4(cube[3], 1.f) * rot);
	cube[4] = static_cast<glm::vec3>(glm::vec4(cube[4], 1.f) * rot);
	cube[5] = static_cast<glm::vec3>(glm::vec4(cube[5], 1.f) * rot);
	cube[6] = static_cast<glm::vec3>(glm::vec4(cube[6], 1.f) * rot);
	cube[7] = static_cast<glm::vec3>(glm::vec4(cube[7], 1.f) * rot);
	cube[0] += origin;
	cube[1] += origin;
	cube[2] += origin;
	cube[3] += origin;
	cube[4] += origin;
	cube[5] += origin;
	cube[6] += origin;
	cube[7] += origin;
}

void CubeHelper::vertexPush(const std::array<glm::vec3, 4>& quad)
{
	vertex.insert(vertex.end(), quad.begin(), quad.end());
}

void CubeHelper::indiciesPush()
{
	indicies.insert(indicies.end(), { countVertex, countVertex + 1, countVertex + 3, countVertex + 1, countVertex + 2, countVertex + 3 });
	countVertex += 4;
}

void CubeHelper::add(std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& UV, std::vector<glm::vec3>& color, std::vector<uint8_t>& AO, std::vector<uint32_t>& indicies, uint32_t& countVertex)
{
	vertex.insert(vertex.end(), this->vertex.begin(), this->vertex.end());
	UV.insert(UV.end(), this->UV.begin(), this->UV.end());
	color.insert(color.end(), this->color.begin(), this->color.end());
	AO.insert(AO.end(), this->AO.begin(), this->AO.end());
	indicies.insert(indicies.end(), this->indicies.begin(), this->indicies.end());
	countVertex = this->countVertex;
}
