#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>

std::vector<int> compactChars(const std::string& text, bool& canTerminate){
	std::map<char, int> m;
    for (char c : text){
		m[c] = 0;
	}

	int compactedInt = 1;
	for(auto& [c, val] : m){
		val = compactedInt++;
	}

	canTerminate = compactedInt >= (int)text.size();

	std::vector<int> compacted;
	compacted.reserve(text.size());
	for(char c : text){
		compacted.push_back(m[c]);
	}

	return compacted;
}

std::vector<int> compactTuples(const std::vector<std::pair<int, int>>& tuples, bool& canTerminate){
	std::map<std::pair<int, int>, int> m;
    for (auto p : tuples){
		m[p] = 0;
	}

	int compactedInt = 0;
	for(auto& [c, val] : m){
		val = compactedInt++;
	}

	canTerminate = compactedInt >= (int)tuples.size();
	// std::cout << "Can terminate: compactedInt = " << compactedInt << " tuples.size() = " << (int)tuples.size() << "\n";

	std::vector<int> compacted;
	compacted.reserve(tuples.size());
	for(auto p : tuples){
		compacted.push_back(m[p]);
	}

	return compacted;
}

/*make run
Fuente: https://www.youtube.com/watch?v=_TUeAdu-U_k
Se ordena lexicográficamente utilizando un map que ordena las tuplas al insertarse.
Estas se compactan en identificadores ordenados por tupla y solamente se termina
hasta que cada tupla / id sean distintos, eso quiere decir que no hay ambiguedad
y que no se necesita comparar más.
Complejidad: O(nlog(n))
*/
std::vector<int> suffixArray(const std::string& text){
	int n = text.size();
	bool canTerminate = false;

	std::vector<int> compacted = compactChars(text, canTerminate);

	// DEBUG compacted chars
	/*
	for(auto p : tuples){
		std::cout << p.first << "-" << p.second << " ";
	}
	std::cout << "\n";
	*/
	// ENDDEBUG

	// First Iteration

	std::vector<std::pair<int, int>> tuples;
	tuples.reserve(n);

	for(int i = 0; i < n; i++){
		tuples.emplace_back(compacted[i], (i+1 < n ? compacted[i+1]: -1));
	}

	compacted = compactTuples(tuples, canTerminate);

	// DEBUG compacted chars
	/*
	for(int i : compacted){
		std::cout << i << " ";
	}
	std::cout << "\n";
	*/
	// ENDDEBUG

	// Next iterations

	while(!canTerminate){
		std::vector<std::pair<int, int>> tuples2;
		tuples2.reserve(n);
		for(int i = 0; i < n; i++){
			tuples2.emplace_back(compacted[i], (i+2 < n ? compacted[i+2]: -1));
		}
		compacted = compactTuples(tuples2, canTerminate);
		// DEBUG compacted chars
		/*
		for(int i : compacted){
			std::cout << i << " ";
		}	
		std::cout << "\n";
		*/
		// ENDDEBUG
	}

	std::vector<int> sA(n, 0);

	for(int i = 0; i < n; i++){
		sA[compacted[i]] = i;
	}

	// DEBUG Suffix Array
	/*
	for(int i : sA){
		std::cout << i << " ";
	}
	std::cout << "\n";
	*/
	// ENDDEBUG

	return sA;
}

int main(){
	std::cout << "Suffix Array\n";

	std::string text = "jhbeqwhjerbajdbfakjhdhb"; // trollolol prueba del video

	std::vector<int> sA = suffixArray(text);

	std::cout << "Getting the Suffix Array of: " << text << "\n";

	for(int index : sA){
		std::cout << index << ": ";
		for(int i = index; i < (int)text.size(); i++){
			std::cout << text[i];
		}
		std::cout << "\n";
	}

	return 0;
}