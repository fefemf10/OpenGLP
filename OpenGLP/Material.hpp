#pragma once
#include <optional>
#include <string>
#include <GLM/vec3.hpp>
#include <JSON/json.hpp>
#include "jsonglm.hpp"


struct Material
{
	enum class Piston : uint8_t
	{
		PUSHED,
		REPLACE,
		BLOCKED
	};
	std::optional<glm::u8vec3> mapColor;
	bool liquid{};
	bool solid{};
	bool solidBlocking{};
	bool blockMovement{};
	bool burn{};
	bool replace{};
	Piston piston{};
};


inline void to_json(nlohmann::json& j, const Material& material)
{
	if (material.mapColor)
		j["map_color"] = material.mapColor.value();
	j["liquid"] = material.liquid;
	j["solid"] = material.solid;
	j["solid_blocking"] = material.solidBlocking;
	j["block_movement"] = material.blockMovement;
	j["burn"] = material.burn;
	j["replace"] = material.replace;
	j["piston"] = material.piston;//waring
}

inline void from_json(const nlohmann::json& j, Material& material)
{
	if (j.contains("map_color"))
	{
		material.mapColor = glm::u8vec3();
		j.at("map_color").get_to(material.mapColor.value());
	}
	j.at("liquid").get_to(material.liquid);
	j.at("solid").get_to(material.solid);
	j.at("solid_blocking").get_to(material.solidBlocking);
	j.at("block_movement").get_to(material.blockMovement);
	j.at("burn").get_to(material.burn);
	j.at("replace").get_to(material.replace);
	const std::string piston = j.at("piston").get<std::string>();
	if (piston.starts_with('p'))
		material.piston = Material::Piston::PUSHED;
	else if (piston.starts_with('r'))
		material.piston = Material::Piston::REPLACE;
	else if (piston.starts_with('b'))
		material.piston = Material::Piston::BLOCKED;
}