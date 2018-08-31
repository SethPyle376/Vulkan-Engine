#include "VulkanBuffer.h"

VulkanBuffer::VulkanBuffer(VulkanDevice * logicalDevice)
{
	device = logicalDevice;
}

void VulkanBuffer::createVertexBuffer(std::vector<vertex> vertices)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices[0]) * vertices.size();
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(*(device->getLogicalDevice()), &bufferInfo, nullptr, &vertexBuffer))
	{
		throw std::runtime_error("FAILED TO CREATE VERTEX BUFFER");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(*(device)->getLogicalDevice(), vertexBuffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device->getPhysicalDevice());
	
	if (vkAllocateMemory(*(device->getLogicalDevice()), &allocInfo, nullptr, &vertexBufferMemory))
	{
		throw std::runtime_error("FAILED TO ALLOCATE VERTEX BUFFER MEMORY");
	}

	vkBindBufferMemory(*(device->getLogicalDevice()), vertexBuffer, vertexBufferMemory, 0);

	void* data;
	vkMapMemory(*(device->getLogicalDevice()), vertexBufferMemory, 0, bufferInfo.size, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferInfo.size);
	vkUnmapMemory(*(device->getLogicalDevice()), vertexBufferMemory);
}

VkBuffer VulkanBuffer::getVertexBuffer()
{
	return vertexBuffer;
}

VulkanBuffer::~VulkanBuffer()
{
	vkDestroyBuffer(*(device->getLogicalDevice()), vertexBuffer, nullptr);
	vkFreeMemory(*(device->getLogicalDevice()), vertexBufferMemory, nullptr);
}
