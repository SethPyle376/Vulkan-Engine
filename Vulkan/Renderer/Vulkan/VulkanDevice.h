#pragma once
#include <vulkan\vulkan.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <set>

#include <glfw3.h>

const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};



class VulkanDevice
{
private:
	friend class VulkanSwapchain;
	void pickPhysicalDevice();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);
	bool isDeviceSuitable(VkPhysicalDevice device);

	

	GLFWwindow *window;

	VkInstance * instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR *surface;

	
public:
	void createLogicalDevice();

	void setSurface(VkSurfaceKHR *surface);

	VkInstance * getInstance();
	VkPhysicalDevice * getPhysicalDevice();
	VkDevice * getLogicalDevice();
	VkQueue getGraphicsQueue();
	VkQueue getPresentQueue();
	VkSurfaceKHR * getSurface();

	VulkanDevice(VkInstance *instance, GLFWwindow *window);

	~VulkanDevice();
};