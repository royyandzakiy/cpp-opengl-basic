#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <fmt/base.h>

class SimpleMario {
  private:
	// Player state
	float playerX = 0.0f;
	float playerY = 0.0f;
	float velocityY = 0.0f;
	bool isOnGround = true;

	static constexpr float playerWidth = 0.08f;
	static constexpr float playerHeight = 0.08f;
	static constexpr float moveSpeed = 0.01f;
	static constexpr float jumpForce = 0.03f;
	static constexpr float gravity = -0.0015f;

	// Ground position (normalized coordinates: -1.0 to 1.0)
	static constexpr float groundY = -0.8f;
	static constexpr float groundThickness = 0.02f;

	// OpenGL objects
	GLuint shaderProgram{};
	GLuint playerVAO{}, playerVBO{};
	GLuint groundVAO{}, groundVBO{};
	GLFWwindow *window = nullptr;

	static constexpr const char *vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		uniform vec2 uOffset;
		uniform vec3 uColor;
		out vec3 fragColor;
		void main() {
			gl_Position = vec4(aPos.x + uOffset.x, aPos.y + uOffset.y, 0.0, 1.0);
			fragColor = uColor;
		}
	)";

	static constexpr const char *fragmentShaderSource = R"(
		#version 330 core
		in vec3 fragColor;
		out vec4 FragColor;
		void main() {
			FragColor = vec4(fragColor, 1.0);
		}
	)";

	void setupShaders() {
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void setupRectangle(GLuint &VAO, GLuint &VBO, float width, float height) {
		float halfW = width / 2.0f;
		float halfH = height / 2.0f;

		std::array<float, 8> vertices = {-halfW, -halfH, halfW, -halfH, halfW, halfH, -halfW, halfH};

		std::array<std::uint32_t, 6> indices = {0, 1, 2, 0, 2, 3};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		GLuint EBO{};
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void updatePhysics() {
		// Apply gravity
		velocityY += gravity;
		playerY += velocityY;

		// Ground collision
		float playerBottom = playerY - playerHeight / 2.0f;
		float groundTop = groundY + groundThickness / 2.0f;

		if (playerBottom <= groundTop) {
			playerY = groundTop + playerHeight / 2.0f;
			velocityY = 0.0f;
			isOnGround = true;
		} else {
			isOnGround = false;
		}
	}

	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	static void keyCallback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action,
							[[maybe_unused]] int mods) {
		auto *game = static_cast<SimpleMario *>(glfwGetWindowUserPointer(window));

		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			switch (key) {
			case GLFW_KEY_UP:
			case GLFW_KEY_SPACE:
				if (game->isOnGround) {
					game->velocityY = jumpForce;
					game->isOnGround = false;
					fmt::println("Jump!");
				}
				break;
			case GLFW_KEY_LEFT:
				game->playerX -= moveSpeed;
				break;
			case GLFW_KEY_RIGHT:
				game->playerX += moveSpeed;
				break;
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			default:
				break;
			}
		}
	}

	void drawRectangle(float x, float y, float r, float g, float b, GLuint VAO) {
		glUniform2f(glGetUniformLocation(shaderProgram, "uOffset"), x, y);
		glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), r, g, b);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

  public:
	int run() {
		fmt::println("Simple Mario - Arrow Keys to Move, Space/Up to Jump!");

		if (!glfwInit()) {
			fmt::print(stderr, "Failed to initialize GLFW\n");
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(800, 600, "Simple Mario", nullptr, nullptr);
		if (!window) {
			fmt::print(stderr, "Failed to create window\n");
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);
		glfwSetKeyCallback(window, keyCallback);

		if (glewInit() != GLEW_OK) {
			fmt::print(stderr, "Failed to initialize GLEW\n");
			return -1;
		}

		fmt::println("OpenGL version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
		fmt::println("Use arrow keys to move, Space/Up to jump. Press ESC to exit.");

		setupShaders();
		setupRectangle(playerVAO, playerVBO, playerWidth, playerHeight);
		setupRectangle(groundVAO, groundVBO, 2.0f, groundThickness); // Full width ground

		// Start player on the ground
		playerX = 0.0f;
		playerY = groundY + groundThickness / 2.0f + playerHeight / 2.0f;

		while (!glfwWindowShouldClose(window)) {
			updatePhysics();

			glClearColor(0.4f, 0.6f, 1.0f, 1.0f); // Sky blue background
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);

			// Draw ground (white)
			drawRectangle(0.0f, groundY, 1.0f, 1.0f, 1.0f, groundVAO);

			// Draw player (red)
			drawRectangle(playerX, playerY, 1.0f, 0.0f, 0.0f, playerVAO);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
	}
};

auto main() -> int {
	SimpleMario game;
	return game.run();
}
