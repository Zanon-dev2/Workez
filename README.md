# Workez
Uma biblioteca que estou desenvolvendo pra facilitar minha vida.
A biblioteca atualmente conta com uma função para somar que foi feita para testes, uma função que usa a crow pra simplificar as routes da crow junto da adição simples de comunicação entre arquivos com parametros e um input de lista.
Ainda está em desenvolvimento e sendo desenvolvida bem lentamente, e sem um único propósito, apenas simplifica coisas que foram problemas ou chatas para mim.
funcionamento:
Zlist:
cpp
#include "workez.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<int> numeros;
    std::cout << "Digite uma lista de numeros separados por espaco: ";
    // Lê toda a entrada de uma vez e popula o vector
    workez::zlist>>numeros; 
    std::cout<<"Total de itens lidos: "<<numeros.size()<<"\n";
}
cpp
Soma():
"#include "workez.hpp"
#include <vector>
#include <iostream>
int main(){
  std::vector<double> lista = {1.2, 2.2, 5.7, 9.9};
  //faz a soma de todos os itens da lista
  double total = soma(lista);
  std::cout<<total;
}"
piper:
"#include "workez.hpp"
#include <iostream>

int main() {
    // 1. Dar permissao ao executavel "calculadora.exe" na categoria "utilitarios"
    workez::piper.parlist.add("utilitarios", "calculadora");

    // 2. Chamar o executavel passando um dado de entrada (ex: "5 + 5")
    std::string retorno = workez::piper.call("5 + 5", "calculadora", "utilitarios");
    
    std::cout << "Saida do programa externo: " << retorno << "\n";

    // 3. Remover a permissao se necessario
    workez::piper.parlist.del("utilitarios", "calculadora");
}```
APIR:
"#include "workez.hpp"

int main() {
    crow::SimpleApp app;

    // Criando uma rota GET simples que retorna uma string de forma direta
    workez::APIR<std::string>(app, "GET", [](req) {
        return "Hello World!";
    }, "/ola");

  //também podemos escrever a lambda por fora
  auto testar = [](req) {
        // ... sua logica de processamento aqui ...
    }, "/salvar");

    // Criando uma rota POST que retorna void (o status 200 e enviado sozinho)
    workez::APIR<void>(app, "POST", testar);
    app.port(18080).multithreaded().run();
}"

#depende da biblioteca Crow e foi pensado pro sistema operacional Windows
Sem a crow a APIR para de funcionar e fora do Windows a piper não funcioa, C++17 ou maior
