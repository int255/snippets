
#include <memory>
class Widget
{
public:
	Widget();
	~Widget();
	
	const char * name() const;
private:
	struct WidgetImp;
	std::unique_ptr<WidgetImp> _pimp;
	//std::shared_ptr<WidgetImp> _pimp;
	
};
