#pragma once


namespace mh {


	typedef std::shared_ptr<class SigleNodeClosure> SigleNodeClosurePtr;
	//�հ�
	class SigleNodeClosure : public ClosureNode {
		REGISTER_FROM_CLOSUER(SigleNodeClosure)
	public:

		SigleNodeClosure(NodePtr node, const std::string& return_type, NodePtr parent)
			:SigleNodeClosure((Action*)node->getData(), 0, node)
		{
			m_node = node;
			m_return_type = return_type;
		}


		virtual std::string toString(TriggerFunction* func) override {

			auto& editor = get_trigger_editor();
			// script_name
			std::string name = editor.getScriptName(m_action);
			std::string func_name = getParentNode()->getFuncName();

			std::string result;

			NodePtr node = shared_from_this();

			//Ϊ��ǰ�հ�����һ������
			NodePtr fake = NodeFromAction(m_action, 0, node);

			//���������뵽������ ���غ�����
			result = "function " + func_name;
			Function* code = new Function(func_name, m_return_type);
			func->push(code);
			if (m_return_type == "nothing") {
				*func << fake->toString(func);
			} else {
				func->current()->nextIsRetn();
				*func << func->getSpaces() + "return " + fake->toString(func) + "\n";
			}
			func->pop();
			func->addFunction(code);
			return result;
		}

		virtual std::string getUpvalue(const Upvalue& info) override {
			std::string result;

			switch (info.uptype) {
			case Upvalue::TYPE::SET_LOCAL:
				result = std::format("YDLocal2Set({}, \"{}\", {})", info.type, info.name, info.value);
				break;
			case Upvalue::TYPE::GET_LOCAL:
				result = std::format("YDLocal2Get({}, \"{}\")", info.type, info.name);
				break;
			case Upvalue::TYPE::SET_ARRAY:
				result = std::format("YDLocal2ArraySet({}, \"{}\", {}, {})", info.type, info.name, info.index, info.value);
				break;
			case Upvalue::TYPE::GET_ARRAY:
				result = std::format("YDLocal2ArrayGet({}, \"{}\", {})", info.type, info.name, info.index);
				break;
			default:
				break;
			}
			return result;
		}

	private:
		NodePtr m_node;
		std::string m_return_type;
	};


	class ForForceMultiple : public ClosureNode {
	public:
		REGISTER_FROM_CLOSUER(ForForceMultiple)

		virtual std::string toString(TriggerFunction* func) override {

			auto& editor = get_trigger_editor();
			// script_name
			std::string name = editor.getScriptName(m_action);
			std::string func_name = getFuncName() + "A";

			std::string result = func->getSpaces() + "call " + name + "(";


			params_finish = false;

			for (auto& param : getParameterList()) {
				result += " " + param->toString(func);
				result += ",";
			}
			result += "function " + func_name + ")\n";

			Function* code = new Function(func_name, "nothing");
			func->push(code);

			params_finish = true;

			for (auto& node : getChildList()) {
				*func << node->toString(func);
			}
			func->pop();
			func->addFunction(code);
			return result;
		}

		virtual std::string getUpvalue(const Upvalue& info) override {

			if (!params_finish) { //����ǲ�����Ķ��� �������Ƿ�����һ��
				return getParentNode()->getUpvalue(info);
			}

			std::string result;
			

			switch (info.uptype) {
			case Upvalue::TYPE::SET_LOCAL:
				result = std::format("YDLocal2Set({}, \"{}\", {})", info.type, info.name, info.value);
				break;
			case Upvalue::TYPE::GET_LOCAL:
				result = std::format("YDLocal2Get({}, \"{}\")", info.type, info.name);
				break;
			case Upvalue::TYPE::SET_ARRAY:
				result = std::format("YDLocal2ArraySet({}, \"{}\", {}, {})", info.type, info.name, info.index, info.value);
				break;
			case Upvalue::TYPE::GET_ARRAY:
				result = std::format("YDLocal2ArrayGet({}, \"{}\", {})", info.type, info.name, info.index);
				break;
			default:
				break;
			}
			return result;
		}

	};


	class YDWEExecuteTriggerMultiple : public ClosureNode {
	public:
		REGISTER_FROM_CLOSUER(YDWEExecuteTriggerMultiple)

		virtual std::string toString(TriggerFunction* func) override {

			auto params = getParameterList();

			//���Ӿֲ�����
			func->current()->addLocal("ydl_trigger", "trigger", std::string(), false);

			std::string result;

			params_finish = false; 
			result += func->getSpaces() + "set ydl_trigger = " + params[0]->toString(func) + "\n";
			result += "YDLocalExecuteTrigger(ydl_trigger)\n";

			params_finish = true;
			for (auto& node : getChildList()) {
				result += node->toString(func);
			}
			result += func->getSpaces() + "call YDTriggerExecuteTrigger(ydl_trigger, " + params[1]->toString(func) + ")\n";

			return result;
		}


		virtual std::string getUpvalue(const Upvalue& info) override {
			if (!params_finish) { //����ǲ�����Ķ��� �������Ƿ�����һ��
				return getParentNode()->getUpvalue(info);
			}

			std::string result;

			switch (info.uptype) {
			case Upvalue::TYPE::SET_LOCAL:
				result = std::format("YDLocal5Set({}, \"{}\", {})", info.type, info.name, info.value);
				break;
			case Upvalue::TYPE::GET_LOCAL:
				result = getParentNode()->getUpvalue(info);
				break;
			case Upvalue::TYPE::SET_ARRAY:
				result = std::format("YDLocal5ArraySet({}, \"{}\", {}, {})", info.type, info.name, info.index, info.value);
				break;
			case Upvalue::TYPE::GET_ARRAY:
				result = getParentNode()->getUpvalue(info);
				break;
			default:
				break;
			}
			return result;
		}
	};


	class YDWETimerStartMultiple : public ClosureNode {
	public:
		REGISTER_FROM_CLOSUER(YDWETimerStartMultiple)

		virtual uint32_t getCrossDomainIndex() override { return 0; }

		virtual bool isCrossDomain() override { return true; }

		std::string getHandleName() {
			if (getCurrentGroupId() <= getCrossDomainIndex()) {
				return "ydl_timer";
			}
			return "GetExpiredTimer()";
		}

		virtual std::string getUpvalue(const Upvalue& info) override {
			if (!params_finish) { //����ǲ�����Ķ��� �������Ƿ�����һ��
				return getParentNode()->getUpvalue(info);
			}

			std::string result;

			bool is_get = info.uptype == Upvalue::TYPE::GET_LOCAL || info.uptype == Upvalue::TYPE::GET_ARRAY;
			if (getCrossDomainIndex() == getCurrentGroupId() && is_get) {	//�����ǰ�Ǵ����� ��ʹ����һ��ľֲ�����
				result = getParentNode()->getUpvalue(info);
				return result;
			}

			switch (info.uptype) {
			case Upvalue::TYPE::SET_LOCAL:
				result = std::format("YDLocalSet({}, {}, \"{}\", {})", getHandleName(), info.type, info.name, info.value);
				break;
			case Upvalue::TYPE::GET_LOCAL:
				result = std::format("YDLocalGet({}, {}, \"{}\")", getHandleName(), info.type, info.name);
				break;
			case Upvalue::TYPE::SET_ARRAY:
				result = std::format("YDLocalArraySet({}, {}, \"{}\", {}, {})", getHandleName(), info.type, info.name, info.index, info.value);
				break;
			case Upvalue::TYPE::GET_ARRAY:
				result = std::format("YDLocalArrayGet({}, {}, \"{}\", {})", getHandleName(), info.type, info.name, info.index);
				break;
			default:
				break;
			}
			return result;
		}

		virtual std::string toString(TriggerFunction* func) override {

			auto params = getParameterList();

			//���Ӿֲ�����
			func->current()->addLocal("ydl_timer", "timer", std::string(), false);

			std::string result;
			std::string save_state;

			std::string func_name = getFuncName() + "T";
			params_finish = false; 

			result += func->getSpaces() + "set ydl_timer = " + params[0]->toString(func) + "\n";

			params_finish = true;

			Function* closure = getBlock(func, func_name, save_state);
			result += save_state;
			result += func->getSpaces() + "call TimerStart(ydl_timer, "
				+ params[1]->toString(func) + ", "
				+ params[2]->toString(func)
				+ ", function " + func_name + ")\n";

		

			func->addFunction(closure);

			return result;
		}


	};

	class YDWERegisterTriggerMultiple : public ClosureNode {
	public:
		REGISTER_FROM_CLOSUER(YDWERegisterTriggerMultiple)

		virtual uint32_t getCrossDomainIndex() override { return 1; }

		virtual bool isCrossDomain() override { return false; }

		std::string getHandleName() {
			if (getCurrentGroupId() <= getCrossDomainIndex()) {
				return "ydl_trigger";
			}
			return "GetTriggeringTrigger()";
		}

		virtual std::string getUpvalue(const Upvalue& info) override {
			std::string result;

			bool is_get = info.uptype == Upvalue::TYPE::GET_LOCAL || info.uptype == Upvalue::TYPE::GET_ARRAY;

			if ((getCrossDomainIndex() == getCurrentGroupId() && is_get) || !params_finish) {	//�����ǰ�Ǵ����� ��ʹ����һ��ľֲ�����
				result = getParentNode()->getUpvalue(info);
				return result;
			}

			switch (info.uptype) {
			case Upvalue::TYPE::SET_LOCAL:
				result = std::format("YDLocalSet({}, {}, \"{}\", {})", getHandleName(), info.type, info.name, info.value);
				break;
			case Upvalue::TYPE::GET_LOCAL:
				result = std::format("YDLocalGet({}, {}, \"{}\")", getHandleName(), info.type, info.name);
				break;
			case Upvalue::TYPE::SET_ARRAY:
				result = std::format("YDLocalArraySet({}, {}, \"{}\", {}, {})", getHandleName(), info.type, info.name, info.index, info.value);
				break;
			case Upvalue::TYPE::GET_ARRAY:
				result = std::format("YDLocalArrayGet({}, {}, \"{}\", {})", getHandleName(), info.type, info.name, info.index);
				break;
			default:
				break;
			}
			return result;
		}

		virtual std::string toString(TriggerFunction* func) override {

			auto params = getParameterList();

			//���Ӿֲ�����
			func->current()->addLocal("ydl_trigger", "trigger", std::string(), false);

			std::string result;
			std::string save_state;

			std::string func_name = getFuncName() + "Conditions";
	
			params_finish = false;

			result += func->getSpaces() + "set ydl_trigger = " + params[0]->toString(func) + "\n";

			params_finish = true;

			Function* closure = getBlock(func, func_name, save_state);
			result += save_state;
			result += func->getSpaces() + "call TriggerAddCondition( ydl_trigger, Condition(function " + func_name + "))\n";

			

			func->addFunction(closure);

			return result;
		}

	};


	class DzTriggerRegisterMouseEventMultiple : public ClosureNode {
	public:
		REGISTER_FROM_CLOSUER(DzTriggerRegisterMouseEventMultiple)

		virtual uint32_t getCrossDomainIndex() override { return 0; }

		virtual bool isCrossDomain() override { return true; }

		virtual std::string getUpvalue(const Upvalue& info) override {
			std::string result;

			bool is_get = info.uptype == Upvalue::TYPE::GET_LOCAL || info.uptype == Upvalue::TYPE::GET_ARRAY;
			if ((getCrossDomainIndex() == getCurrentGroupId() && is_get)|| !params_finish) {	//�����ǰ�Ǵ����� ��ʹ����һ��ľֲ�����
				result = getParentNode()->getUpvalue(info);
				return result;
			}

			switch (info.uptype) {
			case Upvalue::TYPE::SET_LOCAL:
				result = std::format("YDLocal6Set(\"{}\", {}, \"{}\", {})", m_function, info.type, info.name, info.value);
				break;
			case Upvalue::TYPE::GET_LOCAL:
				result = std::format("YDLocal6Get(\"{}\", {}, \"{}\")", m_function, info.type, info.name);
				break;
			case Upvalue::TYPE::SET_ARRAY:
				result = std::format("YDLocal6ArraySet(\"{}\", {}, \"{}\", {}, {})", m_function, info.type, info.name, info.index, info.value);
				break;
			case Upvalue::TYPE::GET_ARRAY:
				result = std::format("YDLocal6ArrayGet(\"{}\", {}, \"{}\", {})", m_function, info.type, info.name, info.index);
				break;
			default:
				break;
			}
			return result;
		}

		virtual std::string toString(TriggerFunction* func) override {

			auto params = getParameterList();

			std::string action_name = *getName();
			action_name = action_name.replace(action_name.find("Multiple"), -1, "ByCode");

			std::string result;

			params_finish = false;

			result += func->getSpaces() + "if GetLocalPlayer() == " + params[0]->toString(func) + " then\n";
			result += getScript(func, action_name, params);
			result += func->getSpaces() + "endif\n";


			std::string upvalues;

			params_finish = true;

			Function* closure = getBlock(func, m_function, upvalues);
			func->addFunction(closure);

			return upvalues + result;
		}

		virtual std::string getScript(TriggerFunction* func, const std::string& name, const std::vector<NodePtr>& params) {

			std::string result;
			std::vector<std::string> args;

			std::string key = "T";
			switch (m_nameId) {
			case "DzTriggerRegisterMouseEventMultiple"s_hash:
				key = "MT";  break;
			case "DzTriggerRegisterKeyEventMultiple"s_hash:
				key = "KT";  break;
			case "DzTriggerRegisterMouseWheelEventMultiple"s_hash:
				key = "WT";  break;
			case "DzTriggerRegisterMouseMoveEventMultiple"s_hash:
				key = "MMT";  break;
			case "DzTriggerRegisterWindowResizeEventMultiple"s_hash:
				key = "WRT";  break;
			case "DzFrameSetUpdateCallbackMultiple"s_hash:
				key = "CT";  break;
				break;
			case "DzFrameSetScriptMultiple"s_hash:
				key = "FT";  break;
			default:
				break;
			}
			m_function = getFuncName() + key;

			switch (m_nameId) {
			case "DzTriggerRegisterMouseEventMultiple"s_hash:
			case "DzTriggerRegisterKeyEventMultiple"s_hash:
				args = { "null", params[2]->toString(func), params[1]->toString(func), "false", "function " + m_function };
				break;
			case "DzTriggerRegisterMouseWheelEventMultiple"s_hash:
			case "DzTriggerRegisterMouseMoveEventMultiple"s_hash:
			case "DzTriggerRegisterWindowResizeEventMultiple"s_hash:
				args = { "null", "false", "function " + m_function };
				break;
			case "DzFrameSetUpdateCallbackMultiple"s_hash:
				args = { "function " + m_function };
				break;
			case "DzFrameSetScriptMultiple"s_hash:
				args = { params[2]->toString(func), params[1]->toString(func), "function " + m_function, "false"};
				break;
			default:
				break;
			}
			result += func->getSpaces(1) + "call " + name + "( ";
			for (size_t i = 0; i < args.size(); i++) {
				auto& arg = args[i];
				result += arg;
				if (i < args.size() - 1) {
					result += ", ";
				}
			}
			result += ")\n";
			return  result;
		}

	private:
		std::string m_function;

	};
}