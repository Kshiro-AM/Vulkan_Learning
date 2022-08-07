#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <set>
#include <optional>
#include <string>

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	struct QueueFamilyIndices {
		std::optional<uint32_t> m_GraphicsFamily;
		std::optional<uint32_t> m_PresentFamily;

		bool isComplete() { return m_GraphicsFamily.has_value() && m_PresentFamily.has_value(); }
	};
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

private:
	void InitWindow();
	void InitVulken();
	void MainLoop();
	void CleanUp();

	// instance
	void CreateInstance();
	std::vector<const char*> GetExtensions();
	bool ExtensionsSupportCheck(std::vector<const char*> requiredExtensions);
	// validationLayer
	bool ValidationLayerSupportCheck();
	VkDebugUtilsMessengerCreateInfoEXT CreateDebugInfo();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	void SetupDebugMessenger();
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	// surface
	void CreateSurface();

	// physical device
	void PickupPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	// logical device
	void CreateLogicalDevice();

	// swap chain
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateSwapChain();

	// swapchainImageViews
	void CreateImageViews();

	// RenderPass
	void CreateRenderPass();

	// pipeline
	void CreateGraphicsPipeline();
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	// framebuffer
	void CreateFramebuffers();

	// command pool
	void CreateCommandPool();
	void CreateCommandBuffer();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	// Sync
	void CreateSyncObjects();

	// draw
	void DrawFrame();

	// helper
	static std::vector<char> ReadFile(const std::string& filename);

private:
	GLFWwindow* m_Window = nullptr;
	VkInstance m_Instance;
	VkDebugUtilsMessengerEXT m_DebugMessenger;
	VkSurfaceKHR m_Surface;
	VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
	VkDevice m_Device = VK_NULL_HANDLE;
	VkQueue m_GraphicsQueue;
	VkQueue m_PresentQueue;
	VkSwapchainKHR m_SwapChain;
	std::vector<VkImage> m_SwapChainImages;
	std::vector<VkImageView> m_SwapChainImageViews;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;
	VkRenderPass m_RenderPass;
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	VkCommandPool m_CommandPool;
	VkCommandBuffer m_CommandBuffer;
	VkSemaphore m_ImageAvailableSemaphore;
	VkSemaphore m_RenderFinishedSemaphore;
	VkFence m_InFlightFence;

	const std::vector<const char*> m_ValidationLayers = {
		"VK_LAYER_KHRONOS_validation",
	};
	const std::vector<const char*> m_DeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	uint32_t m_Width = 800;
	uint32_t m_Height = 600;

#ifdef NDEBUG
	const bool m_EnableValidationLayers = false;
#else
	const bool m_EnableValidationLayers = true;
#endif
};
