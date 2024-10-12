#pragma once

namespace framework {

template<typename... Ts>
struct Overloaded : Ts...
{
	using Ts::operator()...;
};

}
