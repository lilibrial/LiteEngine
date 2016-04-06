#include "stdafx.h"
#include "Application.h"

int Application::Run()
{
	try
	{
		if ( init() && m_engine->Init() )
		{
			while ( !m_engine->IsExit() )
			{
				m_engine->BeginFrame();
				m_engine->Update();
				m_engine->BeginRender();
				m_engine->EndRender();
				m_engine->EndFrame();
			}
			m_engine->Close();
		}

		return 0;
	}
	catch (std::bad_alloc&)
	{
		return EXIT_FAILURE;
	}
}

bool Application::init()
{
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	m_engine = std::make_unique<Engine>();
	
	return true;
}