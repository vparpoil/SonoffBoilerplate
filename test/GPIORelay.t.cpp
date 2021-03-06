#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <Arduino.h>
#include "../GPIORelay.h"

class CapturingRelayObserver : public RelayObserver
{
	public:
		void OPEN()
		{ this->state = sOPEN;
		};
		void CLOSED()
		{ this->state = sCLOSED;
		};
		static int sOPEN;
		static int sCLOSED;
		int state=-1;
};

//Ask about declare and init of static member variables (or const)
int CapturingRelayObserver::sOPEN=1;
int CapturingRelayObserver::sCLOSED=2;


TEST_CASE("GPIO Relay opens and closes", "[GPIORelay]" ) {
	for (int i = 0; i < 12; i++) {
		spyPinMode[i] = -127;
		spyPinState[i] = -127;
	}
			
	SECTION("Relay setup") { 
		const int pinno=12;
		GPIORelay relay{pinno};
		REQUIRE( spyPinMode[pinno] == OUTPUT );
    	}

	SECTION("Opening announces state change") {
		CapturingRelayObserver capturingObserver{};
		const int pinno=12;
		GPIORelay relay{pinno};
		relay.addObserver(&capturingObserver);
		relay.open();
		REQUIRE( capturingObserver.state == CapturingRelayObserver::sOPEN);
		REQUIRE( spyPinState[pinno] == LOW );
	}

	SECTION("Closeing announces state change") {
		CapturingRelayObserver capturingObserver{};
		const int pinno=12;
		GPIORelay relay{pinno};
		relay.addObserver(&capturingObserver);
		relay.close();
		REQUIRE( capturingObserver.state == CapturingRelayObserver::sCLOSED);
		REQUIRE( spyPinState[pinno] == HIGH );
	}

}

