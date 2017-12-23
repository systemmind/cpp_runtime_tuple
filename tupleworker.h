#ifndef __TUPLE_WORKER_H__
#define __TUPLE_WORKER_H__

template<typename Comp, typename Touch, typename Tuple>
struct TupleWorker
{
    TupleWorker(Comp comp, Touch touch, const Tuple& tuple)
        : _comp(comp)
        , _touch(touch)
        , _tuple(tuple)
    {}

    inline void iterate() throw(std::runtime_error)
    {
        visit<Tuple, std::tuple_size<Tuple>::value - 1>(_tuple);
    }

private:
    template<class T, std::size_t at>
    inline typename std::enable_if<at != -1, void>::type
        visit(T& tuple) throw(std::runtime_error)
    {
        auto item = std::get<at>(tuple);
        if (_comp(item))
        {
            _touch(item);
            return;
        }
        visit<T, at - 1>(tuple);
    }

    template<class T, std::size_t at>
    inline typename std::enable_if<at == -1, void>::type
    visit(T& tuple) throw(std::runtime_error) { throw std::runtime_error(std::string("Item not found in tuple")); }

private:

    Comp  _comp;
    Touch _touch;
    Tuple _tuple;
};

template<typename Comp, typename Touch, typename...Args>
void at_tuple(Comp comp, Touch touch, Args & ...args)
{
    auto tuple = std::make_tuple(args...);
    TupleWorker<Comp, Touch, decltype(tuple)>(comp, touch, tuple).iterate();
}

#endif