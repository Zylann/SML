#ifndef __HEADER_SML_VALUE__
#define __HEADER_SML_VALUE__

#include <vector>
#include <unordered_map>
#include <string>

#include "config.h"

namespace sml
{

class Value;

typedef std::string String;
typedef std::vector<Value> Array;
typedef std::unordered_map<std::string, Value> Object;
typedef double real;

class Parser;
struct TypedObject;

enum Type
{
	VT_NULL = 0,
	VT_BOOL,
	VT_INT,
	VT_REAL,
	VT_STRING,
	VT_ARRAY,
	VT_OBJECT,
	VT_TYPEDOBJECT
};

class SML_EXPORT Value
{
public:
	Value():
		m_type(VT_NULL)
	{
		memset(&m_data, 0, sizeof(Data));
	}

	Value(const Value & other);

	~Value()
	{
		reset();
	}

	void loadFromStream(std::istream & ifs);

	Type getType() const { return m_type; }

	void reset();
	void resetArray();
	void resetObject();
	void resetString();

	void setBool(bool v);
	void setInt(int v);
	void setReal(real v);
	void setString(const String & str);

	bool getBool() const;
	int getInt() const;
	real getReal() const;
	const String & getString() const;
	const Array & getArray() const;
	const Object & getObject() const;
	const TypedObject & getTypedObject() const;

	Value & operator[](size_t i);
	Value & operator[](const std::string & key);

	// TODO Copy constructor

	Value & operator=(const Value & rhs);

private:
	void resetTypedObject();

private:
	friend class Parser;

	static const String defaultString;
	static const Array defaultArray;
	static const Object defaultObject;
	static const TypedObject defaultTypedObject;

	union Data
	{
		bool vBool;
		int vInt;
		real vReal;
		String * pString;
		Array * pArray;
		Object * pObject;
		TypedObject * pCustom; // TODO Should be pTypedObject
	};

	Type m_type;
	Data m_data;

};

struct TypedObject
{
	std::string typeName;
	Value value;
};

std::string SML_EXPORT toString(Type t);


} // namespace sml

#endif // __HEADER_SML_VALUE__

