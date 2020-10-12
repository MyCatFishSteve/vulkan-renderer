#pragma once

#include "inexor/vulkan-renderer/wrapper/gpu_memory_buffer.hpp"

#include <vulkan/vulkan_core.h>

#include <string>

namespace inexor::vulkan_renderer::wrapper {

class Device;

/// @brief RAII wrapper class for uniform buffers.
class UniformBuffer : public GPUMemoryBuffer {
protected:
    VkDescriptorBufferInfo m_descriptor_buffer_info{};
    VkDescriptorSet m_descriptor_set{};

public:
    /// @brief Default constructor.
    /// @param device The const reference to a device RAII wrapper instance.
    /// @param name The internal debug marker name of the uniform buffer.
    /// @param size The size of the uniform buffer.
    /// @todo Add overloaded constructor which directly accepts the uniform buffer data.
    UniformBuffer(const Device &device, const std::string &name, const VkDeviceSize &size);

    UniformBuffer(const UniformBuffer &) = delete;
    UniformBuffer(UniformBuffer &&) noexcept;

    ~UniformBuffer() = default;

    UniformBuffer &operator=(const UniformBuffer &) = delete;
    UniformBuffer &operator=(UniformBuffer &&) = default;

    /// @brief Update uniform buffer data.
    /// @param data A pointer to the uniform buffer data.
    /// @param size The size of the uniform buffer memory to copy.
    void update(void *data, const std::size_t size);
};

} // namespace inexor::vulkan_renderer::wrapper
