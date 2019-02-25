#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H


class NonCopyable
{
public:
    NonCopyable(const NonCopyable& ) = delete;
    NonCopyable& operator= (const NonCopyable &) = delete;
    NonCopyable() = default;

    int i = 10;
};

#endif // NONCOPYABLE_H
