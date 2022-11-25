import <iostream>;
import <fstream>;

import <vector>;

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

bool breakPath{ true }; // to break (or not) path in directories for output "/home/user/source.cpp" -> "home" "user" "source.cpp"

auto const inputFile { "build_target_log_plain_edited.txt" };
auto const outputFile{ "warnigns.csv" };

std::string const diagnosticStringFirstSpecialSymbols{ "/." };
auto const diagnosticStringLastSymbol{ ']' };

bool IsDiagnosticString(std::string_view str)
{
    if (str.empty())
    {
        return false;
    }

    return str.back() == diagnosticStringLastSymbol &&
           (std::isalpha(str.front()) ||
           std::any_of(cbegin(diagnosticStringFirstSpecialSymbols), cend(diagnosticStringFirstSpecialSymbols), 
                [str](auto const& c)
                {
                    return c == str.front();
                }));
}

auto const warningLabel{ "warning:" };

bool IsWarningString(std::string_view str)
{
    return IsDiagnosticString(str) &&
        std::string::npos != str.find(warningLabel);
}

auto const pathBeginSymbol{ '/' };
auto const pathEndSymbol{ ':' };

// extract path
std::string ExtPath(std::string_view str)
{
    return std::string{
        std::find(cbegin(str), cend(str), pathBeginSymbol),
        std::find(cbegin(str), cend(str), pathEndSymbol) };
}

auto const lineColumnSeparator{ ':' };
auto const columnWarningSeparator{ ':' };

// returns pair< line, column >
auto ExtLineColumn(std::string_view str)
{
    std::pair< std::string, std::string > result;
    auto& line{ result.first };
    auto& column{ result.second };

    if (auto pathEnd{ std::find(cbegin(str), cend(str), pathEndSymbol) }; 
        pathEnd != str.cend())
    {
        auto lineBegin{ pathEnd + 1 };
        auto lineEnd{ std::find(lineBegin, cend(str), lineColumnSeparator) };
        line = std::string{ lineBegin, lineEnd};

        if (lineEnd != str.cend())
        {
            auto columnBegin{ lineEnd + 1 };
            column = std::string{ columnBegin, std::find(columnBegin, cend(str), columnWarningSeparator) };
        }
    }

    return result;
}

auto GetMessageBegin(std::string_view str)
{
    // end of warning label will be begin of the message
    auto warningLabelBeginIndex{ str.find(warningLabel) };
    return std::string::npos == warningLabelBeginIndex ?
        str.cend() :
        str.cbegin() + warningLabelBeginIndex + strlen(warningLabel);
}

auto const messageEndSymbol{ '[' };

auto GetMessageEnd(std::string_view str)
{
    return std::find(GetMessageBegin(str), cend(str), messageEndSymbol);
}

std::string ExtMessage(std::string_view str)
{
    return InplaceRoundTrim(std::string{ GetMessageBegin(str), GetMessageEnd(str)});
}

auto GetDescriptionBlockBegin(std::string_view str)
{
    return GetMessageEnd(str);
}

auto const descriptionBlockFirstSymbol{ '[' };
auto const descriptionBlockLastSymbol{ ']' };
auto const optionDescriptionSeparator{ ',' };

auto ExtWarningDescription(std::string_view str)
{
    std::pair< std::string, std::string > result;
    auto& option{ result.first };
    auto& description{ result.second };

    std::string descriptionBlock{ GetDescriptionBlockBegin(str), cend(str) };
    descriptionBlock = InplaceRoundTrim(std::move(descriptionBlock), descriptionBlockFirstSymbol, descriptionBlockLastSymbol);

    auto separator{ std::find(cbegin(descriptionBlock), cend(descriptionBlock), optionDescriptionSeparator) };
    option = std::string{ cbegin(descriptionBlock), separator };
    
    if (separator != cend(descriptionBlock))
    {
        separator++;
    }
        
    description = std::string{ separator, cend(descriptionBlock)};

    return result;
}

void TestStringParser()
{
    auto const warningStringExample{ "/opt/vsomeip/lib/cmake/vsomeip/../../../include/vsomeip/export.hpp:9:5: warning: '_WIN32' is not defined, evaluates to 0 [-Wundef,Lexical or Preprocessor Issue]" };
    std::cout << warningStringExample << std::endl;
    std::cout << IsDiagnosticString(warningStringExample) << std::endl;
    std::cout << IsWarningString(warningStringExample) << std::endl;
    std::cout << ExtPath(warningStringExample) << std::endl;
    auto [line, column] { ExtLineColumn(warningStringExample)};
    std::cout << line << " : " << column << std::endl;
    std::cout << ExtMessage(warningStringExample) << std::endl;
    auto [option, description] { ExtWarningDescription(warningStringExample) };
    std::cout << option << " | " << description << std::endl;
    
    std::filesystem::path p{ ExtPath(warningStringExample) };
    std::cout << p.filename().string() << std::endl;
}

struct Warning
{
    std::string path;
    std::string filename;
    std::string line;
    std::string column;
    std::string message;
    std::string compilerOption;
    std::string warningDescription;
};

auto const warningOutputSeparator{ '^' };

std::ostream& operator<<(std::ostream& os, Warning const& w)
{
    os << w.path << warningOutputSeparator << w.filename << warningOutputSeparator << 
          w.line << warningOutputSeparator << w.column << warningOutputSeparator << 
          w.message << warningOutputSeparator << w.compilerOption << warningOutputSeparator << w.warningDescription;
    return os;
}

std::size_t GetParsedPathLength(std::string const& path)
{
    return std::count(cbegin(path), cend(path), warningOutputSeparator);
}

void NormalizePath(std::string& path, std::size_t currLen, std::size_t const normalizedLen)
{
    while (currLen < normalizedLen)
    {
        path.insert(cbegin(path), warningOutputSeparator);
        currLen++;
    }
}

void ParseInputFileWarnings(std::ifstream& is)
{
    std::unordered_map< std::string, std::size_t > warningsRatio{};

    std::size_t longestPathLength{ 0u }; // needed to normalize all broken path Lengths
    std::size_t inputFileLine{ 0u };

    std::vector< Warning > warnings;

    std::string inputStr;
    while (std::getline(is, inputStr))
    {
        if (not IsWarningString(inputStr))
        {
            continue;
        }

        auto path{ ExtPath(inputStr) };
        auto const filename{ std::filesystem::path(path).filename().string() };
        auto const [line, column] { ExtLineColumn(inputStr)};
        auto const message{ ExtMessage(inputStr) };
        auto const [option, description] { ExtWarningDescription(inputStr) };

        if (path.empty())
        {
            std::cout << "path is empty on line " << inputFileLine << std::endl;
        }
        if (filename.empty())
        {
            std::cout << "filename is empty on line " << inputFileLine << std::endl;
        }
        if (line.empty())
        {
            std::cout << "line is empty on line " << inputFileLine << std::endl;
        }
        if (column.empty())
        {
            std::cout << "column is empty on line " << inputFileLine << std::endl;
        }
        if (message.empty())
        {
            std::cout << "message is empty on line " << inputFileLine << std::endl;
        }
        if (option.empty())
        {
            std::cout << "option is empty on line " << inputFileLine << std::endl;
        }
        if (description.empty())
        {
            std::cout << "description is empty on line " << inputFileLine << std::endl;
        }

        if (breakPath)
        {
            auto const pathWithoutFilename{ std::filesystem::path(path).remove_filename() };
            path.clear();
            for (auto const& p : pathWithoutFilename)
            {
                path += p.string() + warningOutputSeparator;
            }

            auto frontSymbolsToRemove = { pathBeginSymbol, warningOutputSeparator };
            auto removeBeginPred = [&path](auto const& c) { return path.front() == c; };
            while (not path.empty() && std::any_of(cbegin(frontSymbolsToRemove), cend(frontSymbolsToRemove), removeBeginPred))
            {
                path.erase(cbegin(path));
            }

            while (not path.empty() && (path.back() == warningOutputSeparator))
            {
                path.pop_back();
            }

            longestPathLength = std::max(longestPathLength, GetParsedPathLength(path));
        }

        warnings.emplace_back(path, filename, line, column, message, option, description);

        warningsRatio[option]++;

        inputFileLine++;
    }
    is.close();
    is.clear();

    if (breakPath)
    {
        // #1 common transform
        //std::transform(begin(warnings), end(warnings), begin(warnings),
        //    [longestPathLength](auto& warning) -> auto&
        //    {
        //        NormalizePath(warning.path, GetParsedPathLength(warning.path), longestPathLength);
        //        return warning;
        //    });

        // #2 simple range-for
        //for (auto& warning : warnings)
        //{
        //    NormalizePath(warning.path, GetParsedPathLength(warning.path), longestPathLength);
        //};

        // #3 views (perfect)
        for (auto& path : warnings | std::views::transform(&Warning::path))
        {
            NormalizePath(path, GetParsedPathLength(path), longestPathLength);
        }
    }

    std::cout << warnings.size() << " warnings parsed." << std::endl;

    std::ofstream os{ outputFile };
    
    for (auto const& warning : warnings)
    {
        os << warning << "\n";
    }
    
    for (auto const& w2c : warningsRatio)
    {
        std::cout << w2c.first << "  " << w2c.second << std::endl;
    }
}

int main()
{
    if (not std::filesystem::exists(inputFile))
    {
        std::cout << inputFile << " doesn't exists" << std::endl;
        return 1;
    }

    if (std::ifstream is{ inputFile })
    {
        ParseInputFileWarnings(is);
    }

    
    return 0;
}