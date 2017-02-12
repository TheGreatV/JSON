#include "JSON.hpp"


#pragma region JSON

#pragma region Object

#pragma region Undefined

const JSON::Object& JSON::Object::Undefined::operator [] (const Name& name_) const
{
	throw Exception();
}
JSON::Object& JSON::Object::Undefined::operator [] (const Name& name_)
{
	throw Exception();
}
const JSON::Object& JSON::Object::Undefined::operator [] (const Size& index_) const
{
	throw Exception();
}
JSON::Object& JSON::Object::Undefined::operator [] (const Size& index_)
{
	throw Exception();
}

JSON::Object::Undefined::operator JSON::Object::Type() const
{
	return Type::Undefined;
}

JSON::Object::Undefined::operator JSON::Null() const
{
	throw Exception();
}
JSON::Object::Undefined::operator bool() const
{
	throw Exception();
}
JSON::Object::Undefined::operator std::double_t() const
{
	throw Exception();
}
JSON::Object::Undefined::operator JSON::String() const
{
	throw Exception();
}

JSON::Object::Text JSON::Object::Undefined::Stringify() const
{
	throw Exception();
}

#pragma endregion

#pragma region Null

JSON::Object::Null::operator JSON::Object::Type() const
{
	return Type::Null;
}
JSON::Object::Null::operator JSON::Null() const
{
	return JSON::Null();
}

JSON::Object::Text JSON::Object::Null::Stringify() const
{
	return "null";
}


#pragma endregion

#pragma region Boolean

JSON::Object::Boolean::operator JSON::Object::Type() const
{
	return Type::Boolean;
}
JSON::Object::Boolean::operator bool() const
{
	return value;
}

JSON::Object::Text JSON::Object::Boolean::Stringify() const
{
	return value ? "true" : "false";
}

#pragma endregion

#pragma region Number

JSON::Object::Number::operator JSON::Object::Type() const
{
	return Type::Number;
}
JSON::Object::Number::operator std::double_t() const
{
	return value;
}

JSON::Object::Text JSON::Object::Number::Stringify() const
{
	return std::to_string(value);
}

#pragma endregion

#pragma region String

JSON::Object::String::operator JSON::Object::Type() const
{
	return Type::String;
}
JSON::Object::String::operator JSON::String() const
{
	return value;
}

JSON::Object::Text JSON::Object::String::Stringify() const
{
	return static_cast<Text>("\"") + value + static_cast<Text>("\"");
}

#pragma endregion

#pragma region Array

const JSON::Object& JSON::Object::Array::operator [] (const Size& index_) const
{
	if(index_ < childs.size())
	{
		return childs[index_];
	}
	else
	{
		throw Exception();
	}
}
JSON::Object& JSON::Object::Array::operator [] (const Size& index_)
{
	if(index_ < childs.size())
	{
		return childs[index_];
	}
	else
	{
		throw Exception();
	}
}
JSON::Object::Text JSON::Object::Array::Stringify() const
{
	Text text = "";

	bool isFirst = true;
	for(auto &child : childs)
	{
		if(isFirst)
		{
			isFirst = false;
		}
		else
		{
			text += ",";
		}

		if(static_cast<Type>(child) != Type::Undefined)
		{
			text += child.Stringify();
		}
	}

	return static_cast<Text>("[") + text + static_cast<Text>("]");
}
JSON::Object::Array::operator JSON::Object::Type() const
{
	return Type::Array;
}

#pragma endregion

#pragma region Map

const JSON::Object& JSON::Object::Map::operator [] (const Name& name_) const
{
	auto it = childs.find(name_);
	if(it != childs.end())
	{
		return (*it).second;
	}
	else
	{
		throw Exception("No child element '" + name_ + "'");
	}
}
JSON::Object& JSON::Object::Map::operator [] (const Name& name_)
{
	auto it = childs.find(name_);
	if(it == childs.end())
	{
		it = childs.insert(it, {name_, Object()});
	}

	return (*it).second;
}

JSON::Object::Text JSON::Object::Map::Stringify() const
{
	Text text = "";

	bool isFirst = true;
	for(auto &child : childs)
	{
		if(isFirst)
		{
			isFirst = false;
		}
		else
		{
			text += ",";
		}

		if(static_cast<Type>(child.second) != Type::Undefined)
		{
			text += static_cast<Text>("\"") + static_cast<Text>(child.first) + static_cast<Text>("\"") + ":" + child.second.Stringify();
		}
	}

	return static_cast<Text>("{") + text + static_cast<Text>("}");
}
JSON::Object::Map::operator JSON::Object::Type() const
{
	return Type::Map;
}

#pragma endregion


#pragma endregion


using namespace JSON;

void Skip(const Object::Text& text_, Size& it_);
bool Match(const Object::Text& text_, Size& it_, const Object::Text& source_);

Object ParseNull(const Object::Text& text_, Size& it_);
Object ParseBoolean(const Object::Text& text_, Size& it_);
Object ParseNumber(const Object::Text& text_, Size& it_);
Object ParseString(const Object::Text& text_, Size& it_);
Object ParseArray(const Object::Text& text_, Size& it_);
Object ParseAny(const Object::Text& text_, Size& it_);

void Skip(const Object::Text& text_, Size& it_)
{
	while(it_ < text_.size() && (
		text_[it_] == ' ' ||
		text_[it_] == '\t' ||
		text_[it_] == '\n'))
	{
		++it_;
	}
}
bool Match(const Object::Text& text_, Size& it_, const Object::Text& source_)
{
	auto o = it_;

	it_ += source_.size();
	if(it_ <= text_.size())
	{
		auto t = text_.substr(o, it_ - o);
		if(t == source_)
		{
			return true;
		}
	}

	it_ = o;
	return false;
}

Object ParseNull(const Object::Text& text_, Size& it_)
{
	auto o = it_;

	Skip(text_, it_);
	if(Match(text_, it_, "null"))
	{
		return Object(nullptr);
	}

	it_ = o;
	return Object(JSON::Undefined());
}
Object ParseBoolean(const Object::Text& text_, Size& it_)
{
	auto o = it_;

	Skip(text_, it_);

	auto t = it_;
	if(Match(text_, it_, "true"))
	{
		return Object(true);
	}
	it_ = t;
	if(Match(text_, it_, "false"))
	{
		return Object(false);
	}

	it_ = o;
	return Object(JSON::Undefined());
}
Object ParseNumber(const Object::Text& text_, Size& it_)
{
	auto o = it_;

	Skip(text_, it_);

	if(it_ < text_.size() && (
		text_[it_] == '-' ||
		text_[it_] == '0' ||
		text_[it_] == '1' ||
		text_[it_] == '2' ||
		text_[it_] == '3' ||
		text_[it_] == '4' ||
		text_[it_] == '5' ||
		text_[it_] == '6' ||
		text_[it_] == '7' ||
		text_[it_] == '8' ||
		text_[it_] == '9'))
	{
		auto t = it_;
		if(it_ < text_.size() && text_[it_] == '-')
		{
			++it_;
			if(!(it_ < text_.size() && (
				text_[it_] == '0' ||
				text_[it_] == '1' ||
				text_[it_] == '2' ||
				text_[it_] == '3' ||
				text_[it_] == '4' ||
				text_[it_] == '5' ||
				text_[it_] == '6' ||
				text_[it_] == '7' ||
				text_[it_] == '8' ||
				text_[it_] == '9')))
			{
				throw Exception();
			}
		}
		while(it_ < text_.size() && (
			text_[it_] == '0' ||
			text_[it_] == '1' ||
			text_[it_] == '2' ||
			text_[it_] == '3' ||
			text_[it_] == '4' ||
			text_[it_] == '5' ||
			text_[it_] == '6' ||
			text_[it_] == '7' ||
			text_[it_] == '8' ||
			text_[it_] == '9'))
		{
			++it_;
		}

		if(it_ < text_.size() && text_[it_] == '.')
		{
			++it_;
			if(it_ < text_.size() && (
				text_[it_] == '0' ||
				text_[it_] == '1' ||
				text_[it_] == '2' ||
				text_[it_] == '3' ||
				text_[it_] == '4' ||
				text_[it_] == '5' ||
				text_[it_] == '6' ||
				text_[it_] == '7' ||
				text_[it_] == '8' ||
				text_[it_] == '9'))
			{
				while(it_ < text_.size() && (
					text_[it_] == '0' ||
					text_[it_] == '1' ||
					text_[it_] == '2' ||
					text_[it_] == '3' ||
					text_[it_] == '4' ||
					text_[it_] == '5' ||
					text_[it_] == '6' ||
					text_[it_] == '7' ||
					text_[it_] == '8' ||
					text_[it_] == '9'))
				{
					++it_;
				}

				auto textFloating = text_.substr(t, it_ - t);
				auto val = std::strtod(textFloating.c_str(), nullptr);

				return Object(val);
			}
			else
			{
				throw Exception();
			}
		}
		else
		{
			auto textInteger = text_.substr(t, it_ - t);
			auto val = std::strtoll(textInteger.c_str(), nullptr, 10);

			return Object(static_cast<int>(val));
		}
	}

	it_ = o;
	return Object(JSON::Undefined());
}
Object ParseString(const Object::Text& text_, Size& it_)
{
	auto o = it_;

	Skip(text_, it_);
	if(it_ < text_.size() && text_[it_] == '"')
	{
		++it_;
		auto t = it_;
		if(it_ < text_.size())
		{
			while(text_[it_] != '"')
			{
				if(text_[it_] == '\\')
				{
					++it_;
					if(!(it_ < text_.size()))
					{
						throw Exception();
					}
				}

				++it_;
				if(!(it_ < text_.size()))
				{
					throw Exception();
				}
			}

			if(it_ < text_.size())
			{
				auto textString = text_.substr(t, it_ - t);
				++it_;
				return Object(textString);
			}
			else
			{
				throw Exception();
			}
		}
		else
		{
			throw Exception();
		}
	}

	it_ = o;
	return Object(JSON::Undefined());
}
Object ParseArray(const Object::Text& text_, Size& it_)
{
	auto o = it_;

	Skip(text_, it_);
	if(it_ < text_.size() && text_[it_] == '[')
	{
		++it_;

		Vector<Object> childs;

		while(true)
		{
			auto child = ParseAny(text_, it_);
			if(static_cast<Object::Type>(child) != Object::Type::Undefined)
			{
				childs.push_back(child);

				Skip(text_, it_);
				if(it_ < text_.size() && text_[it_] == ',')
				{
					++it_;
				}
				else
				{
					break;
				}
			}
		}

		Skip(text_, it_);
		if(it_ < text_.size() && text_[it_] == ']')
		{
			++it_;
			return Object(childs);
		}
		else
		{
			throw Exception();
		}
	}

	it_ = o;
	return Object(JSON::Undefined());
}
Object ParseMap(const Object::Text& text_, Size& it_)
{
	auto o = it_;

	Skip(text_, it_);
	if(it_ < text_.size() && text_[it_] == '{')
	{
		++it_;

		Dictionary<Object::Name, Object> childs;

		while(true)
		{
			auto nameObject = ParseString(text_, it_);
			if(static_cast<Object::Type>(nameObject) != Object::Type::Undefined)
			{
				auto name = static_cast<Object::Name>(nameObject);
				if(!name.empty())
				{
					Skip(text_, it_);
					if(it_ < text_.size() && text_[it_] == ':')
					{
						++it_;
					}
					else
					{
						throw Exception();
					}

					auto child = ParseAny(text_, it_);
					if(static_cast<Object::Type>(child) != Object::Type::Undefined)
					{
						auto it = childs.find(name);
						if(it == childs.end())
						{
							childs.insert(childs.begin(), {name, child});
						}
						else
						{
							throw Exception();
						}
					}
					else
					{
						throw Exception();
					}

					Skip(text_, it_);
					if(it_ < text_.size() && text_[it_] == ',')
					{
						++it_;
					}
					else
					{
						break;
					}
				}
				else
				{
					throw Exception();
				}
			}
			else
			{
				break;
			}
		}

		Skip(text_, it_);
		if(it_ < text_.size() && text_[it_] == '}')
		{
			++it_;
			return Object(childs);
		}
		else
		{
			throw Exception();
		}
	}

	it_ = o;
	return Object(JSON::Undefined());
}
Object ParseAny(const Object::Text& text_, Size& it_)
{
	auto o = it_;

	auto objectNull = ParseNull(text_, it_);
	if(static_cast<Object::Type>(objectNull) != Object::Type::Undefined)
	{
		return objectNull;
	}

	auto objectBoolean = ParseBoolean(text_, it_);
	if(static_cast<Object::Type>(objectBoolean) != Object::Type::Undefined)
	{
		return objectBoolean;
	}

	auto objectNumber = ParseNumber(text_, it_);
	if(static_cast<Object::Type>(objectNumber) != Object::Type::Undefined)
	{
		return objectNumber;
	}

	auto objectString = ParseString(text_, it_);
	if(static_cast<Object::Type>(objectString) != Object::Type::Undefined)
	{
		return objectString;
	}

	auto objectArray = ParseArray(text_, it_);
	if(static_cast<Object::Type>(objectArray) != Object::Type::Undefined)
	{
		return objectArray;
	}

	auto objectMap = ParseMap(text_, it_);
	if(static_cast<Object::Type>(objectMap) != Object::Type::Undefined)
	{
		return objectMap;
	}

	it_ = o;
	return Object(JSON::Undefined());
}

JSON::Object JSON::Parse(const Object::Text& text_)
{
	Size it = 0;

	auto objectAny = ParseAny(text_, it);
	if(static_cast<Object::Type>(objectAny) != Object::Type::Undefined)
	{
		Skip(text_, it);
		if(it != text_.size())
		{
			throw Exception();
		}

		return objectAny;
	}

	throw Exception();
}

#pragma endregion


#pragma region
#pragma endregion






















