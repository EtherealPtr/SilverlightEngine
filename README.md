<h1 align="center">Silverlight Engine</h1>

<p align="center">
  <img src ="https://github.com/user-attachments/assets/59d3eefa-d47b-4818-ab9f-14efaa31aa9a" alt="image">
</p>

<p><strong>Silverlight</strong> is a modern, high-performance C++20 and Vulkan-based 3D rendering engine designed for real-time graphics, simulations, and game development. It features a modular architecture focused on performance and flexibility.</p>

<h2>Key Features</h2>
<ul>
<li> Advanced Lighting System: Directional, point, and spotlights with customizable properties</li>
<li> Dynamic Shadows: Directional shadow mapping support for realistic lighting</li>
<li> Custom Model Loading: glTF/glb support with submesh-based materials</li>
<li> Primitive Shapes: Triangle, cube, and plane with custom textures and vertex coloring</li>
<li> Modern Vulkan Renderer: Efficient rendering pipeline for high-performance rendering</li>
<li> Logging System: Outputs to console and external log files</li>
<li> Camera System: Default first-person style movement</li>
<li> Input System: Keyboard and mouse input for interactive applications</li>
</ul>

<h2>Recently Added Features</h2>
<ul>
<li>✅ An improved lighting system to support more light casters, including directional lights, point lights, and spotlights</li>
<li>✅ Multi-light rendering support within scenes</li>
<li>✅ Real-time directional shadow mapping</li>
</ul>

<h1>Getting Started</h1>
<p>To get started, it's recommended that you have <strong>Visual Studio 2022</strong> installed. 
Silverlight has only been tested on Windows and may not be compatible with other development environments.</p>
<ul>
<li>Clone the repository <code>git clone git@github.com:EtherealPtr/SilverlightEngine.git</code></li>
<li>Run <code>Setup_Windows.bat</code> file found in the root directory, it will check for Vulkan SDK and generate the project files</li>
<li>If the Vulkan SDK is missing, please install it from <a href="https://vulkan.lunarg.com/sdk/home" target="_blank">LunarG's website</a></li>
</ul>

<h1>Architecture Overview</h1>
<p>Silverlight follows a modular layered architecture for maintainability and scalability.</p>
<ul>
<li>Foundation Layer: Core utilities (logging, input system, ECS, timers, resource management)</li>
<li>Graphics Layer: Vulkan-based rendering, shaders, materials, lighting, model loading, and camera systems</li>
<li>Application Layer: The high-level entry point, connecting engine and components for execution</li>
</ul>

<h1>Why Silverlight Engine?</h1>
<p>This is a passion project I’ve been developing on and off in my free time, focusing on performance, flexibility, and future extensibility. I created it to learn more about complex engine architecture and modern low-level rendering with the Vulkan API. Silverlight can be used to make games and simulations or simply serve as a learning platform for developers exploring engine architecture and Vulkan-based rendering.</p>
