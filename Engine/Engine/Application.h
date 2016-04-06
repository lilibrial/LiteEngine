#pragma once

#include "Engine.h"

class Application
{
public:
	int Run();
private:
	bool init();
	
	std::unique_ptr<Engine> m_engine;

};