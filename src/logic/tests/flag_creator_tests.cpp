#include "catch.hpp"

#include <vector>
#include <map>

#include "decoder.hpp"

using SingleFlagEvent = std::pair<jagce::FlagName, jagce::FlagState>;

template <size_t N>
using FlagStates = std::array<SingleFlagEvent, N>;

template <size_t N>
using ExpectedFlagEventMap = std::map<FlagStates<N>, jagce::FlagSetEvent>;


/* These cases are not comprehensive and are meant to catch large breakages.
 * TODO: Make comprehensive cases using templates/macros.
 */
TEST_CASE("flag event creator creates valid and correct events") {

	using namespace jagce;

	constexpr FlagState UNCH = FlagState::UNCH;
	constexpr FlagState SET = FlagState::SET;
	constexpr FlagState RESET = FlagState::RESET;
	constexpr FlagState DEFER = FlagState::DEFER;
	
	SECTION("flag events with size 1") {
		ExpectedFlagEventMap<1> expectedEvents{
		{ {SingleFlagEvent{FlagName::S, SET}}, {SET, UNCH, UNCH, UNCH, UNCH, UNCH, UNCH, UNCH} },
		{ {SingleFlagEvent{FlagName::Z, SET}}, {UNCH, SET, UNCH, UNCH, UNCH, UNCH, UNCH, UNCH} },
		{ {SingleFlagEvent{FlagName::F5, SET}}, {UNCH, UNCH, SET, UNCH, UNCH, UNCH, UNCH, UNCH} },
		{ {SingleFlagEvent{FlagName::H, SET}}, {UNCH, UNCH, UNCH, SET, UNCH, UNCH, UNCH, UNCH} },
		{ {SingleFlagEvent{FlagName::F3, SET}}, {UNCH, UNCH, UNCH, UNCH, SET, UNCH, UNCH, UNCH} },
		{ {SingleFlagEvent{FlagName::PV, SET}}, {UNCH, UNCH, UNCH, UNCH, UNCH, SET, UNCH, UNCH} },
		{ {SingleFlagEvent{FlagName::N, SET}}, {UNCH, UNCH, UNCH, UNCH, UNCH, UNCH, SET, UNCH} },
		{ {SingleFlagEvent{FlagName::C, SET}}, {UNCH, UNCH, UNCH, UNCH, UNCH, UNCH, UNCH, SET} }
		};

		for (const auto& eventPair : expectedEvents) {
			CHECK(flagSetEventCreator(eventPair.first) == eventPair.second);
		}
	}

	SECTION("flag events with size 2") {
		ExpectedFlagEventMap<2> expectedEvents{
		{ {SingleFlagEvent{FlagName::S, SET}, SingleFlagEvent{FlagName::Z, RESET}}, {SET, RESET, UNCH, UNCH, UNCH, UNCH, UNCH, UNCH} },
		{ {SingleFlagEvent{FlagName::S, SET}, SingleFlagEvent{FlagName::H, DEFER}}, {SET, UNCH, UNCH, DEFER, UNCH, UNCH, UNCH, UNCH} },
		{ {SingleFlagEvent{FlagName::N, DEFER}, SingleFlagEvent{FlagName::C, UNCH}}, {UNCH, UNCH, UNCH, UNCH, UNCH, UNCH, DEFER, UNCH} }
		};

		for (const auto& eventPair : expectedEvents) {
			CHECK(flagSetEventCreator(eventPair.first) == eventPair.second);
		}
	}

	SECTION("flag events with size 3") {
		ExpectedFlagEventMap<3> expectedEvents{
		{ {SingleFlagEvent{FlagName::S, SET}, SingleFlagEvent{FlagName::Z, RESET}, SingleFlagEvent{FlagName::F3, DEFER}}, {SET, RESET, UNCH, UNCH, DEFER, UNCH, UNCH, UNCH} },
		{ {SingleFlagEvent{FlagName::S, SET}, SingleFlagEvent{FlagName::H, DEFER}, SingleFlagEvent{FlagName::PV, SET}}, {SET, UNCH, UNCH, DEFER, UNCH, SET, UNCH, UNCH} },
		{ {SingleFlagEvent{FlagName::F5, DEFER}, SingleFlagEvent{FlagName::C, UNCH}, SingleFlagEvent{FlagName::N, SET}}, {UNCH, UNCH, DEFER, UNCH, UNCH, UNCH, SET, UNCH} }
		};
	
		for (const auto& eventPair : expectedEvents) {
			CHECK(flagSetEventCreator(eventPair.first) == eventPair.second);
		}
	}

}
