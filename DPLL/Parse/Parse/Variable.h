#pragma once
#include <string>

using namespace std;

class Variable
{
private:
	bool sign;
	bool value;
	bool real_value;
	bool assigned;
	string name;
	bool pureFlag;
	bool watchedFlag;
public:
	Variable(bool _sign, bool _value, bool _pure, string _name);
	Variable(const Variable &v);
	~Variable();
	bool get_sign() const {
		return sign;
	}
	bool get_value() const {
		return value;
	}
	string get_name() const {
		return name;
	}
	bool get_assigned() const {
		return assigned;
	}
	bool get_real_value() const {
		return sign == true ? value : !value;
	}

	void set_pure(bool flag) {
		pureFlag = flag;
	}

	bool get_pure() const {
		return pureFlag;
	}

	void set_value(bool _value) {
		value = _value;
	}

	void set_sign(bool _val) {
		sign = _val;
	}
	void set_assigned(bool _val) {
		assigned = _val;
	}
	void set_real_value(bool _val) {
		real_value = _val;
	}
	bool get_watched() const {
		return watchedFlag;
	}
	Variable& operator = (const Variable &v) {
		if (this != &v) {
			this->sign = v.get_sign();
			this->value = v.get_value();
			this->pureFlag = v.get_pure();
			this->name = v.get_name();
		}
		return *this;
	}
	bool operator == (const Variable &v) {
		if (this != &v) {
			if (v.get_sign() != this->sign)return false;
			if (v.get_value() != this->value)return false;
			if (v.get_name() != this->name)return false;
			if (v.get_pure() != this->pureFlag)return false;
		}
		return true;
	}
};

