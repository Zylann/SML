#include <sstream>
#include "Parser.hpp"

namespace sml
{

Parser::Parser() :
	m_lastCharacter('%')
{
}

bool Parser::parseValue(std::istream & input, Value & out_value)
{
	bool found = false;

	while (!found && !input.eof())
	{
		const char c = input.peek();

		switch (c)
		{
		case '/':
		case '#':
			parseComment(input);
			found = true;
			break;

		case '{':
			m_lastCharacter = input.get();
			out_value.resetObject();
			parseObject(input, *out_value.m_data.pObject);
			found = true;
			break;

		case '[':
		case '(':
			m_lastCharacter = input.get();
			out_value.resetArray();
			parseArray(input, *out_value.m_data.pArray);
			found = true;
			break;

		case '"':
			m_lastCharacter = input.get();
			out_value.resetString();
			parseString(input, *out_value.m_data.pString);
			found = true;
			break;

		case ' ':
		case '\t':
			m_lastCharacter = input.get();
			break;

		case '\n':
		case '\r':
			m_lastCharacter = input.get();
			break;

		default:
			if (isalpha(c))
			{
				parseTypedObject(input, out_value);
				found = true;
			}
			else if (isdigit(c) || c == '-')
			{
				parseNumber(input, out_value);
				found = true;
			}
			else
			{
				m_lastCharacter = input.get();
			}
			break;
		}
	}

	return found;
}

void Parser::parseNumber(std::istream & input, Value & out_value)
{
	std::stringstream ss;
	if (isdigit(m_lastCharacter) || m_lastCharacter == '-')
	{
		ss << m_lastCharacter;
	}

	Type numberType = VT_INT;

	while (!input.eof())
	{
		m_lastCharacter = input.get();
		const char c = m_lastCharacter;

		if (!isNumberChar(c))
		{
			break;
		}

		if (c == '.')
		{
			numberType = VT_REAL;
		}

		ss << c;
	}

	out_value.reset();
	out_value.m_type = numberType;

	if (numberType == VT_REAL)
	{
		ss >> out_value.m_data.vReal;
	}
	else
	{
		ss >> out_value.m_data.vInt;
	}
}

void Parser::parseComment(std::istream & input)
{
	while (!input.eof())
	{
		char c = input.get();
		if (c == '\n' || c == '\r')
			break;
	}
	while (!input.eof())
	{
		char c = input.get();
		if (c != '\n' && c != '\r')
			break;
	}
}

void Parser::parseObject(std::istream & input, Object & out_value)
{
	while (!input.eof())
	{
		const char c = input.peek();

		if (isalpha(c) || c == '"')
		{
			// Parse key
			std::string key;
			bool allowSpaces = (c == '"');
			parseKey(input, key, allowSpaces);
			Value & value = out_value[key]; // Access creates the pair

			// Make sure to read after a ':'
			if (m_lastCharacter != ':')
			{
				skipUntil(input, ':');
			}

			// Parse value
			parseValue(input, value);

			// If the value parsing encountered the end of the object
			if (m_lastCharacter == '}')
			{
				m_lastCharacter = input.get();
				break;
			}
		}
		else if (c == '}')
		{
			break;
		}
		else
		{
			m_lastCharacter = input.get();
		}
	}
}

/// \brief Starts at a begin quote or an alphanumeric character, ends at a closing quote or non-alphanumeric character
void Parser::parseKey(std::istream & input, std::string & out_value, bool allowSpaces)
{
	//out_value = m_lastCharacter;
	bool next = true;
	while (next && !input.eof())
	{
		const char c = input.peek();

		switch (c)
		{
		case '"':
			if (m_lastCharacter == '"')
				next = false;
			else
				m_lastCharacter = input.get();
			break;

		case ' ':
			if (allowSpaces)
			{
				out_value += c;
				m_lastCharacter = input.get();
			}
			else
				next = false;
			break;

		default:
			if (isKeyChar(c))
			{
				out_value += c;
				m_lastCharacter = input.get();
			}
			else
				next = false;
			break;
		}
	}
}

void Parser::parseTypedObject(std::istream & input, Value & out_value)
{
	// Parse type
	std::string typeKey;
	parseKey(input, typeKey, false);

	if (typeKey == "true")
	{
		out_value.setBool(true);
	}
	else if (typeKey == "false")
	{
		out_value.setBool(false);
	}
	else if (typeKey == "null")
	{
		out_value.reset();
	}
	else
	{
		// TODO FIXME There is a leading space in type names!
		// This is a custom typed object
		out_value.resetTypedObject();
		CustomObject & o = *out_value.m_data.pCustom;
		o.typeName = typeKey;
		// Parse value
		// TODO Can the value be a typed object itself?
		parseValue(input, o.value);
	}
}

void Parser::parseArray(std::istream & input, Array & out_value)
{
	char c;
	bool next = true;

	while (next && !input.eof())
	{
		const char c = m_lastCharacter;

		switch (c)
		{
		case ']':
		case ')':
			next = false;
			m_lastCharacter = input.get();
			break;

		default:
			if (!isWhiteSpace(c))
			{
				out_value.push_back(Value());
				if (!parseValue(input, out_value.back()))
				{
					out_value.pop_back();
				}
			}
			else
			{
				m_lastCharacter = input.get();
			}
			break;
		}
	}
}

void Parser::parseString(std::istream & input, String & out_value)
{
	bool escaping = false;
	bool inString = true;
	while (inString && !input.eof())
	{
		char c = input.get();
		switch (c)
		{
		case '"':
			if (escaping)
			{
				out_value += c;
				escaping = false;
			}
			else
				inString = false;
			break;

		case '\\':
			if (escaping)
			{
				out_value += c;
				escaping = false;
			}
			else
				escaping = true;
			break;

		default:
			if (escaping)
			{
				switch (c)
				{
				case 'n': c = '\n'; break;
				case 'r': c = '\r'; break;
				case 't': c = '\t'; break;
				default: break;
				}
			}
			out_value += c;
			break;
		}
	}
}

void Parser::skipUntil(std::istream & input, char c)
{
	while (!input.eof())
	{
		if (input.get() == c)
			break;
	}
}

bool Parser::isWhiteSpace(char c)
{
	return !!isspace(c);
}

bool Parser::isKeyChar(char c)
{
	return isalpha(c) || isdigit(c) || c == '_' || c == '.';
}

bool Parser::isNumberChar(char c)
{
	return isdigit(c) || c == '-' || c == '.';
}

} // namespace sml

