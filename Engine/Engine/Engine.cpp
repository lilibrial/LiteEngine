#include "stdafx.h"
#include "Engine.h"

Engine::~Engine()
{
	Close();
}

bool Engine::Init()
{
	Thread::SetMainThread(); // TODO: возможно убрать

	m_log.Open("log.txt");

	m_isInit = true;
	return true;
}

void Engine::BeginFrame()
{

}

void Engine::Update()
{

}

void Engine::BeginRender()
{

}

void Engine::EndRender()
{
	
}

void Engine::EndFrame()
{
	m_log.handleEndFrame();
}

void Engine::Close()
{
	if ( !m_isInit ) return;


	m_log.Close();
	m_isInit = false;
}