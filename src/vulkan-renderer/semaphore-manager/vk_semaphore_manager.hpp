#pragma once

#include "../class-templates/manager_template.hpp"
#include "../debug-marker/vk_debug_marker_manager.hpp"

#include <vulkan/vulkan.h>
#include <mutex>

#include <spdlog/spdlog.h>

#include <cassert>


namespace inexor {
namespace vulkan_renderer {


	/// @brief VulkanSemaphoreManager
	class VulkanSemaphoreManager : public ManagerClassTemplate<VkSemaphore>
	{
		private:

			bool semaphore_manager_initialised = false;

			std::mutex semaphore_manager_mutex;
			
			VkDevice device = VK_NULL_HANDLE;

			std::shared_ptr<VulkanDebugMarkerManager> debug_marker_manager;


		public:
			
			VulkanSemaphoreManager();

			~VulkanSemaphoreManager();


		protected:

			
			/// @brief Initialises Vulkan semaphore manager.
			/// @param device [in] The Vulkan device.
			/// @param debug_marker_manager [in] A pointer to the debug marker manager.
			VkResult initialise(const VkDevice& device, const std::shared_ptr<VulkanDebugMarkerManager> debug_marker_manager);


			/// @brief Checks if a semaphore with this name already exists.
			/// @param semaphore_name The name of the semaphore.
			/// @return True if a Vulkan semaphore with this name already exists, false otherwise.
			bool does_semaphore_exist(const std::string& semaphore_name);


			/// @brief Creates a new Vulkan semaphore.
			/// @param device [in] The Vulkan device handle.
			/// @param semaphore_name [in] The unique name of the semaphore.
			std::optional<std::shared_ptr<VkSemaphore>> create_semaphore(const std::string& semaphore_name);


			/// @brief Gets a certain semaphore by name.
			/// @param semaphore_name [in] The name of the semaphore.
			/// @return The acquired semaphore (if existent), std::nullopt otherwise.
			std::optional<std::shared_ptr<VkSemaphore>> get_semaphore(const std::string& semaphore_name);


			/// @brief Destroys all existing semaphores.
			/// @param device [in] The Vulkan device.
			void shutdown_semaphores();

	};
	
};
};