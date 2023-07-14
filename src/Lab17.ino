#include <MQTT.h>

SYSTEM_THREAD(ENABLED);

void callback(char *topic, byte *payload, unsigned int length);
MQTT client("lab.thewcl.com", 1883, callback);

String North_G = "Remy/Lab17/North/g";
String North_Y = "Remy/Lab17/North/y";
String North_R = "Remy/Lab17/North/r";
String East_G = "Remy/Lab17/East/g";
String East_Y = "Remy/Lab17/East/y";
String East_R = "Remy/Lab17/East/r";
String North_Car = "Remy/Lab17/North";
String East_Car = "Remy/Lab17/East";

int direction = 0;
struct State
{
  int output[6];
  unsigned long waitTime;
  int nextState[2];
};
typedef struct State State_t;
State_t fsm[4] =
 {
    {{1, 0, 0, 0, 0, 1}, 10, {0, 1}}, 
    {{0, 1, 0, 0, 0, 1}, 5000, {2, 2}}, 
    {{0, 0, 1, 1, 0, 0}, 10, {3, 2}}, 
    {{0, 0, 1, 0, 1, 0}, 5000, {0, 0}} 
};

int currentState = 0;

void setup()
{
  Serial.begin(9600);
  client.connect(System.deviceID());
  client.subscribe(North_Car);
  client.subscribe(East_Car);
}

void loop()
{
  if (client.isConnected())
  {
    client.loop();
  }
  else
  {
    client.connect(System.deviceID());
    client.subscribe(North_Car);
    client.subscribe(East_Car);
  }
  client.publish(North_G, String(fsm[currentState].output[0]));
  client.publish(North_Y, String(fsm[currentState].output[1]));
  client.publish(North_R, String(fsm[currentState].output[2]));
  client.publish(East_G, String(fsm[currentState].output[3]));
  client.publish(East_Y, String(fsm[currentState].output[4]));
  client.publish(East_R, String(fsm[currentState].output[5]));
  delay(fsm[currentState].waitTime);
  currentState = fsm[currentState].nextState[direction];
}
void callback(char *topic, byte *payload, unsigned int length)
{

  String callbackTopic = topic;

  if (callbackTopic.compareTo(North_Car))
  {
    direction = 1;
  }
  else if (callbackTopic.compareTo(East_Car))
  {
    direction = 0;
  }
}