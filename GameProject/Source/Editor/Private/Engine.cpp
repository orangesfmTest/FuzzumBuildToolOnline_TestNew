// Engine.cpp — единый файл для всех платформ
#ifdef __ANDROID__
    #include <android/log.h>
    #include <pthread.h>
    #include <thread>        // ← добавь это
    #include <chrono>        // ← и это
#else
    #include <iostream>
    #include <thread>
    #include <chrono>
#endif



#ifdef __ANDROID__

    #define LOG_TAG "Engine"
    #define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
    #define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

    // Фоновая функция (если нужна длительная работа)
    static void* background_work(void* arg) {
		LOG_INFO("Background thread started");
		for (int i = 0; i < 5; ++i) {
			LOG_INFO("Working... step %d", i + 1);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		LOG_INFO("Background work finished");
		return nullptr;
	}

    // Автозапуск при загрузке .so
    __attribute__((constructor))
    static void on_library_loaded() {
        LOG_INFO("Engine loaded on Android");

        // Запускаем фон (опционально)
        pthread_t thread;
        if (pthread_create(&thread, nullptr, background_work, nullptr) == 0) {
            pthread_detach(thread);
            LOG_INFO("Background thread launched");
        } else {
            LOG_ERROR("Failed to create thread");
        }

        LOG_INFO("Engine initialization complete");
        // ← ВАЖНО: функция завершается, не блокируя поток!
    }

#else

    // Обычный main() для ПК
    int main() {
        std::cout << "[Engine] Starting on desktop...\n";
        
        // Имитация работы
        for (int i = 0; i < 5; ++i) {
            std::cout << "[Engine] Working... step " << (i + 1) << "\n";
        }
        
        std::cout << "[Engine] Done. Press Enter to exit.\n";
        std::cin.get(); // вместо system("pause") — кроссплатформенно
        return 0;
    }

#endif