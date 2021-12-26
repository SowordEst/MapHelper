#pragma once
#include "..\stdafx.h"
#include <memory>
#include <functional>


int __fastcall fakeGetChildCount(Action* action);
Action::Type get_action_type(Action* action);

#include "Function.hpp"

namespace mh {

	class Node;

	typedef std::shared_ptr<std::string> StringPtr;

	typedef std::shared_ptr<Node> NodePtr;

	typedef std::function<bool(NodePtr node)> NodeFilter;


	class Node :public std::enable_shared_from_this<Node> {
	public:
		enum class TYPE :int {
			ROOT, //���ڵ�
			CALL, //����Ҫ����ֵ
			GET,  //��Ҫ����ֵ
			PARAM, //��һ��������
			CLOSURE, //һ������հ�
		};

		//��ȡ��ǰ������
		virtual void* getData() = 0;

		//��ȡ��ǰ�ڵ�����
		virtual StringPtr getName() = 0;

		//��ǰ����id
		virtual uint32_t getNameId() = 0;

		//�ڵ�����
		virtual TYPE getType() = 0;

		virtual void setType(TYPE type) = 0;

		//��ȡ���ڵ�
		virtual NodePtr getParentNode() = 0;

		//��ȡ���ڵ�
		virtual NodePtr getRootNode() = 0;

		//��ȡ�Ӷ����б�
		virtual std::vector<NodePtr> getChildList() = 0;

		//��ȡֵ
		virtual bool getValue(const NodeFilter& filter) = 0;

		//��ȡ�ı�
		virtual std::string toString(TriggerFunction* func = nullptr) = 0;

		//���ɺ����� 
		virtual std::string getFuncName() = 0;

		//����������
		virtual const std::string& getTriggerVariableName() = 0;

		//����ֲ���������
		enum class UPVALUE_TYPE :int {
			SET_LOCAL,
			GET_LOCAL,
			SET_ARRAY,
			GET_ARRAY,
		};

		//����ֲ������ĺ����� �ڲ�ͬ�ڵ���ʹ�ò�ͬ
		virtual std::string getUpvalueScriptName(UPVALUE_TYPE type) = 0;

		//��ȡ����handle
		virtual std::string getHandleName() = 0;
	};


	NodePtr NodeFromTrigger(Trigger* trigger);

	NodePtr NodeFromAction(Action* action, uint32_t childId, NodePtr parent);

	NodePtr NodeFramParameter(Parameter* parameter, uint32_t index, NodePtr parent);


	typedef NodePtr(*MakeNode)(void* action, uint32_t childId, NodePtr parent);


	extern bool g_YDTrigger;

	extern std::unordered_map<Trigger*, bool> g_initTriggerMap;

	extern std::unordered_map<Trigger*, bool> g_disableTriggerMap;

	extern std::unordered_map<std::string, MakeNode> MakeActionNodeMap;
	extern std::unordered_map<std::string, MakeNode> MakeParameterNodeMap;

}

