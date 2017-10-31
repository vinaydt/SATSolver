#include "Variable.h"


Variable::Variable(bool _sign, bool _value, bool _pure, string _name)
{
	sign = _sign;
	value = _value;
	name = _name;
	assigned = false;
	real_value = false;
	pureFlag = _pure;
	watchedFlag = false;

}

Variable::Variable(const Variable & v)
{
	sign = v.get_sign();
	value = v.get_value();
	assigned = v.get_assigned();
	name = v.get_name();
	pureFlag = v.get_pure();
	watchedFlag = v.get_watched();
}



Variable::~Variable()
{
}
