#include "VulkanSwapchain.h"


void VulkanSwapchain::initSurface()
{
	std::cout << "CREATING WINDOW SURFACE" << std::endl;
	if (glfwCreateWindowSurface(*(vulkanDevice->instance), vulkanDevice->window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("FAILED TO CREATE SURFACE");
	}
	std::cout << "WINDOW SURFACE CREATED" << std::endl;
}

VulkanSwapchain::VulkanSwapchain(VulkanDevice * device)
{
	vulkanDevice = device;
	initSurface();
}

VkSurfaceKHR * VulkanSwapchain::getSurface()
{
	return &surface;
}

SwapChainSupportDetails VulkanSwapchain::querySwapChainSupport()
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanDevice->physicalDevice, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanDevice->physicalDevice, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanDevice->physicalDevice, surface, &formatCount, details.formats.data());
	}
	return details;
}

