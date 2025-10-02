#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <exception>
#include <memory> // Vamos a probar smart pointers profe, las buenas practicas de programación hacen buenos programadores
#include <utility>

class DictTrie{
private:
	struct Node{
		char c;
		bool isEnd;
		std::unordered_map<char, std::unique_ptr<Node>> children;

		Node(char c, bool isEnd = false): c(c), isEnd(isEnd) {}

		Node* addChar(char c, bool isEnd = false){
			auto it = children.find(c);
			if(it == children.end()){ // Not in children
				auto newNode = std::make_unique<Node>(c, isEnd);
				Node* rawPtr = newNode.get();
				children[c] = std::move(newNode);
				return rawPtr;
			} else {
				// If isEnd, its a new word to keep in mind
				if(isEnd) it->second->isEnd = true;
				return it->second.get();
			}
		}
	};

	std::unique_ptr<Node> root;

	void recursivePrint(Node* node, std::string prefix, bool isLast){
		if(!node) return;

		char c;
		switch (node->c) {
			case '\0': c = '#'; break;
			case '\n': c = 'n'; break;
			case '\t': c = 't'; break;
			case '\r': c = 'r'; break;
			default: c = node->c; break;
		}

		std::cout << prefix;
		std::cout << (isLast ? char(192) : char(195)) << char(196) << char(196); // └ ├ ─
		std::cout << c;
		std::cout << (node->isEnd ? "(*)\n" : "\n");

		prefix += (isLast ? ' ' : char(179)) + std::string("  "); // │

		auto it = node->children.begin();
		auto end = node->children.end();

		while (it != end) {
			auto& childNode = it->second;
			if (std::next(it) == end) {
				recursivePrint(childNode.get(), prefix, true);
			} else {
				recursivePrint(childNode.get(), prefix, false);
			}
			++it;
		}
	}

	void recursiveBuildWord(Node* node, std::string prefix, std::vector<std::string>& out){
		for(auto& [c, childNode] : node->children){
			if(childNode.get()->isEnd) out.push_back(prefix + c);
			recursiveBuildWord(childNode.get(), prefix + c, out);
		}
	}

public:
	DictTrie(const std::vector<std::string>& words){
		root = std::make_unique<Node>('\0'); 

		for(const std::string& word : words){
			if(word.empty()) continue;

			Node* current = root.get();
			for (size_t i = 0; i < word.size(); i++) {
                bool isEnd = (i == word.size() - 1);
                current = current->addChar(word[i], isEnd);
            }
		}
	}

	/*
	brief:
		Busca la palabra 'word' en este DictTrie, si la encuentra completa devuelve true
		y si encuentra otras palabras con 'word' como sufijo las regresa en el vector 'out'
		Complejidad: O(n*m) autocompletador
			n: cantidad de palabras
			m: tamaño de la palabra más larga
	params:
		const std::string& word : Cadena a buscar en el árbol
		std::vector<std::string>>& out : Vector donde se guardan los autocompletados
										Palabras con prefijo 'word' (excluyendo 'word')
	return:
		bool : true si se encuentra la cadena 'word' exacta
				false si existe como prefijo o no existe completa
	*/
	bool trySearchWord(const std::string& word, std::vector<std::string>& out){
		Node* current = root.get();
		bool fullWord = true;

		for(char c : word){
			if(current->children.contains(c)){
				current = current->children[c].get();
			} else {
				fullWord = false;
				break;
			}
		}

		recursiveBuildWord(current, word, out);

		return fullWord && current->isEnd;
	}

	void print(){
		std::cout << "#\n";
		
		auto it = root->children.begin();
		auto end = root->children.end();

		while (it != end) {
			auto& childNode = it->second;
			if (std::next(it) == end) {
				recursivePrint(childNode.get(), "", true);
			} else {
				recursivePrint(childNode.get(), "", false);
			}
			++it;
		}
	}
};

class SuffixTrie{
private:
	struct Node{
		char c;
		std::vector<int> occurrences{};
		std::unordered_map<char, std::unique_ptr<Node>> children;

		Node(char c): c(c) {}

		Node* addChar(char c, int index){
			auto it = children.find(c);
			Node* rawPtr = nullptr;
			if(it == children.end()){ // Not in children
				auto newNode = std::make_unique<Node>(c);
				rawPtr = newNode.get();
				children[c] = std::move(newNode);
			} else { // On children
				rawPtr = it->second.get();
			}
			rawPtr->occurrences.push_back(index);
			return rawPtr;
		}
	};

	std::unique_ptr<Node> root;

	void recursivePrint(Node* node, std::string prefix, bool isLast){
		if(!node) return;

		char c;
		switch (node->c) {
			case '\0': c = '#'; break;
			case '\n': c = 'n'; break;
			case '\t': c = 't'; break;
			case '\r': c = 'r'; break;
			default: c = node->c; break;
		}

		std::cout << prefix;
		std::cout << (isLast ? char(192) : char(195)) << char(196) << char(196); // └ ├ ─
		std::cout << c;
		std::cout << " { ";
		if(!node->occurrences.empty()) std::cout << node->occurrences[0];
		for(size_t i = 1; i < node->occurrences.size(); i++){
			std::cout << ", " << node->occurrences[i];
		}
		std::cout << " }\n";

		prefix += (isLast ? ' ' : char(179)) + std::string("  "); // │

		auto it = node->children.begin();
		auto end = node->children.end();

		while (it != end) {
			auto& childNode = it->second;
			if (std::next(it) == end) {
				recursivePrint(childNode.get(), prefix, true);
			} else {
				recursivePrint(childNode.get(), prefix, false);
			}
			++it;
		}
	}

public:
	SuffixTrie(const std::string& word){
		root = std::make_unique<Node>('\0');

		for(size_t i = 0; i < word.size(); i++){
			Node* current = root.get();
			for(size_t j = i; j < word.size(); j++){
				current = current->addChar(word[j], j);
			}
		}
	}

	/*
	brief:
		Busca si la subcaneda existe en el árbol y si existe completa desde la raiz
		quiere decir que existe como sufijo, y se guardan una lista de ocurrencias
		Complejidad: O(n)
			n: tamaño de substring
	params:
		const std::string& substring : string a buscar en el árbol
	return:
		std::vector<std::pair<int, int>> : indices inicio y fin inclusivos de las apariciones de substring
	*/
	std::vector<std::pair<int, int>> findOccurrences(const std::string& substring){
		std::vector<std::pair<int, int>> occurrences{};

		Node* current = root.get();
		bool fullWord = true;

		for(char c : substring){
			if(current->children.contains(c)){
				current = current->children[c].get();
			} else {
				fullWord = false;
				break;
			}
		}
		
		if(fullWord){
			for(size_t i = 0; i < current->occurrences.size(); i++){
				occurrences.emplace_back(current->occurrences[i] - substring.size() +1, current->occurrences[i]);
			}
		}

		return occurrences;
	}

	void print(){
		std::cout << "#\n";
		
		auto it = root->children.begin();
		auto end = root->children.end();

		while (it != end) {
			auto& childNode = it->second;
			if (std::next(it) == end) {
				recursivePrint(childNode.get(), "", true);
			} else {
				recursivePrint(childNode.get(), "", false);
			}
			++it;
		}
	}
};

int main(){
	std::cout << "Arboles Trie\n";

	std::cout << "------------------------------\n";
	std::cout << "|      Diccionario Trie      |\n";
	std::cout << "------------------------------\n";

	int wordsOnDict;
	std::cout << "Number of words on DictTrie: ";
	if(!(std::cin >> wordsOnDict) || wordsOnDict < 0) throw std::runtime_error("Invalid Input!");

	std::vector<std::string> wordsVector;
	wordsVector.reserve(wordsOnDict);
	std::cin.sync();
	for(int i = 0; i < wordsOnDict; i++){
		std::string word = "";
		std::getline(std::cin, word);
		wordsVector.push_back(word);
	}

	DictTrie dictTrie(wordsVector);
	dictTrie.print();

	std::string wordToFind = "";
	std::vector<std::string> autoCompletions;
	std::cout << "Find and autocomplete: ";
	std::cin.sync();
	std::getline(std::cin, wordToFind);
	std::cout << (dictTrie.trySearchWord(wordToFind, autoCompletions) ? "Word Found :)" : "Word not Found :(") << "\n";

	if(autoCompletions.empty()){
		std::cout << "No autocompletions Found :cc\n";
	} else {
		std::cout << "{ " << autoCompletions[0];
		for(size_t i = 1; i < autoCompletions.size(); i++){
			std::cout << ", " << autoCompletions[i];
		}
		std::cout << " }\n";
	}
	std::cout << "\n";

	std::cout << "------------------------------\n";
	std::cout << "|        Suffix Trie         |\n";
	std::cout << "------------------------------\n";

	std::string suffixWord = "";
	std::cout << "Suffix Tree Word: ";
	std::cin.sync();
	std::getline(std::cin, suffixWord);

	SuffixTrie suffixTrie(suffixWord);
	suffixTrie.print();

	std::string suffixSubstring = "";
	std::cout << "Substring to search: ";
	std::cin.sync();
	std::getline(std::cin, suffixSubstring);
	auto occurrences = suffixTrie.findOccurrences(suffixSubstring);

	std::cout << " { ";
	if(!occurrences.empty()) std::cout << "[" << occurrences[0].first << ", " << occurrences[0].second << "]";
	for(size_t i = 1; i < occurrences.size(); i++){
		std::cout << ", [" << occurrences[i].first << ", " << occurrences[i].second << "]";
	}
	std::cout << " }\n";

	return 0;
}