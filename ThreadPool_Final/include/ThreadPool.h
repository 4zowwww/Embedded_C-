#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <vector>

class ThreadPool {
    private:
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> tasks;
        std::mutex mutex;
        std::condition_variable condition;
        bool stop = false;

    public:
        explicit ThreadPool(std::size_t threads);

        template<typename F>
        auto submit(F&& task) -> std::future<std::invoke_result_t<F>> {

            using detected_t = std::invoke_result_t<F>;

            auto TaskPtr = std::make_shared<std::packaged_task<detected_t()>>(std::forward<F>(task));

            std::future<detected_t> result = (*TaskPtr).get_future();

            {
                std::lock_guard<std::mutex> lock(mutex);

                if (stop)
                {
                    throw std::runtime_error("ThreadPool is stopped");
                }

                tasks.push([TaskPtr]
                    {
                        (*TaskPtr)();
                    });
            }

            condition.notify_one();

            return result;
        }

        ~ThreadPool();
};
