#ifndef __HEADER_SML_WRITER__
#define __HEADER_SML_WRITER__

#include "Value.hpp"

namespace sml
{

class SML_EXPORT Writer
{
public:
	void writeValue(std::ostream & os, const Value & value);

private:
	void writeBool(std::ostream & os, bool v);
	void writeString(std::ostream & os, const String & s);
	void writeArray(std::ostream & os, const Array & a);
	void writeObject(std::ostream & os, const Object & o);
	void writeTypedObject(std::ostream & os, const TypedObject & to);
	void writeSeparator(std::ostream & os);

};

} // namespace sml

#endif // __HEADER_SML_WRITER__

