#include "VulkanDevice.h"

VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice)
{
	this->physicalDevice = physicalDevice;

	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	vkGetPhysicalDeviceFeatures(physicalDevice, &features);

	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	queueFamilyProperties.resize(queueFamilyCount);

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

	uint32_t extCount = 0;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);

	if (extCount > 0)
	{
		std::vector<VkExtensionProperties> extensions(extCount);

		if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
		{
			for (auto ext : extensions)
			{
				supportedExtensions.push_back(ext.extensionName);
			}
		}
	}

}

uint32_t VulkanDevice::getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32 * memTypeFound)
{
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((typeBits & 1) == 1)
		{
			if ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				if (memTypeFound)
				{
					*memTypeFound = true;
				}
				return i;
			}
		}
		typeBits >>= 1;
	}

	if (memTypeFound)
	{
		*memTypeFound = false;
		return 0;
	}
	else
	{
		throw std::runtime_error("NO MATCHING MEMORY TYPE FOUND");
	}
}

//Get index of queue family that supports requested flags
uint32_t VulkanDevice::getQueueFamilyIndex(VkQueueFlagBits queueFlags)
{
	if (queueFlags & VK_QUEUE_COMPUTE_BIT)
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
		{
			if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)))
			{
				return i;
				break;
			}
		}
	}

	if (queueFlags & VK_QUEUE_TRANSFER_BIT)
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
		{
			if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
			{
				return i;
				break;
			}
		}
	}

	for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
	{
		if (queueFamilyProperties[i].queueFlags & queueFlags)
		{
			return i;
			break;
		}
	}
	throw std::runtime_error("NO MATCHING QUEUE FAMILY INDEX FOUND");
}

VkResult VulkanDevice::createLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions, bool useSwapChain, VkQueueFlags requestedQueueTypes)
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};


	const float defaultQueuePriority(0.0f);

	if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
	{
		queueFamilyIndices.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
		VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueFamilyIndices.graphics;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &defaultQueuePriority;
		queueCreateInfos.push_back(queueInfo);
	}
	else
	{
		queueFamilyIndices.graphics = VK_NULL_HANDLE;
	}

	if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
	{
		queueFamilyIndices.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
		if (queueFamilyIndices.compute != queueFamilyIndices.graphics)
		{
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = queueFamilyIndices.compute;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(queueInfo);
		}
	}
	else
	{
		queueFamilyIndices.compute = queueFamilyIndices.graphics;
	}

	if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
	{
		queueFamilyIndices.transfer = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
		if ((queueFamilyIndices.transfer != queueFamilyIndices.graphics) && (queueFamilyIndices.transfer != queueFamilyIndices.compute))
		{
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = queueFamilyIndices.transfer;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &defaultQueuePriority;
			queueCreateInfos.push_back(queueInfo);
		}
	}
	else
	{
		queueFamilyIndices.transfer = queueFamilyIndices.graphics;
	}

	std::vector<const char*> deviceExtensions(enabledExtensions);
	if (useSwapChain)
	{
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	}

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

	//TODO
	if (/*extensionSupported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME)*/ true)
	{
		deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
		enableDebugMarkers = true;
	}

	if (deviceExtensions.size() > 0)
	{
		deviceCreateInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	}

	VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);

	if (result == VK_SUCCESS)
	{
		//TODO
		//commandPool = createCommandPool(queueFamilyIndices.graphics);
	}
	this->enabledFeatures = enabledFeatures;
	return result;
}

VkResult VulkanDevice::createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, VkBuffer * buffer, VkDeviceMemory * memory, void * data)
{
	VkBufferCreateInfo bufferCreateInfo = Vulkan::Inits::bufferCreateInfo();
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //Research
	vkCreateBuffer(logicalDevice, &bufferCreateInfo, nullptr, buffer);

	VkMemoryRequirements memReqs;
	VkMemoryAllocateInfo memAlloc = Vulkan::Inits::memoryAllocateInfo();
	vkGetBufferMemoryRequirements(logicalDevice, *buffer, &memReqs);
	memAlloc.allocationSize = memReqs.size;

	memAlloc.memoryTypeIndex = getMemoryType(memReqs.memoryTypeBits, memoryPropertyFlags);
	vkAllocateMemory(logicalDevice, &memAlloc, nullptr, memory);

	if (data != nullptr)
	{
		void *mapped;
		vkMapMemory(logicalDevice, *memory, 0, size, 0, &mapped);
		memcpy(mapped, data, size);

		if ((memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
		{
			VkMappedMemoryRange mappedRange = Vulkan::Inits::mappedMemoryRange();
			mappedRange.memory = *memory;
			mappedRange.offset = 0;
			mappedRange.size = size;
			vkFlushMappedMemoryRanges(logicalDevice, 1, &mappedRange);
		}
	}
	vkBindBufferMemory(logicalDevice, *buffer, *memory, 0);
	return VK_SUCCESS;
}

VkResult VulkanDevice::createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VulkanBuffer * buffer, VkDeviceSize size, void * data)
{
	buffer->device = logicalDevice;

	VkBufferCreateInfo bufferCreateInfo = Vulkan::Inits::bufferCreateInfo(usageFlags, size);
}


