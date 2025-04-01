#pragma once

#include <memory>

namespace Silverlight 
{
    using ApplicationPtr = std::unique_ptr<Application>;
}

extern Silverlight::ApplicationPtr CreateApplication();

int main()
{
    const Silverlight::ApplicationPtr& app = CreateApplication();
    app->PostClientInitialize();
    app->Run();
}