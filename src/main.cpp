#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <optional>

static void vkcheck(VkResult r, const char* msg) {
    if (r != VK_SUCCESS) throw std::runtime_error(msg);
}

std::vector<char> loadFile(const std::string& path) {
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file: " + path);

    size_t size = (size_t)file.tellg();
    std::vector<char> buffer(size);

    file.seekg(0);
    file.read(buffer.data(), size);
    file.close();
    return buffer;
}

VkShaderModule createShaderModule(VkDevice device, const std::vector<char>& code) {
    VkShaderModuleCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = code.size();
    info.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule module;
    vkcheck(vkCreateShaderModule(device, &info, nullptr, &module),
            "Failed to create shader module");
    return module;
}

struct QueueFamilies {
    std::optional<uint32_t> graphics;
    std::optional<uint32_t> present;
    bool complete() const { return graphics && present; }
};

QueueFamilies findQueueFamilies(VkPhysicalDevice dev, VkSurfaceKHR surface) {
    QueueFamilies q;

    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &count, nullptr);
    std::vector<VkQueueFamilyProperties> props(count);
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &count, props.data());

    for (uint32_t i = 0; i < count; i++) {
        if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            q.graphics = i;

        VkBool32 present;
        vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, surface, &present);
        if (present) q.present = i;

        if (q.complete()) break;
    }
    return q;
}

int main() {
    try {

        if (!glfwInit())
            throw std::runtime_error("GLFW init failed");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(1280, 720, "ProjectName", nullptr, nullptr);

        VkApplicationInfo app{};
        app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app.pApplicationName = "ProjectName";
        app.apiVersion = VK_API_VERSION_1_3;

        uint32_t extCount;
        const char** ext = glfwGetRequiredInstanceExtensions(&extCount);

        VkInstanceCreateInfo inst{};
        inst.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        inst.pApplicationInfo = &app;
        inst.enabledExtensionCount = extCount;
        inst.ppEnabledExtensionNames = ext;

        VkInstance instance;
        vkcheck(vkCreateInstance(&inst, nullptr, &instance),
                "Failed to create instance");

        VkSurfaceKHR surface;
        vkcheck(glfwCreateWindowSurface(instance, window, nullptr, &surface),
                "Failed to create surface");

        uint32_t gpuCount = 0;
        vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
        if (gpuCount == 0)
            throw std::runtime_error("No Vulkan GPUs found");

        std::vector<VkPhysicalDevice> gpus(gpuCount);
        vkEnumeratePhysicalDevices(instance, &gpuCount, gpus.data());

        VkPhysicalDevice gpu = VK_NULL_HANDLE;
        QueueFamilies families;

        for (auto d : gpus) {
            auto f = findQueueFamilies(d, surface);
            if (f.complete()) {
                gpu = d;
                families = f;
                break;
            }
        }

        if (gpu == VK_NULL_HANDLE)
            throw std::runtime_error("No suitable GPU");

        float prio = 1.0f;
        std::vector<VkDeviceQueueCreateInfo> qinfos;

        for (uint32_t fam : { families.graphics.value(), families.present.value() }) {
            VkDeviceQueueCreateInfo q{};
            q.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            q.queueFamilyIndex = fam;
            q.queueCount = 1;
            q.pQueuePriorities = &prio;
            qinfos.push_back(q);
        }

        VkDeviceCreateInfo dinfo{};
        dinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        dinfo.queueCreateInfoCount = qinfos.size();
        dinfo.pQueueCreateInfos = qinfos.data();

        VkDevice device;
        vkcheck(vkCreateDevice(gpu, &dinfo, nullptr, &device),
                "Failed to create device");

        VkQueue graphicsQueue;
        VkQueue presentQueue;
        vkGetDeviceQueue(device, families.graphics.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, families.present.value(), 0, &presentQueue);

        std::string shaderBase = "shaders/";

        auto vertCode = loadFile(shaderBase + "vertex.glsl.spv");
        auto fragCode = loadFile(shaderBase + "fragment.glsl.spv");

        VkShaderModule vertModule = createShaderModule(device, vertCode);
        VkShaderModule fragModule = createShaderModule(device, fragCode);

        std::cout << "Shaders carregados com sucesso!\n";

        glm::mat4 proj = glm::perspective(glm::radians(70.0f), 1280.f / 720.f, 0.1f, 100.f);
        std::cout << "Matrix proj[0][0] = " << proj[0][0] << "\n";

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForVulkan(window, true);

        VkDescriptorPoolSize poolSize = {
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = 100;

        VkDescriptorPool imguiPool;
        vkcheck(vkCreateDescriptorPool(device, &poolInfo, nullptr, &imguiPool), "ImGui: pool fail");

        ImGui_ImplVulkan_InitInfo init{};
        init.Instance = instance;
        init.PhysicalDevice = gpu;
        init.Device = device;
        init.Queue = graphicsQueue;
        init.DescriptorPool = imguiPool;

        ImGui_ImplVulkan_Init(&init, VK_NULL_HANDLE);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("ProjectName");
            ImGui::Text("Shaders carregados e modules criados.");
            ImGui::Text("Usando Vulkan + GLFW + GLM + ImGui.");
            ImGui::Text("proj[0][0] = %.3f", proj[0][0]);
            ImGui::End();

            ImGui::Render();
        }

        vkDeviceWaitIdle(device);

        vkDestroyShaderModule(device, vertModule, nullptr);
        vkDestroyShaderModule(device, fragModule, nullptr);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        vkDestroyDescriptorPool(device, imguiPool, nullptr);
        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);
        glfwTerminate();
    }
    catch (std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return -1;
    }

    return 0;
}