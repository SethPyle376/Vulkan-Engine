#include "VulkanBuffer.h"

VkResult VulkanBuffer::map(VkDeviceSize size, VkDeviceSize offset)
{
	return vkMapMemory(device, memory, offset, size, 0, &mapped);
}

void VulkanBuffer::unmap()
{
	if (mapped)
	{
		vkUnmapMemory(device, memory);
		mapped = nullptr;
	}
}

VkResult VulkanBuffer::bind(VkDeviceSize offset)
{
	return vkBindBufferMemory(device, buffer, memory, offset);
}

void VulkanBuffer::setupDescriptor(VkDeviceSize size, VkDeviceSize offset)
{
	descriptor.offset = offset;
	descriptor.buffer = buffer;
	descriptor.range = size;
}

void VulkanBuffer::copyTo(void * data, VkDeviceSize size)
{
	//TODO: MAKE SURE MEMORY IS MAPPED
	memcpy(mapped, data, size);
}

VkResult VulkanBuffer::flush(VkDeviceSize size, VkDeviceSize offset)
{
	VkMappedMemoryRange mappedRange = Vulkan::Inits::mappedMemoryRange();
	mappedRange.memory = memory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkFlushMappedMemoryRanges(device, 1, &mappedRange);

}

VkResult VulkanBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
{
	VkMappedMemoryRange mappedRange = Vulkan::Inits::mappedMemoryRange();
	mappedRange.memory = memory;
	mappedRange.offset = offset;
	mappedRange.size = size;
	return vkInvalidateMappedMemoryRanges(device, 1, &mappedRange);
}

void VulkanBuffer::destroy()
{
	if (buffer)
	{
		vkDestroyBuffer(device, buffer, nullptr);
	}
	if (memory)
	{
		vkFreeMemory(device, memory, nullptr);
	}
}


