#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer(VkInstance* instance, GLFWwindow *window)
{
	swapChain = new VulkanSwapchain(instance, window);
	device = new VulkanDevice(instance, &swapChain->surface);
}
