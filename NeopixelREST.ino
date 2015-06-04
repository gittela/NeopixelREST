#include <FastLED.h>
#include <YunServer.h>
#include <YunClient.h>
#define NUM_LEDS 3
#define DATA_PIN 6
CRGB leds[NUM_LEDS];
YunServer server;

void setup() {
	FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
	Serial.begin(9600);
	Bridge.begin();
	server.listenOnLocalhost();
	server.begin();
	leds[0 - 2] = CRGB::Black;

	FastLED.show();
}

void loop() {
	YunClient client = server.accept();

	if (client) {
		process(client);
		client.stop();
	}

	delay(50);
}
void process(YunClient client) {
// read the command
	String command = client.readStringUntil('/');

	if (command == "node") {
		nodeCommand(client);
	}
}
void nodeCommand(YunClient client) {
	int number, color;
// Read node number
	number = client.parseInt();

	if (client.read() == '/') {
		color = client.parseInt();
		if (color == 0) {
			leds[number] = CRGB::White;
		} else if (color == 1) {
			leds[number] = CRGB::Purple;
		} else if (color == 2) {
			leds[number] = CRGB::DeepSkyBlue;
		} else if (color == 3) {
			leds[number] = CRGB::LimeGreen;
		} else if (color == 4) {
			leds[number] = CRGB::Yellow;
		} else if (color == 5) {
			leds[number] = CRGB::Orange;
		} else if (color == 6) {
			leds[number] = CRGB::Red;
		} else if (color == 7) {
			leds[number] = CRGB::Black;
		}
		FastLED.show();
	}

// Send feedback to client
	client.print(("Node ") + (String) number + (" set to ") + color);

// Update datastore key with the current pin value
	/*String key = "D";
	 key += number;
	 Bridge.put(key, String(color));*/
}
