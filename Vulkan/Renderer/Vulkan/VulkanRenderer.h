#pragma once

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanUtil.h"

#include <glfw3.h>

class VulkanRenderer
{
private:
	

public:
	VulkanDevice * device;
	VulkanSwapchain * swapChain;
	VkInstance *instance;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkRenderPass renderPass;

	void createRenderPass();
	void createGraphicsPipeline(const std::string& vertex, const std::string& fragment);

	VulkanRenderer(VkInstance *instance, GLFWwindow *window);
};