#include "Value.hpp"
#include "Parser.hpp"

namespace sml
{

const String Value::defaultString;
const Array Value::defaultArray;
const Object Value::defaultObject;
const TypedObject Value::defaultTypedObject;

Value::Value(const Value & other)
{
	*this = other;
}

void Value::loadFromStream(std::istream & input)
{
	Parser parser;
	parser.parseValue(input, *this);
}

void Value::reset()
{
	switch (m_type)
	{
	case VT_STRING:
		delete m_data.pString;
		break;
	case VT_ARRAY:
		delete m_data.pArray;
		break;
	case VT_OBJECT:
		delete m_data.pObject;
		break;
	case VT_TYPEDOBJECT:
		delete m_data.pCustom;
		break;
	default:
		break;
	}
	memset(&m_data, 0, sizeof(Data));
	m_type = VT_NULL;
}

void Value::resetArray()
{
	reset();
	m_data.pArray = new Array();
	m_type = VT_ARRAY;
}

void Value::resetObject()
{
	reset();
	m_data.pObject = new Object();
	m_type = VT_OBJECT;
}

void Value::resetTypedObject()
{
	reset();
	m_data.pCustom = new TypedObject();
	m_type = VT_TYPEDOBJECT;
}

void Value::resetString()
{
	reset();
	m_data.pString = new String();
	m_type = VT_STRING;
}

void Value::setBool(bool v)
{
	reset();
	m_data.vBool = v;
	m_type = VT_BOOL;
}

void Value::setInt(int v)
{
	reset();
	m_data.vInt = v;
	m_type = VT_INT;
}

void Value::setReal(real v)
{
	reset();
	m_data.vReal = v;
	m_type = VT_REAL;
}

void Value::setString(const String & str)
{
	switch (m_type)
	{
	case VT_STRING:
		*m_data.pString = str;
		break;
	default:
		reset();
		m_data.pString = new String(str);
		break;
	}
	m_type = VT_STRING;
}

bool Value::getBool() const
{
	return m_type == VT_BOOL ? m_data.vBool : false;
}

int Value::getInt() const
{
	return m_type == VT_INT ? m_data.vInt : 0;
}

real Value::getReal() const
{
	return m_type == VT_REAL ? m_data.vReal : 0;
}

const String & Value::getString() const
{
	return m_type == VT_STRING ? *m_data.pString : defaultString;
}

const Array & Value::getArray() const
{
	return m_type == VT_ARRAY ? *m_data.pArray : defaultArray;
}

const Object & Value::getObject() const
{
	return m_type == VT_OBJECT ? *m_data.pObject : defaultObject;
}

const TypedObject & Value::getTypedObject() const
{
	return m_type == VT_TYPEDOBJECT ? *m_data.pCustom : defaultTypedObject;
}

Value & Value::operator[](size_t i)
{
	if (m_type != VT_ARRAY)
		resetArray();
	Array & a = *m_data.pArray;
	if (i >= a.size())
		a.resize(i + 1);
	return a[i];
}

Value & Value::operator[](const std::string & key)
{
	if (m_type != VT_OBJECT)
		resetObject();
	Object & a = *m_data.pObject;
	return a[key];
}

Value & Value::operator=(const Value & rhs)
{
	// TODO Break this into functions so they can be used individually?
	switch (m_type)
	{
		case VT_STRING:
			setString(*rhs.m_data.pString);
			break;

		case VT_ARRAY:
		{
			resetArray();
			Array & src = *rhs.m_data.pArray;
			Array & dst = *m_data.pArray;
			for (unsigned int i = 0; i < src.size(); ++i)
			{
				dst[i] = src[i];
			}
		}
			break;

		case VT_OBJECT:
		{
			resetObject();
			Object & src = *rhs.m_data.pObject;
			Object & dst = *m_data.pObject;
			for (auto it = src.begin(); it != src.end(); ++it)
			{
				dst[it->first] = it->second;
			}
		}
			break;

		case VT_TYPEDOBJECT:
			resetTypedObject();
			m_data.pCustom->typeName = rhs.m_data.pCustom->typeName;
			m_data.pCustom->value = rhs.m_data.pCustom->value;
			break;

		default:
			m_type = rhs.m_type;
			m_data = rhs.m_data;
			break;
	}

	return *this;
}

std::string toString(Type t)
{
	switch (t)
	{
	case VT_BOOL: return "Bool";
	case VT_INT: return "Int";
	case VT_REAL: return "Real";
	case VT_STRING: return "String";
	case VT_ARRAY: return "Array";
	case VT_OBJECT: return "Object";
	case VT_TYPEDOBJECT: return "Custom";
	default: return "Null";
	}
}

} // namespace sml

