#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan\vulkan.h>
#include <glfw3.h>
#include <glfw3native.h>

#include <stdexcept>
#include <iostream>
#include <vector>
#include <algorithm>

#include "VulkanDevice.h"

#define WIDTH 1280
#define HEIGHT 720

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain
{
private:
	friend class VulkanRenderer;
	VulkanDevice * vulkanDevice;
	VkSurfaceKHR surface;

	SwapChainSupportDetails querySwapChainSupport();

	GLFWwindow *window;

	void initSurface();
	bool swapChainAdequate();
	void createSwapchain();
	void createImageViews();

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void cleanup();

public:
	VulkanSwapchain(VulkanDevice *vulkanDevice, GLFWwindow *window);
	~VulkanSwapchain();
	
	VkSurfaceKHR getSurface();

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkSurfaceFormatKHR surfaceFormat;
	VkExtent2D extent;
};