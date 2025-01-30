#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>

constexpr short int num_prioridades = 6, quantum = 4;

using namespace std;

class Processo {
    private:
        short int tempo_execucao, prioridade;
        string id, comando, usuario;

    public:
        Processo(string& id, short int& tempo_execucao, short int& prioridade, string& comando, string& usuario) : id(id), tempo_execucao(tempo_execucao), 
        prioridade(prioridade), comando(comando), usuario(usuario) { }

        inline void reduzirTempo(void) {
            this->tempo_execucao--;
        }

        inline short int tempoRestante(void) {
            return this->tempo_execucao;
        }

        inline short int obterPrioridade(void) {
            return this->prioridade;
        }

        inline string obterId(void) {
            return this->id;
        }

        inline string obterComando(void) {
            return this->comando;
        }

        inline string obterUsuario(void) {
            return this->usuario;
        }
};

string removerAspas(string& s) {
    string aspas = "”";
    for (char c : aspas)
        s.erase(remove(s.begin(), s.end(), c), s.end());
    return s;
}

int main(void) { 
    
    vector<vector<Processo>> filas(6);

    ifstream arquivo("processos.txt");  

    if (!arquivo) {
        cerr << "Erro na abertura do arquivo" << endl;
        return 1;
    }

    string linha;

    while (getline(arquivo, linha)) {
        linha = removerAspas(linha);

        stringstream ss(linha);
        string id, tempo_execucao, prioridade, comando, usuario;

        getline(ss, id, ';');
        getline(ss, tempo_execucao, ';');
        getline(ss, prioridade, ';');
        getline(ss, comando, ';');
        getline(ss, usuario, ';');

        short int prioridade_int = stoi(prioridade);
        short int tempo_execucao_int = stoi(tempo_execucao);
        short int pos = num_prioridades - prioridade_int;

        filas[pos].push_back(Processo(id, tempo_execucao_int, prioridade_int, comando, usuario));
    }
    
    for (int i = 0; i < num_prioridades; i++) {
        short int indice = 0;
        while (filas[i].size()) {
            short int cont = quantum;
            bool finalizado = false;
            Processo& processo_atual = filas[i][indice];
            
            while (true) {
                system("clear");
                if (!cont)
                    break;

                if (!processo_atual.tempoRestante()) {
                    finalizado = true;
                    break;
                }

                processo_atual.reduzirTempo();
                cont--;

                cout << "Executando processo "  << processo_atual.obterId() << " - Tempo restante: " << processo_atual.tempoRestante() << endl;
                this_thread::sleep_for(chrono::milliseconds(500));
            }

            if (finalizado) {
                cout << "*** Processo " << processo_atual.obterId() << " concluido ***" << endl;
                filas[i].erase(filas[i].begin() + indice);
                this_thread::sleep_for(chrono::seconds(1));

                if (filas[i].empty())
                    break;
            }

            indice = (indice == (filas[i].size() - 1)) ? 0 : indice + 1;
        }
    }

    arquivo.close();

    system("clear");
    cout << "Todos os processos foram executados com sucesso!" << endl;

     return 0;
}