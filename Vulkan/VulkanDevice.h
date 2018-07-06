#pragma once

#include <vector>
#include <iostream>

#include <vulkan\vulkan.h>
#include <assert.h>

#include "VulkanInits.h"
#include "VulkanBuffer.h"

#define DEFAULT_FENCE_TIMEOUT 100000000000

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

	bool enableDebugMarkers = false;

	struct
	{
		uint32_t graphics;
		uint32_t compute;
		uint32_t transfer;
	} queueFamilyIndices;

	operator VkDevice() { return logicalDevice; }

public:
	VulkanDevice(VkPhysicalDevice physicalDevice);

	uint32_t getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32 *memTypeFound = nullptr);

	//Might move this to tools
	uint32_t getQueueFamilyIndex(VkQueueFlagBits queueFlags);

	VkResult createLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions, bool useSwapChain = true, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

	VkResult createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, VkBuffer *buffer, VkDeviceMemory *memory, void *data = nullptr);
	VkResult createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VulkanBuffer *buffer, VkDeviceSize size, void *data = nullptr);
	void copyBuffer(VulkanBuffer *src, VulkanBuffer *dst, VkQueue queue, VkBufferCopy *copyRegion = nullptr);

	VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	VkCommandBuffer createCommandBuffer(VkCommandBufferLevel level, bool begin = false);
	void flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free = true);

	bool extensionSupported(std::string extension);
};