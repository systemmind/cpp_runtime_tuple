#ifndef __TUPLE_WORKER_H__
#define __TUPLE_WORKER_H__

#include <tuple>

template<class Touch, class End, class Tuple>
struct TupleWorker
{
    TupleWorker(Touch& tch, End& end, Tuple& tuple)
        : _touch(tch)
        , _end(end)
        , _tuple(tuple)        
    {}
    
    inline void each()
    {
        visit<Tuple, std::tuple_size<Tuple>::value - 1>(_tuple);
    }

    template<class T, std::size_t at>
    inline typename std::enable_if<at != -1, void>::type
        visit(T& tuple)
    {
        auto& item = std::get<at>(tuple);
        auto next = _touch(item);
        if (!next)
          return;
        
        visit<T, at - 1>(tuple);
    }

    template<class T, std::size_t at>
    inline typename std::enable_if<at == -1, void>::type
    visit(T& tuple){ _end(tuple); }
    
private:
    Touch& _touch;
    End&   _end;
    Tuple& _tuple;    
};

template<typename ...Args>
struct Each
{
  template<typename Touch, typename End>
  static inline each(Touch touch, End end, Args& ...args)
  {
    auto tuple = std::make_tuple(args...);
    Each<decltype(tuple)>::each(touch, end, tuple);
  }
};

template<typename... Args>
struct Each<std::tuple<Args...> >
{
  typedef std::tuple<Args...> TL;
  template<typename Touch, typename End>
  inline static each(Touch& touch, End& end, TL& tuple)
  {
    typedef TupleWorker<Touch, End, TL> Worker;
    Worker(touch, end, tuple).each();
  }
};

template<typename... Args>
struct Each<std::tuple<Args...>& >
{
  typedef std::tuple<Args...> TL;
  template<typename Touch, typename End>
  inline static each(Touch& touch, End& end, TL& tuple)
  {
    Each<TL>::each(touch, end, tuple);
  }
};

template<typename Touch, typename End, typename...Args>
inline void tuple_each(Touch touch, End end, Args&& ...args)
{
  Each<Args...>::each(touch, end, args...);
}

template<typename Comp, typename Touch, typename...Args>
void tuple_find(Comp cmp, Touch touch, const Args&& ...args)
{
    auto tuple = std::make_tuple(args...);
    auto end = [](auto& tuple)
    {
      throw std::runtime_error("not found");
    };
    
    auto each = [cmp, touch](auto& item) -> auto
    {
      auto found = false;
      if (found = cmp(item))
      {
        touch(item);
      }
      
      return !found;
    };
    
    tuple_each(each, end, tuple);
}

#endif