#include "MQTTInbound.h"
#include <Arduino.h>


MQTTInbound::MQTTInbound(char * topic, SonoffApplicationCore * appcore) :
  mqttTopic{topic}
  , appCore{appcore}
{

}

//http://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void MQTTInbound::message(String topic, String payload)
{

  if (topic == this->mqttTopic) {
    Serial.println("exact match");
    //no commands in topic so fast exit
    return;
  }

  if (topic.startsWith(this->mqttTopic))
  {
    Serial.println("for this device");

    // strip the topic from the start of the message, suggests topic shouldnt be known in this method
    topic = topic.substring(strlen(this->mqttTopic) + 1);

    //TODO need to put channel ID in config
    String channelString = getValue(topic, '/', 0);
    if (!channelString.startsWith("channel-")) {
      Serial.println("no channel");
      return;
    }
    channelString.replace("channel-", "");
    int channel = channelString.toInt();
    Serial.println(channel);
    if (payload == "on") {
      this->appCore->externalOn();
    }
    if (payload == "off") {
      this->appCore->externalOff();
    }
    if (payload == "toggle") {
      this->appCore->externalToggle();
    }

    // TODO remodel this
    //      if(payload == "") {
    //        updateMQTT(channel);
    //      }

  }
  else
  {
    Serial.print("Topic didnt start with: ");
    Serial.println(this->mqttTopic);
  }
}


