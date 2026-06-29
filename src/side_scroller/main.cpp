#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <fmt/base.h>
#include <unordered_set>

class SimpleMario {
  private:
	// Player state
	float playerX = 0.0f;
	float playerY = 0.0f;
	float velocityY = 0.0f;
	bool isOnGround = true;

	static constexpr float playerWidth = 0.08f;
	static constexpr float playerHeight = 0.16f;
	static constexpr float moveSpeed = 1.0f;		 // Units per second
	static constexpr float jumpForce = 2.0f;		 // Initial jump velocity (increased)
	static constexpr float gravity = -8.0f;			 // Gravity acceleration (increased for snappier feel)
	static constexpr float groundTolerance = 0.001f; // Small tolerance for ground detection

	// Ground position (normalized coordinates: -1.0 to 1.0)
	static constexpr float groundY = -0.8f;
	static constexpr float groundThickness = 0.02f;

	// Input tracking
	std::unordered_set<int> keysPressed;
	double lastFrameTime = 0.0;

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

	void updatePhysics(float deltaTime) {
		// Apply gravity (always)
		velocityY += gravity * deltaTime;
		playerY += velocityY * deltaTime;

		// Ground collision
		float playerBottom = playerY - playerHeight / 2.0f;
		float groundTop = groundY + groundThickness / 2.0f;

		// Check if player is on or penetrating ground
		if (playerBottom <= groundTop) {
			// Snap out of ground
			playerY = groundTop + playerHeight / 2.0f;

			// If we were falling (negative velocity), we've landed
			bool isFalling = velocityY < 0.0f;
			if (isFalling) {
				velocityY = 0.0f;
				isOnGround = true;
			}
			// If velocityY >= 0, we just started jumping from ground level
			// Don't change isOnGround - let the jump handler set it to false
		} else if (playerBottom > groundTop + groundTolerance) {
			// We're clearly in the air (above tolerance zone)
			isOnGround = false;
		}
		// If we're in the tolerance zone but above ground, maintain current state
		// This prevents rapid toggling between ground/air states

		// Smooth horizontal movement based on held keys
		if (keysPressed.count(GLFW_KEY_LEFT)) {
			playerX -= moveSpeed * deltaTime;
		}
		if (keysPressed.count(GLFW_KEY_RIGHT)) {
			playerX += moveSpeed * deltaTime;
		}

		// Keep player in bounds
		if (playerX < -1.0f + playerWidth / 2.0f)
			playerX = -1.0f + playerWidth / 2.0f;
		if (playerX > 1.0f - playerWidth / 2.0f)
			playerX = 1.0f - playerWidth / 2.0f;
	}

	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	static void keyCallback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action,
							[[maybe_unused]] int mods) {
		auto *game = static_cast<SimpleMario *>(glfwGetWindowUserPointer(window));

		if (action == GLFW_PRESS) {
			game->keysPressed.insert(key);

			if (key == GLFW_KEY_UP || key == GLFW_KEY_SPACE) {
				// Allow jumping if on ground or very close to ground
				if (game->isOnGround) {
					game->velocityY = jumpForce;
					game->isOnGround = false;
					fmt::println("Jump! Velocity: {:.2f}", game->velocityY);
				}
			}
		} else if (action == GLFW_RELEASE) {
			game->keysPressed.erase(key);
		}

		if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
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
		setupRectangle(groundVAO, groundVBO, 2.0f, groundThickness);

		// Start player on the ground
		playerX = 0.0f;
		playerY = groundY + groundThickness / 2.0f + playerHeight / 2.0f;

		lastFrameTime = glfwGetTime();

		while (!glfwWindowShouldClose(window)) {
			// Calculate delta time for frame-rate independence
			double currentTime = glfwGetTime();
			auto deltaTime = static_cast<float>(currentTime - lastFrameTime);
			lastFrameTime = currentTime;

			// Cap delta time to avoid huge jumps if debugging
			if (deltaTime > 0.1f)
				deltaTime = 0.1f;

			updatePhysics(deltaTime);

			glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
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
