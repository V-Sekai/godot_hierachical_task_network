#include "planning_domain.h"

void PlanningDomain::add_operator(const String &p_task_name, const OperatorFunction &p_operatorFunc) {
	auto operators = operator_table.find(p_task_name);

	if (operators == operator_table.end()) {
		operator_table[p_task_name] = std::vector<OperatorFunction>{ p_operatorFunc };
		return;
	}
	operators->second.push_back(p_operatorFunc);
}

void PlanningDomain::add_method(const String &p_task_name, const MethodFunction &p_method_func) {
	auto methods = method_table.find(p_task_name);

	if (methods == method_table.end()) {
		method_table[p_task_name] = std::vector<MethodFunction>{ p_method_func };
		return;
	}
	methods->second.push_back(p_method_func);
}

std::vector<OperatorWithParams> PlanningDomain::get_applicable_operators(const State &p_current_state, const Task &p_task) const {
	std::vector<OperatorWithParams> operatorsWithParams;

	if (operator_table.empty()) {
		return operatorsWithParams;
	}

	for (const auto &element : operator_table) {
		if (element.first == p_task.task_name) {
			for (const auto &_operator : element.second) {
				if (_operator(p_current_state, p_task.parameters)) {
					operatorsWithParams.emplace_back(p_task, _operator);
				}
			}
			break;
		}
	}

	return operatorsWithParams;
}

std::optional<std::vector<MethodWithParams>> PlanningDomain::get_relevant_methods(const State &p_current_state, const Task &p_task) const {
	std::vector<MethodWithParams> methodsWithParams;

	if (method_table.empty()) {
		return std::nullopt;
	}

	for (const auto &element : method_table) {
		if (element.first == p_task.task_name) {
			for (const auto &method : element.second) {
				if (method(p_current_state, p_task.parameters)) {
					methodsWithParams.emplace_back(p_task, method);
				}
			}
			break;
		}
	}

	return methodsWithParams;
}

bool PlanningDomain::task_is_operator(const String &p_task_name) const {
	return (operator_table.find(p_task_name) != operator_table.end());
}

bool PlanningDomain::task_is_method(const String &p_task_name) const {
	return (method_table.find(p_task_name) != method_table.end());
}
