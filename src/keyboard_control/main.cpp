#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <fmt/base.h>

class MovingBox {
  private:
	// Box state
	float boxX = 0.0f;
	float boxY = 0.0f;
	static constexpr float boxSize = 0.1f;
	static constexpr float moveSpeed = 0.01f;

	// OpenGL objects
	GLuint shaderProgram{};
	GLuint VAO{};
	GLuint VBO{};
	GLFWwindow *window = nullptr;

	// Shader sources
	static constexpr const char *vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec2 aPos;
		uniform vec2 uOffset;
		void main() {
			gl_Position = vec4(aPos.x + uOffset.x, aPos.y + uOffset.y, 0.0, 1.0);
		}
	)";

	static constexpr const char *fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		void main() {
			FragColor = vec4(1.0, 0.0, 0.0, 1.0);
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

	void setupBox() {
		std::array<float, 8> vertices = {-boxSize, -boxSize, boxSize, -boxSize, boxSize, boxSize, -boxSize, boxSize};

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

	// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
	static void keyCallback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action,
							[[maybe_unused]] int mods) {
		auto *app = static_cast<MovingBox *>(glfwGetWindowUserPointer(window));

		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			switch (key) {
			case GLFW_KEY_UP:
				app->boxY += moveSpeed;
				fmt::println("Up pressed - Box position: ({:.3f}, {:.3f})", app->boxX, app->boxY);
				break;
			case GLFW_KEY_DOWN:
				app->boxY -= moveSpeed;
				fmt::println("Down pressed - Box position: ({:.3f}, {:.3f})", app->boxX, app->boxY);
				break;
			case GLFW_KEY_LEFT:
				app->boxX -= moveSpeed;
				fmt::println("Left pressed - Box position: ({:.3f}, {:.3f})", app->boxX, app->boxY);
				break;
			case GLFW_KEY_RIGHT:
				app->boxX += moveSpeed;
				fmt::println("Right pressed - Box position: ({:.3f}, {:.3f})", app->boxX, app->boxY);
				break;
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
			default:
				break;
			}
		}
	}

  public:
	int run() {
		fmt::println("Hello OpenGL - Arrow Keys to Move Box!");

		if (!glfwInit()) {
			fmt::print(stderr, "Failed to initialize GLFW\n");
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(800, 600, "Moving Box", nullptr, nullptr);
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
		fmt::println("Use arrow keys to move the red box. Press ESC to exit.");

		setupShaders();
		setupBox();

		while (!glfwWindowShouldClose(window)) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);
			glUniform2f(glGetUniformLocation(shaderProgram, "uOffset"), boxX, boxY);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
	}
};

auto main() -> int {
	MovingBox app;
	return app.run();
}
