#pragma once

#define GLFW_INCLUDE_VULKAN

#include <glfw3.h>
#include <glm.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <set>
#include <algorithm>

#include "Renderer\Vulkan\VulkanInits.h"

#include "Renderer\Vulkan\VulkanRenderer.h"


const std::vector<const char*> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };
//const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}

class Renderer
{
public:

	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow * window;

	VulkanRenderer *renderer;

	VkInstance instance;

	VkDebugReportCallbackEXT callback;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	//The VkSurfaceKHR creation is system dependent
	VkSurfaceKHR surface;

	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;

	VkPipeline graphicsPipeline;

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	struct QueueFamilyIndices
	{
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isComplete()
		{
			return graphicsFamily >= 0 && presentFamily >= 0;
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	void initWindow()
	{
		//Initialized glfw windowing libary
		glfwInit();
		//Tell glfw not to initialize an openGL context by default
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//Disable window resizing until we can support it in the renderer
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		//Create the actual window
		//Use hardcoded size constants to keep references consistent
		window = glfwCreateWindow(WIDTH, HEIGHT, "VULKAN", nullptr, nullptr);
	}

	void initVulkan()
	{
		createInstance();
		setupDebugCallback();

		renderer = new VulkanRenderer(&instance, window);
	}


	void mainLoop()
	{
		//Keep loop running until kill signal is sent to glfw
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			renderer->drawFrame();
		}
		vkDeviceWaitIdle(*(renderer->device->getLogicalDevice()));
	}


	void createSurface()
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: FAILED TO CREATE WINDOW SURFACE");
		}
	}



	void setupDebugCallback()
	{
		if (!enableValidationLayers) return;

		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		createInfo.pfnCallback = debugCallback;

		if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: FAILED TO SET UP DEBUG CALLBACK");
		}
	}

	void cleanup()
	{
		delete renderer;
		glfwDestroyWindow(window);
		glfwTerminate();
	}


	//! Creates Vulkan Instance
	/*!
		Creates an instance of Vulkan.
		If validation layers are enabled they will be specified during instance creation.
	*/
	void createInstance()
	{
		if (enableValidationLayers && !checkValidationLayerSupport())
		{
			throw std::runtime_error("VALIDATION REQUESTED BUT NOT AVAILABLE");
		}

		VkApplicationInfo appInfo = getApplicationInfo("Peacemaker", "Peacemaker");

		VkInstanceCreateInfo createInfo = getInstanceCreateInfo(&appInfo);
		//TODO: Consolidate instances into getInstanceCreateInfo eventually.
		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
		{
			throw std::runtime_error("FAILED TO CREATE VULKAN INSTANCE");
		}
	}

	bool checkValidationLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		}
		return extensions;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData)
	{
		std::cerr << "VALIDATION LAYER: " << msg << std::endl;
		return VK_FALSE;
	}
};