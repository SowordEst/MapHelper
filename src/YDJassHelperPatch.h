#pragma once
#include "stdafx.h"
#include <base\hook\inline.h>
class YDJassHelperPatch {
public:
	YDJassHelperPatch();

	//��������������
	YDJassHelperPatch(HANDLE process);

	~YDJassHelperPatch();

	//��������������
	void insert();


	//��jasshelper.exe ���õ�
	void attach();

	//��jasshelper.exe ���õ�
	void detach();

	uint32_t getAddress(uintptr_t addr);
	

	void onStart();

	void onEnd();
private:
	bool m_attach;

	hook::hook_t* m_hookCompileStart;
	hook::hook_t* m_hookCompileEnd;

	HANDLE m_process;

	std::vector<std::string> m_triggers;

	std::unordered_map<std::string, bool> m_func_map;
};