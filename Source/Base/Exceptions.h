#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class BaseException
{
    protected:
        char* buffer;
    public:
        virtual ~BaseException() = 0;
        const char* message() const;
        const char* trace() const;
};

class InitializationException : public BaseException
{
    public:
        InitializationException(const char* what);
};

class MissingFileException : public BaseException
{
    public:
        MissingFileException(const char* filename);
};

#endif // EXCEPTIONS_H