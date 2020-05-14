
#pragma once

#include <cstdint>
#include <algorithm>
#include <variant>
#include <string>

#include "core.hpp"

namespace esofun {
	enum {
		TYPE_STRING, TYPE_INTEGER, TYPE_UINT,
		TYPE_NIL, TYPE_FPU
	};

	class value {
		private:
			std::variant<std::string, std::int32_t, float, std::uint32_t> data;
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
			
			value(std::uint32_t u) {
				this->type = TYPE_UINT;
				this->data = u;
			}
			
			value(std::int32_t i) {
				this->type = TYPE_INTEGER;
				this->data = i;
			}
			
			operator std::string() const {
				// Note: Perform type correction to string.
				switch(this->type) {
					case TYPE_STRING: {
						return std::get<std::string>(data);
					}
					
					case TYPE_FPU: {
						std::string f = std::to_string(std::get<float>(data));
						
						if(esofun::adverbs['.'])
							std::replace(f.begin(), f.end(), ',', '.'); 
						else
							std::replace(f.begin(), f.end(), '.', ','); 
						
						return f;
					}
					
					case TYPE_INTEGER: {
						if(esofun::adverbs['b']) {
							const std::string sheet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";
							std::string r = "";
							uint32_t val;
							int coeff;
							
							if(std::get<int32_t>(data) < 0) {
								coeff = 1;
								val = -std::get<int32_t>(data);
							} else {
								coeff = 0;
								val = std::get<int32_t>(data);
							}
							
							uint32_t number = std::get<int32_t>(data);
							uint32_t base = std::get<int32_t>(data);
							
							number &= 0x03FFFFFF;
							base   &= 0xFC000000;
							base   >>= 26;
							base   += 1;
							
							if(base > 65) {
								return std::to_string(std::get<int32_t>(data));
							}
							
							while(number > 0) {
								r += sheet[number % base];
								number /= base;
							}
							
							std::reverse(r.begin(), r.end());
							
							if(coeff)
								r = std::string("-") + r;
							
							return r;
						} else {
							return std::to_string(std::get<int32_t>(data));
						}
					}
					
					case TYPE_NIL: {
						return "null";
					}
					
					case TYPE_UINT: {
						if(esofun::adverbs['b']) {
							const std::string sheet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";
							std::string r = "";
							
							uint32_t val, number, base;
							val = number = base = std::get<uint32_t>(data);
							
							number &= 0x03FFFFFF;
							base   &= 0xFC000000;
							base   >>= 26;
							base   += 1;
							
							if(base > 65) {
								return std::to_string(std::get<uint32_t>(data));
							}
							
							while(number > 0) {
								r += sheet[number % base];
								number /= base;
							}
							
							std::reverse(r.begin(), r.end());
							
							return r;
						} else {
							return std::to_string(std::get<uint32_t>(data));
						}
					}
					
					
				}
			}
			
			operator float() const {
				// Note: Perform type correction to fpu.
			}
			
			operator std::int32_t() const {
				// Note: Perform type correction to integer.
			}
			
			operator std::uint32_t() const {
				// Note: Perform type correction to unsigned integer.
			}
	};
}
