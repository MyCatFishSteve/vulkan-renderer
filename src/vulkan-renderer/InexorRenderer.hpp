#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <vulkan/vulkan.h>

#ifdef _WIN32
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif

// Custom error handling class for Inexor renderer.
#include "error-handling/VulkanErrorHandling.hpp"

// Change these definitions if you want to fork the renderer!
// These definitions will be used in the create_vulkan_instance() method.
#define INEXOR_ENGINE_VERSION VK_MAKE_VERSION(1,0,0)
#define INEXOR_APPLICATION_VERSION VK_MAKE_VERSION(1,0,0)
#define INEXOR_APPLICATION_NAME "Inexor-Application"
#define INEXOR_ENGINE_NAME "Inexor-Engine"


namespace inexor {
namespace vulkan_renderer {

	// 
	class InexorRenderer : public VulkanErrorHandling
	{
		private:

			// 
			GLFWwindow* window;

			// 
			VkInstance vulkan_instance;

			// 
			VkDevice vulkan_device;

			// 
			VkSurfaceKHR vulkan_surface;

			// 
			VkResult create_vulkan_instance(const std::string& application_name, const std::string& engine_name, const uint32_t application_version, const uint32_t engine_version, bool enable_validation_layers = true);

			// 
			VkResult create_physical_device(const VkPhysicalDevice& graphics_card);

			// 
			void enumerate_physical_devices();

			// The number of graphics cards on the machine.
			uint32_t number_of_physical_devices;

			// The graphics cards on this machine.
			std::vector<VkPhysicalDevice> graphics_cards;

			// 
			bool init_vulkan();

			// 
			void shutdown_vulkan();

			// Initialises a window using the GLFW library.
			void init_window(const int width, const int height, const std::string& window_name);
			
			// 
			void shutdown_window();

			// Gets the information on the graphics card and prints it to the console.
			void print_graphics_card_info(const VkPhysicalDevice& graphics_card);

			// 
			void print_physical_device_queue_families(const VkPhysicalDevice& graphics_card);

			// 
			void print_instance_layer_properties();

			// 
			void print_instance_extensions();
			
			// 
			void print_device_layers(const VkPhysicalDevice& graphics_card);

			// 
			void print_surface_capabilities(const VkPhysicalDevice& graphics_card);

			// 
			void print_supported_surface_formats(const VkPhysicalDevice& graphics_card);

			// 
			void print_presentation_modes(const VkPhysicalDevice& graphics_card);

		public:
			
			// 
			InexorRenderer();
			
			// 
			~InexorRenderer();

			// 
			void init();

			// 
			void run();

			// 
			void cleanup();

	};

};
};
