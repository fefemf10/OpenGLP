#pragma once
#include <GLM/glm.hpp>
#include <JSON/json.hpp>
namespace glm
{
	inline void to_json(nlohmann::json& j, const glm::i8vec2& vec)
	{
		j = std::vector<int8_t>{ vec.x, vec.y };
	}
	inline void to_json(nlohmann::json& j, const glm::i8vec3& vec)
	{
		j = std::vector<int8_t>{ vec.x, vec.y, vec.z };
	}
	inline void to_json(nlohmann::json& j, const glm::i8vec4& vec)
	{
		j = std::vector<int8_t>{ vec.x, vec.y, vec.z, vec.w };
	}
	inline void from_json(const nlohmann::json& j, glm::i8vec2& vec)
	{
		std::vector<int8_t> v = j.get<std::vector<int8_t>>();
		vec.x = v[0];
		vec.y = v[1];
	}
	inline void from_json(const nlohmann::json& j, glm::i8vec3& vec)
	{
		std::vector<int8_t> v = j.get<std::vector<int8_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
	}
	inline void from_json(const nlohmann::json& j, glm::i8vec4& vec)
	{
		std::vector<int8_t> v = j.get<std::vector<int8_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
		vec.w = v[3];
	}
	inline void to_json(nlohmann::json& j, const glm::u8vec2& vec)
	{
		j = std::vector<uint8_t>{ vec.x, vec.y };
	}
	inline void to_json(nlohmann::json& j, const glm::u8vec3& vec)
	{
		j = std::vector<uint8_t>{ vec.x, vec.y, vec.z };
	}
	inline void to_json(nlohmann::json& j, const glm::u8vec4& vec)
	{
		j = std::vector<uint8_t>{ vec.x, vec.y, vec.z, vec.w };
	}
	inline void from_json(const nlohmann::json& j, glm::u8vec2& vec)
	{
		std::vector<uint8_t> v = j.get<std::vector<uint8_t>>();
		vec.x = v[0];
		vec.y = v[1];
	}
	inline void from_json(const nlohmann::json& j, glm::u8vec3& vec)
	{
		std::vector<uint8_t> v = j.get<std::vector<uint8_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
	}
	inline void from_json(const nlohmann::json& j, glm::u8vec4& vec)
	{
		std::vector<uint8_t> v = j.get<std::vector<uint8_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
		vec.w = v[3];
	}
	inline void to_json(nlohmann::json& j, const glm::i16vec2& vec)
	{
		j = std::vector<int16_t>{ vec.x, vec.y };
	}
	inline void to_json(nlohmann::json& j, const glm::i16vec3& vec)
	{
		j = std::vector<int16_t>{ vec.x, vec.y, vec.z};
	}
	inline void to_json(nlohmann::json& j, const glm::i16vec4& vec)
	{
		j = std::vector<int16_t>{ vec.x, vec.y, vec.z, vec.w };
	}
	inline void from_json(const nlohmann::json& j, glm::i16vec2& vec)
	{
		std::vector<int16_t> v = j.get<std::vector<int16_t>>();
		vec.x = v[0];
		vec.y = v[1];
	}
	inline void from_json(const nlohmann::json& j, glm::i16vec3& vec)
	{
		std::vector<int16_t> v = j.get<std::vector<int16_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
	}
	inline void from_json(const nlohmann::json& j, glm::i16vec4& vec)
	{
		std::vector<int16_t> v = j.get<std::vector<int16_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
		vec.w = v[3];
	}
	inline void to_json(nlohmann::json& j, const glm::u16vec2& vec)
	{
		j = std::vector<uint16_t>{ vec.x, vec.y };
	}
	inline void to_json(nlohmann::json& j, const glm::u16vec3& vec)
	{
		j = std::vector<uint16_t>{ vec.x, vec.y, vec.z };
	}
	inline void to_json(nlohmann::json& j, const glm::u16vec4& vec)
	{
		j = std::vector<uint16_t>{ vec.x, vec.y, vec.z, vec.w };
	}
	inline void from_json(const nlohmann::json& j, glm::u16vec2& vec)
	{
		std::vector<uint16_t> v = j.get<std::vector<uint16_t>>();
		vec.x = v[0];
		vec.y = v[1];
	}
	inline void from_json(const nlohmann::json& j, glm::u16vec3& vec)
	{
		std::vector<uint16_t> v = j.get<std::vector<uint16_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
	}
	inline void from_json(const nlohmann::json& j, glm::u16vec4& vec)
	{
		std::vector<uint16_t> v = j.get<std::vector<uint16_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
		vec.w = v[3];
	}
	inline void to_json(nlohmann::json& j, const glm::i32vec2& vec)
	{
		j = std::vector<int32_t>{ vec.x, vec.y };
	}
	inline void to_json(nlohmann::json& j, const glm::i32vec3& vec)
	{
		j = std::vector<int32_t>{ vec.x, vec.y, vec.z };
	}
	inline void to_json(nlohmann::json& j, const glm::i32vec4& vec)
	{
		j = std::vector<int32_t>{ vec.x, vec.y, vec.z, vec.w };
	}
	inline void from_json(const nlohmann::json& j, glm::i32vec2& vec)
	{
		std::vector<int32_t> v = j.get<std::vector<int32_t>>();
		vec.x = v[0];
		vec.y = v[1];
	}
	inline void from_json(const nlohmann::json& j, glm::i32vec3& vec)
	{
		std::vector<int32_t> v = j.get<std::vector<int32_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
	}
	inline void from_json(const nlohmann::json& j, glm::i32vec4& vec)
	{
		std::vector<int32_t> v = j.get<std::vector<int32_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
		vec.w = v[3];
	}
	inline void to_json(nlohmann::json& j, const glm::u32vec2& vec)
	{
		j = std::vector<uint32_t>{ vec.x, vec.y };
	}
	inline void to_json(nlohmann::json& j, const glm::u32vec3& vec)
	{
		j = std::vector<uint32_t>{ vec.x, vec.y, vec.z };
	}
	inline void to_json(nlohmann::json& j, const glm::u32vec4& vec)
	{
		j = std::vector<uint32_t>{ vec.x, vec.y, vec.z, vec.w };
	}
	inline void from_json(const nlohmann::json& j, glm::u32vec2& vec)
	{
		std::vector<uint32_t> v = j.get<std::vector<uint32_t>>();
		vec.x = v[0];
		vec.y = v[1];
	}
	inline void from_json(const nlohmann::json& j, glm::u32vec3& vec)
	{
		std::vector<uint32_t> v = j.get<std::vector<uint32_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
	}
	inline void from_json(const nlohmann::json& j, glm::u32vec4& vec)
	{
		std::vector<uint32_t> v = j.get<std::vector<uint32_t>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
		vec.w = v[3];
	}
	inline void to_json(nlohmann::json& j, const glm::vec2& vec)
	{
		j = std::vector<float>{ vec.x, vec.y };
	}
	inline void to_json(nlohmann::json& j, const glm::vec3& vec)
	{
		j = std::vector<float>{ vec.x, vec.y, vec.z };
	}
	inline void to_json(nlohmann::json& j, const glm::vec4& vec)
	{
		j = std::vector<float>{ vec.x, vec.y, vec.z, vec.w };
	}
	inline void from_json(const nlohmann::json& j, glm::vec2& vec)
	{
		std::vector<float> v = j.get<std::vector<float>>();
		vec.x = v[0];
		vec.y = v[1];
	}
	inline void from_json(const nlohmann::json& j, glm::vec3& vec)
	{
		std::vector<float> v = j.get<std::vector<float>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
	}
	inline void from_json(const nlohmann::json& j, glm::vec4& vec)
	{
		std::vector<float> v = j.get<std::vector<float>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
		vec.w = v[3];
	}
	inline void from_json(const nlohmann::json& j, glm::dvec2& vec)
	{
		std::vector<double> v = j.get<std::vector<double>>();
		vec.x = v[0];
		vec.y = v[1];
	}
	inline void from_json(const nlohmann::json& j, glm::dvec3& vec)
	{
		std::vector<double> v = j.get<std::vector<double>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
	}
	inline void from_json(const nlohmann::json& j, glm::dvec4& vec)
	{
		std::vector<double> v = j.get<std::vector<double>>();
		vec.x = v[0];
		vec.y = v[1];
		vec.z = v[2];
		vec.w = v[3];
	}
}