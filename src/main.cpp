#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return EXIT_FAILURE;
    }

    if (!glfwVulkanSupported()) {
        std::cerr << "GLFW reports Vulkan not supported!" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    std::cout << "GLFW initialized and Vulkan supported!" << std::endl;

    // Vulkan instance setup
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Test Vulkan App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance! Error code: " << result << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    std::cout << "Vulkan instance created successfully!" << std::endl;

    // Enumerate physical devices
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        std::cerr << "No Vulkan-compatible GPU found!" << std::endl;
    } else {
        std::cout << "Found " << deviceCount << " Vulkan-compatible GPU(s)." << std::endl;
    }

    // Clean up
    vkDestroyInstance(instance, nullptr);
    glfwTerminate();

    std::cout << "Cleanup complete. Everything works!" << std::endl;
    return EXIT_SUCCESS;
}