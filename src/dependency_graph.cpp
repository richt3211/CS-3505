// Hi


#include "dependency_graph.h"
#include <string>



Backend::dependency_graph::dependency_graph()
{
  
  //depends_on_graph;
  
  //depended_on_by_graph;

  num_pairs=0;

}

Backend::dependency_graph::~dependency_graph()
{
  //depends_on_graph = NULL;
  
  //depended_on_by_graph = NULL;

  num_pairs=0;

}

int Backend::dependency_graph::get_size()
{
  return this->num_pairs;
}

int Backend::dependency_graph::get_size_of_dependees(std::string input)
{


  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found = depends_on_graph.find(input);
  

  if (found == depends_on_graph.end())
    {
      return 0;
    }
  else
    {
      return (found->second).size();
    }
}

bool Backend::dependency_graph::has_dependents(std::string input)
{
  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found = depended_on_by_graph.find(input);
  

  if (found == depended_on_by_graph.end())
    {
      return false;
    }
  else
    {
      return (found->second).size() > 0;
    }
}

bool Backend::dependency_graph::has_dependees(std::string input)
{

  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found = depends_on_graph.find(input);
  

  if (found == depends_on_graph.end())
    {
      return false;
    }
  else
    {
      return (found->second).size() > 0;
    }
}






void Backend::dependency_graph::get_dependents(std::string input)
{

  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found = depended_on_by_graph.find(input);
  

  if (found == depended_on_by_graph.end())
    {
      //Return empty iterator
    }
  else
    {
      //return (found->second.begin());
    }

}

void Backend::dependency_graph::get_dependees(std::string input)
{

  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found = depends_on_graph.find(input);
  

  if (found == depends_on_graph.end())
    {
      //return //Empty iterator
    }
  else
    {
      //return (depends_on_graph[found->second].begin());
    }

}



void Backend::dependency_graph::add_dependency(std::string first_par, std::string second_par)
{
  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found_depends = depends_on_graph.find(second_par);
  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found_depended = depended_on_by_graph.find(first_par);

  
  // Dealing with first parameter
  if (found_depends != depends_on_graph.end())
    {
      (found_depends->second).insert(first_par);
    }
  else
    {
      std::pair<std::string, std::unordered_set<std::string>> new_pair (second_par, std::unordered_set<std::string>);
      depends_on_graph.insert(new_pair);
      found_depends->second.insert(first_par);
    }



  // Dealing with second parameter
  if (found_depended != depended_on_by_graph.end())
    {
      found_depended->second.insert(second_par);
    }
  else
    {
      std::pair<std::string, std::unordered_set<std::string>> new_pair (first_par, std::unordered_set<std::string>);
      depended_on_by_graph.insert(new_pair);
      found_depended->second.insert(second_par);
    }

  

}

void Backend::dependency_graph::remove_dependency(std::string first_par, std::string second_par)
{
  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found_depends = depends_on_graph.find(second_par);
  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found_depended = depended_on_by_graph.find(first_par);

  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found_s = depends_on_graph[found->second].find(first_par);
  std::unordered_map<std::string, std::unordered_set<std::string>>::const_iterator found_t = depended_on_by_graph[found->second].find(second_par);

  if (found_depended != depended_on_by_graph.end() && found_depends != depends_on_graph.end())
    {
      if (found_s != found->second.end() && found_t != found->second.end())
	{
	  found->second.erase(second_par);
	  found->second.erase(first_par);
	}
    }
  

}



