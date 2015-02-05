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
			const auto childIdentifier = arrayToElementType(child.getType());
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

ValueTree JsonExporter::fromJson(var json, Identifier rootType) {
	if (json == var::null) {
		return ValueTree(rootType);
	}

	// everything is an object!
	jassert(json.isObject() && !json.isArray());

	ValueTree tree(rootType);
	const NamedValueSet& properties = json.getDynamicObject()->getProperties();
	for (int i = 0; i < properties.size(); i++) {
		const Identifier type = properties.getName(i);
		var value = properties.getValueAt(i);
		if (value.isObject() && !value.isArray()) {
			tree.addChild(fromJson(value, type), -1, nullptr);
		} else if (value.isArray()) {
			ValueTree arrayParent(type);
			const auto elementType = arrayToElementType(type);
			for (int j = 0; j < value.size(); j++) {
				// nobody will ever put arrays inside of other arrays - I hope!
				arrayParent.addChild(fromJson(value[j], elementType), -1, nullptr);
			}
			tree.addChild(arrayParent, -1, nullptr);
		} else {
			// let's hope somebody put something sane in here and we don't end
			// up having a dynamic javascript method in our timeline
			tree.setProperty(type, value, nullptr);
		}
	}

	return tree;
}

Identifier JsonExporter::arrayToElementType(Identifier arrayType) {
	// drop last 6 characters ("sArray") to get the array elements' Identifier name
	return Identifier(arrayType.toString().dropLastCharacters(6));
}
