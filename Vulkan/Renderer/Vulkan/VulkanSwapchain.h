#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan\vulkan.h>
#include <glfw3.h>
#include <glfw3native.h>

#include <stdexcept>
#include <iostream>

class VulkanSwapchain
{
private:
	VkInstance * instance;
	

	GLFWwindow *window;

	void initSurface();

public:
	VulkanSwapchain(VkInstance *instance, GLFWwindow *window);
	VkSurfaceKHR surface;
};