#pragma once
#include "stdafx.h"
#include "inline.h"
#include "EditorData.h"
#include "WorldEditor.h"

template<typename dst_type, typename src_type>
dst_type union_cast(src_type src)
{
	union {
		src_type s;
		dst_type d;
	}u;
	u.s = src;
	return u.d;
}


class Helper
{
public:
	Helper();
	~Helper();
	
	//static Helper* getInstance();


	static bool IsEixt();

	void enableConsole();

	void attach();//����

	void detach();//����

	int getConfig() const;
private:

	//�����ͼ
	static uintptr_t onSaveMap();

	//ѡ��ת��ģʽ
	int onSelectConvartMode();

	//���Զ���ת������ʱ
	int onConvertTrigger(Trigger* trg);


protected:
	bool m_bAttach;

	//�Զ���jass��������hook
	hook::hook_t* m_hookSaveMap;
	hook::hook_t* m_hookConvertTrigger;

	//��̬�������� �������͵�hook
	hook::hook_t* m_hookCreateUI;
	hook::hook_t* m_hookReturnTypeStrcmp;

	//�Զ��嶯�����hook
	hook::hook_t* m_hookGetChildCount;
	hook::hook_t* m_hookGetString;
	hook::hook_t* m_hookGetActionType;
	
	//�ǶȻ��Ȼ�����hook����
	hook::hook_t* m_hookParamTypeStrncmp1;
	hook::hook_t* m_hookParamTypeStrncmp2;
	hook::hook_t* m_hookGetParamType;


	fs::path m_configPath;
};
extern Helper g_CHelper;
Helper& get_helper();
