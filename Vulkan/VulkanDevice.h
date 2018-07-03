#pragma once

#include <vector>

#include <vulkan\vulkan.h>

class VulkanDevice
{
private:
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;

	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures features;
	VkPhysicalDeviceFeatures enabledFeatures;
	VkPhysicalDeviceMemoryProperties memoryProperties;

	std::vector<VkQueueFamilyProperties> queueFamilyProperties;
	std::vector<std::string> supportedExtensions;

	VkCommandPool commandPool = VK_NULL_HANDLE;

	struct
	{
		uint32_t graphics;
		uint32_t compute;
		uint32_t transfer;
	} queueFarmilyIndices;

	operator VkDevice() { return logicalDevice; }

public:
	VulkanDevice(VkPhysicalDevice physicalDevice);

	uint32_t getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32 *memTypeFound = nullptr);

	uint32_t getQueueFamilyIndex(VkQueueFlagBits queueFlags);



};