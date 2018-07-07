#pragma once

#include <vulkan\vulkan.h>

#ifdef _WIN32
#pragma comment(linker, "/subsystem:windows")
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <ShellScalingApi.h>
#endif

#include <iostream>
#include <chrono>
#include <sys/stat.h>

#include <string>
#include <array>
#include <numeric>

#include <glm.hpp>

#include "VulkanInits.h"
#include "VulkanDevice.h"

class VulkanBase
{
private:
	float fpsTimer = 0.0f;

	std::string getWindowTitle();

	//Indicates view has updated and matrix buffers need updated
	bool viewUpdated = false;

	uint32_t destWidth;
	uint32_t destHeight;
	bool resizing = false;
	//TODO: imgui implementation
	//VulkanOverlay *uiOverlay = nullptr

	void windowResize();
	void handleMouseMove(int32_t x, int32_t y);

protected:
	uint32_t frameCounter = 0;
	uint32_t lastFPS = 0;

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	VkPhysicalDeviceMemoryProperties deviceMemoryProperties;

	VkPhysicalDeviceFeatures enabledFeatures{};

	std::vector<const char*> enabledDeviceExtensions;
	std::vector<const char*> enabledInstanceExtensions;

	//Logical device
	VkDevice device;
	//Handle to device graphics queue to submit command buffers to
	VkQueue queue;

	//Depth buffer format
	VkFormat depthFormat;

	//Command buffer pool
	VkCommandPool cmdPool;

	VkPipelineStageFlags submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submitInfo;

	//Command buffers for rendering
	std::vector<VkCommandBuffer> drawCmdBuffers;

	//Global render pass for frame buffer writes
	VkRenderPass renderPass;

	//List of available frame buffers
	std::vector<VkFramebuffer> frameBuffers;

	uint32_t currentBuffer = 0;

	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

	//List of shader modules created for cleanup later
	std::vector<VkShaderModule> shaderModules;

	//Pipeline cache object
	VkPipelineCache pipelineCache;

	//TODO: swapchain encapsulation doodarino
	//VulkanSwapChain swapChain;

	struct {
		VkSemaphore presentComplete;
		VkSemaphore renderComplete;
		VkSemaphore overlayComplete;
	} semaphores;

	std::vector<VkFence> waitFences;

public:
	bool prepared = false;
	uint32_t width = 1280;
	uint32_t height = 720;

	float frameTimer = 1.0f;
	const std::string getAssetPath();

	VulkanDevice *vulkanDevice;

	struct settings {
		bool validation = false;
		bool fullscreen = false;
		bool vsync = false;
		bool overlay = false;
	} settings;

	VkClearColorValue defaultClearColor = { {0.025f, 0.025f, 0.025, 1.0f} };

	float zoom = 0;
	static std::vector<const char*> args;

	float timer = 0.0f;
	float timerSpeed = 0.25f;
	bool paused = false;

	float rotationSpeed = 1.0f;
	float zoomSpeed = 1.0f;

	//TODO: MAKE CAMERA CLASS
	//Camera camera;

	glm::vec3 rotation = glm::vec3();
	glm::vec3 cameraPos = glm::vec3();
	glm::vec2 mousePos;

	std::string title = "PEACEMAKER - VULKAN";
	std::string name = "peacemakerVulkan";
	uint32_t apiVersion = VK_API_VERSION_1_0;

	struct {
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	} depthStencil;

	struct {
		glm::vec2 axisLeft = glm::vec2(0.0f);
		glm::vec2 axisRight = glm::vec2(0.0f);
	} gamePadState;

	struct {
		bool left = false;
		bool right = false;
		bool middle = false;
	} mouseButtons;
	
#if defined(_WIN32)
	HWND window;
	HINSTANCE windowInstance;
#endif
};
