#pragma once

class Engine
{
public:
	~Engine();

	bool Init();

	void BeginFrame();
	void Update();
	void BeginRender();
	void EndRender();
	void EndFrame();

	void Close();

	bool IsExit() const { return m_isExit; }

	void Exit() { m_isExit = true; }
private:
	Log m_log;

	bool m_isInit = false;
	bool m_isExit = false;
	
};