#include "ColoredObject.h"

void ColoredObjectStuff::setPriorityForAll(std::vector<ColoredObject>& vec, const ColoredObject::Color lowColor, 
	const ColoredObject::Color mediumColor,
	const ColoredObject::Color highColor)
{
	auto it = vec.begin();
	for (; it < vec.end(); ++it)
	{
		if (it->getColor() == lowColor)
		{
			it->setPriority(ColoredObject::Low);
		}
		else if (it->getColor() == mediumColor)
		{
			it->setPriority(ColoredObject::Medium);
		}
		else
		{
			it->setPriority(ColoredObject::High);
		}
	}
}