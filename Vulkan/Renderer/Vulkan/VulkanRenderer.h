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

	std::vector<VkFramebuffer> swapchainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkRenderPass renderPass;
	VkCommandPool commandPool;

	void createRenderPass();
	void createGraphicsPipeline(const std::string& vertex, const std::string& fragment);
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSemaphores();

	void drawFrame();

	VulkanRenderer(VkInstance *instance, GLFWwindow *window);
};