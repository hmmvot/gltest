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
#include "Camera.h"
#include "Renderer.h"

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void MouseMoveCallback(GLFWwindow *window, double xpos, double ypos);

const float ScreenWidth = 1280;
const float ScreenHeight = 720;

Camera MyCamera;
ObjectRef Cube;

int main()
{
	//Инициализация GLFW
	glfwInit();

	//Настройка GLFW
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

	const auto standardShader = std::make_shared<ShaderProgram>("shaders/forward.vert", "shaders/forward.frag");
	const auto lightSourceShader = std::make_shared<ShaderProgram>("shaders/forward.vert", "shaders/lightSource.frag");

	const auto texture0 = std::make_shared<Texture2D>("res/wall.jpg");
	const auto texture1 = std::make_shared<Texture2D>("res/box.png");
	const auto texture2 = std::make_shared<Texture2D>("res/lava.jpg");

	auto material0 = std::make_shared<Material>(standardShader);
	material0->mainTex.texture = texture1;
	material0->shiness = 1;

	auto material1 = std::make_shared<Material>(standardShader);
	material1->mainTex.texture = texture0;
	//material1->tex1.texture = texture2;
	material1->mainTex.intensity = 0.5f;
	material1->tex1.intensity = 0.5f;
	material1->mainTex.scale = {0.1f, 0.1f};
	material1->tex1.scale = {0.1f, 0.1f};
	material1->shiness = 64;

	auto lightSourceMaterial = std::make_shared<Material>(lightSourceShader);

	auto cube = Object::Create();
	cube->material = material0;
	cube->mesh = Mesh::CreateCube();
	cube->transform.SetPosition({0, 1, 0});
	cube->transform.SetRotation({30, 0, 0});
	cube->transform.SetScale({0.5f, 0.5f, 0.5f});

	Cube = cube;

	auto plane = Object::Create();
	plane->material = material1;
	plane->mesh = Mesh::CreatePlane();
	plane->transform.SetPosition({0, 0, 0});
	plane->transform.SetScale({20, 20, 1});
	plane->transform.SetRotation({-90, 0, 0});

	auto sun = Object::Create();
	sun->material = lightSourceMaterial;
	sun->mesh = Mesh::CreateCube();
	sun->light = std::make_shared<Light>(Light::Type::Directional);
	//sun->light->SetColor({1.0f, 0.5f, 0});
	sun->light->SetColor({1, 1, 1});
	sun->light->intensity = 0.3f;
	sun->transform.SetPosition({0, 2, 1});
	sun->transform.SetScale({0.1f, 0.1f, 0.1f});
	sun->transform.SetRotation({-90, 0, 0});

	auto lamp1 = Object::Create();
	lamp1->material = lightSourceMaterial;
	lamp1->mesh = Mesh::CreateCube();
	lamp1->light = std::make_shared<Light>(Light::Type::Point);
	//lamp1->light->SetColor({1.0f, 0, 0});
	lamp1->light->constant = 1.0f;
	lamp1->light->linear = 0.35f;
	lamp1->light->quadratic = 0.44f;
	lamp1->transform.SetPosition({0.5, 1, 1});
	lamp1->transform.SetScale({0.1f, 0.1f, 0.1f});

	auto lamp2 = Object::Create();
	lamp2->material = lightSourceMaterial;
	lamp2->mesh = Mesh::CreateCube();
	lamp2->light = std::make_shared<Light>(Light::Type::Flashlight);
	lamp2->light->SetColor({1.0f, 0, 0});
	lamp2->light->constant = 1.0f;
	lamp2->light->linear = 0.35f;
	lamp2->light->quadratic = 0.44f;
	lamp2->light->cutoff = 30.0f;
	lamp2->light->cutoffDelta = 5.0f;
	lamp2->transform.SetPosition({-0.5, 1, 1});
	lamp2->transform.SetScale({0.1f, 0.1f, 0.1f});
	lamp2->transform.SetRotation({0, 0, 0});

	std::vector<ObjectRef> objects = {plane, cube, sun, lamp1, lamp2};

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
			//auto r = cube->transform.GetRotation();
			//r.y += 30 * dt;
			//cube->transform.SetRotation(r);

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
	// Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
	// и приложение после этого закроется
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
		auto r = Cube->transform.GetRotation();
		r.x += dy / 10;
		r.y += dx / 10;
		Cube->transform.SetRotation(r);
	}
	else if (MouseMiddleButtonPressed)
	{
		MyCamera.Translate(MyCamera.GetOrientation() * glm::vec3(-dx / 100, dy / 100, 0));
	}

	MousePrevX = xpos;
	MousePrevY = ypos;
}
