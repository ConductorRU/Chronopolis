#pragma once
//
//  онтейнер дл€ хранени€ указател€ на метод.
class IArgs { public: virtual ~IArgs() {} };
template<typename... Args>
class Argums: public IArgs
{
public:
	std::tuple<Args...> arg;
	Argums(Args... args)
	{
		arg = make_tuple(args...);
	}
};
class ICont
{
public:
	virtual void Call(char *v, IArgs* i_args) = 0;
};

namespace detail
{
	template <typename C, typename M, typename Tuple, bool Done, int Total, int... N>
	struct call_impl
	{
		static void call(C c, M m, Tuple && t)
		{
			call_impl<C, M, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(c, m, std::forward<Tuple>(t));
		}
	};

	template <typename C, typename M, typename Tuple, int Total, int... N>
	struct call_impl<C, M, Tuple, true, Total, N...>
	{
		static void call(C c, M m, Tuple && t)
		{
			(c->*m)(std::get<N>(std::forward<Tuple>(t))...);
		}
	};
}

// user invokes this
template <typename C, typename M, typename Tuple>
void call(C c, M m, Tuple && t)
{
	typedef typename std::decay<Tuple>::type ttype;
	detail::call_impl<C, M, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value>::call(c, m, std::forward<Tuple>(t));
}

template<class...>struct types { using type = types; };
template<class Sig> struct args;
template<class R, class...Args>
struct args<R(Args...)>:types<Args...> {};
template<class Sig> using args_t = typename args<Sig>::type;


template<typename C, typename M, typename... Args>
class ContA: public ICont
{
private:
	M m_method;
	typedef Argums<Args...> A;
	//	typename result_of<decltype(m_method)(C)>::type val;
public:
	ContA(M m): m_method(m) {}
	void Call(char *v, IArgs* i_args)
	{
		//decltype(m_method);
		A* a = dynamic_cast<A*>(i_args);
		assert(a);
		if(a) call((C*)v, m_method, a->arg);
	}
};

class Delegate
{
	ICont* mCont = nullptr;
public:
	template<class C, typename... Args, class M> void Connect(M method)
	{
		if(mCont) delete mCont;
		mCont = new ContA<C, M, Args...>(method);
	}
	template<class M> void Connect(M method)
	{
		//if(mCont) delete mCont;
		//mCont = new ContV<M>(method);
	}
	/*template<typename... Args> void Call(Args... args)
	{
	//mCont->Call(args...);
	}*/
	template<typename... Args> void Call(void *v, Args... args)
	{
		mCont->Call((char*)v, &Argums<Args...>(args...));
	}
};