#include "Shader.hpp"

Shader::Shader() : program{}, vs{}, fs{}, cs{}
{
}

Shader::Shader(Shader& other)
{
	this->program = other.program;
	this->vs = other.vs;
	this->fs = other.fs;
	this->cs = other.cs;
	this->locations = other.locations;
	other.program = other.vs = other.fs = other.cs = 0;
	other.locations.clear();
}

Shader::Shader(Shader&& other) noexcept
{
	this->program = other.program;
	this->vs = other.vs;
	this->fs = other.fs;
	this->cs = other.cs;
	this->locations = other.locations;
	other.program = other.vs = other.fs = other.cs = 0;
	other.locations.clear();
}

Shader::Shader(const std::vector<std::string>& shaderFilePaths) : program{}, vs{}, fs{}, cs{}
{
	program = glCreateProgram();
	for (const auto& shader : shaderFilePaths)
	{
		if (shader.rfind("vert") != std::string::npos)
			vs = compileShader(GL_VERTEX_SHADER, Shader::getStringShader(shader));
		else if (shader.rfind("frag") != std::string::npos)
			fs = compileShader(GL_FRAGMENT_SHADER, Shader::getStringShader(shader));
		else if (shader.rfind("comp") != std::string::npos)
			cs = compileShader(GL_COMPUTE_SHADER, Shader::getStringShader(shader));
	}
}

Shader::~Shader()
{
	if (program)
	{
		if (vs)
		{
			glDetachShader(program, vs);
			glDeleteShader(vs);
		}
		if (fs)
		{
			glDetachShader(program, fs);
			glDeleteShader(fs);
		}
		if (cs)
		{
			glDetachShader(program, cs);
			glDeleteShader(cs);
		}
		glDeleteProgram(program);
	}
}

void Shader::bindAttrib(GLuint index, const std::string& name)
{
	glBindAttribLocation(program, index, name.c_str());
}

void Shader::link()
{
	if (program)
	{
		if (vs)
		{
			glAttachShader(program, vs);
		}
		if (fs)
		{
			glAttachShader(program, fs);
		}
		if (cs)
		{
			glAttachShader(program, cs);
		}
		glLinkProgram(program);
		glValidateProgram(program);
	}
}

void Shader::use()
{
	if (program)
		glUseProgram(program);
}

void Shader::setFloat(const std::string& name, const float& value)
{
	glProgramUniform1f(program, getLocation(name), value);
}

void Shader::setInt(const std::string& name, const int& value)
{
	glProgramUniform1i(program, getLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value)
{
	glProgramUniform2f(program, getLocation(name), value.x, value.y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value)
{
	glProgramUniform3f(program, getLocation(name), value.x, value.y, value.z);
}

void Shader::setMatrix(const std::string& name, const size_t& count, const bool& traspose, const glm::mat4& mat)
{
	glProgramUniformMatrix4fv(program, getLocation(name), 1, GL_FALSE, &mat[0][0]);
}

GLint Shader::getLocation(const std::string& name)
{
	if (locations.find(name) != locations.end())
		return locations[name];
	GLint location = glGetUniformLocation(program, name.c_str());
	locations[name] = location;
	return location;
}

GLint Shader::getID() const
{
	return program;
}

GLuint Shader::compileShader(GLenum type, const std::string& source)
{
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << '\n' << message << '\n';
		glDeleteShader(id);
		return 0;
	}
	return id;
}

std::string Shader::getStringShader(const std::string& path)
{
	std::ifstream file(path, std::ios::in);
	std::string code{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	file.close();
	return code;
}