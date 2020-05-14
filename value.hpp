
#pragma once

#include <variant>
#include <string>

namespace esofun {
	enum {
		TYPE_STRING, TYPE_INTEGER, TYPE_UINT,
		TYPE_NIL, TYPE_FPU
	};

	class value {
		private:
			std::variant<std::string, int, float, unsigned int> data;
			unsigned char type;
		public:
			value(std::string s) {
				this->type = TYPE_STRING;
				this->data = s;
			}
			
			value(float f) {
				this->type = TYPE_FPU;
				this->data = f;
			}
			
			value(unsigned int u) {
				this->type = TYPE_UINT;
				this->data = u;
			}
			
			value(int i) {
				this->type = TYPE_INTEGER;
				this->data = i;
			}
			
			operator std::string() const {
				// Note: Perform type correction to string.
			}
			
			operator float() const {
				// Note: Perform type correction to fpu.
			}
			
			operator int() const {
				// Note: Perform type correction to integer.
			}
			
			operator unsigned int() const {
				// Note: Perform type correction to unsigned integer.
			}
	};
}
