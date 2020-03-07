#include "Widget.h"
#include <vector>
#include <string>
struct Widget::WidgetImp
{
	std::string name;
	std::vector<int> data;

};

Widget::Widget()
: _pimp(std::make_unique<WidgetImp>())
{
	_pimp->name = "first widget imp";
}

Widget::~Widget()
{}

const char * Widget::name() const
{
	return _pimp->name.c_str();
}


