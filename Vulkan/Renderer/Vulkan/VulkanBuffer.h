#pragma once
#include <vector>

#include <vulkan/vulkan.h>

#include "VulkanDevice.h"
#include "VulkanUtil.h"
#include "Model.h"

class VulkanBuffer
{
public:
	VulkanBuffer(VulkanDevice* logicalDevice);
	~VulkanBuffer();
	void createVertexBuffer(std::vector<vertex> vertices);
	VkBuffer getVertexBuffer();

private:
	VulkanDevice *device;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
};