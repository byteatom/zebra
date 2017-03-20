#ifndef UTIL_H
#define UTIL_H

namespace Util
{

struct Guard {
	typedef std::function<void()> D;
	Guard(const D& d) : d{d} {}
	~Guard() { if (d) d(); }
	void release(){if (d) {d();d = nullptr;}}
	void dismess(){d = nullptr;}
	D d;
};

template <class Box>
void raiseElement(Box& box, typename const Box::value_type& value)
{
	if (box.size() < 2) return;
	Box::iterator it = box.begin();
	while (it != box.end()) {
		if (*it == value) break;
		++it;
	}
	if (it == box.begin()) return;
	Box::iterator itPre = it;
	--itPre;
	if (itPre == box.end()) return;
	std::swap(*itPre, *it);
}

template <class Box>
void lowerElement(Box& box, typename const Box::value_type& value)
{
	if (box.size() < 2) return;
	Box::iterator it = box.begin();
	while (it != box.end()) {
		if (*it == value) break;
		++it;
	}
	if (it == box.end()) return;
	Box::iterator itNext = it;
	++itNext;
	if (itNext == box.end()) return;
	std::swap(*it, *itNext);
}

}

#endif // UTIL_H
