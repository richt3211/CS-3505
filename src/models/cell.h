/**
 * Cell class for storing values of the spreadsheet.
 *
 *
 *
 */
#ifndef CELL_H
#define CELL_H

#include <string>
#include <vector>

namespace Backend
{
  class cell
  {
  public:
    cell(std::string input, std::vector<std::string> dependents);
    ~cell();
    void set_cell_value(std::string input);
    std::string get_cell_value();
    std::vector<std::string> get_cell_direct_dependents();
    void set_cell_direct_dependents(std::vector<std::string>);
  private:
    std::string current_value;
    std::vector<std::string> direct_dependents;
  };
}

#endif
