#include "Writer.hpp"

namespace sml
{

void Writer::writeValue(std::ostream & os, const Value & value)
{
	switch (value.getType())
	{
	case VT_BOOL: os << value.getBool(); break;
	case VT_INT: os << value.getInt(); break;
	case VT_REAL: os << value.getReal(); break;

	case VT_STRING:
		writeString(os, value.getString());
		break;

	case VT_ARRAY:
		writeArray(os, value.getArray());
		break;

	case VT_OBJECT:
		writeObject(os, value.getObject());
		break;

	case VT_TYPEDOBJECT:
		writeTypedObject(os, value.getTypedObject());
		break;

	default:
		os << "null";
		break;
	}
}

void Writer::writeString(std::ostream & os, const String & s)
{
	os << '"' << s << '"';
}

void Writer::writeArray(std::ostream & os, const Array & a)
{
	os << "[";

	for (unsigned int i = 0; i < a.size(); ++i)
	{
		writeValue(os, a[i]);
		if (i != a.size() - 1)
		{
			writeSeparator(os);
		}
	}

	os << "]";
}

void Writer::writeObject(std::ostream & os, const Object & o)
{
	os << "{";

	bool sep = false;
	for (auto it = o.begin(); it != o.end(); ++it)
	{
		if (sep)
		{
			writeSeparator(os);
		}

		os << it->first << ":";
		writeValue(os, it->second);

		sep = true;
	}

	os << "}";
}

void Writer::writeTypedObject(std::ostream & os, const TypedObject & to)
{
	os << to.typeName;
	writeValue(os, to.value);
}

void Writer::writeSeparator(std::ostream & os)
{
	os << ',';
}


} // namespace sml
