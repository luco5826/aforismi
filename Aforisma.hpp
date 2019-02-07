#ifndef AFORISMA_H
#define AFORISMA_H
#include <string>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>

using boost::algorithm::contains;
using std::string;

namespace afo
{
class Aforisma
{
  private:
    string frase;
    string autore;

  public:
    Aforisma(string frase, string autore)
    {
        this->frase = frase;
        this->autore = autore;
    }

    ~Aforisma()
    {
        this->frase = nullptr;
        this->autore = nullptr;
    }

    string getFrase()
    {
        return this->frase;
    }

    string getAutore()
    {
        return this->autore;
    }
};

std::vector<Aforisma *> popolateVectorFromFile(string fileName)
{
    std::ifstream input;
    input.open(fileName);

    std::vector<Aforisma *> aforismi;

    std::string data;
    std::string frase;
    std::string autore;

    while (getline(input, data))
    {
        //Remove any ' charcater in order to not interfere with SQL syntax
        boost::replace_all(data, "\'", "");
        boost::replace_all(data, "\\", "");
        boost::trim(data);

        //If is the author line
        if (contains(data, "("))
        {
            autore = data;
            aforismi.push_back(new Aforisma(frase, autore));
            frase.clear();
        }
        else
        {
            frase.append(data);
        }
    }
    input.close();

    return aforismi;
}
} // namespace afo
#endif

