#pragma once

#define GLFW_INCLUDE_VULKAN

#include <glfw3.h>
#include <glm.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <set>
#include <algorithm>

#include "Renderer\Vulkan\VulkanInits.h"

#include "Renderer\Vulkan\VulkanRenderer.h"

class Renderer
{
public:

	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow * window;
	VulkanRenderer *renderer;
	VkInstance instance;
	VkSurfaceKHR surface;

	void initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(WIDTH, HEIGHT, "VULKAN", nullptr, nullptr);
	}

	void initVulkan()
	{
		renderer = new VulkanRenderer(window);
	}

	void mainLoop()
	{
		//Keep loop running until kill signal is sent to glfw
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			renderer->drawFrame();
		}
		vkDeviceWaitIdle(*(renderer->device->getLogicalDevice()));
	}

	void createSurface()
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: FAILED TO CREATE WINDOW SURFACE");
		}
	}

	void cleanup()
	{
		delete renderer;
		glfwDestroyWindow(window);
		glfwTerminate();
	}
};