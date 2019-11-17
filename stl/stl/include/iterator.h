#pragma once

// iterator category
struct InputIterator {};
struct OutputIterator {};
struct ForwardIterator : InputIterator {};
struct BidirectionalIterator : ForwardIterator{};
struct RandomIterator : BidirectionalIterator {};

// iterator traits
template<class I>
struct IteratorTraits
{
	typedef I::Iterator::ValueType			ValueType;
	typedef I::Iterator::DifferenceType		DifferenceType;
	typedef I::Iterator::Pointer			Pointer;
	typedef I::Iterator::Reference			Reference;
	typedef I::Iterator::IteratorCategory	IteratorCategory;
};

template<class T>
struct IteratorTraits<T*>
{
	typedef T			ValueType;
	typedef ptrdiff_t	DifferenceType;
	typedef T*			Pointer;
	typedef T&			Reference;
	typedef 	IteratorCategory;
};
