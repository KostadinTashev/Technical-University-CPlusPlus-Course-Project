#include <iostream>
#include <cstring>
#include<fstream>
#include <iomanip>
#include <string>
#include <algorithm> 


class Constituency {
private:
	char* constituencyName;
	int votersCount;
	int partiesCount;
	int* votesPerParty;
	char** partyNames;

public:
	// Constructor for the Constituency class, executed when an object is created.
	Constituency(const char* name, int voters, int parties)
		: votersCount(voters), partiesCount(parties) {
		constituencyName = new char[strlen(name) + 1];
		strcpy_s(constituencyName, strlen(name) + 1, name);

		votesPerParty = new int[partiesCount];
		for (int i = 0; i < partiesCount; ++i) {
			votesPerParty[i] = 0;
		}

		partyNames = new char* [partiesCount];
		for (int i = 0; i < partiesCount; ++i) {
			partyNames[i] = nullptr;
		}
	}

	// Destructor for the Constituency class, executed when an object is destroyed.
	~Constituency() {
		delete[] constituencyName;
		delete[] votesPerParty;

		for (int i = 0; i < partiesCount; ++i) {
			delete[] partyNames[i];
		}
		delete[] partyNames;
	}

	// Method to set the votes for a particular party
	void setVotesForParty(int partyIndex, int votes) {
		if (partyIndex >= 0 && partyIndex < partiesCount) {
			votesPerParty[partyIndex] = votes;
		}
		else {
			std::cout << "Invalid party index!" << std::endl;
		}
	}

	// Method to get the name of the constituency
	const char* getConstituencyName() const {
		return constituencyName;
	}
	// Method to get the number of voters in the constituency
	int getVotersCount() const {
		return votersCount;
	}

	// Method to get the number of parties in the constituency
	int getPartiesCount() const {
		return partiesCount;
	}

	// Method for entering party data in a constituency.
	void inputPartyData() {
		// Inputting the constituency name.
		std::cout << "Enter constituency name: ";
		char name[100];
		std::cin.getline(name, 100);
		constituencyName = new char[strlen(name) + 1];
		strcpy_s(constituencyName, strlen(name) + 1, name);

		// Inputting the number of voters and parties.
		std::cout << "Enter number of voters for the constituency: ";
		std::cin >> votersCount;
		std::cin.ignore();

		std::cout << "Enter number of parties: ";
		std::cin >> partiesCount;
		std::cin.ignore();

		// Creating arrays for party names and their respective votes.
		partyNames = new char* [partiesCount];
		votesPerParty = new int[partiesCount];

		for (int i = 0; i < partiesCount; ++i) {
			partyNames[i] = new char[100];

			// Inputting the party name.
			std::cout << "Enter party name " << i + 1 << ": ";
			std::cin.getline(partyNames[i], 100);

			// Inputting the votes for each party.
			std::string votesStr;
			std::cout << "Enter votes for party " << partyNames[i] << ": ";
			std::getline(std::cin, votesStr);

			try {
				votesPerParty[i] = std::stoi(votesStr);
			}
			catch (const std::exception& e) {
				std::cout << "Invalid input for votes. Please enter a number." << std::endl;
				votesPerParty[i] = 0;
			}
		}
		std::cout << "--------------------------------------------\n";
	}

	// Method to save constituency information to a file.
	static void saveConstituenciesToFile(Constituency** constituencies, int numberOfConstituencies, const char* filename) {
		std::ofstream outFile(filename);
		if (outFile.is_open()) {
			for (int i = 0; i < numberOfConstituencies; ++i) {
				outFile << "Constituency Name -> " << constituencies[i]->getConstituencyName() << "\n"
					<< "Voters -> " << constituencies[i]->getVotersCount() << "\n"
					<< "Parties -> " << constituencies[i]->getPartiesCount() << "\n\n";
				for (int j = 0; j < constituencies[i]->getPartiesCount(); ++j) {
					float percentage = (static_cast<float>(constituencies[i]->votesPerParty[j]) / static_cast<float>(constituencies[i]->votersCount)) * 100.0f;
					outFile << "Party: " << constituencies[i]->partyNames[j] << ", Percentage of Votes: " << std::fixed << std::setprecision(2) << percentage << "%\n";
				}

				outFile << "\n";
			}


			outFile.close();
			std::cout << "Successfully saved to " << filename << std::endl;
		}
		else {
			std::cout << "Failed to open " << filename << " for writing!" << std::endl;
		}
		std::cout << "--------------------------------------------\n";
	}

	// Method to display party percentages for a constituency.
	void displayPartyPercentages() const {
		std::cout << "Constituency Name: " << constituencyName << "\n";
		for (int i = 0; i < partiesCount; ++i) {
			float percentage = (static_cast<float>(votesPerParty[i]) / static_cast<float>(votersCount)) * 100.0f;
			std::cout << "Party: " << partyNames[i] << ", Percentage of Votes: " << std::fixed << std::setprecision(2) << percentage << "%\n";
		}
		std::cout << "\n";
		std::cout << "--------------------------------------------\n";
	}

	// Operator overloading (<) to compare Constituency objects based on their names.
	bool operator<(const Constituency& other) const {
		return std::strcmp(constituencyName, other.constituencyName) < 0;
	}


	// Method to identify constituencies without a clear winner.
	static void constituenciesWithoutWinner(Constituency** constituencies, int constituenciesCount) {
		for (int i = 0; i < constituenciesCount; ++i) {
			bool hasWinner = false;
			int maxVotes = 0;
			for (int j = 0; j < constituencies[i]->partiesCount; ++j) {
				if (constituencies[i]->votesPerParty[j] > maxVotes) {
					maxVotes = constituencies[i]->votesPerParty[j];
					hasWinner = true;
				}
				else if (constituencies[i]->votesPerParty[j] == maxVotes) {
					hasWinner = false; // Има равенство, няма ясен победител
				}
			}
			if (!hasWinner) {
				std::cout << "No clear winner in constituency: " << constituencies[i]->getConstituencyName() << std::endl;
			}
		}
		std::cout << "--------------------------------------------\n";
	}

	// Method to calculate the total votes for a constituency.
	int sumVotes() const {
		int totalVotes = 0;
		for (int i = 0; i < partiesCount; ++i) {
			totalVotes += votesPerParty[i];
		}
		return totalVotes;
	}

	// Method to write constituencies without a clear winner to a file.
	static void writeConstituenciesWithoutWinner(Constituency** constituencies, int constituenciesCount, const char* filename) {
		std::ofstream outputFile(filename);
		if (!outputFile.is_open()) {
			std::cerr << "Error opening file for recording!" << std::endl;
			return;
		}

		for (int i = 0; i < constituenciesCount; ++i) {
			bool hasWinner = false;
			int maxVotes = 0;

			for (int j = 0; j < constituencies[i]->getPartiesCount(); ++j) {
				if (constituencies[i]->votesPerParty[j] > maxVotes) {
					maxVotes = constituencies[i]->votesPerParty[j];
					hasWinner = true;
				}
				else if (constituencies[i]->votesPerParty[j] == maxVotes) {
					hasWinner = false;
				}
			}

			if (!hasWinner) {
				outputFile << "Constituency name: " << constituencies[i]->getConstituencyName() << std::endl;
				float nonVotersPercentage = static_cast<float>(constituencies[i]->getVotersCount() - constituencies[i]->sumVotes()) / constituencies[i]->getVotersCount() * 100.0;
				outputFile << "Percentage of non-voters: " << std::fixed << std::setprecision(2) << nonVotersPercentage << "%" << std::endl;
				outputFile << std::endl;
			}
		}

		outputFile.close();
		std::cout << "Successfully saved to " << filename << std::endl;
	}

};
int main() {
	// Getting the number of constituencies from the user.
	int numberOfConstituencies;
	std::cout << "Enter number of constituencies: ";
	std::cin >> numberOfConstituencies;
	std::cin.ignore();
	std::cout << "--------------------------------------------\n";

	// Creating an array of pointers to Constituency objects.
	Constituency** constituencies = new Constituency * [numberOfConstituencies];

	// Inputting data for each constituency.
	for (int i = 0; i < numberOfConstituencies; ++i) {
		constituencies[i] = new Constituency("", 0, 0);

		std::cout << "Entering data for constituency " << i + 1 << ":\n";
		constituencies[i]->inputPartyData();
	}

	// Saving constituency details to a file.
	Constituency::saveConstituenciesToFile(constituencies, numberOfConstituencies, "constituency.txt");

	// Sorting constituencies based on their names.
	std::sort(constituencies, constituencies + numberOfConstituencies, [](Constituency* a, Constituency* b) {
		return *a < *b;
		});

	// Displaying party percentages for each constituency.
	for (int i = 0; i < numberOfConstituencies; ++i) {
		constituencies[i]->displayPartyPercentages();
	}

	// Finding constituencies without a clear winner and displaying them.
	std::cout << "Constituencies without a winner:\n";
	Constituency::constituenciesWithoutWinner(constituencies, numberOfConstituencies);

	// Writing constituencies without a clear winner to a file.
	Constituency::writeConstituenciesWithoutWinner(constituencies, numberOfConstituencies, "non-winner.txt");

	// Freeing memory by deleting dynamically allocated objects.
	for (int i = 0; i < numberOfConstituencies; ++i) {
		delete constituencies[i];
	}
	delete[] constituencies;

	return 0;
}
