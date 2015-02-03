#ifndef JSONEXPORTER_H
#define JSONEXPORTER_H

#include <juce>

class JsonExporter {
	public:
		static var toJson(ValueTree tree);
		static ValueTree fromJson(var json);
};

#endif // JSONEXPORTER_H
