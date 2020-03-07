
#include <memory>
class Widget
{
public:
	Widget();

	// pimp with via unique_ptr
	// must declared dtor()
	// otherwise compiler will generate default dtor, which will call default dtor for unique_ptr, which implies calling delete WidgetImp.
	~Widget();
	
	const char * name() const;
private:
	struct WidgetImp;
	std::unique_ptr<WidgetImp> _pimp;
	
};
