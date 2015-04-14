#ifndef __HEADER_SML_PARSER__
#define __HEADER_SML_PARSER__

#include <istream>
#include "Value.hpp"

namespace sml
{

class SML_EXPORT Parser
{
public:
	Parser();

	bool parseValue(std::istream & input, Value & out_value);

private:
	void parseComment(std::istream & input);
	void parseObject(std::istream & input, Object & out_value);
	void parseKey(std::istream & input, std::string & out_value, bool allowSpaces);
	void parseTypedObject(std::istream & input, Value & out_value);
	void parseArray(std::istream & input, Array & out_value);
	void parseNumber(std::istream & input, Value & out_value);
	void parseString(std::istream & input, String & out_value);

	void skipUntil(std::istream & input, char c);

	bool isWhiteSpace(char c);
	bool isKeyChar(char c);
	bool isNumberChar(char c);

private:
	char m_lastCharacter;

};

} // namespace sml

#endif // __HEADER_SML_PARSER__

