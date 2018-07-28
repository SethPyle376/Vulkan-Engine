#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer(VkInstance* instance, GLFWwindow *window)
{
	device = new VulkanDevice(instance, window);
	swapChain = new VulkanSwapchain(device);
	device->setSurface(swapChain->getSurface());
	device->createLogicalDevice();
}
