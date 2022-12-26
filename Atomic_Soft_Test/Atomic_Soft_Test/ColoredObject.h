#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

namespace ColoredObjectStuff
{
	class ColoredObject
	{
	public:

		enum Priority
		{
			Low = 0,
			Medium = 1,
			High = 2
		};

		enum Color
		{
			Red = 0,
			Green = 1,
			Blue = 2
		};

		ColoredObject() = default;

		ColoredObject(const Color color, const Priority priority = Low)
			: m_color(color), m_priority(priority)
		{};

		~ColoredObject() = default;

		void setColor(const Color color)
		{
			m_color = color;
		}

		int getColor() const
		{
			return m_color;
		}

		void setPriority(const Priority priority)
		{
			m_priority = priority;
		}

		int getPriority() const
		{
			return m_priority;
		}

	private:
		Color m_color;
		Priority m_priority;
	};

	constexpr const char* colorToString(const int i) noexcept
	{
		switch (i)
		{
		case 0: return "R";
		case 1: return "G";
		case 2: return "B";
		}
	}

	void setPriorityForAll(std::vector<ColoredObject>& vec, const ColoredObject::Color lowColor,
		const ColoredObject::Color mediumColor,
		const ColoredObject::Color highColor);

	
}