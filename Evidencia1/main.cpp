#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <cmath>

const std::string SARS_COV_2_2019_PATH = "Evidencia1/resources/SARS-COV-2-MN908947.3.txt";
const std::string SARS_COV_2_2020_PATH = "Evidencia1/resources/SARS-COV-2-MT106054.1.txt";

const std::string GEN_M_PATH = "Evidencia1/resources/gen-M.txt";
const std::string GEN_ORF1AB_PATH = "Evidencia1/resources/gen-ORF1AB.txt";
const std::string GEN_S_PATH = "Evidencia1/resources/gen-S.txt";

// Mapa {codón, aminoacido}
const std::unordered_map<std::string, std::string> codonMap = {
    // Alanina (A)
    {"GCT", "A"}, {"GCC", "A"}, {"GCA", "A"}, {"GCG", "A"},
    // Arginina (R)
    {"CGT", "R"}, {"CGC", "R"}, {"CGA", "R"}, {"CGG", "R"}, {"AGA", "R"}, {"AGG", "R"},
    // Asparagina (N)
    {"AAT", "N"}, {"AAC", "N"},
    // Ácido Aspártico (D)
    {"GAT", "D"}, {"GAC", "D"},
    // Cisteína (C)
    {"TGT", "C"}, {"TGC", "C"},
    // Glutamina (Q)
    {"CAA", "Q"}, {"CAG", "Q"},
    // Ácido Glutámico (E)
    {"GAA", "E"}, {"GAG", "E"},
    // Glicina (G)
    {"GGT", "G"}, {"GGC", "G"}, {"GGA", "G"}, {"GGG", "G"},
    // Histidina (H)
    {"CAT", "H"}, {"CAC", "H"},
    // Isoleucina (I)
    {"ATT", "I"}, {"ATC", "I"}, {"ATA", "I"},
    // Leucina (L)
    {"CTT", "L"}, {"CTC", "L"}, {"CTA", "L"}, {"CTG", "L"}, {"TTA", "L"}, {"TTG", "L"},
    // Lisina (K)
    {"AAA", "K"}, {"AAG", "K"},
    // Metionina (M) / INICIO
    {"ATG", "M"},
    // Fenilalanina (F)
    {"TTT", "F"}, {"TTC", "F"},
    // Prolina (P)
    {"CCT", "P"}, {"CCC", "P"}, {"CCA", "P"}, {"CCG", "P"},
    // Serina (S)
    {"TCT", "S"}, {"TCC", "S"}, {"TCA", "S"}, {"TCG", "S"}, {"AGT", "S"}, {"AGC", "S"},
    // Treonina (T)
    {"ACT", "T"}, {"ACC", "T"}, {"ACA", "T"}, {"ACG", "T"},
    // Triptófano (W)
    {"TGG", "W"},
    // Tirosina (Y)
    {"TAT", "Y"}, {"TAC", "Y"},
    // Valina (V)
    {"GTT", "V"}, {"GTC", "V"}, {"GTA", "V"}, {"GTG", "V"},
    // STOP
    {"TAA", "*"}, {"TGA", "*"}, {"TAG", "*"}
};

struct SequenceInfo{
    std::string info;
    std::string sequence;

    friend std::ostream& operator<<(std::ostream& os, const SequenceInfo& si) {
        os << "Info: " << si.info << "\n";
        os << "Sequence: " << si.sequence.substr(0, 12) << (si.sequence.size() > 12 ? "..." : "");
        return os;
    }
};

std::ostream& operator<<(std::ostream& os, const std::pair<int,int>& p) {
    return os << "{" << p.first << ", " << p.second << "}";
}

/*
Crea un LPS (Longest Prefix Sufix) y cada vez que falla en
una comparación del KMP, se regresa al indice LPS[j-1] del
patrón para verificar evitar iterar el patrón desde el inicio
Complejidad: O(n)
*/
std::vector<std::pair<int, int>> stringMatch(const std::string& text, const std::string& pattern){
    int n = text.size();
    int m = pattern.size();

    if(n < m || n <= 0 || m <= 0) return {}; // No empty strings or longer pattern lenght

    std::vector<std::pair<int, int>> result{};

    // Generate LPS
    int* LPS = new int[m]();
    LPS[0] = 0;
    
    for(int i = 1; i < m; i++){
        int len = LPS[i-1]; // up to match lenght
        while (len > 0 && pattern[i] != pattern[len]) {
            len = LPS[len - 1]; // fall back
        }
        if(pattern[i] == pattern[len]) LPS[i] = len+1;
    }

    // Bebug
    /*
    std::cout << "Debug LPS:\n";
    for(int i = 0; i < m; i++){
        std::cout << pattern[i] << " ";
    }
    std::cout << "\n";
    for(int i = 0; i < m; i++){
        std::cout << LPS[i] << " ";
    }
    std::cout << "\n";
    */
    // ENDDEBUG

    // KMP

    int i = 0;
    int j = 0;
    while(i < n){
        if(text[i] == pattern[j]){
        i++; j++;
        } else {
        if(j > 0){
            j = LPS[j-1];
        } else {
            i++;
        }
        }

        if(j == m){
        result.emplace_back(i-m, i);
        j = LPS[j-1];
        }
    }

    delete[] LPS;
    
    return result;
}
  
std::pair<int, int> stringMatchFirst(const std::string& text, const std::string& pattern) {
    auto all = stringMatch(text, pattern);
    if (!all.empty()) return all.front();
    return {-1, -1};
}

// Print match and underline matches
void printFormatedStringMatch(const SequenceInfo& text, const SequenceInfo& pattern, const std::vector<std::pair<int, int>>& matches){
    std::cout << "Buscando en la secuencia de virus: '" << text.info << "'\n";
    std::cout << "Gen buscado: '" << pattern.info << "'\n";

    // print pairs
    std::cout << "Matches: ";
    if(matches.size() > 0){
    for(auto pair : matches){
        std::cout << "\n" << pair;
    }
    } else {
    std::cout << "No match found :(";
    }
    std::cout << "\n";

    // Primeros 2 caracteres
    int i = 0;
    std::cout << "Primeros 12 caracteres: ";
    while(i < (int)pattern.sequence.size() && i < 12){
        std::cout << pattern.sequence[i];
        i++;
    }

    std::cout << "\n";
}

SequenceInfo readSequenceFile(const std::string& fileName){
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("File '" + fileName +"' could not openbe opened.");
    }
    SequenceInfo sequenceInfo{};
    if(!std::getline(file, sequenceInfo.info)) throw std::runtime_error("Empty header on '" + fileName +"' file.");

    std::string line = "";
    std::ostringstream oss;
    while (std::getline(file, line)) {
        oss << line;
    }
    sequenceInfo.sequence = oss.str();

    file.close();

    return sequenceInfo;
}

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
    /*
	for(int i = 0; i < e; i++){
		std::cout << newText[i] << " ";
	}
	std::cout << "\n";
	for(int i = 0; i < e; i++){
		std::cout << P[i] << " ";
	}
	std::cout << "\n";
    */
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
	std::cout << "E1. Actividad Integradora 1 (Avance)\n\n";

    std::cout << "--------------------------------------------------\n";
    std::cout << "| 1. Aparición de genes en SARS-COV-2-MN908947.3 |\n";
    std::cout << "--------------------------------------------------\n";
	SequenceInfo sars_sequence = readSequenceFile(SARS_COV_2_2019_PATH);

	SequenceInfo genM_sequence = readSequenceFile(GEN_M_PATH);
    std::vector<std::pair<int, int>> matches_genM = stringMatch(sars_sequence.sequence, genM_sequence.sequence);
    printFormatedStringMatch(sars_sequence, genM_sequence, matches_genM);
    std::cout << "\n";

    SequenceInfo genORF_sequence = readSequenceFile(GEN_ORF1AB_PATH);
    std::vector<std::pair<int, int>> matches_genORF = stringMatch(sars_sequence.sequence, genORF_sequence.sequence);
    printFormatedStringMatch(sars_sequence, genORF_sequence, matches_genORF);
    std::cout << "\n";

    SequenceInfo genS_sequence = readSequenceFile(GEN_S_PATH);
    std::vector<std::pair<int, int>> matches_genS = stringMatch(sars_sequence.sequence, genS_sequence.sequence);
    printFormatedStringMatch(sars_sequence, genS_sequence, matches_genS);
    std::cout << "\n";

    std::cout << "--------------------------------------------------\n";
    std::cout << "|            2. Palindromo más largo              |\n";
    std::cout << "--------------------------------------------------\n";

    std::cout << genM_sequence.info << "\n";
    std::cout << largestPalindrome(genM_sequence.sequence) << "\n\n";
    std::cout << genM_sequence.info << "\n";
    std::cout << largestPalindrome(genORF_sequence.sequence) << "\n\n";
    std::cout << genM_sequence.info << "\n";
	std::cout << largestPalindrome(genS_sequence.sequence) << "\n\n";
	

	return 0;
}