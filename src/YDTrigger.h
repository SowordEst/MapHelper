#pragma once

#include "stdafx.h"
#include "TriggerEditor.h"
#include "include\EditorData.h"
#include "ActionNode.h"

//ר�Ÿ�������ydwe�Ĺ���

struct LocalVar
{
	std::string name;
	std::string type;
	std::string value;
};

class YDTrigger
{
public:
	YDTrigger();
	~YDTrigger();
	static YDTrigger* getInstance();

	//����ͷ�ļ�
	void onGlobals(BinaryWriter& writer);
	void onEndGlobals(BinaryWriter& writer);

	//ʵ��������� ����������
	bool onRegisterEvent(std::string& events,ActionNodePtr node);
	void onRegisterEvent2(std::string& events,ActionNodePtr node);

	void onRegisterTrigger(std::string& output, const std::string& trigger_name, const std::string& var_name);
	//ÿ����������ʱ
	bool onActionToJass(std::string& output,ActionNodePtr node, std::string& pre_actions, bool nested);

	//ÿ����������ʱ
	bool onParamterToJass(Parameter* parameter, ActionNodePtr node, std::string& actions, std::string& pre_actions, bool nested);

	//���������ɺ�����ʼʱ д��ֲ�����
	void onActionsToFuncBegin(std::string& funcCode, ActionNodePtr node);
	
	//���������ɺ�������ʱ ����ֲ�����
	void onActionsToFuncEnd(std::string& funcCode, ActionNodePtr node);

	//�жϴ����Ƿ����� true Ϊ����״̬
	bool hasDisableRegister(Trigger* trigger);

	bool isEnable();
private:
	std::string localVarTransfer(std::string& output, ActionNodePtr node, std::string& pre_actions);
	
	std::string setLocal(ActionNodePtr node, const std::string& name, const std::string& type, const std::string& value, bool add = false);
	std::string getLocal(ActionNodePtr node, const std::string& name, const std::string& type);

	std::string setLocalArray(ActionNodePtr node, const  std::string& name, const std::string& type, const std::string& index, const std::string& value);
	std::string getLocalArray(ActionNodePtr node, const std::string& name, const std::string& type, const std::string& index);

	bool seachHashLocal(Parameter** parameters, uint32_t count, std::map<std::string, std::string>* mapPtr = NULL);
protected: 
	bool m_bEnable;
	int m_enumUnitStack;
	bool m_hasAnyPlayer;

	int m_funcStack;
	std::map<Trigger*, bool> m_triggerHasDisable;


};