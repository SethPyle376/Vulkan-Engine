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

const int MAX_FRAMES_IN_FLIGHT = 2;



class VulkanRenderer
{
private:
	friend class Renderer;
	VulkanDevice * device;
	VulkanSwapchain * swapChain;
	VkInstance instance;

	GLFWwindow *window;

	VkDebugReportCallbackEXT callback;

	std::vector<VkFramebuffer> swapchainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	size_t currentFrame = 0;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkRenderPass renderPass;
	VkCommandPool commandPool;

	void initWindow();
	void setupDebugCallback();
	void createRenderPass();
	void createGraphicsPipeline(const std::string& vertex, const std::string& fragment);
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSemaphores();
	void createInstance();

	void recreateSwapchain();

	void cleanupSwapchain();

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto renderer = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
		renderer->framebufferResize = true;
	}

public:
	VulkanRenderer();
	~VulkanRenderer();

	bool framebufferResize = false;

	void drawFrame();
};