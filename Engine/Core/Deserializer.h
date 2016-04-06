#pragma once

class Deserializer
{
protected:
	virtual ~Deserializer() = default;

	unsigned m_position = 0;
	unsigned m_size = 0;
};