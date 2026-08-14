#pragma once
#include <crow.h>
#include <functional>
#include <iostream>
#include <cctype>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <windows.h>
#include <string>


//parte da workez com foco na comunicação entre arquivos

namespace workez{
    class comu{
        private:
        std::string outcm;
        //parametros pra não permitir qualquer chamada de executavel
        using segr = std::unordered_map<std::string, std::unordered_set<std::string>>;
        segr permit = {{}};

        public:
        comu() : parlist(*this){};
        std::string par();
        std::string call(const std::string& ent, const std::string& arq, const std::string& cate) {
            outcm = "";
            //verificação de segurança -> evitar que façam chamadas de executáveis que não existem
            auto itcate = permit.find(cate);
            if (itcate == permit.end()) {
                return "ERRO - CATEGORIA INEXISTENTE";
            }
            auto itarq = itcate->second.find(arq);
            if (itarq == itcate->second.end()) {
                return "ERRO 404 - ITEM NÂO EXISTE OU È PROÍBIDO";
            }
            //declarando os caminhos
            HANDLE inread = nullptr, inwrite = nullptr;
            HANDLE outread = nullptr, outwrite = nullptr;

            SECURITY_ATTRIBUTES sa{};
            sa.nLength = sizeof(sa);
            sa.bInheritHandle = true;
            CreatePipe(&inread, &inwrite, &sa, 0);
            CreatePipe(&outread, &outwrite, &sa, 0);
            SetHandleInformation(inwrite, HANDLE_FLAG_INHERIT, 0);
            SetHandleInformation(outread, HANDLE_FLAG_INHERIT, 0);
            STARTUPINFOA si{};
            si.cb = sizeof(si);
            si.dwFlags = STARTF_USESTDHANDLES;

            si.hStdInput = inread;
            si.hStdOutput = outwrite;
            si.hStdError = outwrite;

            PROCESS_INFORMATION pi{};
            std::string zcall = arq + ".exe";
            std::vector<char> comando(zcall.begin(), zcall.end());
            comando.push_back('\0');

            bool sucesso = CreateProcessA(
                nullptr,
                comando.data(),
                nullptr,
                nullptr,
                true,
                0,
                nullptr,
                nullptr,
                &si,
                &pi
            );
            //termina caso não tenha funcionado
            if(!sucesso){
                CloseHandle(inread);
                CloseHandle(inwrite);
                CloseHandle(outread);
                CloseHandle(outwrite);
                return "ERRO";
            }

            CloseHandle(inread);
            CloseHandle(outwrite);

            DWORD escr;
            std::string entq = ent + "\n";

            WriteFile(
                inwrite,
                entq.c_str(),
                static_cast<DWORD>(entq.size()),
                &escr,
                nullptr
            );

            CloseHandle(inwrite);

            char buffer[4096];
            DWORD rec;

            while (ReadFile(
                outread,
                buffer,
                sizeof(buffer) - 1,
                &rec,
                nullptr
            ) && rec > 0) {
                buffer[rec] = '\0';
                outcm += buffer;
            }
            CloseHandle(outread);


            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return outcm;
        }
        class parametros {
        private:
            comu& pai;
        public:
            parametros(comu& _pai) : pai(_pai){};
            void add(const std::string& cate, const std::string& itm) {
                pai.permit[cate].insert(itm);
            }
            void del(const std::string& cate, const std::string& itm) {
                auto it = pai.permit.find(cate);
                if (it != pai.permit.end()) {
                    it->second.erase(itm);
                    if (it->second.empty()) {
                        pai.permit.erase(it);
                    }
                }
            }
        };
        parametros parlist;
    };
    inline comu piper;
    template<typename T, typename AppType>
    void APIR(AppType &app, const std::string &metd, std::function<T(const crow::request&)> exec, std::string dir = "/") {
        std::string mtd = metd;
        std::transform(mtd.begin(), mtd.end(), mtd.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        crow::HTTPMethod rmethod;
        if (mtd == "get")        rmethod = crow::HTTPMethod::Get;
        else if (mtd == "put")   rmethod = crow::HTTPMethod::Put;
        else if (mtd == "del")   rmethod = crow::HTTPMethod::Delete;
        else if (mtd == "patch") rmethod = crow::HTTPMethod::Patch;
        else                     rmethod = crow::HTTPMethod::Post;

        app.route_dynamic(dir).methods(rmethod)([exec](const crow::request& req) {
            if constexpr (std::is_same_v<T, crow::response>) {
                return exec(req);
            }
            // 2. Se T é void
            else if constexpr (std::is_same_v<T, void>) {
                exec(req);
                return crow::response(200);
            }
            // 3. Se T é conversível para string ou é um JSON (wvalue)
            else if constexpr (std::is_convertible_v<T, std::string> || std::is_same_v<T, crow::json::wvalue>) {
                return crow::response(200, exec(req));
            }
            // 4. Caso genérico para tipos numéricos (int, float, etc)
            else {
                return crow::response(200, std::to_string(exec(req)));
            }
        });
    }
}