#pragma once


#include <vector>
#include <string>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <optional>

class IrcMessage
{
public:
  IrcMessage(std::stringstream ss);
  IrcMessage(std::string str): IrcMessage{std::stringstream{str}} {}
  IrcMessage(std::string&& prefix, std::string&& command, std::vector<std::string>&& args);
  IrcMessage(std::string&& command, std::vector<std::string>&& args);
  IrcMessage(std::unordered_map<std::string, std::optional<std::string>>&& tags, std::string&& prefix, std::string&& command, std::vector<std::string>&& args);
  ~IrcMessage() = default;

  IrcMessage(const IrcMessage&) = delete;
  IrcMessage(IrcMessage&&) = default;
  IrcMessage& operator=(const IrcMessage&) = delete;
  IrcMessage& operator=(IrcMessage&&) = default;

  std::unordered_map<std::string, std::optional<std::string>> tags;
  std::string prefix;
  std::string command;
  std::vector<std::string> arguments;
};

std::ostream& operator<<(std::ostream& os, const IrcMessage& message);


