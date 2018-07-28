#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan\vulkan.h>
#include <glfw3.h>
#include <glfw3native.h>

#include <stdexcept>
#include <iostream>
#include <vector>

#include "VulkanDevice.h"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain
{
private:
	VulkanDevice * vulkanDevice;
	VkSurfaceKHR surface;

	SwapChainSupportDetails querySwapChainSupport();

	void initSurface();

public:
	VulkanSwapchain(VulkanDevice *vulkanDevice);
	
	VkSurfaceKHR * getSurface();
};