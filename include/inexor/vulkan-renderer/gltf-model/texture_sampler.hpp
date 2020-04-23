#pragma once

#include <vulkan/vulkan.h>

namespace inexor::vulkan_renderer::gltf_model {

struct TextureSampler {
    VkFilter magFilter;
    VkFilter minFilter;
    VkSamplerAddressMode addressModeU;
    VkSamplerAddressMode addressModeV;
    VkSamplerAddressMode addressModeW;
};

} // namespace inexor::vulkan_renderer::gltf_model