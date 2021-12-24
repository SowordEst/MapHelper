#pragma once
#include "stdafx.h"
#include <memory>
#include <functional>

namespace mh {
	class Node;

	typedef std::shared_ptr<std::string> StringPtr;

	typedef std::shared_ptr<Node> NodePtr;

	typedef std::vector<std::string> Codes;

	typedef std::function<bool(NodePtr node)> NodeFilter;

	class Node :public std::enable_shared_from_this<Node> {
	public:
		enum class TYPE :int {
			ROOT, //���ڵ�
			CALL, //����Ҫ����ֵ
			GET,  //��Ҫ����ֵ
			PARAM //��һ��������
		};

		//��ȡ��ǰ������
		virtual void* getData() = 0;

		//��ȡ��ǰ�ڵ�����
		virtual StringPtr getName() = 0;

		//��ǰ����id
		virtual uint32_t getNameId() = 0;

		//�ڵ�����
		virtual TYPE getType() = 0;

		//��ȡ���ڵ�
		virtual NodePtr getParentNode() = 0;

		//��ȡ���ڵ�
		virtual NodePtr getRootNode() = 0;

		//��ȡ�Ӷ����б�
		virtual std::vector<NodePtr> getChildList() = 0;

		//��ȡֵ
		virtual bool getValue(Codes& result, const NodeFilter& filter) = 0;

		//��ȡ�ı�
		virtual std::string toString(std::string& pre_actions) = 0;
	};

	NodePtr NodeFromTrigger(Trigger* trigger);

	NodePtr NodeFromAction(Action* action, uint32_t childId, NodePtr parent);

	NodePtr NodeFramParameter(Parameter* parameter, uint32_t index, NodePtr parent);

	std::string CodeConnent(Codes& codes);

	typedef NodePtr(*MakeNode)(void* action, uint32_t childId, NodePtr parent);


	void SpaceAdd();
	void SpaceRemove();
	std::string& Spaces(int offset = 0);

}