#include "VulkanSwapchain.h"


void VulkanSwapchain::initSurface()
{
	std::cout << "CREATING WINDOW SURFACE" << std::endl;
	if (glfwCreateWindowSurface(*instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("FAILED TO CREATE SURFACE");
	}
	std::cout << "WINDOW SURFACE CREATED" << std::endl;
}

VulkanSwapchain::VulkanSwapchain(VkInstance *instance, GLFWwindow * window)
{
	this->window = window;
	this->instance = instance;

	initSurface();
}

