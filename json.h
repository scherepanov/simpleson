#ifndef JSON_H
#define JSON_H

#include <string>
#include <vector>
#include <cstdio>
#include <stdexcept>

#define NUMBER_TO_STRING_BUFFER_LENGTH 100

namespace json
{
	/* Data types */
	namespace jtype
	{
		enum jtype { jstring, jnumber, jobject, jarray, jbool, jnull, not_valid };
		jtype detect(const std::string input);
	}

	namespace basic
	{
		class istringvalue
		{
		protected:
			std::string string;
		};

		class istringnumber : public istringvalue
		{
		public:
			// To basic type
			operator int() { int result; std::sscanf(string.c_str(), "%i", &result); return result; }
			operator unsigned int() { unsigned int result; std::sscanf(string.c_str(), "%u", &result); return result; }
			operator long() { long result; std::sscanf(string.c_str(), "%li", &result); return result; }
			operator unsigned long() { unsigned long result; std::sscanf(string.c_str(), "%lu", &result); return result; }
			operator char() { char result; std::sscanf(string.c_str(), "%c", &result); return result; }
			operator double() { double result; std::sscanf(string.c_str(), "%lf", &result); return result; }
			operator float() { float result; std::sscanf(string.c_str(), "%f", &result); return result; }
			operator std::string() { return this->string; }

			// From basic type
			istringnumber & istringnumber::operator=(const int input) { char cstr[NUMBER_TO_STRING_BUFFER_LENGTH]; std::sprintf(cstr, "%i", input); string = std::string(cstr); }
			istringnumber & istringnumber::operator=(const unsigned int input) { char cstr[NUMBER_TO_STRING_BUFFER_LENGTH]; std::sprintf(cstr, "%u", input); string = std::string(cstr); }
			istringnumber & istringnumber::operator=(const long input) { char cstr[NUMBER_TO_STRING_BUFFER_LENGTH]; std::sprintf(cstr, "%li", input); string = std::string(cstr); }
			istringnumber & istringnumber::operator=(const unsigned long input) { char cstr[NUMBER_TO_STRING_BUFFER_LENGTH]; std::sprintf(cstr, "%lu", input); string = std::string(cstr); }
			istringnumber & istringnumber::operator=(const char input) { char cstr[NUMBER_TO_STRING_BUFFER_LENGTH]; std::sprintf(cstr, "%c", input); string = std::string(cstr); }
			istringnumber & istringnumber::operator=(const double input) { char cstr[NUMBER_TO_STRING_BUFFER_LENGTH]; std::sprintf(cstr, "%lf", input); string = std::string(cstr); }
			istringnumber & istringnumber::operator=(const float input) { char cstr[NUMBER_TO_STRING_BUFFER_LENGTH]; std::sprintf(cstr, "%f", input); string = std::string(cstr); }
		};
	}

	class jnumber : public basic::istringnumber
	{
	public:
		// Parsers
		static jnumber parse(const std::string input);
		static jnumber parse(const std::string input, std::string& remainder);
	private:
		static std::string read_digits(const std::string input);
		static std::string read_digits(const std::string input, std::string& remainder);
		jnumber(const std::string input);
	};

	class jarray : public std::vector<std::string>
	{
	public:
		// Parsers
		static jarray parse(const std::string input);
		static jarray parse(const std::string input, std::string& remainder);

		// Casters
		operator std::vector<int>()
		{
			std::vector<int> result;
			int value;

			for (size_t i = 0; i < this->size(); i++)
			{
				std::sscanf(this->at(i).c_str(), "%i", &value);
				result.push_back(value);
			}
			return result;
		}
		operator std::vector<unsigned int>()
		{
			std::vector<unsigned int> result;
			unsigned int value;

			for (size_t i = 0; i < this->size(); i++)
			{
				std::sscanf(this->at(i).c_str(), "%u", &value);
				result.push_back(value);
			}
			return result;
		}
		operator std::vector<long>()
		{
			std::vector<long> result;
			long value;

			for (size_t i = 0; i < this->size(); i++)
			{
				std::sscanf(this->at(i).c_str(), "%li", &value);
				result.push_back(value);
			}
			return result;
		}
		operator std::vector<unsigned long>()
		{
			std::vector<unsigned long> result;
			unsigned long value;

			for (size_t i = 0; i < this->size(); i++)
			{
				std::sscanf(this->at(i).c_str(), "%lu", &value);
				result.push_back(value);
			}
			return result;
		}
		operator std::vector<double>()
		{
			std::vector<double> result;
			double value;

			for (size_t i = 0; i < this->size(); i++)
			{
				std::sscanf(this->at(i).c_str(), "%lf", &value);
				result.push_back(value);
			}
			return result;
		}
		operator std::vector<float>()
		{
			std::vector<float> result;
			float value;

			for (size_t i = 0; i < this->size(); i++)
			{
				std::sscanf(this->at(i).c_str(), "%f",&value);
				result.push_back(value);
			}
			return result;
		}
		operator std::string() { return to_string(); }
		operator jtype::jtype() { return type; }
	private:
		jtype::jtype type;
		std::string to_string(void);
	};

	class jvalue : public basic::istringnumber
	{
	public:
		// Parsers
		static jvalue parse(const std::string input);
		static jvalue parse(const std::string input, std::string &remainder);

		// Constructors
		inline jvalue() { this->type = jtype::jnull; }
		inline jvalue(const std::string value) { this->string = value; this->type = jtype::jstring; }
		inline jvalue(jarray value) { this->string = (std::string)value; this->type = jtype::jarray; }

		// Casters
		operator jtype::jtype() { return this->type; }
		operator bool()
		{
			if (this->type == jtype::jbool)
			{
				return this->string == "true";
			}
			if (this->type == jtype::jnumber)
			{
				if (this->string == "1")
				{
					return true;
				}
				if (this->string == "0")
				{
					return false;
				}
			}
			throw std::runtime_error("Value is not a bool");
		}
		operator jarray()
		{
			if (this->type == jtype::jarray)
			{
				return jarray::parse(this->string);
			}
			throw std::invalid_argument("RHS was not an array");
		}
		inline jtype::jtype get_type(void) { return this->type; }
		inline bool is_null(void) { return this->type == jtype::jnull; }
		inline bool is_string(void) { return this->type == jtype::jstring; }
	private:
		jtype::jtype type;
	};

	class key_value_pair
	{
	public:
		// Parsers
		static key_value_pair parse(const std::string input);
		static key_value_pair parse(const std::string input, std::string& remainder);

		// Operators
		operator std::string() { return to_string(); }

		// Properties
		std::string key;
		jvalue value;
	private:
		std::string to_string(void);
	};

	class jobject : public std::vector<key_value_pair>
	{
	public:
		// Parsers
		static jobject parse(const std::string input);
		static jobject parse(const std::string input, std::string& remainder);

		// Casters
		operator std::string() { return to_string(); }

		// Methods
		std::vector<std::string> get_keys(void);
		bool has_key(const std::string key);
		key_value_pair get_entry(const std::string key);
	private:
		std::string to_string(void);
	};

	std::string remove_leading_spaces(const std::string input);
}

#endif // !JSON_H