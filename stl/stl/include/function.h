#pragma once
#include "log.h"

#include <functional>
#include <iostream>

template<class T>
class TJudge : public std::unary_function<T, bool>
{
public:
	TJudge() {}

	bool operator()(T t)
	{
		bool bRet = t.Val();
		LOG_DEBUG << "TJudge::operator() val : " << bRet;
		return bRet;
	}
};

template<class T>
class TPrint : public std::unary_function<T, void>
{
public:
	TPrint() {}

	void operator()(T t)
	{
		t.Print();
	}
};

// adapter for Unary function(one param and return bool)
template<class Unary>
class TUnaryNegate : public std::unary_function<typename Unary::argument_type, bool>
{
protected:
	Unary m_oOp;
public:
	explicit TUnaryNegate(const Unary& oTmp) : m_oOp(oTmp) {}
	
	bool operator()(const typename Unary::argument_type& t)
	{
		LOG_DEBUG << "TUnaryNegate::operator()...";
		return !m_oOp(t);
	}
};

// function for get Unary Negate adapter
template<class Unary>
TUnaryNegate<Unary> UnaryNegate(const Unary& tmp)
{
	return TUnaryNegate<Unary>(tmp);
}


template<class T>
class TCompare : public std::binary_function<T, T, bool>
{
public:
	TCompare() {};
	bool operator()(const T& t1, const T& t2)
	{
		LOG_DEBUG << "TCompare::operator()...";
		return t1 < t2;
	}
};

template<class Binary>
class TBinaryNegate : public std::binary_function<typename Binary::first_argument_type, typename Binary::second_argument_type, bool>
{
protected:
	Binary m_oOp;
public:
	TBinaryNegate(const Binary& oTmp) : m_oOp(oTmp) {}

	bool operator()(const typename Binary::first_argument_type & oP1, const typename Binary::second_argument_type& oP2)
	{
		LOG_DEBUG << "TBinaryNegate::operator()...";
		return !m_oOp(oP1, oP2);
	}
};

template<class Binary>
TBinaryNegate<Binary> BinaryNegate(const Binary& tmp)
{
	return TBinaryNegate<Binary>(tmp);
}

template<class T>
class TAdd : public std::binary_function<T, T, T>
{
public:
	TAdd() {};

	T operator()(const T& t1, const T& t2)
	{
		return t1 + t2;
	}
};

template<class Binary>
class TBind1st : public std::binary_function<typename Binary::first_argument_type,
	typename Binary::second_argument_type, typename Binary::result_type>
{
protected:
	Binary m_oOp;
private:
	typename Binary::first_argument_type m_oP1;

public:
	TBind1st(const Binary& oOp, const typename Binary::first_argument_type& oP1) : m_oOp(oOp), m_oP1(oP1) {}

	typename Binary::result_type operator()(const typename Binary::second_argument_type& oP2)
	{
		return m_oOp(m_oP1, oP2);
	}
};

template<class Binary>
TBind1st<Binary> Bind1st(const Binary& oOp, const typename Binary::first_argument_type& oP1)
{
	return TBind1st<Binary>(oOp, oP1);
}

template<class T, class R>
class TMemFunc : public std::unary_function<T, R>
{
protected:
	R (T::*m_fFunc)();
public:
	explicit TMemFunc(R (T::*f)()) : m_fFunc(f) {}
	R operator()(T* pObj) const
	{
		return (pObj->*m_fFunc)();
	}
};
