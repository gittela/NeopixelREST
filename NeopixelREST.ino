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
		if (color == 1) {
			leds[number] = CRGB::Red;
		} else if (color == 2) {
			leds[number] = CRGB::Blue;
		} else if (color == 3) {
			leds[number] = CRGB::Green;
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
