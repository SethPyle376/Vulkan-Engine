#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

#include <glfw3.h>

class VulkanRenderer
{
private:
	

public:
	VulkanDevice * device;
	VulkanSwapchain * swapChain;
	VkInstance *instance;

	VulkanRenderer(VkInstance *instance, GLFWwindow *window);
};