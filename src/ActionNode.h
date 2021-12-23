#pragma once
#include "stdafx.h"
#include "..\include\EditorData.h"


//�����ڵ�

//ÿ��������Ϊһ�� root ���ڵ� ���ڵ�� action �� parent ΪNULL 
//ÿ������ �� �ڵ�Ϊ ����,���ڵ�  ���ڵ�Ϊ��һ��֧�� ��if then else  ��Ϊroot�ڵ�
//ÿ�������Ľڵ�Ϊ action ��ǰ�����ĺ������ö���,���ڵ�Ϊ���ڵĶ���

typedef std::shared_ptr<class ActionNode> ActionNodePtr;

typedef std::shared_ptr<std::map<std::string, std::string>> VarTablePtr;

class ActionNode :public std::enable_shared_from_this<ActionNode>
{
public:

	//�ڵ�����
	enum class Type
	{
		trigger,
		action,
		parameter,
	};

	ActionNode();
	ActionNode(Trigger* root);
	ActionNode(Action* action, ActionNodePtr parent);
	ActionNode(Action* action, Parameter* owner, ActionNodePtr parent);

	//��ȡ�ڵ�Ķ���
	Action* getAction();

	//��ȡ�ڵ�����������
	Trigger* getTrigger();

	std::shared_ptr<std::string> getTriggerNamePtr();

	void setFunctionNamePtr(std::string func_name);
	std::shared_ptr<std::string> getFunctionNamePtr();

	//��ȡ������
	std::string getName() const;

	//��ȡ������id �ַ�����ϣֵ
	uint32_t getNameId() const;

	//��ȡ���������Ӷ���id
	uint32_t getActionId();

	//�жϸýڵ��Ƿ��Ǹ��ڵ�
	bool isRootNode();

	//��ȡ���ڵ�
	ActionNodePtr getParentNode();

	//��ȡ���ڵ�
	ActionNodePtr getRootNode();

	//��ȡ֧�� action Ϊ֧���µĵ�һ������  ����ֵ->getParentNode()���Ի��ʵ�ʷ�֧�Ľڵ�
	ActionNodePtr getBranchNode();

	//��ȡ�Ӷ�������
	size_t size();

	//node[1] ȡ���Ӷ���
	ActionNodePtr operator[](size_t n);

	//��ȡȫ���Ӷ����ڵ�
	void getChildNodeList(std::vector<ActionNodePtr>& list);
	


	//��ȡ��������
	size_t count() const;
	
	// node(1) ȡ�ö����Ĳ���
	Parameter* operator()(size_t n) const;


	Action::Type getActionType();

	VarTablePtr getVarTable();

	VarTablePtr getLastVarTable();

	VarTablePtr getLocalTable();

	int getParentGroupCount();

	bool canHasVarTable();

	bool m_haveHashLocal;

	//�ýڵ��ڴ������еķ������¼�/����/����
	Action::Type rootType;
	
private:

protected:
	ActionNodePtr m_parent;//���ڵ�

	uint32_t m_nameId;//�������Ĺ�ϣֵid

	Action* m_action;//��ǰ����

	Parameter* m_parameter;//�������� ֻ�нڵ�����Ϊ����ʱ����Ч

	Trigger* m_trigger;//����������

	std::shared_ptr<std::string> m_trigger_name;

	std::shared_ptr<std::string> m_function_name;

	Type m_type;

	//������¼���������ʱ���ľֲ����� �Ա�����һ�㺯��������
	std::shared_ptr<std::map<std::string, std::string>> m_hashVarTablePtr;

	//��¼�ֲ�������mapָ��
	std::shared_ptr<std::map<std::string, std::string>> m_localTablePtr;

	int m_group_count;

};


Action::Type get_action_type(Action* action);
