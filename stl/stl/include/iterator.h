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
	typedef typename I::Iterator::ValueType			ValueType;
	typedef typename I::Iterator::DifferenceType		DifferenceType;
	typedef typename I::Iterator::Pointer			Pointer;
	typedef typename I::Iterator::Reference			Reference;
	typedef typename I::Iterator::IteratorCategory	IteratorCategory;
};

template<class T>
struct IteratorTraits<T*>
{
	typedef T				ValueType;
	typedef ptrdiff_t		DifferenceType;
	typedef T*				Pointer;
	typedef T&				Reference;
	typedef RandomIterator	IteratorCategory;
};

template<class T>
struct IteratorTraits<const T*>
{
	typedef T				ValueType;
	typedef ptrdiff_t		DifferenceType;
	typedef const T*		Poniter;
	typedef const T&		Reference;
	typedef RandomIterator	IteratorCategory;
};
