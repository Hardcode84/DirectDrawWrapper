#ifndef COM_PTR_HPP
#define COM_PTR_HPP

template<typename T>
class com_ptr
{
    T* px;

    typedef com_ptr this_type;
public:
    com_ptr(): px(nullptr) {}
    com_ptr(T* p, bool addRef = false):px(p)
    {
        if(nullptr != px && addRef) px->AddRef();
    }
    ~com_ptr()
    {
        if(nullptr != px) px->Release();
    }

    com_ptr(const this_type& rhs):px(rhs.get())
    {
        if(nullptr != px) px->AddRef();
    }

    com_ptr(this_type&& rhs):px(rhs.px)
    {
        rhs.px = nullptr;
    }

    this_type& operator=(const this_type& rhs)
    {
        if(nullptr != px) px->Release();
        px = rhs.get();
        if(nullptr != px) px->AddRef();
        return *this;
    }

    this_type& operator=(this_type&& rhs)
    {
        px = rhs.px;
        rhs.px = nullptr;
        return *this;
    }

    T& operator*() const
    {
        return *px;
    }

    T* operator->() const
    {
        return px;
    }

    const T** operator&() const
    {
        return &px;
    }

    T** operator&()
    {
        return &px;
    }

    T* get() const
    {
        return px;
    }

    void reset(T* p = 0, bool addRef = false)
    {
        if(nullptr != px) px->Release();
        px = p;
        if(nullptr != px && addRef) px->AddRef();
    }

    operator bool() const
    {
        return (nullptr != px);
    }

    bool operator==(const this_type& rhs) const
    {
        return rhs.px == px;
    }
    bool operator!=(const this_type& rhs) const
    {
        return rhs.px != px;
    }
    bool operator==(const T* rhs) const
    {
        return rhs == px;
    }
    bool operator!=(const T* rhs) const
    {
        return rhs != px;
    }
};

#endif // COM_PTR_HPP
