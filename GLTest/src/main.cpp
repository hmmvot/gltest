#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <memory>
#include <chrono>
#include "Mesh.h"
#include "Texture2D.h"
#include "Material.h"
#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Renderer.h"

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void MouseMoveCallback(GLFWwindow *window, double xpos, double ypos);

const float ScreenWidth = 800;
const float ScreenHeight = 600;

Camera MyCamera;

int main()
{
	//������������� GLFW
	glfwInit();

	//��������� GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window;

	{
		window = glfwCreateWindow(ScreenWidth, ScreenHeight, "LearnOpenGL", nullptr, nullptr);
		if (window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
	}

	{
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return -1;
		}
		glEnable(GL_DEPTH_TEST);
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MouseMoveCallback);

	const auto shaderProgram = std::make_shared<ShaderProgram>("shaders/forward.vert", "shaders/forward.frag");
	const auto texture0 = std::make_shared<Texture2D>("res/wall.jpg");
	const auto texture1 = std::make_shared<Texture2D>("res/wall1.jpg");
	const auto texture2 = std::make_shared<Texture2D>("res/lava.jpg");

	auto material0 = std::make_shared<Material>(shaderProgram);
	material0->mainTex.texture = texture1;

	auto material1 = std::make_shared<Material>(shaderProgram);
	material1->mainTex.texture = texture0;
	material1->tex1.texture = texture2;
	material1->mainTex.intensity = 0.5f;
	material1->tex1.intensity = 0.5f;
	material1->mainTex.scale = {0.33f, 0.33f};

	auto cube = std::make_shared<Object>();
	cube->SetMaterial(material0);
	cube->SetMesh(Mesh::CreateCube());
	cube->SetPosition({0, 1, 0});
	cube->SetRotation({0, 0, 0});
	cube->SetScale({0.5f, 0.5f, 0.5f});

	auto plane = std::make_shared<Object>();
	plane->SetMaterial(material1);
	plane->SetMesh(Mesh::CreatePlane());
	plane->SetPosition({0, 0, 0});
	plane->SetScale({3, 1, 3});
	plane->SetRotation({90, 0, 0});

	std::vector<std::shared_ptr<Object>> objects = {plane, cube};

	MyCamera.SetProjection(45.0f, ScreenWidth / ScreenHeight, 0.1f, 100.0f);
	MyCamera.SetPosition({0, 1.5f, 3});
	MyCamera.SetOrientation(glm::quat(glm::vec3({glm::radians(-10.0f), 0, 0})));

	Renderer renderer;

	auto prev = std::chrono::steady_clock::now();
	while (!glfwWindowShouldClose(window))
	{
		const auto now = std::chrono::steady_clock::now();
		const float dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - prev).count() / 1000.f;
		prev = now;

		{
			auto r = cube->GetRotation();
			r.y += 30 * dt;
			cube->SetRotation(r);

			auto shift = material1->tex1.GetShift();
			shift.x += 0.25f * dt;
			material1->tex1.SetShift(shift);
		}

		glfwPollEvents();

		renderer.Render(MyCamera, objects);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// ����� ������������ �������� ESC, �� ������������� �������� WindowShouldClose � true, 
	// � ���������� ����� ����� ���������
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

bool MouseRighButtonPressed = false;
bool MouseMiddleButtonPressed = false;
double MousePrevX = 0;
double MousePrevY = 0;

void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			MouseRighButtonPressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			MouseRighButtonPressed = false;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS)
		{
			MouseMiddleButtonPressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			MouseMiddleButtonPressed = false;
		}
	}
}

void MouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
{
	const auto dx = static_cast<float>(xpos - MousePrevX);
	const auto dy = static_cast<float>(ypos - MousePrevY);

	if (MouseRighButtonPressed)
	{
		MyCamera.Pitch(dy / 10);
		MyCamera.Yaw(dx / 10);
	}
	else if (MouseMiddleButtonPressed)
	{
		MyCamera.Translate(MyCamera.GetOrientation() * glm::vec3(-dx / 100, dy / 100, 0));
	}

	MousePrevX = xpos;
	MousePrevY = ypos;
}