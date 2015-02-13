#ifndef SCENES_H
#define SCENES_H

#include <vector>
#include <string>
#include <string>
#include <memory>
#include <unordered_map>

template<typename SceneObject>
class Scenes {
	public:
		Scenes(std::vector<std::unique_ptr<SceneObject>> objects_) :
			objects(std::move(objects_))
		{
			unsigned int i = 0;
			for(const auto& object : objects) {
				objectLookup.emplace(object->getName(), i);
				i++;
			}
		}

		SceneObject& getObject(int n) const {
			return *objects[n];
		}

		int getObjectId(const std::string& name) const {
			const auto it = objectLookup.find(name);
			if(it == objectLookup.end()) {
				return -1;
			} else {
				return it->second;
			}
		}

		unsigned int getNumShaders() const {
			return objects.size();
		}

	private:
		std::vector<std::unique_ptr<SceneObject>> objects;
		std::unordered_map<std::string, unsigned int> objectLookup;
};

#endif
