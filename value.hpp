
#pragma once

#include <cstdint>
#include <algorithm>
#include <variant>
#include <string>
#include <cmath>

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
				switch(this->type) {
					case TYPE_NIL: {
						return 1.0f;
					}
					
					case TYPE_STRING: {
						std::string v = std::get<std::string>(data);
						
						while(v.find_first_of('.') != v.find_last_of('.')) {
							v.erase(v.find_first_of('.'));
						}
						
						while(v.find_first_of(',') != v.find_last_of(',')) {
							v.erase(v.find_first_of(','));
						}
						
						std::replace(v.begin(), v.end(), ',', '.');
						
						return std::stof(v);
					}
					
					case TYPE_UINT: {
						int32_t val = std::get<uint32_t>(data);
						return val;
					}
					
					case TYPE_INTEGER: {
						return std::get<int32_t>(data);
					}
					
					case TYPE_FPU: {
						return std::get<float>(data);
					}
				}
			}
			
			operator std::int32_t() const {
				// Note: Perform type correction to integer.
				switch(this->type) {
					case TYPE_NIL: {
						return 0;
					}
					
					case TYPE_INTEGER: {
						return std::get<int32_t>(data);
					}
					
					case TYPE_UINT: {
						int32_t val = std::get<uint32_t>(data);
						return val;
					}
					
					case TYPE_FPU: {
						return esofun::adverbs['t'] ? std::get<float>(data) : std::round(std::get<float>(data));
					}
					
					case TYPE_STRING: {
						const int asciihex2int[] = {
							-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
							-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
							-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
							 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
							-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
							-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
							-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
							-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
							-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
							-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
							-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
							-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
							-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
							-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
							-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
							-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
						};
						
						const std::string sheet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";
						
						std::string num = std::get<std::string>(data);
						
						if(num[0] != '-') {
							if(num[0] == '0') {
								uint32_t base = asciihex2int[num[1]] * 16 + asciihex2int[num[2]];
								
								if(base >= 0 && base <= 64) {
									std::string x = num.substr(3);
									int32_t n = 0;
									
									std::reverse(x.begin(), x.end());
									
									for(char& c : x) {
										size_t z = sheet.find(c);
										
										if(z == std::find::npos) {
											break;
										}
										
										n *= base;
										n += z;
									}
									
									return n;
								}
							} else {
								return std::stoi(num);
							}
						} else {
							if(num[1] == '0') {
								uint32_t base = asciihex2int[num[2]] * 16 + asciihex2int[num[3]];
								
								if(base >= 0 && base <= 64) {
									std::string x = num.substr(4);
									int32_t n = 0;
									
									std::reverse(x.begin(), x.end());
									
									for(char& c : x) {
										size_t z = sheet.find(c);
										
										if(z == std::find::npos) {
											break;
										}
										
										n *= base;
										n += z;
									}
									
									return n;
								}
							} else {
								return std::stoi(num);
							}
						}
					}
				}
			}
			
			operator std::uint32_t() const {
				// Note: Perform type correction to unsigned integer.
			}
	};
}
