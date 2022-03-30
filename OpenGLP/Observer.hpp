#pragma once
#include "Event.hpp"
class Observer
{
public:
	virtual ~Observer() = default;
	virtual void handle(const Event& event) = 0;
};

