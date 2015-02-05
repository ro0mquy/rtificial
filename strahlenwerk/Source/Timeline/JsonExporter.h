#ifndef JSONEXPORTER_H
#define JSONEXPORTER_H

#include <juce>

class JsonExporter {
	public:
		static var toJson(ValueTree tree);
		static ValueTree fromJson(var json, Identifier rootType);
	private:
		static Identifier arrayToElementType(Identifier arrayType);
};

#endif // JSONEXPORTER_H
