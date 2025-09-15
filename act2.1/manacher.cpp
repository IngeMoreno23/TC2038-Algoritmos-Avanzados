#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

struct PalindromeInfo{
	int size;
	int sizeWithSpaces;
	int beginIndex;
	int endIndex;
	std::string palindrome;

	PalindromeInfo() : size(), sizeWithSpaces(), beginIndex(), endIndex(), palindrome() {}

	PalindromeInfo(int size, int sizeWithSpaces, int beginIndex, int endIndex, std::string palindrome){
		this->size = size;
		this->sizeWithSpaces = sizeWithSpaces;
		this->beginIndex = beginIndex;
		this->endIndex = endIndex;
		this->palindrome = palindrome;
	}

	friend std::ostream& operator<<(std::ostream& os, const PalindromeInfo& info) {
        os << "{ size: " << info.size
		   << ", sizeWithSpaces: " << info.sizeWithSpaces
           << ", beginIndex: " << info.beginIndex
           << ", endIndex: " << info.endIndex
           << ", palindrome: '" << info.palindrome << "' }";
        return os;
    }
};

/*
Utiliza el algoritmo Manacher para encontrar el palindromo más largo.
Genera una ventana y centro que crece a medida que encuentra coincidencias,
si la posicion actual se concidera simétrica, copia el valor de un indice anterior
para no recalcular coincidencias.
Complejidad: O(n)
*/
PalindromeInfo largestPalindrome(const std::string& text){
	std::string cleaned;
	std::vector<int> mapToOriginal;
	// Maps the cleaned string to the original text to ignore spaces	
    for (int i = 0; i < (int)text.size(); i++) {
        char c = text[i];
        if (c != ' ' && c != '\t') {
            cleaned.push_back(c);
            mapToOriginal.push_back(i);
        }
    }

	int e = 2*cleaned.size() + 3;

	std::string newText;
	newText.reserve(e);

	newText.push_back('@');
	for(char c : cleaned){
		newText.push_back('$'); newText.push_back(c);
	}
	newText.push_back('$'); newText.push_back('#');

	int* P = new int[e]();
	int maxIndex = 0;
	int maxLenght = 0;

	int center = 0;
	int limit = 0;
	for(int i = 1; i < e-1; i++){
		if(i < limit){
			int simetric = 2 * center - i;
			P[i] = std::min(limit - i, P[simetric]);
		}
		int gap = P[i] + 1;
		while(newText[i-gap] == newText[i+gap]){
			P[i]++;
			gap++;
		}
		if(i + P[i] > limit){
			limit = i + P[i];
			center = i;
		}
		if(P[i] > maxLenght){
			maxLenght = P[i];
			maxIndex = i;
		}
	}

	// DEBUG

	for(int i = 0; i < e; i++){
		std::cout << newText[i] << " ";
	}
	std::cout << "\n";
	for(int i = 0; i < e; i++){
		std::cout << P[i] << " ";
	}
	std::cout << "\n";

	// ENDDEBUG

	int beginIndex = floor((maxIndex - P[maxIndex] - 1) / 2);
	int endIndex = maxLenght + beginIndex - 1;

	int originalBeginIndex = mapToOriginal[beginIndex];
	int originalEndIndex = mapToOriginal[endIndex];

	int originalLenght = originalEndIndex - originalBeginIndex + 1;

	std::string palindrome = text.substr(originalBeginIndex, originalLenght); 

	delete[] P;

	return PalindromeInfo(maxLenght, originalLenght, originalBeginIndex, originalEndIndex+1, palindrome);
}


int main(){
	std::cout << "Algoritmo Manacher para Palíndromos\n";
	std::string text = "ble ble somos o no somos ñiñi";

	PalindromeInfo palindromeInfo = largestPalindrome(text);

	std::cout << "Searching the largest palindrome in: " << text << "\n";
	std::cout << palindromeInfo << "\n";

	return 0;
}