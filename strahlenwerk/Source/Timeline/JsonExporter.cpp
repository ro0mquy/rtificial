#include "JsonExporter.h"

var JsonExporter::toJson(ValueTree tree) {
	auto const object = new DynamicObject;

	// copy properties
	for (int i = 0; i < tree.getNumProperties(); i++) {
		const auto identifier = tree.getPropertyName(i);
		object->setProperty(identifier, tree.getProperty(identifier));
	}

	for (int i = 0; i < tree.getNumChildren(); i++) {
		ValueTree child = tree.getChild(i);
		if (child.getNumProperties() == 0 && child.getType().toString().endsWith("sArray")) {
			// if the child has no properties and is named *sArray,
			// inline the array

			// drop the last 6 characters to get the array elements' identifier name
			const Identifier childIdentifier(child.getType().toString().dropLastCharacters(6));
			Array<var> children;
			children.resize(child.getNumChildren());
			for (int j = 0; j < child.getNumChildren(); j++) {
				ValueTree element = child.getChild(j);
				// assume all children have the same type
				jassert(element.getType() == childIdentifier);
				children.set(j, toJson(element));
			}
			object->setProperty(child.getType(), children);
		} else {
			// else put the child directly into this object's properties
			object->setProperty(child.getType(), toJson(child));
		}
	}
	return var(object);
}

ValueTree JsonExporter::fromJson(var json) {
	// TODO
	return ValueTree();
}
