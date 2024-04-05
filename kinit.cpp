#include <iostream>
#include <stdio.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

bool plain_html_project = false;
std::string html_name = "index.html";
std::string css_name = "style.css";
std::string js_name = "main.js";
// for c++ projects
bool cpp_project = false;
std::string cpp_name = "";
// for python projects
bool pyproject = "";
std::string py_name = "";
std::vector<std::string> librarys_to_include = {};

void projectinit()
{
    if (plain_html_project)
    {
        std::ofstream htmlfile(html_name);
        std::ofstream cssfile(css_name);
        std::ofstream jsfile(js_name);
        htmlfile << "<!DOCTYPE html>\n"
                    "<html>\n"
                    "   <head>\n"
                    "       <meta charset=\"utf-8\">\n"
                    "       <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
                    "       <link rel=\"stylesheet\" href=\""
                 << css_name << "\">\n"
                                "       <meta name=\"description\" content=\"\">\n"
                                "       <title>document</title>\n"
                                "   </head>\n"
                                "<body>\n\n\n"
                                "<script src=\""
                 << js_name << "\"></script>\n"
                               "</body>\n"
                               "</html>";

        cssfile << "body{\n    background-color: black;\n    color: white;\n}";
        jsfile << "console.log(\"Hello World!\")";
    }
    if (cpp_project)
    {
        std::ofstream cppfile(cpp_name);
        cppfile << "#include <iostream>\nint main(){\n    std::cout << \"Hello World!\" << std::endl;\n    return 0;\n}";
    }
    if (pyproject)
    {
        std::ofstream pyfile(py_name);
        pyfile << "print(\"Hello World!\")";
    }
}
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "enough parameters not specfied" << std::endl;
    }
    for (int i = 0; i < argc; i++)
    {
        if (std::string(argv[i]) == "-html")
        {
            plain_html_project = true;
            if (argv[i + 1])
            {
                html_name = argv[i + 1];
            }
            if(argv[i+2]){
            css_name = argv[i + 2];
            }
            if(argv[i+3]){
            js_name = argv[i + 3];
            }
            js_name = argv[i + 3];
            std::cout << "found html tag" << std::endl;
        }
        if (argv[i] == "-cpp")
        {
            cpp_project = true;
            cpp_name = argv[i + 1];
        }
        if (argv[i] == "-py")
        {
            pyproject = true;
            py_name = argv[i + 1];
        }
    }
    auto start_time = std::chrono::high_resolution_clock::now();
    projectinit();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << std::endl;
    std::cout << "\033[1;32mSuccess:\033[0m project created" << std::endl;
    std::cout << duration.count() << " ms " << std::endl;
    return 0;
}