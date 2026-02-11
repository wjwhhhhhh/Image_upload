#pragma once
namespace upload
{
    class Pipeline
    {
    public:
        Pipeline() = default;
        void start();
        void stop();
        ~Pipeline() = default;
    };
} // namespace upload
