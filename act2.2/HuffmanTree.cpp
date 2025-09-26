#include "HuffmanTree.hpp"

Node::Node(char c, float prob): c(c), prob(prob), l(nullptr), r(nullptr) {}
Node::Node(Node* nodeL, Node* nodeR): c('\0'), prob(nodeL->prob + nodeR->prob), l(nodeL), r(nodeR) {}

/*
Comparator for priority_queue:
Priority = lower probability, then lower char alphabetical order
*/
bool Comparador::operator()(const Node* a, const Node* b) const {
    if(a->prob == b->prob){
        return (int)a->c > (int)b->c;
    }
    return a->prob > b->prob; 
}

void HuffmanTree::printTree(Node* root, int depth) {
	if(!root) return;
	printTree(root->r, depth + 1);
	for(int i = 0; i < depth; i++) std::cout << "\t";

	std::cout << "[";
	if(root->c == '\n'){
		std::cout << "\\n";
	} else if(root->c == '\t'){
		std::cout << "\\t";
	} else if(root->c == ' '){
		std::cout << "_";
	} else {
		std::cout << root->c;
	}
	std::cout << ":" << std::fixed << std::setprecision(2) << root->prob << "]\n";
	printTree(root->l, depth + 1);
}

void HuffmanTree::collectLeafNodes(Node* node, std::vector<std::pair<char, float>>& leafNodes){
	if(!node) return;
    
	if(node->c != '\0'){
		leafNodes.push_back({node->c, node->prob});
		return;
	}

	collectLeafNodes(node->l, leafNodes);
	collectLeafNodes(node->r, leafNodes);
}

/*
Crea un HuffmanTree a travez de un map (char, prob)
utilizando una priority_queue que mantiene los nodos ordenados
para crear el árbol con los nodos de probabilidad más baja hasta
el fondo del arbol
Complejidad O(nlog(n)) : n son los caracteres únicos
*/
HuffmanTree::HuffmanTree(const std::string& text): text(text){
	int n = text.size();
	if(n < 1){
		root = nullptr;
		encoderMap = {};
		decoderMap = {};
		return;
	}

	// Generate the initial queue with char probabilities
	std::unordered_map<char, int> map;
	std::priority_queue<Node*, std::vector<Node*>, Comparador> queue{};
	for(int i = 0; i < n; i++){
		map[text[i]] += 1;
	}
	for(auto pair : map){
		queue.push(new Node(pair.first, pair.second / (float)n));
	}

	// Generate the HuffmanTree until only the root node is left
	while(queue.size() > 1){
		// Create a new node, and add the first 2 elements (already sorted) as children
		Node* nodeL = queue.top(); queue.pop();
		Node* nodeR = queue.top(); queue.pop();
		queue.push(new Node(nodeL, nodeR));
	}
	root = queue.top();
	queue.pop();

	// Generate Encoder
	std::queue<std::pair<Node*, std::string>> queue2;
	queue2.push({root, ""});
	std::string prefix = "";

	while(!queue2.empty()){
		auto [current, prefix] = queue2.front();
		queue2.pop();
		if(!current) continue;
		if(current->c != '\0'){
			encoderMap[current->c] = prefix;
		} else {
			if (current->l) queue2.push({current->l, prefix + "0"});
			if (current->r) queue2.push({current->r, prefix + "1"});
		}
	}

	// Generate Decoder
	for(auto [c, val] : encoderMap){
		decoderMap[val] = c;
	}
}

HuffmanTree* HuffmanTree::loadFromBin(const std::string& filename){
	std::ifstream file(filename, std::ios::binary);
	if(!file.is_open()){
		std::cerr << "Error: Could not open file " << filename << "\n";
		return nullptr;
	}
	
	// HEADER SIZE
	uint32_t headerSize;
	file.read(reinterpret_cast<char*>(&headerSize), sizeof(uint32_t));
	if(file.fail()){
		std::cerr << "Error: Failed to read header size.\n";
		file.close();
		return nullptr;
	}
	
	// HEADER DATA
	std::unordered_map<char, float> cProbMap;
	for(uint32_t i = 0; i < headerSize; i++){
		char c;
		float prob;
        
		file.read(&c, sizeof(char));
		file.read(reinterpret_cast<char*>(&prob), sizeof(float));
        
		if(file.fail()){
			std::cerr << "Error: Failed to read c-prob pair " << i << ".\n";
			file.close();
			return nullptr;
		}
        
		cProbMap[c] = prob;
	}

	// TEXT SIZE
	uint32_t textSize;
	file.read(reinterpret_cast<char*>(&textSize), sizeof(uint32_t));
	if(file.fail()){
		std::cerr << "Error: Failed to read text size.\n";
		file.close();
		return nullptr;
	}

	// Pre ENCODE - Generate decoderMap
	// Generate the initial queue with char probabilities
	Node* root = nullptr;
	std::unordered_map<char, std::string> encoderMap;
    std::unordered_map<std::string, char> decoderMap;

	std::priority_queue<Node*, std::vector<Node*>, Comparador> queue{};

	for(auto pair : cProbMap){
		queue.push(new Node(pair.first, pair.second));
	}

	// Generate the HuffmanTree until only the root node is left
	while(queue.size() > 1){
		// Create a new node, and add the first 2 elements (already sorted) as children
		Node* nodeL = queue.top(); queue.pop();
		Node* nodeR = queue.top(); queue.pop();
		queue.push(new Node(nodeL, nodeR));
	}
	root = queue.top();
	queue.pop();

	// Generate Encoder
	std::queue<std::pair<Node*, std::string>> queue2;
	queue2.push({root, ""});
	std::string prefix = "";

	while(!queue2.empty()){
		auto [current, prefix] = queue2.front();
		queue2.pop();
		if(!current) continue;
		if(current->c != '\0'){
			encoderMap[current->c] = prefix;
		} else {
			if (current->l) queue2.push({current->l, prefix + "0"});
			if (current->r) queue2.push({current->r, prefix + "1"});
		}
	}

	// Generate Decoder
	for(auto [c, val] : encoderMap){
		decoderMap[val] = c;
	}
	// ENCODED DATA
	std::string result = "";
	std::string currentVal = "";
	uint8_t currentByte = 0;
	uint8_t charCount = 0;
	while(file.read(reinterpret_cast<char*>(&currentByte), 1) && charCount < textSize){
		for (int i = 7; i >= 0; --i) {
			char bitChar = ((currentByte >> i) & 1) ? '1' : '0';
			currentVal += bitChar;

			if (decoderMap.contains(currentVal)) {
				result += decoderMap[currentVal];
				currentVal.clear();
				charCount++;
			}
		}
	}
	if(charCount < textSize) std::cerr << "'" << textSize - 1 - charCount << "' could not be parsed\n";

	return new HuffmanTree(result);
}

/*
Payload
[HEADER SIZE: uint32_t][HEADER DATA: (char, float)[]][TEXT SIZE: uint32_t][ENCODED DATA: bit[]]
*/
void HuffmanTree::saveToBin(const std::string& filename){
	std::ofstream file(filename, std::ios::binary);
	if(!file.is_open()){
		std::cerr << "Error: Could not open file " << filename << "\n";
		return;
	}

	std::vector<std::pair<char, float>> leafNodes;
	collectLeafNodes(root, leafNodes);

	// HEADER SIZE
	uint32_t headerSize = static_cast<uint32_t>(leafNodes.size());
	file.write(reinterpret_cast<const char*>(&headerSize), sizeof(uint32_t));
	
	// HEADER DATA
	for(const auto& [c, prob] : leafNodes){
		file.write(&c, sizeof(char));
		file.write(reinterpret_cast<const char*>(&prob), sizeof(float));
	}

	// TEXT SIZE
	std::string encodedText = encode();
	uint32_t textSize = static_cast<uint32_t>(encodedText.size());
	file.write(reinterpret_cast<const char*>(&textSize), sizeof(uint32_t));

	// ENCODED DATA
	if(encodedText.empty()) return;
    
	std::vector<uint8_t> bytes;
	uint8_t currentByte = 0;
	int bitCount = 0;
    
	for(char bit : encodedText){
		if(bit == '1'){
			currentByte |= (1 << (7 - bitCount));
		}
		bitCount++;
        
		if(bitCount == 8){
			bytes.push_back(currentByte);
			currentByte = 0;
			bitCount = 0;
		}
	}
    
	// Remaining bits
	if(bitCount > 0){
		bytes.push_back(currentByte);
	}

	file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

std::string HuffmanTree::encode(){
	std::string result = "";
	for(char c : text){
		result += encoderMap[c];
	}
	return result;
}

// ups
std::string HuffmanTree::decode(){
	return text;
}

std::string HuffmanTree::encode(const std::string& text){
	std::string result = "";
	for(char c : text){
		if(encoderMap.contains(c)){
			result += encoderMap[c];
		} else {
			std::cerr << "'" << c << "' not found on the HuffmanTree\n";
			break;
		}
	}
	return result;
}

std::string HuffmanTree::decode(const std::string& text){
	std::string result = "";
	std::string currentVal = "";
	for(char val : text){
		currentVal += val;
		if(decoderMap.contains(currentVal)){
			result += decoderMap[currentVal];
			currentVal.clear();
		}
	}
	if(!currentVal.empty()) std::cerr << "'" << currentVal << "' could not be parsed\n";
	return result;
}

void HuffmanTree::printEncoder(){
	std::cout << "Encoder map:\n";
	for(auto [c, val] : encoderMap){
		std::cout << c << " : " << val << "\n";
	}
}

void HuffmanTree::printDecoder(){
	std::cout << "Decoder map:\n";
	for(auto [val, c] : decoderMap){
		std::cout << val << " : " << c << "\n";
	}
}

void HuffmanTree::printTable(){
	std::vector<std::pair<char, float>> leafNodes;
	collectLeafNodes(root, leafNodes);
	std::cout << "Character Table:\n";
	std::cout << "Char\tFreq\tProb\tCode\n";
	std::cout << "----\t----\t----\t----\n";

	for(const auto& [c, prob] : leafNodes){
		int frequency = (int)(prob * text.size());
		std::string code = encoderMap.at(c);
		
		if(c == '\n'){
			std::cout << "\\n\t";
		} else if(c == '\t'){
			std::cout << "\\t\t";
		} else if(c == ' '){
			std::cout << "_\t";
		} else {
			std::cout << c << "\t";
		}
		
		std::cout << frequency << "\t" << std::fixed << std::setprecision(2) << prob << "\t" << code << "\n";
	}
}

void HuffmanTree::printTree(){
	std::cout << "HuffmanTree [char:probability]:\n";
	printTree(root);
}