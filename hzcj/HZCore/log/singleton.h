#ifndef singleton_h
#define singleton_h

#include <mutex>

using std::mutex;

namespace singleton
{
template <typename T>
class Singleton
{
public:
    static T& Instance()
    {
        if (NULL == mInstance)
        {
            mMutex.lock();
            if (NULL == mInstance) {
                mInstance = new T();
                atexit(Destroy);
            }
            mMutex.unlock();
            return *mInstance;
        }
        return *mInstance;
    }

protected:
    Singleton(void) {}
    virtual ~Singleton(void) {}

private:
    Singleton(const Singleton& rhs) {}

    static void Destroy()
    {
        if (mInstance == NULL) { return; }

        delete mInstance;
        mInstance = NULL;
    }

    static T* volatile mInstance;
    static mutex mMutex;
};

template <typename T> T* volatile Singleton<T>::mInstance = NULL;
template <typename T> mutex Singleton<T>::mMutex;
}

#endif //singleton_h
