#include "Query.h"
#include "TextQuery.h"
#include <memory>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<QueryBase> QueryBase::factory(const string& s)
{

    regex oneWord("^\\b[\\w']+\\b$");
    regex notWord("^\\bNOT [\\w']+\\b$");
    regex wordAndWord("^\\b[\\w']+ AND [\\w']+\\b$");
    regex wordOrWord("^\\b[\\w']+ OR [\\w']+\\b$");
    regex wordNumWord("^\\b[\\w']+ \\d+ [\\w']+\\b$");

    if(regex_match(s,oneWord))
        return std::shared_ptr<QueryBase>(new WordQuery(s));

    else if(regex_match(s, notWord)){
        istringstream f(s);
        string x="";
        string word;
        while (getline(f, x, ' ')) {
            if(x.compare("NOT")!=0)
            word=x;
        }
        return std::shared_ptr<QueryBase>(new NotQuery(word));
    }

    else if(regex_match(s, wordAndWord)){
        istringstream f(s);
        string words[2] ;
        string x="";
        string word;
        int i=0;
        while (getline(f, x, ' ')) {
            if(x.compare("AND")!=0){
                words[i]=x;
                i++;
            }
        }

        return std::shared_ptr<QueryBase>(new AndQuery(words[0],words[1]));
    }
    else if(regex_match(s, wordOrWord)){
        istringstream f(s);
        string words[2] ;
        string x="";
        string word;
        int i=0;
        while (getline(f, x, ' ')) {
            if(x.compare("OR")!=0){
                words[i]=x;
                i++;
            }
        }

        return std::shared_ptr<QueryBase>(new OrQuery(words[0],words[1]));

    }
    else if(regex_match(s, wordNumWord)){

        istringstream f(s);
        string words[3] ;
        string x="";
        string word;
        int i=0;
        while (getline(f, x, ' ')) {
                words[i]=x;
                i++;

        }
        int distance=stoi(words[1]);

        return std::shared_ptr<QueryBase>(new NQuery(words[0],words[2],distance));
    }
    else{
        cout<<"Unrecognized search"<<endl;


    }





    // if(s == "smart") return std::shared_ptr<QueryBase>(new WordQuery("smart"));
}
////////////////////////////////////////////////////////////////////////////////
QueryResult NotQuery::eval(const TextQuery &text) const
{
  QueryResult result = text.query(query_word);
  auto ret_lines = std::make_shared<std::set<line_no>>();
  auto beg = result.begin(), end = result.end();
  auto sz = result.get_file()->size();
  
  for (size_t n = 0; n != sz; ++n)
  {
    if (beg==end || *beg != n)
		ret_lines->insert(n);
    else if (beg != end)
		++beg;
  }
  return QueryResult(rep(), ret_lines, result.get_file());
    
}

QueryResult AndQuery::eval (const TextQuery& text) const
{
  QueryResult left_result = text.query(left_query);
  QueryResult right_result = text.query(right_query);
  
  auto ret_lines = std::make_shared<std::set<line_no>>();
  std::set_intersection(left_result.begin(), left_result.end(),
      right_result.begin(), right_result.end(), 
      std::inserter(*ret_lines, ret_lines->begin()));

  return QueryResult(rep(), ret_lines, left_result.get_file());
}

QueryResult OrQuery::eval(const TextQuery &text) const
{
  QueryResult left_result = text.query(left_query);
  QueryResult right_result = text.query(right_query);
  
  auto ret_lines = 
      std::make_shared<std::set<line_no>>(left_result.begin(), left_result.end());

  ret_lines->insert(right_result.begin(), right_result.end());

  return QueryResult(rep(), ret_lines, left_result.get_file());
}
/////////////////////////////////////////////////////////
QueryResult NQuery::eval(const TextQuery &text) const
{

}
/////////////////////////////////////////////////////////