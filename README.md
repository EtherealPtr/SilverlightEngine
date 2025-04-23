<h1 align="center">Silverlight Engine</h1>

<p align="center">
<img src="https://github.com/user-attachments/assets/40e3b578-1e6c-4ddf-95a9-543797c2084b" alt="Teapot" width="600"/>
</p>

<p><strong>Silverlight Engine</strong> is a modern, high-performance C++ Vulkan rendering engine designed for real-time graphics, simulations, and game development. It follows a layer-based architecture that promotes modularity, maintainability, and scalability.</p>

<h1>Key Features</h1>
<ul>
<li> Directional shadow mapping</li>
<li> Cubemaps</li>
<li> Lighting system with support for directional, point, and spotlights with customizable properties</li>
<li> Custom glTF/glb model loading system with support for materials</li>
<li> Support for primitive shapes rendering along with the ability to apply custom textures</li>
<li> Logging system that outputs logs to console and an external log file</li>
<li> Default first-person view style camera movement</li>
<li> Input system that supports keyboard and mouse input</li>
</ul>

<h1>Recently Added Features</h1>
<ul>
<li>✅ Multi-light rendering support in scenes</li>
<li>✅ Real-time directional shadow mapping</li>
<li>✅ Cubemaps</li>
</ul>

<h1>Getting Started</h1>
<p>To get started, it's recommended that you have <strong>Visual Studio 2022</strong> installed. You'll also need CMake to generate the project files. 
Silverlight has only been tested on Windows and may not be compatible with other development environments.</p>
<ul>
<li>Clone the repository <code>git clone git@github.com:EtherealPtr/SilverlightEngine.git</code></li>
<li>Run <code>Setup_Windows.bat</code> file found in the root directory, it will check for Vulkan SDK and generate the project files</li>
<li>If the Vulkan SDK is missing, please install version 1.2 or higher from <a href="https://vulkan.lunarg.com/sdk/home" target="_blank">LunarG's website</a></li>
</ul>

<h1>How To Render</h1>
<p>Rendering with Silverlight is super fast and simple! All you have to do is attach a mesh component to your entity. Example:</p>

```cpp
class Teapot : public Silverlight::Entity
{
public:
    Teapot()
    {
        AddComponent<Silverlight::CustomMeshComponent>("Models/teapot.gltf");
    }
};
```
<img src="https://github.com/user-attachments/assets/265f177e-e9b7-43a0-ba75-e72f5e5651bb" alt="image" width="400"/>
<p>Et voilà! Just like that. Magic! Just attach a mesh component to your entity and you're good to go. </p>
<p>Want your object to move around? Attach a <code>TransformComponent</code> then override the <code>Update</code> method to add your logic. Example:</p>

```cpp
class Teapot : public Silverlight::Entity
{
public:
    Teapot()
    {
        AddComponent<Silverlight::CustomMeshComponent>("Models/teapot.gltf");
        m_Transform = AddComponent<Silverlight::TransformComponent>();
    }

    void Update(const float _deltaTime) override
    {
        const glm::quat rotation{ glm::angleAxis(m_RotationSpeed * _deltaTime, glm::vec3(0.0f, 1.0f, 0.0f)) };
        m_Transform->Rotate(rotation);
    }

private:
    float m_RotationSpeed{ glm::radians(45.0f) };
    std::shared_ptr<Silverlight::TransformComponent> m_Transform;
};
```

<img src="https://github.com/user-attachments/assets/3d702c84-3def-4422-95a2-3f00ed3906d0" alt="Teapot" width="400"/>

<h1>Architecture Overview</h1>
<p>Silverlight Engine follows a layered architecture for better modularity, maintainability, and scalability.</p>
<ul>
<li>Application Layer: This layer serves as the main entry point that ties the engine's subsystems and user-defined logic together to create a runnable application</li>
<li>Graphics Layer: Vulkan code and other graphics-related code live here. This layer handles all rendering responsibilities such as pipeline creation, materials, lighting, model loading, and more</li>
<li>Foundation Layer: This is the base layer that all other layers build on top of. It provides core utilities like logging, input system, entity/component, timers, and resource handling</li>
</ul>

<h1>Why Silverlight Engine?</h1>
<p>This is a passion project I've been developing in my free time. My goal is to create a powerful and robust rendering engine that uses the Vulkan API while providing a simple and user-friendly interface. There's still a long way to go, but I'm overall satisfied with the current state of the project. I set this challenge for myself to deepen my understanding of complex engine architecture and modern low-level rendering with Vulkan, as well as to explore new C++ features and enhance my C++ skills. The engine can be used as a visualization, simulation, or a prototyping tool, or it can serve as an educational tool for developers interested in learning about engine architecture and Vulkan rendering.</p>
