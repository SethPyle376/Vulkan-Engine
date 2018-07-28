
#include <vulkan\vulkan.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <set>

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
	
	void pickPhysicalDevice();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice);
	bool isDeviceSuitable(VkPhysicalDevice device);

	void createLogicalDevice();
public:

	VkInstance * instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSurfaceKHR *surface;

	VulkanDevice(VkInstance *instance, VkSurfaceKHR *surface);

	~VulkanDevice();
};