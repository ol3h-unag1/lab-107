import <iostream>;
import <fstream>;

import <vector>;

import <map>;
import <unordered_map>;

import <string>;
import <string_view>;

import <filesystem>;

import <algorithm>;

import <ranges>;

import <source_location>;

import Misc;

using std::begin;
using std::cbegin;

using std::end;
using std::cend;

auto const build_log { "build_target_log.txt" };
auto const output_file{ "warnigns.csv" };

std::size_t const C_expected_capacity{ 4096u };

std::string const C_diagnostic_string_first_symbols{ "/." };
auto const C_diagnostic_string_last_symbol{ ']' };

bool IsDiagnosticString(std::string_view str)
{
    if (str.empty())
    {
        return false;
    }

    return str.back() == C_diagnostic_string_last_symbol &&
           (std::isalpha(str.front()) ||
           std::any_of(cbegin(C_diagnostic_string_first_symbols), cend(C_diagnostic_string_first_symbols), 
                [str](auto const& c)
                {
                    return c == str.front();
                }));
}

auto const C_warning_label{ "warning:" };

bool IsWarningString(std::string_view str)
{
    return IsDiagnosticString(str) &&
        std::string::npos != str.find(C_warning_label);
}

auto const C_path_begin_symbol{ '/' };
auto const C_path_end_symbol{ ':' };

// extract path
std::string ExtPath(std::string_view str)
{
    return std::string{
        std::find(cbegin(str), cend(str), C_path_begin_symbol),
        std::find(cbegin(str), cend(str), C_path_end_symbol) };
}

auto const C_line_column_separator{ ':' };
auto const C_column_warning_separator{ ':' };

// returns pair< line, column >
auto ExtLineColumn(std::string_view str)
{
    std::pair< std::string, std::string > result;
    auto& line{ result.first };
    auto& column{ result.second };

    if (auto path_end{ std::find(cbegin(str), cend(str), C_path_end_symbol) }; 
        path_end != str.cend())
    {
        auto line_begin{ path_end + 1 };
        auto line_end{ std::find(line_begin, cend(str), C_line_column_separator) };
        line = std::string{ line_begin, line_end};

        if (line_end != str.cend())
        {
            auto column_begin{ line_end + 1 };
            column = std::string{ column_begin, std::find(column_begin, cend(str), C_column_warning_separator) };
        }
    }

    return result;
}

auto GetMessageBegin(std::string_view str)
{
    // end of warning label will be begin of the message
    auto C_warning_label_begin_index{ str.find(C_warning_label) };
    return std::string::npos == C_warning_label_begin_index ?
        str.cend() :
        str.cbegin() + C_warning_label_begin_index + strlen(C_warning_label);
}

auto const C_message_end_symbol{ '[' };

auto GetMessageEnd(std::string_view str)
{
    return std::find(GetMessageBegin(str), cend(str), C_message_end_symbol);
}

std::string ExtMessage(std::string_view str)
{
    return InplaceRoundTrim(std::string{ GetMessageBegin(str), GetMessageEnd(str)});
}

auto GetDescriptionBlockBegin(std::string_view str)
{
    return GetMessageEnd(str);
}

auto const C_description_block_first_symbol{ '[' };
auto const C_description_block_last_symbol{ ']' };
auto const C_option_description_separator{ ',' };

auto ExtWarningDescription(std::string_view str)
{
    std::pair< std::string, std::string > result;
    auto& option{ result.first };
    auto& description{ result.second };

    std::string description_block{ GetDescriptionBlockBegin(str), cend(str) };
    description_block = InplaceRoundTrim(std::move(description_block), C_description_block_first_symbol, C_description_block_last_symbol);

    auto separator{ std::find(cbegin(description_block), cend(description_block), C_option_description_separator) };
    option = std::string{ cbegin(description_block), separator };
    
    if (separator != cend(description_block))
    {
        separator++;
    }
        
    description = std::string{ separator, cend(description_block)};

    return result;
}

struct Warning
{                                    
    std::string broken_path;          // path that's broken by separator for each level of directory 
    std::string path;                 // path w/o filename 
    std::string filename;            
    std::string line;                
    std::string column;              
    std::string message;             
    std::string compiler_option;     // compiler option that enables that warning, e.g. -Wblablabla
    std::string warning_description; // human-readable description of compiler option
};

auto const C_warning_output_separator{ '^' };

std::ostream& operator<<(std::ostream& os, Warning const& w)
{
    os << w.broken_path << C_warning_output_separator << w.path << C_warning_output_separator << w.filename << C_warning_output_separator <<
          w.line << C_warning_output_separator << w.column << C_warning_output_separator << 
          w.message << C_warning_output_separator << w.compiler_option << C_warning_output_separator << w.warning_description;
    return os;
}

std::size_t GetBrokenPathLength(std::string const& path)
{
    return std::count(cbegin(path), cend(path), C_warning_output_separator);
}

void NormalizePath(std::string& path, std::size_t currLen, std::size_t const normalizedLen)
{
    while (currLen < normalizedLen)
    {
        path.insert(cbegin(path), C_warning_output_separator);
        currLen++;
    }
}

void PrintIfEmpty(Warning const& w, std::size_t line)
{
    if (w.broken_path.empty())
    {
        std::cout << "brokenPath is empty on line " << line << std::endl;
    }
    if (w.path.empty())
    {
        std::cout << "path is empty on line " << line << std::endl;
    }
    if (w.filename.empty())
    {
        std::cout << "filename is empty on line " << line << std::endl;
    }
    if (w.line.empty())
    {
        std::cout << "line is empty on line " << line << std::endl;
    }
    if (w.column.empty())
    {
        std::cout << "column is empty on line " << line << std::endl;
    }
    if (w.message.empty())
    {
        std::cout << "message is empty on line " << line << std::endl;
    }
    if (w.compiler_option.empty())
    {
        std::cout << "option is empty on line " << line << std::endl;
    }
    if (w.warning_description.empty())
    {
        std::cout << "description is empty on line " << line << std::endl;
    }
}

auto BreakPath(std::string const& path)
{
    auto const path_without_filename{ std::filesystem::path(path).remove_filename() };
    std::string broken_path{};
    for (auto const& p : path_without_filename)
    {
        broken_path += p.string() + C_warning_output_separator;
    }

    auto front_symbols_to_remove = { C_path_begin_symbol, C_warning_output_separator };
    auto remove_begin_pred = [&broken_path](auto const& c) { return broken_path.front() == c; };
    while (not broken_path.empty() && std::any_of(cbegin(front_symbols_to_remove), cend(front_symbols_to_remove), remove_begin_pred))
    {
        broken_path.erase(cbegin(broken_path));
    }

    while (not broken_path.empty() && (broken_path.back() == C_warning_output_separator))
    {
        broken_path.pop_back();
    }

    return broken_path;
}

void ParseInputFileWarnings(std::ifstream& is)
{
    std::unordered_map< std::string, std::size_t > warnings_ratio{};

    std::size_t longest_broken_path_length{ 0u }; // needed to normalize all broken path Lengths
    std::size_t current_input_file_line{ 0u };

    std::vector< Warning > warnings;
    warnings.reserve(C_expected_capacity);

    std::string input_str;
    while (std::getline(is, input_str))
    {
        if (not IsWarningString(input_str))
        {
            continue;
        }

        auto path{ ExtPath(input_str) };
        auto const filename{ std::filesystem::path(path).filename().string() };
        auto const [line, column] { ExtLineColumn(input_str)};
        auto const message{ ExtMessage(input_str) };
        auto const [option, description] { ExtWarningDescription(input_str) };
  
        auto const broken_path{ BreakPath(path) };

        longest_broken_path_length = std::max(longest_broken_path_length, GetBrokenPathLength(broken_path));

        warnings.emplace_back(broken_path, path, filename, line, column, message, option, description);

        PrintIfEmpty(warnings.back(), current_input_file_line);

        warnings_ratio[option]++;

        current_input_file_line++;
    }
    is.close();
    is.clear();

    // #1 common transform
    //std::transform(begin(warnings), end(warnings), begin(warnings),
    //    [longest_broken_path_length](auto& warning) -> auto&
    //    {
    //        NormalizePath(warning.broken_path, GetBrokenPathLength(warning.broken_path), longest_broken_path_length);
    //        return warning;
    //    });

    // #2 simple range-for
    //for (auto& warning : warnings)
    //{
    //    NormalizePath(warning.broken_path, GetBrokenPathLength(warning.broken_path), longest_broken_path_length);
    //};

    // #3 views (perfect)
    for (auto& br_path : warnings |
        std::views::transform(&Warning::broken_path))
    {
        NormalizePath(br_path, GetBrokenPathLength(br_path), longest_broken_path_length);
    }


    std::cout << warnings.size() << " warnings parsed." << std::endl;

    std::ofstream os{ output_file };
    
    for (auto const& warning : warnings)
    {
        os << warning << "\n";
    }
    
    std::map< std::size_t, std::string > sorted_warnings_ratio;
    for (auto const& wr : warnings_ratio)
    {
        sorted_warnings_ratio[wr.second] = wr.first;
    }

    for (auto const& swr : sorted_warnings_ratio)
    {
        std::cout << swr.second << " " << swr.first << std::endl;
    }
}

int main()
{
    if (not std::filesystem::exists(build_log))
    {
        std::cout << build_log << " doesn't exists" << std::endl;
        return 1;
    }

    if (std::ifstream is{ build_log })
    {
        ParseInputFileWarnings(is);
    }
  
    return 0;
}

// outdated
void TestStringParser()
{
    auto const warning_string_example{ "/opt/vsomeip/lib/cmake/vsomeip/../../../include/vsomeip/export.hpp:9:5: warning: '_WIN32' is not defined, evaluates to 0 [-Wundef,Lexical or Preprocessor Issue]" };
    std::cout << warning_string_example << std::endl;
    std::cout << IsDiagnosticString(warning_string_example) << std::endl;
    std::cout << IsWarningString(warning_string_example) << std::endl;
    std::cout << ExtPath(warning_string_example) << std::endl;
    auto [line, column] { ExtLineColumn(warning_string_example)};
    std::cout << line << " : " << column << std::endl;
    std::cout << ExtMessage(warning_string_example) << std::endl;
    auto [option, description] { ExtWarningDescription(warning_string_example) };
    std::cout << option << " | " << description << std::endl;

    std::filesystem::path p{ ExtPath(warning_string_example) };
    std::cout << p.filename().string() << std::endl;
}