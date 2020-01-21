#pragma once

namespace wind
{
	class CNonCopy
	{
	public:
		CNonCopy(const CNonCopy &) = delete;
		CNonCopy & operator=(const CNonCopy &) = delete;
	};
};// namespace wind
