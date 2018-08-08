#pragma once

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanUtil.h"
#include "VulkanInits.h"

#include <glfw3.h>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


class VulkanRenderer
{
private:
	VkDebugReportCallbackEXT callback;
	

public:
	VulkanDevice * device;
	VulkanSwapchain * swapChain;
	VkInstance instance;

	std::vector<VkFramebuffer> swapchainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkRenderPass renderPass;
	VkCommandPool commandPool;

	void setupDebugCallback();
	void createRenderPass();
	void createGraphicsPipeline(const std::string& vertex, const std::string& fragment);
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSemaphores();
	void createInstance();

	void drawFrame();

	VulkanRenderer(GLFWwindow *window);
	~VulkanRenderer();
};