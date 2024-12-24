
// SELENAY HUR
// BİLGİSAYAR MÜHENDİSLİĞİ BİÇİMSEL DİLLER A GRUBU ÖDEVİ
//G221210088

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream> // Girdi işlemek için
using namespace std;

// DFA yapısını temsil eden sınıf
class DFA {
public:
    vector<string> states;                       // Durumlar
    string start_state;                          // Başlangıç durumu
    set<string> final_states;                    // Final durumları
    map<pair<string, char>, string> transitions; // Geçiş fonksiyonları

    // Kullanıcıdan DFA bilgilerini al
    void initialize() {
        int num_states;
        cout << "Durum sayisini giriniz: ";
        cin >> num_states;

        // Durumları oluştur
        for (int i = 1; i <= num_states; ++i) {
            states.push_back("q" + to_string(i));
        }

        // Başlangıç durumu belirle
        cout << "Baslangic durumunu giriniz (q1 - q" << num_states << "): ";
        cin >> start_state;

        // Final durumlarını belirle
        cout << "Final durumlarini giriniz (örnek: q1 q3): ";
        cin.ignore();
        string line;
        getline(cin, line);
        stringstream ss(line);
        string state;
        while (ss >> state) {
            final_states.insert(state);
        }

        // Geçişleri belirle
        cout << "Geçişleri giriniz (örnek: q1 a q2, q1 b q3):" << endl;
        for (const string& state : states) {
            for (char symbol : {'a', 'b'}) {
                string to_state;
                cout << state << " " << symbol << " -> ";
                cin >> to_state;
                transitions[{state, symbol}] = to_state;
            }
        }
    }

    // Denk durumları bul
    vector<set<string>> minimize() {
        // Başlangıç partition: Final ve final olmayan durumları ayır
        vector<set<string>> partitions = {final_states};
        set<string> non_final_states;
        for (const string& state : states) {
            if (final_states.find(state) == final_states.end()) {
                non_final_states.insert(state);
            }
        }
        if (!non_final_states.empty()) {
            partitions.push_back(non_final_states);
        }

        // Partitionları güncelle
        bool updated;
        do {
            updated = false;
            vector<set<string>> new_partitions;

            for (const auto& group : partitions) {
                map<map<char, int>, set<string>> refinement;
                for (const string& state : group) {
                    map<char, int> signature;
                    for (char symbol : {'a', 'b'}) {
                        string to_state = transitions[{state, symbol}];
                        signature[symbol] = find_partition(partitions, to_state);
                    }
                    refinement[signature].insert(state);
                }

                for (const auto& [_, refined_group] : refinement) {
                    new_partitions.push_back(refined_group);
                }
                if (refinement.size() > 1) {
                    updated = true;
                }
            }

            partitions = new_partitions;
        } while (updated);

        return partitions;
    }

    // Denk durumları yazdır
    void print_equivalent_states(const vector<set<string>>& partitions) {
        set<string> printed_states;

        cout << "Denk durumlar ve diğer durumlar:" << endl;

        // Denk durumlar
        for (const auto& group : partitions) {
            if (!group.empty()) {
                cout << "{ ";
                for (const string& state : group) {
                    cout << state << " ";
                    printed_states.insert(state); // Yazılanları işaretle
                }
                cout << "}" << endl;
            }
        }

        // Tek kalan durumlar
        for (const string& state : states) {
            if (printed_states.find(state) == printed_states.end()) {
                cout << "{ " << state << " }" << endl;
            }
        }
    }

private:
    // Bir durumun hangi partitionda olduğunu bul
    int find_partition(const vector<set<string>>& partitions, const string& state) {
        for (int i = 0; i < partitions.size(); ++i) {
            if (partitions[i].find(state) != partitions[i].end()) {
                return i;
            }
        }
        return -1; // Eğer bulunamazsa
    }
};

int main() {
    DFA dfa;
    dfa.initialize();

    cout << endl << "Durum indirgeme işlemi başlıyor..." << endl;
    vector<set<string>> partitions = dfa.minimize();
    dfa.print_equivalent_states(partitions);

    return 0;
}
