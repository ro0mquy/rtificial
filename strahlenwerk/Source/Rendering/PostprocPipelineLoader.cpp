#include <regex>
#include <queue>
#include <stack>

#include "PostprocPipelineLoader.h"
#include "PostprocShader.h"

// TODO: improve error logging

PostprocPipelineLoader::~PostprocPipelineLoader() = default;

std::vector<std::unique_ptr<PostprocShader>> PostprocPipelineLoader::load(
		OpenGLContext& context,
		const std::string& mappingSource,
		const std::vector<std::pair<std::string, std::string>>& shaderSources)
{
	auto shaderIds = loadShaders(context, shaderSources);


	auto mapping = loadMapping(shaderIds, mappingSource);
	auto order = createOrder(mapping);
	std::vector<std::unique_ptr<PostprocShader>> shadersInOrder;
	if(order.empty()) {
		return shadersInOrder;
	}
	connectStages(order, mapping);

	shadersInOrder.resize(order.size());
	for(int i = 0; i < order.size(); i++) {
		shadersInOrder[i] = std::move(shaders[order[i]]);
	}
	shadersInOrder.pop_back();
	return shadersInOrder;
}

std::unordered_map<std::string, int> PostprocPipelineLoader::loadShaders(
		OpenGLContext& context,
		const std::vector<std::pair<std::string, std::string>>& shaderSources)
{
	std::unordered_map<std::string, int> shaderIds;

	shaderIds.clear();
	shaderIds.emplace("input", 0);
	shaderIds.emplace("output", 1);

	shaders.clear();
	shaders.emplace_back(new PostprocShader(context));
	shaders.back()->load("#version 330\nout vec3 color;\n void main() {}");
	shaders.emplace_back(new PostprocShader(context));
	shaders.back()->load("#version 330\nuniform sampler2D color; // vec3\n void main() {}");

	for(const auto& p : shaderSources) {
		shaderIds.emplace(p.first, shaders.size());
		shaders.emplace_back(new PostprocShader(context));
		shaders.back()->load(p.second);
	}

	return shaderIds;
}

std::vector<std::vector<int>> PostprocPipelineLoader::loadMapping(const std::unordered_map<std::string, int>& shaderIds, const std::string& mappingSource) {
	std::vector<std::vector<int>> edges(shaders.size());
	const std::regex edgeRegex(R"regex((\w+)\.(\w+)[ \t]+(\w+)\.(\w+)\n)regex");
	const std::sregex_iterator end;
	for(std::sregex_iterator it(mappingSource.begin(), mappingSource.end(), edgeRegex); it != end; ++it) {
		const auto& match = *it;
		const auto& outputShaderName = match[1];
		const auto& inputShaderName  = match[3];
		const auto& outputName = match[2];
		const auto& inputName  = match[4];

		const auto outputId = shaderIds.find(outputShaderName);
		if(outputId == shaderIds.end()) {
			std::cerr << "Shader " << outputShaderName << " not found." << std::endl;
			continue;
		}
		const auto inputId = shaderIds.find(inputShaderName);
		if(inputId == shaderIds.end()) {
			std::cerr << "Shader " << inputShaderName << " not found." << std::endl;
			continue;
		}

		const auto& outputShader = shaders[outputId->second];
		const Output* output = outputShader->findOutput(outputName);
		if(output == nullptr) {
			std::cerr << "Shader " << outputShaderName << " has no output named " << outputName << std::endl;
			continue;
		}
		const auto& inputShader = shaders[inputId->second];
		const Input* input = inputShader->findInput(inputName);
		if(input == nullptr) {
			std::cerr << "Shader " << inputShaderName << " has no input named " << inputName << std::endl;
			continue;
		}

		if(input->components != output->components) {
			std::cerr << outputShaderName << "." << outputName << " and "
				<< inputShaderName << "." << inputName
				<< " don't have a matching number of components!" << std::endl;
			continue;
		}

		// get index of output in output shader
		if(input->bindingId != -1) {
			std::cerr << inputShaderName << "." << inputName << " has multiple outputs assigned!" << std::endl;
			continue;
		}
		// store output shader local output id temporarily as bindingId
		inputShader->setInputBindingId(input - &inputShader->getInputs()[0],
				output - &outputShader->getOutputs()[0]);

		// insert inverse edge
		edges[inputId->second].push_back(outputId->second);
	}
	return edges;
}

std::vector<int> PostprocPipelineLoader::createOrder(const std::vector<std::vector<int>>& mapping) {
	std::vector<int> outdegree(shaders.size(), 0);

	// calculate indegree of the from output reachable subgraph using DFS
	// note that we are working on the inverse graph, so this actually is the outdegree
	std::stack<int> s;
	s.push(1);
	int nodeCounter = 0;
	while(!s.empty()) {
		nodeCounter++;
		const int node = s.top();
		s.pop();
		for(int origin : mapping[node]) {
			if(outdegree[origin]++ == 0) {
				// node is unvisited
				s.push(origin);
			}
		}
	}

	// shaders[1] is output node, so outdegree should be 0
	// TODO assertions!

	std::vector<int> order;
	std::queue<int> q;
	q.push(1);
	while(!q.empty()) {
		nodeCounter--;
		const int node = q.front();
		q.pop();
		order.push_back(node);
		for(int origin : mapping[node]) {
			if(--outdegree[origin] == 0) {
				q.push(origin);
			}
		}
	}

	if(nodeCounter > 0) {
		// error - cycle detected
		std::cerr << "Cyclic dependency in postprocessing detected" << std::endl;
		order.clear();
	}

	for(const int shaderId : order) {
		const auto& inputs = shaders[shaderId]->getInputs();
		const auto& unboundInput = [] (const Input& input) { return input.bindingId == -1; };
		if(std::any_of(inputs.begin(), inputs.end(), unboundInput)) {
			// error - not all inputs bound
			std::cerr << "Input has no output assigned" << std::endl;
			order.clear();
		}
	}

	std::reverse(order.begin(), order.end());
	return order;
}

void PostprocPipelineLoader::connectStages(const std::vector<int>& order, const std::vector<std::vector<int>>& mapping) {
	int nextBindingId = 0;
	for(const int shaderId : order) {
		auto& shader = shaders[shaderId];
		auto& outputs = shader->getOutputs();
		for(int i = 0; i < outputs.size(); i++) {
			shader->setOutputBindingId(i, nextBindingId++);
		}

		auto& inputs = shader->getInputs();
		// implicit assertion of this loop:
		// shader->getInputs()[i] corresponds to mapping[shaderId][id]
		// TODO verify order matches
		for(int i = 0; i < inputs.size(); i++) {
			// replace output local output id with global binding id
			const int outputShaderId = mapping[shaderId][i];
			// holy shit, hope this works
			shader->setInputBindingId(i, shaders[outputShaderId]->getOutputs()[inputs[i].bindingId].bindingId);
		}
	}
	for(const int shaderId : order) {
		// TODO verify assumption that position in inputPositions corresponds to shaderId
		shaders[shaderId]->insertBindings();
	}
}
