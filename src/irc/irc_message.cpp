#include <irc/irc_message.hpp>
#include <iostream>
#include <string>
#include <utils/split.hpp>
#include <optional>

IrcMessage::IrcMessage(std::stringstream ss)
{
  if (ss.peek() == '@')
    {
      std::string raw_tags;
      ss.ignore();
      ss >> raw_tags;
      std::vector<std::string> tags = utils::split(raw_tags, ';', false);
      for (auto &tag: tags)
        {
          auto pos = tag.find('=');
          if (pos == std::string::npos)
              this->tags[tag] = {};
          else
              this->tags[tag.substr(0, pos)] = tag.substr(pos+1);
        }
      ss.ignore();
    }
  if (ss.peek() == ':')
    {
      ss.ignore();
      ss >> this->prefix;
    }
  ss >> this->command;
  while (ss >> std::ws)
    {
      std::string arg;
      if (ss.peek() == ':')
        {
          ss.ignore();
          std::getline(ss, arg);
        }
      else
        {
          ss >> arg;
          if (arg.empty())
            break;
        }
      this->arguments.push_back(std::move(arg));
    }
}

IrcMessage::IrcMessage(std::string&& prefix,
                       std::string&& command,
                       std::vector<std::string>&& args):
  tags(),
  prefix(std::move(prefix)),
  command(std::move(command)),
  arguments(std::move(args))
{
}

IrcMessage::IrcMessage(std::string&& command,
                       std::vector<std::string>&& args):
  tags(),
  prefix(),
  command(std::move(command)),
  arguments(std::move(args))
{
}

IrcMessage::IrcMessage(std::unordered_map<std::string, std::optional<std::string>>&& tags,
                       std::string&& prefix,
                       std::string&& command,
                       std::vector<std::string>&& args):
  tags(std::move(tags)),
  prefix(std::move(prefix)),
  command(std::move(command)),
  arguments(std::move(args))
{
}

std::ostream& operator<<(std::ostream& os, const IrcMessage& message)
{
  os << "IrcMessage";
  os << "{";
  for (auto& tag: message.tags)
    {
      os << tag.first;
      if (tag.second.has_value())
         os << "=" << tag.second.value();
      os << ";";
    }
  os << "}[" << message.command << "]";
  for (const std::string& arg: message.arguments)
    {
      os << "{" << arg << "}";
    }
  if (!message.prefix.empty())
    os << "(from: " << message.prefix << ")";
  return os;
}
