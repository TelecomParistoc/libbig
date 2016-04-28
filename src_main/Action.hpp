#include <functional>


class Action
{
    public:
        Action(const std::function<void()>& start, const std::function<void()>& pause, const std::function<void()>& cont, const std::function<bool()>& isFinished);
        //provide C binding
        Action(void (*start)(void), void (*pause)(void), void (*cont)(void), int (*isFinished)(void));

        void start();
        void pauseAction();
        void continueAction();
        bool isFinished() const;

        static std::function<bool()> convertCtoCpp(int (*isFinished)(void));
        static bool convertIntCtoBool(const std::function<int()>& f);

    private:
        std::function<void()> startFunction;
        std::function<void()> pauseFunction;
        std::function<void()> continueFunction;
        std::function<bool()> isFinishedFunction;
};
