#include "Window.hpp"

Window::Window(const std::string& title)
{
	std::cin >> dist;
	dist = glm::clamp((int)dist, 0, 64);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GL::loadSettings();
	if (GL::settings.fullscreen)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		window = glfwCreateWindow(glfwGetVideoMode(monitor)->width, glfwGetVideoMode(monitor)->height, title.c_str(), GL::settings.fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	}
	else
	{
		window = glfwCreateWindow(GL::settings.width, GL::settings.height, title.c_str(), nullptr, nullptr);
	}
	if (!window)
	{
		std::cout << "Window don't created" << std::endl;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(GL::settings.vsync);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew dont Init" << std::endl;
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
	ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\comicbd.ttf", 18, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
}

Window::~Window()
{
	glfwDestroyWindow(window);
}

void Window::loop()
{
	srand(static_cast<unsigned int>(time(0)));
	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(Window::message_callback, nullptr);
	RM::loadShader("crosshair", Shader({ "crosshair.vert", "crosshair.frag" }));
	Shader& crosshairShader = RM::getShader("crosshair");
	crosshairShader.link();

	RM::loadShader("shader", Shader({ "shader.vert", "shader.frag" }));
	Shader& shader = RM::getShader("shader");
	shader.link();
	{
		Blocks::loadBlocks(); Blocks::loadBlockMaterials();
		Blocks::loadBlockStates(); Blocks::loadModels();
		Biomes::loadBiomes();
		/*ThreadPool pool(2);
		pool.enqueue([]() {  });
		pool.enqueue([]() {  });
		pool.enqueue();*/
	}
	Blocks::loadTextures();
	


	VAO crosshair;
	crosshair.bind();
	crosshair.addVBO(1);
	crosshair.setTypeIndices(GL_UNSIGNED_BYTE);
	crosshair.loadData(0, std::vector<glm::vec2>{ {-0.01f, -0.01f }, { 0.01f, 0.01f }, { -0.01f, 0.01f }, { 0.01f, -0.01f } });
	crosshair.loadIndices(std::vector<uint8_t>{ 0, 1, 2, 3 });

	Player player;
	player.setRenderDistance(dist);
	//player.setPosition({144, 500, 144});
	//player.setPosition({112, 400, 112});
	Camera camera(player);
	World world("1189", player, camera);

	shader.setMatrix("projection", 1, GL_FALSE, camera.getProjection());

	int typeblock = 1;
	bool menu = true;
	bool polygon = true;
	bool vsync = true;
	UBO bufferTransform(sizeof(glm::mat4) * 2, 0);
	bufferTransform.bind();
	bufferTransform.loadData(0, camera.getProjection());
	bufferTransform.unbind();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		typeblock = (typeblock + (int)(ImGui::GetIO().MouseWheel)) % 23u;
		/*{
			ImGui::ShowDemoWindow();
			ImGui::Begin("Hotbar###MainHotbar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus);
			ImGui::SetWindowPos("Hotbar###MainHotbar", { 0.25f * width, 0.9f * height });
			ImGui::SetWindowSize("Hotbar###MainHotbar", { 0.5f * width, 0.1f * height });
			ImGui::End();
		}*/
		if (menu)
		{
			ImGui::Begin("Debug###Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNavFocus);
			ImGui::SetWindowPos("Debug###Debug", {});
			ImGui::Text("Type block %u", typeblock);
			ImGui::Text("Render distance %u", player.getRenderDistance());
			ImGui::Text("Seed %llu", world.getSeed());
			ImGui::Text("Count chunks %u", world.getCountChunks());
			ImGui::Text("Count visible sections %u", world.countVisibleSection);
			ImGui::Text("Count all vertexs %u", world.allVertex);
			ImGui::Text("Count vertex %u", world.countVertex);
			ImGui::Text("Count transperent vertexs %u", world.countVertexTransperent);
			ImGui::Text("Look player x = %.2f y = %.2f z = %.2f", player.getLook().x, player.getLook().y, player.getLook().z);
			ImGui::Text("Position player x = %.2f y = %.2f z = %.2f", player.getPosition().x, player.getPosition().y, player.getPosition().z);
			ImGui::Text("Position player chunk x = %i y = %i z = %i", (int)(player.getPosition().x) >> 4, (int)(player.getPosition().y) >> 4, (int)(player.getPosition().z) >> 4);
			ImGui::Text("Position player region x = %i z = %i", (int)(player.getPosition().x) >> 9, (int)(player.getPosition().z) >> 9);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (ImGui::IsKeyReleased(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, true);
		if (ImGui::IsKeyReleased(GLFW_KEY_F3))
			menu ^= 1;
		if (ImGui::IsKeyReleased(GLFW_KEY_1))
		{
			polygon ^= 1;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE + polygon);
		}

		if (ImGui::IsKeyReleased(GLFW_KEY_2))
		{
			vsync ^= 1;
			glfwSwapInterval(vsync);
		}
		float dt = ImGui::GetIO().DeltaTime;
		player.update(dt);
		camera.update(dt);
		world.update(dt);
		if (ImGui::IsMouseReleased(GLFW_MOUSE_BUTTON_LEFT))
		{
			glm::vec3 end;
			glm::vec3 norm;
			glm::vec3 iend;
			std::string block = world.rayCast(player.getPosition() + glm::vec3(0.f, 1.8f, 0.f), player.getLook(), 10.f, end, norm, iend);
			if (block != "minecraft:air" && block != "minecraft:bedrock")
			{
				world.setBlock(iend, "minecraft:air");
			}
		}
		else if (ImGui::IsMouseReleased(GLFW_MOUSE_BUTTON_RIGHT))
		{
			glm::vec3 end;
			glm::vec3 norm;
			glm::vec3 iend;
			std::string block = world.rayCast(player.getPosition() + glm::vec3(0.f, 1.8f, 0.f), player.getLook(), 10.f, end, norm, iend);
			if (block != "minecraft:air")
			{
				world.setBlock(iend + norm, block);
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GL::useShader(RM::getIDShader("shader"));
		bufferTransform.bind();
		bufferTransform.loadData(sizeof(glm::mat4), camera.getView());
		GL::useTexture(RM::getIDAtlas("blocks"));
		world.draw();
		bufferTransform.unbind();
		GL::useShader(RM::getIDShader("crosshair"));
		crosshair.draw(GL_LINES);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	world.saveWorld();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Window::message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	std::string const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
	}();

	std::string const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
	}();

	std::string const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
	}();

	Logger::log(std::string(src_str) + ", " + std::string(type_str) + ", " + std::string(severity_str) + ", " + std::to_string(id) + ": " + std::string(message));
}