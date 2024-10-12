#pragma once

#include <any>
#include <filesystem>
#include <map>
#include <string>
#include <unordered_map>

namespace framework::cli {

template<typename T>
class ArgumentParseResult;

enum class GroupMode
{
	Single,
	Flag,
	Many
};

namespace detail {

enum class ArgumentType
{
	String,
	Number,
	Path,
	Choice,
	Boolean
};

template<typename T>
struct ArgumentPusher
{
	T name;
	GroupMode grouping;
	ArgumentParseResult<T> *result;

	template<typename E>
	void push(E value)
	{
		result->push(name, value, grouping);
	}
};

template<typename T>
using ParsingFunctionPtr_t = void (*)(ArgumentPusher<T>&, const std::string&);

template<typename T, typename E>
void parseInteger(ArgumentPusher<T>& pusher, const std::string& param)
{
	if constexpr(std::is_signed_v<E>) {
		pusher.push(static_cast<E>(std::stoull(param)));
	} else {
		pusher.push(static_cast<E>(std::stoll(param)));
	}
}

template<typename T, typename E>
void parseSimple(ArgumentPusher<T>& pusher, const std::string& param)
{
	if constexpr(std::is_integral_v<E>) {
		parseInteger<E>(param);
	} else {
		pusher.push(E(param));
	}
}

template<typename T>
struct Argument
{
	T name;
	GroupMode groupMode;
	ParsingFunctionPtr_t<T> parsingFunction;
};

template<typename T>
struct ArgumentTypeMatcher;

#define DEFINE_ARGUMENT_TYPE_MATCHER(type)                                                                             \
	template<>                                                                                                         \
	struct ArgumentTypeMatcher<type>                                                                                   \
	{                                                                                                                  \
		template<typename T>                                                                                           \
		static constexpr ParsingFunctionPtr_t<T> PARSING_FUNC = &parseSimple<T, type>;                                    \
	}

DEFINE_ARGUMENT_TYPE_MATCHER(std::filesystem::path);
DEFINE_ARGUMENT_TYPE_MATCHER(std::string);
DEFINE_ARGUMENT_TYPE_MATCHER(int32_t);
DEFINE_ARGUMENT_TYPE_MATCHER(int64_t);
DEFINE_ARGUMENT_TYPE_MATCHER(int16_t);
DEFINE_ARGUMENT_TYPE_MATCHER(uint32_t);
DEFINE_ARGUMENT_TYPE_MATCHER(uint64_t);
DEFINE_ARGUMENT_TYPE_MATCHER(uint16_t);
DEFINE_ARGUMENT_TYPE_MATCHER(bool);

#undef DEFINE_ARGUMENT_TYPE_MATCHER

constexpr auto OPTIONAL_ESCAPE_CHARACTER = '-';

/**
	Parses the optional flag name out of a param string.

	@param param - The string to parse shit out of.
 */
inline std::string_view parseOptionalFlagFromParam(const std::string& param)
{
	size_t startIndex;
	for(startIndex = 0; startIndex < param.length() && OPTIONAL_ESCAPE_CHARACTER == param[startIndex]; ++startIndex) {}

	return { param.begin() + startIndex, param.end() };
}

class ArgumentsQueue final : public std::list<std::string>
{
public:
	explicit ArgumentsQueue(const std::vector<std::string>& arguments, const bool skipFirst)
		: list(arguments.begin(), arguments.end())
	{
		if(skipFirst) {
			pop_front();
		}
	}

	std::string dequeue_front()
	{
		auto current = std::move(front());
		pop_front();

		return current;
	}
};

}

template<typename T>
class ArgumentParseResult
{
public:
	explicit ArgumentParseResult() = default;

	template<typename E>
	const E& operator[](const T key) const
	{
		return std::any_cast<const E&>(m_Values.at(key));
	}

	template<typename E>
	const E& get(const T key) const
	{
		return operator[]<E>(key);
	}

	template<typename E>
	void push(T key, E value, const GroupMode grouping)
	{
		if(GroupMode::Many != grouping) {
			m_Values[key] = std::move(value);
			return;
		}

		append(key, std::move(value));
	}


private:
	template<typename E>
	void append(T key, E value)
	{
		if(!m_Values.contains(key)) {
			m_Values[key] = std::any{ std::vector<E>{} };
		}

		std::any_cast<std::vector<E>&>(m_Values.at(key)).push_back(std::move(value));
	}

	std::unordered_map<T, std::any> m_Values;
};

template<typename T>
class ArgumentParser
{
public:
	using Result_t = ArgumentParseResult<T>;

	explicit ArgumentParser()
		: m_Argument()
	{}

	/**
		Adds an argument to this argument parser.

		@tparam E - The type of argument to accept.

		@param name - The name of the argument.
		@param aliases - The names of the aliases to work on.
		@param grouping - The grouping options.
	 */
	template<typename E>
	ArgumentParser& addArgument(T name,
								std::initializer_list<std::string> aliases,
								GroupMode grouping = GroupMode::Single);
	template<typename E>
	ArgumentParser& addArgument(T name);

	/**
		Parses the arguments from the command line.

		@param arguments - The arguments to parse.
		@param skipFirst - Should skip the first argument.
	 */
	[[nodiscard]] Result_t parse(const std::vector<std::string>& arguments, bool skipFirst = true) const;

private:
	using Argument_t = detail::Argument<T>;

	/**
		Adds an argument to the optional argument list.

		@param name - The name of the argument.
		@param alias - The alias of key.
		@param grouping - The grouping method to use.
	 */
	template<typename E>
	ArgumentParser& addArgument(T name, std::string alias, GroupMode grouping);

	/**
		Creates a new result object for this parser.
	 */
	[[nodiscard]] Result_t createResultObject() const;

	/**
		Returns the info of an optional argument.

		@param param - The argument info name.
	 */
	std::optional<Argument_t> getOptionalArgumentInfo(const std::string& param) const;

	std::vector<Argument_t> m_Argument;
	std::unordered_map<std::string, Argument_t> m_OptionalArguments;
};

/**
	Creates a new argument parser.
 */
template<typename T>
ArgumentParser<T> makeParser();

template<typename T>
template<typename E>
ArgumentParser<T>& ArgumentParser<T>::addArgument(const T name,
												  const std::initializer_list<std::string> aliases,
												  const GroupMode grouping)
{
	for(const std::string& alias : aliases) {
		(void)addArgument<E>(name, alias, grouping);
	}

	return *this;
}

template<typename T>
template<typename E>
ArgumentParser<T>& ArgumentParser<T>::addArgument(T name)
{
	m_Argument.emplace_back(
		/* name = */ name,
		/* groupMode = */ GroupMode::Single,
		/* parsing_function = */ detail::ArgumentTypeMatcher<E>::template PARSING_FUNC<T>
	);

	return *this;
}

template<typename T>
auto ArgumentParser<T>::parse(const std::vector<std::string>& arguments, const bool skipFirst) const -> Result_t
{
	auto result = createResultObject();
	auto currentLinearArgument = m_Argument.begin();
	detail::ArgumentsQueue queue(arguments, /* skipFirst = */ skipFirst);

	while(!queue.empty()) {
		const auto current = queue.dequeue_front();
		if(const auto argument = getOptionalArgumentInfo(current); argument) {
			if(GroupMode::Flag == argument->groupMode) {
				result.push(argument->name, true, GroupMode::Flag);
				continue;
			}

			// Make sure we cant pop the next one
			if(queue.empty()) {
				std::stringstream stream;
				stream << "Got option " << current << " without value";
				throw std::runtime_error(stream.str());
			}

			const auto value = queue.dequeue_front();
			detail::ArgumentPusher<T> pusher{ argument->name, argument->groupMode, &result };
			argument->parsingFunction(pusher, value);
			continue;
		}

		if(m_Argument.end() == currentLinearArgument) {
			throw std::runtime_error("Too many parameters!");
		}

		const auto& argument = *currentLinearArgument;
		detail::ArgumentPusher<T> pusher{ argument.name, argument.groupMode, &result };
		argument.parsingFunction(pusher, current);

		++currentLinearArgument;
	}

	if(m_Argument.end() != currentLinearArgument) {
		throw std::runtime_error("Too few arguments");
	}

	return result;
}

template<typename T>
template<typename E>
ArgumentParser<T>& ArgumentParser<T>::addArgument(T name, std::string alias, GroupMode grouping)
{
	if(m_OptionalArguments.contains(alias)) {
		throw std::runtime_error("Argument alias is already in use: " + alias);
	}

	Argument_t argument{
		name,
		grouping,
		detail::ArgumentTypeMatcher<E>::template PARSING_FUNC<T>,
	};

	m_OptionalArguments[std::move(alias)] = std::move(argument);
	return *this;
}

template<typename T>
auto ArgumentParser<T>::createResultObject() const -> Result_t
{
	Result_t result;
	for(const auto& [_, value] : m_OptionalArguments) {
		if(GroupMode::Flag != value.groupMode) {
			continue;
		}

		result.push(value.name, false, GroupMode::Flag);
	}

	return result;
}

template<typename T>
auto ArgumentParser<T>::getOptionalArgumentInfo(const std::string& param) const -> std::optional<Argument_t>
{
	if(0 >= param.length() || detail::OPTIONAL_ESCAPE_CHARACTER != param[0]) {
		return std::nullopt;
	}

	const auto optionalKey = detail::parseOptionalFlagFromParam(param);
	auto iterator = std::ranges::find_if(m_OptionalArguments, [&](const auto& key) { return key.first == optionalKey; });
	if(m_OptionalArguments.end() == iterator) {
		std::stringstream stream;
		stream << "No option that matches name '" << param << "'";
		throw std::runtime_error(stream.str());
	}

	return iterator->second;
}
}
